	#include "GlobalHeaders.h"

	#include "ModelsOperator.h"


// uniwersalne zmienne

	extern  int	MAP_X,      
				MAP_Z,        
				MAP_SCALE;     

	extern  float	(*g_terrain)[3];


// Zmienne kursora

	extern  int		cursorX, cursorZ;



/*****************************************************************
	Metody do pracy na modelach.
*****************************************************************/
		
	//void ModelsOperator::placeModel      ()
	//{
	//	if( strcmp( currentModel, "" ) != 0 )
	//	{
	//		Model *newModel = new Model;


	//		newModel->IsObstacle = ObstacleCheck;

	//	// pobranie mnoznikow kolorow

	//		newModel->R = (float)GetDlgItemInt(models, IDC_MODEL_RED,   FALSE, FALSE) / 255.f;
	//		newModel->G = (float)GetDlgItemInt(models, IDC_MODEL_GREEN, FALSE, FALSE) / 255.f;
	//		newModel->B = (float)GetDlgItemInt(models, IDC_MODEL_BLUE,  FALSE, FALSE) / 255.f;
	//		newModel->A = (float)GetDlgItemInt(models, IDC_MODEL_ALPHA, FALSE, FALSE) / 255.f;

	//	// ustawienie poczatkowego wspolczynnika skali modelu

	//		newModel->scale = 1.f;

	//	// ustawienie stanu obrotu

	//		newModel->rotateX = 0;
	//		newModel->rotateY = 0;
	//		newModel->rotateZ = 0;


	//	// pobranie id vbo modelu (wierzcholki, texCoords, etc.), ktory jest trzymany w wektorze unikalnych modeli

	//		for(int i = 0; i < modelArray.size(); i++)
	//		{
	//			if( strcmp(modelArray[i]->filename, currentModel) == 0 )
	//			{
	//				newModel->vboIDv  = modelArray[i]->vboIDv;
	//				newModel->vboIDvt = modelArray[i]->vboIDvt;
	//				newModel->vboIDvn = modelArray[i]->vboIDvn;

	//				newModel->meshVector = modelArray[i]->meshVector;

	//				newModel->sizeX = modelArray[i]->sizeX;
	//				newModel->sizeY = modelArray[i]->sizeY;
	//				newModel->sizeZ = modelArray[i]->sizeZ;

	//				break;
	//			}
	//		}

	//	// utworzenie nowego Object'a

	//		// obliczenie pozycji Y na mapie na podstawie pozycji kursora
	//		int index = cursorX/MAP_SCALE + MAP_X * -cursorZ/MAP_SCALE;

	//		ObjectOperator::Object newObject(cursorX, g_terrain[index][1], cursorZ,

	//										 newModel->sizeX, newModel->sizeY, newModel->sizeZ,

	//										 newModel,
	//										 preModel,
	//										 drawModel,
	//										 postModel,
	//										 moveModel,
	//										 removeModel,
	//										 modelKeyManager,
	//										 modelText,
	//										 NULL,
	//										 MODEL_NAME,
	//										 FALSE,
	//										 FALSE,
	//										 TRUE);


	//		ObjectOperator::Singleton().Add(newObject);
	//		modelCounter++;
	//	}
	//	else
	//		MessageBox(g_hwnd, "Model object hasn't been chosen.", "Error!", MB_OK);
	//}

	/*void ModelsOperator::removeAllModels ()
	{
		if ( !ObjectOperator::Singleton().removeSpecificObjects(MODEL_NAME) )
		{
			MessageBox(g_hwnd, "There are no models on the map.", "Error!", MB_OK);
		}
	}*/
//
/*****************************************************************
	Ladowanie / usuwanie obiektow modeli.
*****************************************************************/

	bool ModelsOperator::loadModel          (char *filename) throw(std::string)
	{
		bool LoadedNewModel;


		if( ModelIsOriginal(filename) )
		{
			LoadedNewModel = true;

			try
			{
				OBJstruct *newOBJ = OBJ_Loader::Singleton().LoadOBJFile(filename);

				model_DataObject *newModel = ComposeModelDataObjectFromObjectStructure(filename, newOBJ);
		
				modelArray.push_back(newModel);
			//	AddNewModelsNameToModelComboBox(filename);

				delete newOBJ;
			}
			catch(std::string error)
			{
				MessageBox(GetParent(models), error.c_str(), "Error!", MB_OK);
				LoadedNewModel = false;
			}
		}
		else
			LoadedNewModel = false;


		return LoadedNewModel;
	}
//
		bool ModelsOperator::ModelIsOriginal (char *filename)
		{
			bool originalModel;

			if( modelArray.empty() )
			{
				originalModel = true;
			}
			else
			{
				originalModel = true;

				for(int i = 0; i < modelArray.size(); i++)
				{
					if( strcmp(filename, modelArray[i]->filename) == 0 )
					{
						originalModel = false;
						break;
					}
				}
			}
			
			return originalModel;
		}
//
		model_DataObject* ModelsOperator::ComposeModelDataObjectFromObjectStructure (char *filename, OBJstruct *OBJ)
		{
			model_DataObject *newModel = new model_DataObject(filename);
				
			if( ObjectLacksNormals(OBJ) )
			{
				CalculateNormals(OBJ);
			}

			newModel->meshVector = ExtractMeshData(OBJ);

			// VBO's need to be generated after assigning mesh data.
			// Without the mesh data, there is no index counters.

			newModel->vboIDv  = GenerateVertexVBO(newModel->GetIndexCount(), OBJ->vertices);
			newModel->vboIDvt = GenerateTexCoordVBO(newModel->GetIndexCount(), OBJ->texCoords);
			newModel->vboIDvn = GenerateNormalVBO(newModel->GetIndexCount(), OBJ->normals);

			newModel->CalculateModelSize(OBJ->vertices, OBJ->vertexCount);

			return newModel;
		}
//
		bool ModelsOperator::ObjectLacksNormals (OBJstruct *OBJ)
		{
			return OBJ->normals == NULL;
		}

		void ModelsOperator::CalculateNormals   (OBJstruct *OBJ)
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
		std::vector <Mesh> ModelsOperator::ExtractMeshData (OBJstruct *OBJ)
		{
			bool EachMeshHasTextureAssigned = OBJ->indexCounters.size() == OBJ->textures.size();
			assert( EachMeshHasTextureAssigned );


			std::vector <Mesh> meshVector;

			for(int i = 0; i < OBJ->textures.size(); i++)
			{
				Mesh newMesh;

				newMesh.indexCounter = OBJ->indexCounters[i];
				newMesh.textureID = 0;

				for(int j = 0; j < modelTextureArray.size(); j++)
				{
					std::string materialName = OBJ->textures[i].substr(OBJ->textures[i].find("_")+1, std::string::npos);

					if( StripName(materialName, false) == StripName(modelTextureArray[j].filename, false) )
					{
						newMesh.textureID = modelTextureArray[j].textureID;
						break;
					}
				}

				meshVector.push_back(newMesh);
			}

			return meshVector;
		}
//
		unsigned int ModelsOperator::GenerateVertexVBO   (int const &indexCount, float (*vertexArray)[3])
		{
			unsigned int vertexVBO;

			glGenBuffersARB(1, &vertexVBO);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexVBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, indexCount*3*sizeof(float), vertexArray, GL_STATIC_DRAW_ARB);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			return vertexVBO;
		}

		unsigned int ModelsOperator::GenerateTexCoordVBO (int const &indexCount, float (*texCoordArray)[2])
		{
			unsigned int texCoordVBO;

			glGenBuffersARB(1, &texCoordVBO);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordVBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, indexCount*2*sizeof(float), texCoordArray, GL_STATIC_DRAW_ARB);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			return texCoordVBO;
		}

		unsigned int ModelsOperator::GenerateNormalVBO   (int const &indexCount, float (*normalArray)[3])
		{
			unsigned int normalVBO;

			glGenBuffersARB(1, &normalVBO);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalVBO);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, indexCount*3*sizeof(float), normalArray, GL_STATIC_DRAW_ARB);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			return normalVBO;
		}
//
		/*void ModelsOperator::AddNewModelsNameToModelComboBox (char *filename)
		{
			std::string newItemName = StripName(filename, false);

			HWND comboBox = GetDlgItem(models, IDC_MODEL_COMBO);
			ComboBox_AddString(comboBox, newItemName.c_str());
		}*/
//
//
	int  ModelsOperator::loadModels         ()
	{
		std::vector <std::string> folders, files;

		HANDLE folderHandle, fileHandle;
		WIN32_FIND_DATAA folderData, fileData;


	// zmienna okreslajaca czy znaleziono pozycje w folderze (plik lub folder)

		bool FoundItem = true;

	// przegladanie podfolderow

		// przejscie przez "." i ".." (current i parent directory)
		folderHandle = FindFirstFileA("TerrainData\\Models\\*.*", &folderData);
		FindNextFileA(folderHandle, &folderData);

		while( FoundItem )
		{
			FindNextFileA(folderHandle, &folderData);

			if ( folderData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
			// jezeli nazwa pozycji sie powtarza (jezeli natknieto sie na ostatni folder, bedzie sie on powtarzac)

				if( !folders.empty() && strcmp( folderData.cFileName, folders.back().c_str() ) == 0 )
				{
					FoundItem = false;
				}
				else
					folders.push_back(folderData.cFileName);
			}
		}


		for(int folder = 0; folder < folders.size(); folder++)
		{
			bool FoundFile;


		// string ze sciezka do podfolderu

			char path[MAX_PATH] = "";
			strcat_s( path, "TerrainData\\Models\\" );
			strcat_s( path, folders[folder].c_str() );

		// Ladowanie OBJ

			// otworzenie folderu i pobranie pierwszego pliku

				char copyPath[MAX_PATH] = "";
				strcpy_s( copyPath, path );

				fileHandle = FindFirstFileA(strcat( copyPath, "\\*.obj" ), &fileData);

			// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

				if( fileHandle != INVALID_HANDLE_VALUE )
				{
					std::string helper;

					helper.insert(0, path, 0, MAX_PATH);
					helper.append("\\");
					helper.append(fileData.cFileName);

					files.push_back(helper);


					FoundFile = true;

				// petla pobierajaca i zapisujaca nazwy plikow w folderze
					while( FoundFile )
					{
						FoundFile = FindNextFileA(fileHandle, &fileData);

						if( FoundFile )
						{
							std::string helper;

							helper.insert(0, path, 0, MAX_PATH);
							helper.append("\\");
							helper.append(fileData.cFileName);

							files.push_back(helper);
						}
					}
				}

		}// for lecacy przez wektor folderow

			

	// przygotowanie stringa ze sciezka do aplikacji

		int argCount;
	    LPWSTR *szArgList = CommandLineToArgvW((LPCWSTR)GetCommandLineA(), &argCount);

		std::string programFolder;
		programFolder.insert(0, (const char*)szArgList[0], 0, MAX_PATH);

		programFolder.erase( programFolder.rfind('\\')+1, std::string::npos );  // usuniecie nazwy aplikacji
		programFolder.erase( programFolder.begin() );							// usuniecie pierwszego znaku (cudzyslow)

	// licznik nowozaladowanych modeli

		int loadedModels = 0;

	// ladowanie tekstur na podstawie wektora nazw

		for(int i = 0; i < files.size(); i++)
		{
		// zmienna ktora bedzie przechowywac sciezke do pliku z tekstura

			char szFileName[MAX_PATH] = "";

		// przygotowanie nazwy pliku z tekstura

			strcpy_s( szFileName, programFolder.c_str() );
			strcat_s( szFileName, files[i].c_str() );


		// Ladowanie tekstury o nazwie szFileName

			if( loadModel(szFileName) )
			{
				loadedModels++;
			}
		}

		return loadedModels;
	}
	/*void ModelsOperator::reloadModels       ()
	{
		int newModels = loadModels();


		char buffer[MAX_PATH] = "";
		_itoa_s( newModels, buffer, 10 );

		if( newModels == 1 )
		{
			strcat_s( buffer, " new model loaded!" );
		}
		else
		{
			strcat_s( buffer, " new models loaded!" );
		}


		MessageBox(g_hwnd, buffer, "Reloading finished.", MB_OK);
	}*/

	//void ModelsOperator::changeModel        ()
	//{
	//	std::string cbIndexText;
	//	char buffer[MAX_PATH] = "";

	//	HWND comboBox = GetDlgItem(models, IDC_MODEL_COMBO);
	//	ComboBox_GetText(comboBox, buffer, MAX_PATH);

	//	cbIndexText.insert(0, buffer);

	//	if( !cbIndexText.empty() )
	//	{

	//		for(int i = 0; i < modelArray.size(); i++)
	//		{
	//			std::string helper;
	//			helper.insert(0, modelArray[i]->filename);

	//			bool trigger = false;
	//			std::string::size_type result;

	//			while( !trigger )
	//			{
	//				result = helper.find('\\');

	//				if(result != std::string::npos)
	//				{
	//					helper.erase(0, result+1);
	//				}
	//				else
	//				{
	//					result = helper.rfind('.');
	//					helper.erase(result, std::string::npos);

	//					trigger = true;
	//				}
	//			}


	//			if( cbIndexText == helper )
	//			{
	//				strcpy_s(currentModel, modelArray[i]->filename);

	//				break;
	//			}
	//		}

	//		ObjectOperator::Singleton().keyPressed = 'L';

	//	}// !cbIndexText.empty()
	//	else
	//	{
	//		HWND hPicture = GetDlgItem(models, IDC_MODEL_PIC);
	//		SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, NULL); 
	//	}
	//}

	void ModelsOperator::removeModelObjects ()
	{
		// Usuniecie VBO modeli.
		for(int i = 0; i < modelArray.size(); i++)
		{
			glDeleteBuffersARB( 1, &ModelsOperator::Singleton().modelArray[i]->vboIDv  );
			glDeleteBuffersARB( 1, &ModelsOperator::Singleton().modelArray[i]->vboIDvt );
			glDeleteBuffersARB( 1, &ModelsOperator::Singleton().modelArray[i]->vboIDvn );
		}
	}
//
/*****************************************************************
	Metody do pracy na teksturach modeli.
*****************************************************************/

	bool ModelsOperator::loadModelTexture    (char *filename)
	{
		bool originalTexture;


		if( modelTextureArray.empty() )
		{
			originalTexture = true;
		}
		else
		{
			originalTexture = true;

			for(int i = 0; i < modelTextureArray.size(); i++)
			{
				if( strcmp(filename, modelTextureArray[i].filename) == 0 )
				{
					originalTexture = false;
					break;
				}
			}
		}
		

		if( originalTexture )
		{
			try
			{
				LoadTexture(filename, TRUE, generateModelTexture);
			}
			catch(std::string error)
			{
				MessageBox(g_hwnd, error.c_str(), "Error!", MB_OK);
				return false;
			}

		// dodaj nowy obiekt tekstury + sciezke do wektora tekstur

			modelTexture_DataObject newModelTexture;

			strcpy_s(newModelTexture.filename, filename);
			newModelTexture.textureID = modelBrush;

			modelTextureArray.push_back(newModelTexture);

		// zerowanie brusha

			modelBrush = 0;
		}

		return originalTexture;
	}

	int  ModelsOperator::loadModelTextures   ()
	{
		// lista nazw plikow do zaladowania
			std::vector < std::string > folders, files;

		// uchwyt do folderu

			HANDLE directory, file;

		// struktura z danymi pliku/folderu

			WIN32_FIND_DATAA folderData, data;


		// zmienna okreslajaca czy znaleziono pozycje w folderze (plik lub folder)

			bool FoundItem = true;

		// przegladanie podfolderow

			// przejscie przez "." i ".." (current i parent directory)
			directory = FindFirstFileA("TerrainData\\Models\\*.*", &folderData);
			FindNextFileA(directory, &folderData);

			while( FoundItem )
			{
				FindNextFileA(directory, &folderData);

				if ( folderData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
				// jezeli nazwa pozycji sie powtarza (jezeli natknieta sie na ostatni folder, bedzie sie on powtarzac)

					if( !folders.empty() && strcmp( folderData.cFileName, folders.back().c_str() ) == 0 )
						FoundItem = false;
					else
						folders.push_back(folderData.cFileName);
				}
			}


			for(int folder = 0; folder < folders.size(); folder++)
			{

			// bool okreslajacy czy znaleziono plik

				bool FoundFile;


			// string ze sciezka do podfolderu

				char path[MAX_PATH] = "";
				strcat_s( path, "TerrainData\\Models\\" );
				strcat_s( path, folders[folder].c_str() );
	
			// Ladowanie BMP

				// otworzenie folderu i pobranie pierwszego pliku

					char copyPath[MAX_PATH] = "";
					strcpy_s( copyPath, path );

					file = FindFirstFileA(strcat( copyPath, "\\*.bmp" ), &data);

				// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

					if( file != INVALID_HANDLE_VALUE )
					{
						std::string helper;

						helper.insert(0, path, 0, MAX_PATH);
						helper.append("\\");
						helper.append(data.cFileName);

						files.push_back(helper);


						FoundFile = true;

					// petla pobierajaca i zapisujaca nazwy plikow w folderze
						while( FoundFile )
						{
							FoundFile = FindNextFileA(file, &data);

							if( FoundFile )
							{
								std::string helper;

								helper.insert(0, path, 0, MAX_PATH);
								helper.append("\\");
								helper.append(data.cFileName);

								files.push_back(helper);
							}
						}
					}

			// Ladowanie JPG

				// otworzenie folderu i pobranie pierwszego pliku

					strcat_s( copyPath, "" );
					strcpy_s( copyPath, path );
					strcat_s( copyPath, "\\*.jpg" );

					file = FindFirstFileA( copyPath, &data);

				// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

					if( file != INVALID_HANDLE_VALUE )
					{
						std::string helper;

						helper.insert(0, path, 0, MAX_PATH);
						helper.append("\\");
						helper.append(data.cFileName);

						files.push_back(helper);


						FoundFile = true;

					// petla pobierajaca i zapisujaca nazwy plikow w folderze
						while( FoundFile )
						{
							FoundFile = FindNextFileA(file, &data);

							if( FoundFile )
							{
								std::string helper;

								helper.insert(0, path, 0, MAX_PATH);
								helper.append("\\");
								helper.append(data.cFileName);

								files.push_back(helper);
							}
						}
					}

			// Ladowanie TGA

				// otworzenie folderu i pobranie pierwszego pliku

					strcat_s( copyPath, "" );
					strcpy_s( copyPath, path );

					file = FindFirstFileA(strcat( copyPath, "\\*.tga" ), &data);

				// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

					if( file != INVALID_HANDLE_VALUE )
					{
						std::string helper;

						helper.insert(0, path, 0, MAX_PATH);
						helper.append("\\");
						helper.append(data.cFileName);

						files.push_back(helper);


						FoundFile = true;

					// petla pobierajaca i zapisujaca nazwy plikow w folderze
						while( FoundFile )
						{
							FoundFile = FindNextFileA(file, &data);

							if( FoundFile )
							{
								std::string helper;

								helper.insert(0, path, 0, MAX_PATH);
								helper.append("\\");
								helper.append(data.cFileName);

								files.push_back(helper);
							}
						}
					}

			}// for lecacy przez wektor folderow


		// przygotowanie stringa ze sciezka do aplikacji

			int argCount;
		    LPWSTR *szArgList = CommandLineToArgvW((LPCWSTR)GetCommandLineA(), &argCount);

			std::string programFolder;
			programFolder.insert(0, (const char*)szArgList[0], 0, MAX_PATH);

			programFolder.erase( programFolder.rfind('\\')+1, std::string::npos );  // usuniecie nazwy aplikacji
			programFolder.erase( programFolder.begin() );							// usuniecie pierwszego znaku (cudzyslow)

		// licznik zaladowanych tekstur

			int loadedTextureCounter = 0;

		// ladowanie tekstur na podstawie wektora nazw

			for(int i = 0; i < files.size(); i++)
			{
			// zmienna ktora bedzie przechowywac sciezke do pliku z tekstura

				char szFileName[MAX_PATH] = "";

			// przygotowanie nazwy pliku z tekstura

				strcpy_s( szFileName, programFolder.c_str() );
				strcat_s( szFileName, files[i].c_str() );


			// Ladowanie tekstury o nazwie szFileName

				if( loadModelTexture(szFileName) )
					loadedTextureCounter++;
			}

			return loadedTextureCounter;
	}

	//void ModelsOperator::reloadModelTextures ()
	//{
	//	int newTextures = loadModelTextures();

	//	char buffer[MAX_PATH] = "";

	//	_itoa_s( newTextures, buffer, 10 );
	//	strcat_s( buffer, " new textures loaded!" );

	//	MessageBox(g_hwnd, buffer, "Reloading finished.", MB_OK);
	//}

	//void ModelsOperator::changeModelTexture  ()
	//{

	//		std::string cbIndexText;
	//		char buffer[MAX_PATH] = "";

	//		HWND comboBox = GetDlgItem(models, IDC_MODEL_TEXTURE_COMBO);
	//		ComboBox_GetText(comboBox, buffer, MAX_PATH);

	//		cbIndexText.insert(0, buffer);

	//		if( !cbIndexText.empty() )
	//		{

	//			for(int i = 0; i < modelTextureArray.size(); i++)
	//			{
	//				std::string helper;
	//				helper.insert(0, modelTextureArray[i].filename);

	//				bool trigger = false;
	//				std::string::size_type result;

	//				while( !trigger )
	//				{
	//					result = helper.find('\\');

	//					if(result != std::string::npos)
	//					{
	//						helper.erase(0, result+1);
	//					}
	//					else
	//					{
	//						result = helper.rfind('.');
	//						helper.erase(result, std::string::npos);

	//						trigger = true;
	//					}
	//				}


	//				if( cbIndexText == helper )
	//				{
	//					modelBrush = modelTextureArray[i].textureID;

	//					// ustaw obrazek ...

	//						HBITMAP hBitmap;
	//						hBitmap = (HBITMAP)LoadImage(NULL, modelTextureArray[i].filename, IMAGE_BITMAP, 128, 128,
	//						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	//						HWND hPicture = GetDlgItem(models, IDC_MODEL_PIC);
	//						SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); 

	//					break;
	//				}
	//			}

	//			ObjectOperator::Singleton().keyPressed = 'L';

	//		}// !cbIndexText.empty()
	//		else
	//		{
	//			HWND hPicture = GetDlgItem(models, IDC_MODEL_PIC);
	//			SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, NULL); 
	//		}

	//}

	void ModelsOperator::removeModelTextures ()
	{
		for(int i = 0; i < modelTextureArray.size(); i++)
		{
			glDeleteTextures(1, &modelTextureArray[i].textureID);
		}
	}
//
/*****************************************************************
	Metody do zapisu / odczytu modeli.
*****************************************************************/

	//void ModelsOperator::saveModels(char *filename)
	//{
	//	std::fstream file;

	//	file.open( filename, std::ios::out | std::ios::app | std::ios::binary );


	//// ustawienie markera poczatkowego

	//	file << "\nModels\n";

	//// zapis licznika modeli

	//	file << "\t" << ModelsOperator::Singleton().modelCounter << "\n";

	//// zapisywanie danych poszczegolnych modeli

	//	int counter = 0;

	//	for(int obj = 0; obj < ObjectOperator::Singleton().objectArray.size(); obj++)
	//	{
	//		if( ObjectOperator::Singleton().objectArray[obj].GetType() == MODEL_NAME )
	//		{
	//			Model *model = reinterpret_cast <Model*> ( ObjectOperator::Singleton().objectArray[obj].GetObject() );

	//		// zapis pozycji i rozmiarow obiektu modelu

	//			Vector3 position = ObjectOperator::Singleton().objectArray[obj].GetPosition(), 
	//		            size     = ObjectOperator::Singleton().objectArray[obj].GetSize();

	//			file << "\t" << position.posX << " "
	//						 << position.posY << " "
	//						 << position.posZ << "\n";

	//			file << "\t" << size.posX << " "
	//						 << size.posY << " "
	//						 << size.posZ << "\n";

	//		// na podstawie ID VBO wierzcholkow, odnalezienie nazwy modelu i zapisanie jej

	//			bool ModelFound = false;

	//			for(int i = 0; i < ModelsOperator::Singleton().modelArray.size(); i++)
	//			{
	//				if( model->vboIDv == ModelsOperator::Singleton().modelArray[i]->vboIDv )
	//				{
	//					file << "\t" << StripName( ModelsOperator::Singleton().modelArray[i]->filename, false ) << "\n";
	//					ModelFound = true;
	//					break;
	//				}
	//			}

	//			assert( ModelFound );

	//		// zapis kolorow modelu

	//			file << "\t" << model->R << " "
	//						 << model->G << " "
	//						 << model->B << " "
	//						 << model->A << "\n";

	//		// zapis skali

	//			file << "\t" << model->scale << "\n";

	//		// zapis wartosci rotacji wokol osi X, Y, Z 

	//			file << "\t" << model->rotateX << " "
	//				         << model->rotateY << " "
	//						 << model->rotateZ << "\n";

	//	    // zapis flagi okreslajacej, czy model jest przeszkoda

	//			file << "\t" << model->IsObstacle << "\n";

	//		// inkrementacja licznika znalezionych obiektow modeli

	//			counter++;
	//		}
	//	}


	//	assert( counter == ModelsOperator::Singleton().modelCounter );

	//	// ustawienie markera koncowego
	//	file << "End\n\n";


	//	file.close();
	//	file.clear();
	//}

	bool ModelsOperator::loadModels(char *filename)
	{
		std::fstream      file;

		file.open( filename, std::ios::in | std::ios::binary );

		if( !file )
		{
			return false;
		}


	// podstawowe flagi informujace o stanie loader'a

		bool markerFound      = false, 
			 loadingCommenced = false,
			 loadingCorrect   = true;


		char initialErrorInfo[MAX_PATH];

		strcpy_s( initialErrorInfo, "Save file \"" );
		strcat_s( initialErrorInfo, filename );
		strcat_s( initialErrorInfo, "\" is corrupted!\n\n" );

		std::string errorInfo = initialErrorInfo;
		std::string line;

	// ladowanie danych z pliku

		while( !file.eof() )
		{
			file >> line;

			if( line == "Models" )
			{
				if( !markerFound )
				{
					markerFound = true;
				}
				else
				{
					loadingCorrect = false;
					errorInfo += "There are two or more models info markers (should be one).";
					break;
				}
			}

			if( markerFound && !loadingCommenced )
			{
				loadingCommenced = true;

			// pobranie licznika modeli

				file >> line;

				if( line == "End"  )
				{
					loadingCorrect = false;
					errorInfo += "\"End\" marker found too early, while loading Models color values.";
					break;
				}
				else if( !IsNumber(line, true) )
				{
					loadingCorrect = false;
					errorInfo += "Word where modelCounter value should be, isn't a number!\n\n";
					errorInfo += "\"";
					errorInfo += line;
					errorInfo += "\"";
					break;
				}
				else
				{
					ModelsOperator::Singleton().modelCounter = atoi(line.c_str());
				}

			// ladowanie modeli

				for(int m = 0; m < ModelsOperator::Singleton().modelCounter; m++)
				{

				// pobranie pozycji i rozmiarow obiektu modelu

					Vector3 position, size;

					for(int i = 0; i < 6; i++)
					{
						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading Models values.";
							break;
						}
						else if( !IsNumber(line, false) )
						{
							loadingCorrect = false;
							errorInfo += "Word where Models value should be, isn't a number!\n\n";
							errorInfo += "\"";
							errorInfo += line;
							errorInfo += "\"";
							break;
						}
						else
						{
							switch( i )
							{
								case 0:
									position.x = atof(line.c_str());
								break;

								case 1:
									position.y = atof(line.c_str());
								break;

								case 2:
									position.z = atof(line.c_str());
								break;

								case 3:
									size.x = atof(line.c_str());
								break;

								case 4:
									size.y = atof(line.c_str());
								break;

								case 5:
									size.z = atof(line.c_str());
								break;
							}
						}
					}

				// utworzenie nowego modelu i wypelnienie go danymi z pliku

					Model *newModel = new Model;

				// pobranie nazwy modelu

					file >> line;

					if( line == "End"  )
					{
						loadingCorrect = false;
						errorInfo += "\"End\" marker found too early, while loading Model name.";
						break;
					}
					else
					{
					// odnalezienie nazwy modelu w rejestrze

						bool ModelFound = false;

						for(int i = 0; ModelsOperator::Singleton().modelArray.size(); i++)
						{
						// pobranie identyfikatorow do VBO oraz licznikow indeksow tablic

							if( line == StripName( ModelsOperator::Singleton().modelArray[i]->filename, false ) )
							{
								newModel->vboIDv  = ModelsOperator::Singleton().modelArray[i]->vboIDv;
								newModel->vboIDvt = ModelsOperator::Singleton().modelArray[i]->vboIDvt;
								newModel->vboIDvn = ModelsOperator::Singleton().modelArray[i]->vboIDvn;

								newModel->sizeX = ModelsOperator::Singleton().modelArray[i]->sizeX;
								newModel->sizeY = ModelsOperator::Singleton().modelArray[i]->sizeY;
								newModel->sizeZ = ModelsOperator::Singleton().modelArray[i]->sizeZ;

								newModel->meshVector = ModelsOperator::Singleton().modelArray[i]->meshVector;

								ModelFound = true;
								break;
							}
						}

						if( !ModelFound )
						{
							loadingCorrect = false;

							char buffer[MAX_PATH];

							strcpy_s( buffer, "Model name hasn't been found in modelArray register: " ); 
							strcat_s( buffer, line.c_str() );

							errorInfo += buffer;
							break;
						}
					}

				// pobranie wartosci float modelu

					for(int i = 0; i < 5; i++)
					{
						file >> line;
						
						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading Models values.";
							break;
						}
						else if( !IsNumber(line, false) )
						{
							loadingCorrect = false;
							errorInfo += "Word where Models value should be, isn't a number!\n\n";
							errorInfo += "\"";
							errorInfo += line;
							errorInfo += "\"";
							break;
						}
						else
						{
							switch( i )
							{
								case 0:
									newModel->R = atof(line.c_str());
								break;

								case 1:
									newModel->G = atof(line.c_str());
								break;

								case 2:
									newModel->B = atof(line.c_str());
								break;

								case 3:
									newModel->A = atof(line.c_str());
								break;

								case 4:
									newModel->scale = atof(line.c_str());
								break;
							}
						}
					}

				// pobranie wartosci integer (i jednego bool'a) modelu

					for(int i = 0; i < 4; i++)
					{
						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading Models values.";
							break;
						}
						else if( !IsNumber(line, true) )
						{
							loadingCorrect = false;
							errorInfo += "Word where Models value should be, isn't a number!\n\n";
							errorInfo += "\"";
							errorInfo += line;
							errorInfo += "\"";
							break;
						}
						else
						{
							switch( i )
							{
								case 0:
									newModel->rotateX = atoi(line.c_str());
								break;

								case 1:
									newModel->rotateY = atoi(line.c_str());
								break;

								case 2:
									newModel->rotateZ = atoi(line.c_str());
								break;

								case 3:
									newModel->IsObstacle = atoi(line.c_str());
								break;
							}
						}
					}

				// dodanie modelu do wektora obiektow

					ObjectOperator::Object newObject(position.x, position.y, position.z,
													 size.x,     size.y,     size.z,

													 newModel,
													 preModel,
													 drawModel,
													 postModel,
													 NULL,
													 removeModel,
													 NULL,
													 NULL,
													 NULL,
													 MODEL_NAME,
													 FALSE,
													 FALSE,
													 TRUE);

					ObjectOperator::Singleton().Add(newObject);
				}
			
			}// if( markerFound && !loadingCommenced )
		}

	// zamkniecie i wyczyszczenie strumienia

		file.close();
		file.clear();


		if( !markerFound )
		{
			loadingCorrect = false;
			errorInfo += "\"Models\" marker hasn't been found!";
		}


		return loadingCorrect;
	}
//
/*****************************************************************
	Konstruktor, destruktor ModelsOperator
*****************************************************************/

	ModelsOperator::ModelsOperator(): models(NULL), modelBrush(0), modelCounter(0), ObstacleCheck(false)
	{
		strcpy_s(currentModel, "");
	}

	ModelsOperator::~ModelsOperator()
	{
		removeModelObjects();
		removeModelTextures();
	}



