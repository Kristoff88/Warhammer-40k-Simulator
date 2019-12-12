#include "GlobalHeaders.h"
#include "obj_loader.h"


OBJstruct::OBJstruct() : vertices(NULL), colors(NULL),
normals(NULL), texCoords(NULL)
{}
OBJstruct::~OBJstruct()
{
	CleanUp();
}

void OBJstruct::CleanUp()
{
	if (vertices != NULL)
	{
		delete[] vertices;
		vertices = NULL;
	}

	if (texCoords != NULL)
	{
		delete[] texCoords;
		texCoords = NULL;
	}

	if (colors != NULL)
	{
		delete[] colors;
		colors = NULL;
	}

	if (normals != NULL)
	{
		delete[] normals;
		normals = NULL;
	}
}

bool OBJ_Loader::Init()
{
	// Inicjalizuje interpreter python'a.
	Py_Initialize();

	// Sciezka do modulu, modul oraz jego slownik (przestrzen nazw).
	PyObject* pModulePath, * pModule, * pDict;

	pModulePath = PyString_FromString(MODULE_PATH);
	pModule = PyImport_Import(pModulePath);
	pDict = PyModule_GetDict(pModule);

	// pobranie funkcji
	OBJLoader = PyDict_GetItemString(pDict, FUNCTION_NAME);


	// Czyszczenie referencji.
	Py_DECREF(pModulePath);
	Py_DECREF(pModule);
	//	Py_DECREF(dictionary);	// <-- nie wiem dlaczego, ale jak chce to wyczyscic,
								// to pozniej przy Py_Finalize() wyskakuje Access Violation


	// sprawdzenie, czy jest mozliwosc wywolywania pobranej funkcji z modulu

	if (!PyCallable_Check(OBJLoader))
	{
		return false;
	}
	else
		return true;
}

void OBJ_Loader::Close()
{
	// czyszczenie
	Py_DECREF(OBJLoader);

	// konczy prace interpretera python'a
	Py_Finalize();
}
//
/*************************************************************/

OBJstruct* OBJ_Loader::LoadOBJFile(const char* filename) throw(std::string)
{
	PyObject* pResult = ExecutePythonLoadingScript(filename);

	if (ResultIsCorrect(pResult))
	{
		OBJstruct* newOBJ = LoadOBJData(pResult);

		Py_DECREF(pResult);
		return newOBJ;
	}
	else if (ResultIsErrorInfo(pResult))
	{
		Py_DECREF(pResult);
		ThrowErrorInfo(pResult);
	}
	else
	{
		Py_DECREF(pResult);
		InformAboutUnexpectedResult(filename);
	}

	return nullptr;
}

PyObject* OBJ_Loader::ExecutePythonLoadingScript(const char* filename)
{
	PyObject* pFileName, * pArg, * pResult;

	pFileName = PyString_FromString(filename);

	pArg = PyTuple_New(1);
	PyTuple_SetItem(pArg, 0, pFileName);

	pResult = PyObject_CallObject(OBJLoader, pArg);

	Py_DECREF(pFileName);
	Py_DECREF(pArg);

	return pResult;
}

bool OBJ_Loader::ResultIsErrorInfo(PyObject* pResult)
{
	return PyObject_IsInstance(pResult, (PyObject*)& PyString_Type);
}

void OBJ_Loader::ThrowErrorInfo(PyObject* pResult) throw(std::string)
{
	std::string result = PyString_AsString(pResult);
	throw result;
}

bool OBJ_Loader::ResultIsCorrect(PyObject* pResult)
{
	return PyObject_IsInstance(pResult, (PyObject*)& PyList_Type);
}

OBJstruct* OBJ_Loader::LoadOBJData(PyObject* pResult)
{
	PyObject* pVertexCount = PyList_GetItem(pResult, 0),
		* pVertexArray = PyList_GetItem(pResult, 1),

		* pTexCoordCount = PyList_GetItem(pResult, 2),
		* pTexCoordArray = PyList_GetItem(pResult, 3),

		* pNormalCount = PyList_GetItem(pResult, 4),
		* pNormalArray = PyList_GetItem(pResult, 5),

		* pTextureCount = PyList_GetItem(pResult, 6),
		* pTextureArray = PyList_GetItem(pResult, 7),

		* pFaceCount = PyList_GetItem(pResult, 8),
		* pFaceArray = PyList_GetItem(pResult, 9),

		* pIndicesCount = PyList_GetItem(pResult, 10),
		* pIndiexCountersArray = PyList_GetItem(pResult, 11);


	int vertexCount = PyInt_AsLong(pVertexCount),
		texCoordCount = PyInt_AsLong(pTexCoordCount),
		normalCount = PyInt_AsLong(pNormalCount),

		faceCount = PyInt_AsLong(pFaceCount),

		textureCount = PyInt_AsLong(pTextureCount),
		indicexCountersCount = PyInt_AsLong(pIndicesCount);

	bool ObjectHasVertices = vertexCount > 0;
	bool ObjectHasTexCoords = texCoordCount > 0;
	bool ObjectHasNormals = normalCount > 0;
	bool ObjectHasFaces = faceCount > 0;

	bool IndicesCountEqualsTextureCount = indicexCountersCount == textureCount;

	assert(ObjectHasVertices && ObjectHasTexCoords && ObjectHasFaces);
	assert(IndicesCountEqualsTextureCount);

	int pointCount = faceCount * 3;
	OBJstruct* newOBJ = GenerateEmptyObject(pointCount, ObjectHasNormals);

	for (int face = 0, index = 0; face < pointCount / 3; face++, index += 3)
	{
		PyObject* pFace = PyList_GetItem(pFaceArray, face);

		for (int i = index, point = 0; point < 3; i++, point++)
		{
			PyObject* pPoint = PyList_GetItem(pFace, point);

			const Data3D vertexData = FromPoint_GetVertexData(pVertexArray, pPoint);

			newOBJ->vertices[i][0] = vertexData.x;
			newOBJ->vertices[i][1] = vertexData.y;
			newOBJ->vertices[i][2] = vertexData.z;

			const Data2D texCoordData = FromPoint_GetTexCoordData(pTexCoordArray, pPoint);

			newOBJ->texCoords[i][0] = texCoordData.x;
			newOBJ->texCoords[i][1] = texCoordData.z;

			if (ObjectHasNormals)
			{
				const Data3D normalData = FromPoint_GetNormalData(pNormalArray, pPoint);

				newOBJ->normals[i][0] = normalData.x;
				newOBJ->normals[i][1] = normalData.y;
				newOBJ->normals[i][2] = normalData.z;
			}
		}
	}

	for (int i = 0; i < indicexCountersCount; i++)
	{
		PyObject* pIndexCounter = PyList_GetItem(pIndiexCountersArray, i);
		PyObject* pTexture = PyList_GetItem(pTextureArray, i);

		int indexCounter = PyFloat_AsDouble(pIndexCounter);
		std::string texture = PyString_AsString(pTexture);

		newOBJ->indexCounters.push_back(indexCounter);
		newOBJ->textures.push_back(texture);
	}

	newOBJ->vertexCount = vertexCount;
	newOBJ->texCoordCount = texCoordCount;
	newOBJ->normalCount = normalCount;

	return newOBJ;
}

OBJstruct* OBJ_Loader::GenerateEmptyObject(int const& arraySize, bool const& ObjectHasNormals)
{
	OBJstruct* newOBJ = new OBJstruct;

	newOBJ->vertices = new float[arraySize][3];
	newOBJ->texCoords = new float[arraySize][2];

	if (ObjectHasNormals)
	{
		newOBJ->normals = new float[arraySize][3];
	}

	newOBJ->colors = new float[arraySize][3];

	for (int i = 0; i < arraySize; i++)
	{
		newOBJ->colors[i][0] = newOBJ->colors[i][1] =
			newOBJ->colors[i][2] = 1.f;
	}

	return newOBJ;
}

int OBJ_Loader::FromPoint_GetVertexIndex(PyObject* pPoint)
{
	PyObject* pVertexIndex = PyList_GetItem(pPoint, 0);

	char* v = PyString_AsString(pVertexIndex);

	return atoi(v) - 1;
}

int OBJ_Loader::FromPoint_GetTexCoordIndex(PyObject* pPoint)
{
	PyObject* pTexCoordIndex = PyList_GetItem(pPoint, 1);

	char* vt = PyString_AsString(pTexCoordIndex);

	return atoi(vt) - 1;
}

int OBJ_Loader::FromPoint_GetNormalIndex(PyObject* pPoint)
{
	PyObject* pNormalIndex = PyList_GetItem(pPoint, 2);

	char* vn = PyString_AsString(pNormalIndex);

	return atoi(vn) - 1;
}

OBJ_Loader::Data3D OBJ_Loader::FromPoint_GetVertexData(PyObject* pVertexArray, PyObject* pPoint)
{
	unsigned int vertexIndex = FromPoint_GetVertexIndex(pPoint);

	char* x = PyString_AsString(PyList_GetItem(PyList_GetItem(pVertexArray, vertexIndex), 0));
	char* y = PyString_AsString(PyList_GetItem(PyList_GetItem(pVertexArray, vertexIndex), 1));
	char* z = PyString_AsString(PyList_GetItem(PyList_GetItem(pVertexArray, vertexIndex), 2));

	return Data3D{ atof(x), atof(y), atof(z) };
};

OBJ_Loader::Data2D OBJ_Loader::FromPoint_GetTexCoordData(PyObject* pTexCoordArray, PyObject* pPoint)
{
	unsigned int texCoordIndex = FromPoint_GetTexCoordIndex(pPoint);

	char* x = PyString_AsString(PyList_GetItem(PyList_GetItem(pTexCoordArray, texCoordIndex), 0)),
		* z = PyString_AsString(PyList_GetItem(PyList_GetItem(pTexCoordArray, texCoordIndex), 1));

	return Data2D{ atof(x), atof(z) };
}

OBJ_Loader::Data3D OBJ_Loader::FromPoint_GetNormalData(PyObject* pNormalArray, PyObject* pPoint)
{
	unsigned int normalIndex = FromPoint_GetNormalIndex(pPoint);

	char* x = PyString_AsString(PyList_GetItem(PyList_GetItem(pNormalArray, normalIndex), 0)),
		* y = PyString_AsString(PyList_GetItem(PyList_GetItem(pNormalArray, normalIndex), 1)),
		* z = PyString_AsString(PyList_GetItem(PyList_GetItem(pNormalArray, normalIndex), 2));

	return Data3D{ atof(x), atof(y), atof(z) };
}

void OBJ_Loader::InformAboutUnexpectedResult(const char* filename) throw(std::string)
{
	std::string result;

	result = "Unexpected result of work by python module loadOBJ(filename), when loading";
	result += filename;
	result += "\n\n";
	result += "Expected string or tuple.";

	throw result;
}

OBJ_Loader::OBJ_Loader()
{}

OBJ_Loader::~OBJ_Loader()
{}

#undef MODULE_PATH
#undef FUNCTION_NAME