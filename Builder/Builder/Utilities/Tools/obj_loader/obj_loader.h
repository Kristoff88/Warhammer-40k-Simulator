#pragma once

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif


/*
	NOTE: Loader assumes that loaded models consist of triangles.
*/

#define MODULE_PATH    "OBJ_Loader_Module"
#define FUNCTION_NAME  "LoadOBJ"


struct OBJstruct
{
	std::vector<unsigned int> indexCounters;
	std::vector<std::string> textures;

	float(*vertices)[3],
		(*texCoords)[2],
		(*colors)[3],
		(*normals)[3];

	int vertexCount,
		texCoordCount,
		normalCount;

	OBJstruct();
	~OBJstruct();

	void CleanUp();
};


class OBJ_Loader
{
public:

	static OBJ_Loader& Singleton()
	{
		static OBJ_Loader s;
		return s;
	}

	bool Init();
	void Close();

	OBJstruct* LoadOBJFile(const char* filename) throw(std::string);
private:
	struct Data3D
	{
		double x, y, z;
	};

	struct Data2D
	{
		double x, z;
	};

	struct NormalData
	{};

	inline PyObject* ExecutePythonLoadingScript(const char* filename);

	inline bool ResultIsErrorInfo(PyObject* pResult);
	inline void ThrowErrorInfo(PyObject* pResult) throw(std::string);

	inline bool ResultIsCorrect(PyObject* pResult);


	OBJstruct* LoadOBJData(PyObject* pResult);

	OBJstruct* GenerateEmptyObject(int const& arraySize, bool const& ObjectHasNormals);

	Data3D FromPoint_GetVertexData(PyObject* pVertexArray, PyObject* pPoint);
	int    FromPoint_GetVertexIndex(PyObject* pPoint);

	Data2D FromPoint_GetTexCoordData(PyObject* pTexCoordArray, PyObject* pPoint);
	int    FromPoint_GetTexCoordIndex(PyObject* pPoint);

	Data3D FromPoint_GetNormalData(PyObject* pNormalArray, PyObject* pPoint);
	int    FromPoint_GetNormalIndex(PyObject* pPoint);

	void InformAboutUnexpectedResult(const char* filename) throw(std::string);

	// Wskaznik na funkcje FUNCTION_NAME z modulu MODULE_PATH.
	PyObject* OBJLoader;

	OBJ_Loader();
	~OBJ_Loader();
};