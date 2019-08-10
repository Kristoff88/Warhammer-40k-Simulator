	#include "GlobalHeaders.h"

	#include "DataManager.h"

	#include "..//..//Utilities//LineReader//LineReader.h"
	#include "..//..//Utilities//StripName//StripName.h"

// Paths to Utility resources.

	#define TEXTURES_PATH	"GUI_Textures\\"
	

/******************************************************
	Functions responsible for loading/removing 
	textures from TEXTURES_PATH.
******************************************************/

	int  GUI_DataManager::LoadTextures   () throw( std::string )
	{
		std::vector <std::string> folders, files;

		folders = GetFolderList(TEXTURES_PATH);

		int loadedTexturesCount = 0;

		for(int i = 0; i < folders.size(); i++)
		{
			files = GetFileListFromSubfolder(TEXTURES_PATH, folders[i].c_str(), "*.bmp");
			loadedTexturesCount += LoadTexturesFromFileList( files );

			files = GetFileListFromSubfolder(TEXTURES_PATH, folders[i].c_str(), "*.tga");
			loadedTexturesCount += LoadTexturesFromFileList( files );

			files = GetFileListFromSubfolder(TEXTURES_PATH, folders[i].c_str(), "*.jpg");
			loadedTexturesCount += LoadTexturesFromFileList( files );
		}

		return loadedTexturesCount;
	}

//
		int  GUI_DataManager::LoadTexturesFromFileList (std::vector <std::string> const &TexturesList)
		{
			int originalTexturesCount = 0;

			std::string programFolder = GetProgramFolderPath();

			for(int i = 0; i < TexturesList.size(); i++)
			{
				char szFileName[MAX_PATH] = "";

				strcpy_s( szFileName, programFolder.c_str() );
				strcat_s( szFileName, TexturesList[i].c_str() );


				if( LoadTexture(szFileName) )
				{
					originalTexturesCount++;
				}
			}

			return originalTexturesCount;
		}

		bool GUI_DataManager::LoadTexture              (char const *filename) throw( std::string )
		{
			bool originalTexture;

			if( textureArray.empty() )
			{
				originalTexture = true;
			}
			else
			{
				originalTexture = true;

				for(int i = 0; i < textureArray.size(); i++)
				{
					if( strcmp(filename, textureArray[i].filename) == 0 )
					{
						originalTexture = false;
						break;
					}
				}
			}

			if( originalTexture )
			{
				unsigned int generatedTextureID;

				try
				{
										 // function from Bitmap.h
					generatedTextureID = ::LoadTexture(filename, TRUE, GenerateTexture);
				}
				catch( std::string error )
				{
					throw error;
				}

				textureObj newtTexture;

				strcpy_s( newtTexture.filename, filename );
				newtTexture.textureID = generatedTextureID;

				textureArray.push_back(newtTexture);
			}

			return originalTexture;
		}
//
		std::string               GUI_DataManager::GetProgramFolderPath     ()
		{																		  // argumentCount is not needed
			LPWSTR *argumentList = CommandLineToArgvW( (LPCWSTR)GetCommandLineA(), new int );

			std::string programFolder;

			programFolder.insert(0, (const char*)argumentList[0], 0, MAX_PATH);

			programFolder.erase( programFolder.rfind('\\')+1, std::string::npos );  // usuniecie nazwy aplikacji
			programFolder.erase( programFolder.begin() );							// usuniecie pierwszego znaku (cudzyslow)

			return programFolder;
		}

		std::vector <std::string> GUI_DataManager::GetFolderList            (char const *path)
		{
			HANDLE			  directory;
			WIN32_FIND_DATAA  folderData;

			std::vector <std::string> folderList;


			char pathName[MAX_PATH];

			strcpy( pathName, path );
			strcat( pathName, "*.*" );

		// przejscie przez "." i ".." (current i parent directory)

			directory = FindFirstFileA(pathName, &folderData);
			FindNextFileA(directory, &folderData);


			bool FoundItem = true;

			while( FoundItem )
			{
				FindNextFileA(directory, &folderData);

				if ( folderData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
				// jezeli nazwa pozycji sie powtarza (jezeli natknieta sie na ostatni folder, bedzie sie on powtarzac)

					if( !folderList.empty() && strcmp( folderData.cFileName, folderList.back().c_str() ) == 0 )
					{
						FoundItem = false;
					}
					else
						folderList.push_back(folderData.cFileName);
				}
			}

			return folderList;
		}

		std::vector <std::string> GUI_DataManager::GetFileListFromSubfolder (char const *path, char const *folder, char *fileExtension)
		{
			HANDLE            file;
			WIN32_FIND_DATAA  fileData;

			std::vector <std::string> fileList;


			char folderPath[MAX_PATH] = "";

			strcat_s( folderPath, path );
			strcat_s( folderPath, folder );
			strcat_s( folderPath, "\\" );


			char searchLocation[MAX_PATH] = "";

			strcat_s( searchLocation, folderPath );
			strcat_s( searchLocation, fileExtension );


			file = FindFirstFileA( searchLocation, &fileData );

			if( file != INVALID_HANDLE_VALUE )
			{
				std::string firstFile;

				firstFile.insert(0, folderPath, 0, MAX_PATH);
				firstFile.append(fileData.cFileName);

				fileList.push_back(firstFile);


				bool FoundFile = true;

				while( FoundFile )
				{
					FoundFile = FindNextFileA( file, &fileData );

					if( FoundFile )
					{
						std::string nextFile;

						nextFile.insert(0, folderPath, 0, MAX_PATH);
						nextFile.append(fileData.cFileName);

						fileList.push_back(nextFile);
					}
				}
			}

			return fileList;
		}
//
	void GUI_DataManager::RemoveTextures ()
	{
		for(int i = 0; i < textureArray.size(); i++)
		{
			glDeleteTextures(1, &textureArray[i].textureID);
		}
	}



//
/******************************************************
		Texture registry managment.
******************************************************/

	unsigned int GUI_DataManager::GetTexture (char const *filename)
	{
		unsigned int textureID = 0;

		for(int i = 0; i < textureArray.size(); i++)
		{
			if( StripName(filename, false) == StripName(textureArray[i].filename, false) )
			{
				assert( textureArray[i].textureID != 0 );
				textureID = textureArray[i].textureID;
				break;
			}
		}

		return textureID;
	}

	unsigned int GUI_DataManager::GetTexture (int const &index)
	{
		if( index >= 0 && index < textureArray.size() )
		{
			assert( textureArray[index].textureID != 0 );
			return textureArray[index].textureID;
		}
		else
			return 0;
	}
//
	void GUI_DataManager::AddTexture         (char const *filename, unsigned int const &textureID)  throw( std::string )
	{
		CheckForTextureCollision(filename, textureID);
		CreateAndInsertTextureObj(filename, textureID);
	}
//
		void GUI_DataManager::CheckForTextureCollision  (char const *filename, unsigned int const &textureID)  throw( std::string )
		{
			std::string error;
			bool Collision = false;

			for(int i = 0; i < textureArray.size(); i++)
			{
				if( StripName(filename, false) == StripName(textureArray[i].filename, false) )
				{
					Collision = true;

					error  = "Texture name \"";
					error += filename;
					error += "\" is colliding with one of the already registered textures.";

					break;
				}

				if( textureID == textureArray[i].textureID )
				{
					Collision = true;

					char ID_AsString[MAX_PATH];
					_itoa_s( textureID, ID_AsString, MAX_PATH, 10);

					error  = "Texture ID ";
					error += ID_AsString;
					error += " of texture \"";
					error += filename;
					error += "\" is colliding with texture ID of the registered texture \"";
					error += textureArray[i].filename;
					error += "\"";

					break;
				}
			}

			if( Collision )
			{
				throw error;
			}
		}

		void GUI_DataManager::CreateAndInsertTextureObj (char const *filename, unsigned int const &textureID)
		{
			textureObj newTexture;

			strcpy( newTexture.filename, filename );
			newTexture.textureID = textureID;

			textureArray.push_back(newTexture);
		}

//
//
	std::vector <textureObj> GUI_DataManager::GetTextures      ()
	{
		std::vector <textureObj> textures = textureArray;

		for(int i = 0; i < textures.size(); i++)
		{
			strcpy( textures[i].filename, StripTextureLocation( textures[i].filename ).c_str() );
		}

		return textures;
	}

	std::vector <textureObj> GUI_DataManager::GetSavedTextures ()
	{
		return loadedTexturesFromSaveFile;
	}
//
/******************************************************
		Basic managment functions.
******************************************************/
	
	void GUI_DataManager::SetBorders            (RECT const &borders)
	{
		GUIborder.left   = (borders.left < borders.right)  ? borders.left : borders.right;
		GUIborder.top    = (borders.top  > borders.bottom) ? borders.top  : borders.bottom;
		GUIborder.right  = (borders.left > borders.right)  ? borders.left : borders.right;
		GUIborder.bottom = (borders.top  < borders.bottom) ? borders.top  : borders.bottom;
	}

	void GUI_DataManager::AdjustGUIToClientArea (HWND const &hwnd) throw( std::string )
	{
		WINDOWINFO info;
	
		if( !GetWindowInfo(hwnd, &info) )
		{
			std::string error = "Invalid window handle.\n\nOperation: Adjusting GUI to client area.";
			throw error;
		}

		int clientAreaHeight = info.rcClient.bottom - info.rcClient.top;
		int clientAreaWidth  = info.rcClient.right  - info.rcClient.left; 

		// Windows coordinates are vertically inverted, 
		// compared to default OpenGL coordinates.
		GUIborder.top    = clientAreaHeight; 
		GUIborder.bottom = 0;                

		GUIborder.left  = 0;  
		GUIborder.right = clientAreaWidth;
	}
//	
	void GUI_DataManager::SetColor              (float const &Red, float const &Green, float const &Blue, float const &Alpha)
	{
		GUIcolor.SetColor(Red, Green, Blue, Alpha);
	}

	void GUI_DataManager::SetColor              (Color const &color)
	{
		GUIcolor = color;
	}
//
	RECT  GUI_DataManager::GetBorders ()
	{
		return GUIborder;
	}

	Color GUI_DataManager::GetColor   ()
	{
		return GUIcolor;
	}

//
/*****************************************************/

	void GUI_DataManager::Save (std::fstream &file)
	{
		SaveGUIborder(file);
		SaveGUIcolor(file);
		SaveTextures(file);
	}
//
		void GUI_DataManager::SaveGUIborder (std::fstream &file)
		{
			file << "#GUIborder:\n\n"

			     << "\t" << "#Left:"   << "\t" << GUIborder.left   << "\n" 
				 << "\t" << "#Top:"    << "\t" << GUIborder.top    << "\n"

				 << "\t" << "#Right:"  << "\t" << GUIborder.right  << "\n" 
				 << "\t" << "#Bottom:" << "\t" << GUIborder.bottom << "\n\n";
		}

		void GUI_DataManager::SaveGUIcolor  (std::fstream &file)
		{
			file << "#GUIcolor:\n\n"

			     << "\t" << "#Red:"   << "\t" << GUIcolor.GetRed()   << "\n"
				 << "\t" << "#Green:" << "\t" << GUIcolor.GetGreen() << "\n"
				 << "\t" << "#Blue:"  << "\t" << GUIcolor.GetBlue()  << "\n"
				 << "\t" << "#Alpha:" << "\t" << GUIcolor.GetAlpha() << "\n\n";
		}

		void GUI_DataManager::SaveTextures  (std::fstream &file)
		{
			file << "#Texture_array:\n\n";

			file << "\t" << "#Texture_count:\t" << textureArray.size() << "\n";

			for(int i = 0; i < textureArray.size(); i++)
			{
				file << "\t" << textureArray[i].textureID 
					 << "\t" << StripTextureLocation( textureArray[i].filename )
					 << "\n";
			}

			file << "\n";
		}
		std::string GUI_DataManager::StripTextureLocation (char *filename)
		{
			std::string fullPath      = filename,
			            folderPath    = GetProgramFolderPath(),
						utilitiesPath = TEXTURES_PATH;
		
			fullPath.erase( 0, folderPath.size() );
			fullPath.erase( 0, utilitiesPath.size() );

			return fullPath;
		}
//
	void GUI_DataManager::Load (std::fstream &file)
	{
		LoadGUIborder(file);
		LoadGUIcolor(file);
		LoadTextures(file);
	}
//
		void GUI_DataManager::LoadGUIborder (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		GUIborder.left   = atof(line.c_str());
			line = ReadNewValue(file, '#');		GUIborder.top    = atof(line.c_str());
			line = ReadNewValue(file, '#');		GUIborder.right  = atof(line.c_str());
			line = ReadNewValue(file, '#');		GUIborder.bottom = atof(line.c_str());
		}

		void GUI_DataManager::LoadGUIcolor  (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		GUIcolor.SetRed   ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		GUIcolor.SetGreen ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		GUIcolor.SetBlue  ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		GUIcolor.SetAlpha ( atof(line.c_str()) );
		}

		void GUI_DataManager::LoadTextures  (std::fstream &file)
		{
			loadedTexturesFromSaveFile.clear();

			GetSavedTexturesNames(file);
			LoadSavedTextures();
		}
//
			void GUI_DataManager::GetSavedTexturesNames     (std::fstream &file)
			{
				std::string line;
				int textureCount;

				line = ReadNewValue(file, '#');
				textureCount = atoi(line.c_str());

				for(int i = 0; i < textureCount; i++)
				{
					textureObj newTexture;

					line = ReadNewValue(file, '#');	  
					newTexture.textureID = atoi(line.c_str());

					line = ReadNewValue(file, '#');
					strcpy( newTexture.filename, line.c_str() );
					
					loadedTexturesFromSaveFile.push_back(newTexture);
				}
			}

			void GUI_DataManager::LoadSavedTextures         ()
			{
				std::string texturesNotFound = "Textures that for some reason couldn't be loaded:\n\n";
				bool LoadingFailure = false;

				for(int i = 0; i < loadedTexturesFromSaveFile.size(); i++)
				{
					char filename[MAX_PATH];

					strcpy( filename, GetProgramFolderPath().c_str() );
					strcat( filename, TEXTURES_PATH );
					strcat( filename, loadedTexturesFromSaveFile[i].filename );

					try
					{
						LoadTexture(filename);
					}
					catch(std::string)
					{
						texturesNotFound += "- ";
						texturesNotFound += filename;
						texturesNotFound += "\n";

						LoadingFailure = true;
					}
				}


				if( LoadingFailure )
				{
					InformAboutLoadingFailure(texturesNotFound);
				}
			}
			void GUI_DataManager::InformAboutLoadingFailure (std::string failedTextureList)
			{
				MessageBox(NULL, failedTextureList.c_str(), 
					       "Texture loading failure.", MB_OK);
			}
//
/******************************************************
		Hidden consructor and destructor.
******************************************************/

	GUI_DataManager::GUI_DataManager  ()
	{}

	GUI_DataManager::~GUI_DataManager ()
	{
		RemoveTextures();
		textureArray.clear();
	}