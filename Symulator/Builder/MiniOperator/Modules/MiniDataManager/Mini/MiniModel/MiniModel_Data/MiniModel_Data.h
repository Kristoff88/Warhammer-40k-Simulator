#pragma once


	class MiniModel_Data
	{
		protected:

			struct Mesh
			{
				unsigned int firstIndex, indexCount,
							 textureID;
			};
		//
			struct Texture
			{
				unsigned int textureID;
				std::string textureName;

				Texture(): textureID(0)
				{}
			};
		//

		public:

			protected:
			MiniModel_Data ();
			//MiniModel_Data (MiniModel_Data *modelData);
			//MiniModel_Data& operator= (MiniModel_Data *modelData);
			public:

			MiniModel_Data (std::string const &filename);

			~MiniModel_Data ();

	/****************************************************************/

			void ClearModelData ();
				private:
				inline void ClearVBOData     ();
				inline void ClearTextureData ();
			public:

	/****************************************************************/

			void CopyVBOandMeshData (MiniModel_Data const &model);
			void CopyModelData      (MiniModel_Data *model);

	/****************************************************************/

			void InsertModelData (OBJstruct *OBJ, std::string const &locationPath) throw (std::string);
				private:
				inline bool ObjectLacksNormals (OBJstruct *OBJ);
				inline void CalculateNormals   (OBJstruct *OBJ);

				inline std::vector <Mesh> ExtractMeshData (OBJstruct *OBJ, std::string const &locationPath);
				inline bool TextureIsOriginal                  (std::string textureName);
				inline void LoadAndInsertNewTextureToTheVector (std::string locationPath, std::string textureName);
				inline unsigned int GetTextureID (std::string textureName);
				static unsigned int generateModelTexture (unsigned char *bits, unsigned int &width, unsigned int &height)
				{
					unsigned int textureID;

					glGenTextures(1, &textureID);
					glBindTexture(GL_TEXTURE_2D, textureID);
				
		
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);//_MIPMAP_LINEAR);

					glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
					glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


					gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height,
								  GL_RGBA, GL_UNSIGNED_BYTE, bits );


					glBindTexture(GL_TEXTURE_2D, 0);

					return textureID;
				}
				//
				inline unsigned int GenerateVertexVBO   (int const &indexCount, float (*vertexArray)[3]);
				inline unsigned int GenerateTexCoordVBO (int const &indexCount, float (*texCoordArray)[2]);
				inline unsigned int GenerateNormalVBO   (int const &indexCount, float (*normalArray)[3]);

				inline int GetIndexCount       ();
				inline void CalculateModelSize (float (*vertexArray)[3], int const &vertexCount);
			public:		

	/****************************************************************/

			std::string GetModelName ();

			Vector3 GetModelSize ();

	/****************************************************************/

			static void ClearStaticTextureVector ()
			{
				for(int i = 0; i < textureVector.size(); i++)
				{
					if( textureVector[i].textureID )
					{
						glDeleteTextures(1, &textureVector[i].textureID);
					}
				}

				textureVector.clear();
			}
	//
	/****************************************************************/

			void Save (std::fstream &file);
				private:
				inline void SaveModelName (std::fstream &file);
				inline void SaveSize      (std::fstream &file);
			public:

			void Load (std::fstream &file);
				private:
				inline void LoadModelName (std::fstream &file);
				inline void LoadSize      (std::fstream &file);
			public:

		protected:

			std::string modelName;

			Vector3 size;

			unsigned int vboIDv,
						 vboIDvt,
						 vboIDvn;

			std::vector <Mesh> meshVector;


			static std::vector <Texture> textureVector;
	};