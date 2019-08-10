#pragma once


	#include "TextureObject.h"


	class ControlManager;

	class InputProcessor;
	class GUI;



	class GUI_DataManager
	{
		friend ControlManager;
		friend InputProcessor;
		friend GUI;

		public:

			static GUI_DataManager& Singleton ()
			{
				static GUI_DataManager singleton;
				return singleton;
			}
	//
	/******************************************************
		Functions responsible for loading/removing 
		textures from TEXTURES_PATH.
	******************************************************/

			int  LoadTextures   () throw( std::string );
				private:
				int                 LoadTexturesFromFileList (std::vector <std::string> const &TexturesList);
				bool                LoadTexture              (char const *filename) throw( std::string );
				static unsigned int GenerateTexture          (unsigned char *bits, unsigned int &width, unsigned int &height)
				{
					unsigned int newTextureID;

					glGenTextures(1, &newTextureID);
					glBindTexture(GL_TEXTURE_2D, newTextureID);
	

					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


					gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height,
									  GL_RGBA, GL_UNSIGNED_BYTE, bits);

					glBindTexture(GL_TEXTURE_2D, 0);


					return newTextureID;
				}
				//
				std::string               GetProgramFolderPath     ();
				std::vector <std::string> GetFolderList            (char const *path);
				std::vector <std::string> GetFileListFromSubfolder (char const *path, char const *folder, char *fileExtension);
			public:
		
			void RemoveTextures ();

	/******************************************************
			Texture registry managment.
	******************************************************/

			unsigned int GetTexture (char const *filename);
			unsigned int GetTexture (int const &index);

			void         AddTexture (char const *filename, unsigned int const &textureID)  throw( std::string );
				private:
				void CheckForTextureCollision  (char const *filename, unsigned int const &textureID)  throw( std::string );
				void CreateAndInsertTextureObj (char const *filename, unsigned int const &textureID);
			public:


			private:
			std::vector <textureObj> GetTextures      ();
			std::vector <textureObj> GetSavedTextures ();
			public:

	/******************************************************
			Basic managment functions.
	******************************************************/

			void SetBorders            (RECT const &borders);
			void AdjustGUIToClientArea (HWND const &hwnd) throw( std::string );

			void SetColor              (float const &Red, float const &Green, float const &Blue, float const &Alpha);
			void SetColor              (Color const &color);

			RECT  GetBorders ();
			Color GetColor   ();

		private:

			std::vector <textureObj> textureArray;
			std::vector <textureObj> loadedTexturesFromSaveFile;


			RECT  GUIborder;
			Color GUIcolor;

	/*****************************************************/

			void Save (std::fstream &file);

				void SaveGUIborder (std::fstream &file);	
				void SaveGUIcolor  (std::fstream &file);	
				void SaveTextures  (std::fstream &file);
					inline std::string StripTextureLocation (char *filename);

			void Load (std::fstream &file);

				void LoadGUIborder (std::fstream &file);	
				void LoadGUIcolor  (std::fstream &file);	
				void LoadTextures  (std::fstream &file);
					inline void GetSavedTexturesNames     (std::fstream &file);
					inline void LoadSavedTextures         ();
					inline void InformAboutLoadingFailure (std::string failedTextureList);


	/******************************************************
		Hidden consructor and destructor.
	******************************************************/

			GUI_DataManager  ();
			~GUI_DataManager ();
	};