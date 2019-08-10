	#include "GlobalHeaders.h"

	#include "MiniModel_Data.h"

	extern  PFNGLGENBUFFERSARBPROC	         glGenBuffersARB;           // VBO Name Generation Procedure
	extern  PFNGLBINDBUFFERARBPROC			 glBindBufferARB;           // VBO Bind Procedure
	extern  PFNGLBUFFERDATAARBPROC			 glBufferDataARB;           // VBO Data Loading Procedure
	extern  PFNGLBUFFERSUBDATAARBPROC		 glBufferSubDataARB;        // VBO Sub Data Loading Procedure
	extern  PFNGLDELETEBUFFERSARBPROC		 glDeleteBuffersARB;        // VBO Deletion Procedure
	extern  PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB; // return various parameters of VBO
	extern  PFNGLMAPBUFFERARBPROC			 glMapBufferARB;            // map VBO procedure
	extern  PFNGLUNMAPBUFFERARBPROC			 glUnmapBufferARB;      


	// Initialization of static texture registry.

		std::vector <MiniModel_Data::Texture> MiniModel_Data::textureVector;



	MiniModel_Data::MiniModel_Data ()
	{
		vboIDv = vboIDvt = vboIDvn = 0;
	}

	/*MiniModel_Data::MiniModel_Data (MiniModel_Data *modelData)
	{
		this->operator =(modelData);
	}

	MiniModel_Data& MiniModel_Data::operator= (MiniModel_Data *modelData)
	{
		CopyModelData(modelData);
		return *this;
	}*/
//
	MiniModel_Data::MiniModel_Data  (std::string const &filename)
	{
		modelName = filename;
		vboIDv = vboIDvt = vboIDvn = 0;
	}
//
	MiniModel_Data::~MiniModel_Data ()
	{
		//ClearVBOData();
		//ClearTextureData();		
	}
//
	void MiniModel_Data::CopyVBOandMeshData (MiniModel_Data const &model)
	{
		vboIDv  = model.vboIDv;
		vboIDvt = model.vboIDvt;
		vboIDvn = model.vboIDvn;

		meshVector.clear();

		for(int i = 0; i < model.meshVector.size(); i++)
		{
			meshVector.push_back( model.meshVector[i] );
		}
	}

	void MiniModel_Data::CopyModelData      (MiniModel_Data *model)
	{
		modelName = model->GetModelName();
		CopyVBOandMeshData(*model);
		size = model->GetModelSize();
	}
//
/****************************************************************/

	void MiniModel_Data::ClearModelData ()
	{
		ClearVBOData();
		ClearTextureData();
	}
//
		void MiniModel_Data::ClearVBOData     ()
		{	
			if( vboIDv != 0 )
			{
				glDeleteBuffersARB(1, &vboIDv);
			}

			if( vboIDvt != 0 )
			{
				glDeleteBuffersARB(1, &vboIDvt);
			}

			if( vboIDvn != 0 )
			{
				glDeleteBuffersARB(1, &vboIDvn);
			}
		}

		void MiniModel_Data::ClearTextureData ()
		{
			for(int i = 0; i < meshVector.size(); i++)
			{
				assert( meshVector[i].textureID != 0 );
				glDeleteTextures(1, &meshVector[i].textureID);
			}
		}
//
/****************************************************************/

	void MiniModel_Data::InsertModelData (OBJstruct *OBJ, std::string const &locationPath)
	{
		assert( OBJ != NULL );

		if( ObjectLacksNormals(OBJ) )
		{
			CalculateNormals(OBJ);
		}

		meshVector = ExtractMeshData(OBJ, locationPath);

		// VBO's need to be generated after assigning mesh data.
		// Without the mesh data, there is no index counters.

		vboIDv  = GenerateVertexVBO(GetIndexCount(), OBJ->vertices);
		vboIDvt = GenerateTexCoordVBO(GetIndexCount(), OBJ->texCoords);
		vboIDvn = GenerateNormalVBO(GetIndexCount(), OBJ->normals);

		CalculateModelSize(OBJ->vertices, OBJ->vertexCount);
	}
//
		bool MiniModel_Data::ObjectLacksNormals (OBJstruct *OBJ)
		{
			return OBJ->normals == NULL;
		}

		void MiniModel_Data::CalculateNormals   (OBJstruct *OBJ)
		{
			int arraySize = 0;

			for(int i = 0; i < OBJ->indexCounters.size();    i++)
			{
				arraySize += OBJ->indexCounters[i];
			}

			OBJ->normals = new float [arraySize][3];

			for(int i = 0; i < OBJ->indexCounters.size(); i++)
			{
				crossProduct(OBJ->vertices[i], OBJ->vertices[i+1], OBJ->vertices[i+2],
					         OBJ->normals[i]);
			}
		}
//
		std::vector <MiniModel_Data::Mesh> MiniModel_Data::ExtractMeshData (OBJstruct *OBJ, std::string const &locationPath)
		{
			bool EachMeshHasTextureAssigned = OBJ->indexCounters.size() == OBJ->textures.size();
			assert( EachMeshHasTextureAssigned );

			std::vector <Mesh> meshVector;


			int firstIndex = 0;

			for(int i = 0; i < OBJ->textures.size(); i++)
			{
				Mesh newMesh;

				newMesh.firstIndex = firstIndex;
				newMesh.indexCount  = OBJ->indexCounters[i];

				firstIndex += OBJ->indexCounters[i];


				std::string materialName = OBJ->textures[i].substr(OBJ->textures[i].find("_")+1, std::string::npos);

				if( TextureIsOriginal(materialName) )
				{
					LoadAndInsertNewTextureToTheVector(locationPath, materialName);
				}

				newMesh.textureID = GetTextureID(materialName);

				
				meshVector.push_back(newMesh);
			}


			return meshVector;
		}
//
		bool MiniModel_Data::TextureIsOriginal                  (std::string textureName)
		{
			bool TextureIsOriginal = true;

			for(int i = 0; i < textureVector.size(); i++)
			{
				if( textureName == textureVector[i].textureName )
				{
					TextureIsOriginal = false;
					break;
				}
			}

			return TextureIsOriginal;
		}

		void MiniModel_Data::LoadAndInsertNewTextureToTheVector (std::string locationPath, std::string textureName)
		{
			char texturePath[MAX_PATH];
			strcpy_s(texturePath, MAX_PATH, locationPath.c_str());
			strcat_s(texturePath, MAX_PATH, textureName.c_str());

			Texture newTexture;

			newTexture.textureName = textureName;
			newTexture.textureID = LoadTexture(texturePath, true, generateModelTexture);

			MiniModel_Data::textureVector.push_back(newTexture);
		}

		unsigned int MiniModel_Data::GetTextureID (std::string textureName)
		{
			unsigned int textureID = 0;

			for(int i = 0; i < textureVector.size(); i++)
			{
				if( textureName == textureVector[i].textureName )
				{
					textureID = textureVector[i].textureID;
					break;
				}
			}

			return textureID;
		}
//
		unsigned int MiniModel_Data::GenerateVertexVBO   (int const &indexCount, float (*vertexArray)[3])
		{
			unsigned int vertexVBO;

			glGenBuffersARB(1, &vertexVBO);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexVBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, indexCount*3*sizeof(float), vertexArray, GL_STATIC_DRAW_ARB);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			return vertexVBO;
		}

		unsigned int MiniModel_Data::GenerateTexCoordVBO (int const &indexCount, float (*texCoordArray)[2])
		{
			unsigned int texCoordVBO;

			glGenBuffersARB(1, &texCoordVBO);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordVBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, indexCount*2*sizeof(float), texCoordArray, GL_STATIC_DRAW_ARB);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			return texCoordVBO;
		}

		unsigned int MiniModel_Data::GenerateNormalVBO   (int const &indexCount, float (*normalArray)[3])
		{
			unsigned int normalVBO;

			glGenBuffersARB(1, &normalVBO);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalVBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, indexCount*3*sizeof(float), normalArray, GL_STATIC_DRAW_ARB);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			return normalVBO;
		}
//
		int MiniModel_Data::GetIndexCount       ()
		{
			int indexCount = 0;

			for(int i = 0; i < meshVector.size(); i++)
			{
				indexCount += meshVector[i].indexCount;
			}

			return indexCount;
		}

		void MiniModel_Data::CalculateModelSize (float (*vertexArray)[3], int const &vertexCount)
		{
			assert( vertexArray != NULL && vertexCount > 0 );


			float minX, minY, minZ, 
				  maxX, maxY, maxZ,

				  x, y, z;

			minX = maxX = vertexArray[0][0];
			minY = maxY = vertexArray[0][1];
			minZ = maxZ = vertexArray[0][2];


			for(int i = 0; i < vertexCount; i++)
			{
				x = vertexArray[i][0];
				y = vertexArray[i][1];
				z = vertexArray[i][2];

				if( maxX < x )	maxX = x;
				if( minX > x )	minX = x;

				if( maxY < y )	maxY = y;
				if( minY > y )	minY = y;

				if( maxZ < z )	maxZ = z;
				if( minZ > z )	minZ = z;
			}

			size.x = (maxX - minX) / 2.f;
			size.y = (maxY - minY) / 2.f;
			size.z = (maxZ - minZ) / 2.f;
		}
//
/****************************************************************/

	std::string MiniModel_Data::GetModelName ()
	{
		return modelName;
	}

	Vector3 MiniModel_Data::GetModelSize ()
	{
		return size;
	}
//
/****************************************************************/

	void MiniModel_Data::Save (std::fstream &file)
	{
		SaveModelName(file);
		SaveSize(file);
	}
//
		void MiniModel_Data::SaveModelName (std::fstream &file)
		{
			file << "#Model_name:" << "\t" << modelName << "\n\n";
		}

		void MiniModel_Data::SaveSize      (std::fstream &file)
		{
			file << "#Size:" << "\n"
				 << "#X:" << "\t" << size.x << "\t" 
				 << "#Y:" << "\t" << size.y << "\t" 
				 << "#Z:" << "\t" << size.z << "\n\n";
		} 
//
	void MiniModel_Data::Load (std::fstream &file)
	{
		LoadModelName(file);
		LoadSize(file);
	}
//
		void MiniModel_Data::LoadModelName (std::fstream &file)
		{
			modelName = ReadNewValue(file, '#');
		}

		void MiniModel_Data::LoadSize      (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		size.x = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		size.y = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		size.z = atoi( line.c_str() );
		}