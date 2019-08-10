#include "GlobalHeaders.h"

#include "Clouds.h"

// Zmienne, stale zewnetrzne

	extern  const int 	OVERALL_MIN_HEIGHT,
						OVERALL_MAX_HEIGHT;

	extern  int		    MAP_X, MAP_Z, MAP_SCALE;


	extern  char terrainKey;
	extern  bool toolBoxReady,
				 enableLighting,

				 showCloud;

	extern  HWND g_hwnd;

	// syf do VBO
		
		extern  PFNGLGENBUFFERSARBPROC	         glGenBuffersARB;           // VBO Name Generation Procedure
		extern  PFNGLBINDBUFFERARBPROC			 glBindBufferARB;           // VBO Bind Procedure
		extern  PFNGLBUFFERDATAARBPROC			 glBufferDataARB;           // VBO Data Loading Procedure
		extern  PFNGLBUFFERSUBDATAARBPROC		 glBufferSubDataARB;        // VBO Sub Data Loading Procedure
		extern  PFNGLDELETEBUFFERSARBPROC		 glDeleteBuffersARB;        // VBO Deletion Procedure
		extern  PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB; // return various parameters of VBO
		extern  PFNGLMAPBUFFERARBPROC			 glMapBufferARB;            // map VBO procedure
		extern  PFNGLUNMAPBUFFERARBPROC			 glUnmapBufferARB;          // unmap VBO procedure



// Zmienne lokalne

	HWND cloud = NULL;

	unsigned  int cloudBrush = 0,	// "pedzel" chmur (id tekstury)
				 
				  cloudVBOv  = 0,	// VBO chmur
				  cloudVBOvt = 0,
				  cloudVBOvc = 0;

	float		  cloudR, cloudG, cloudB, cloudA,	// wartosci koloru chmur

		          cloudHeight = 150.f,				// wysokosc chmur

				  textureSpread = 1.f,				// okresla rozciagniecie tekstury chmur
				  textureCurrentSpreadX = 1.f,
				  textureCurrentSpreadZ = 1.f,

				  textureProgressX = 0.f,
				  textureProgressZ = 0.f,

				  textureSpeed = 1.f/100000.f;

	int			  directionX = 1,					// kierunek ruchu chmur
		          directionZ = 1;					// wzdluz osi X, Z



	// do zapisu / odczytu poszczegolnych tekstur
		struct cloudTextureObj
		{
			char	        filename[MAX_PATH];
			unsigned int    textureID;
		};

	std::vector <cloudTextureObj> cloudTextureArray;



// Definicje funkcji


// ------------------------------

	void generateCloud()
	{

		// pomocnicy (identyczni jak w generateCubemap)

			float minX = -MAP_X*MAP_SCALE*2.5f,
				  maxX =  MAP_X*MAP_SCALE*5.f,
				  
				  minZ = -MAP_Z*MAP_SCALE*5.f,
				  maxZ =  MAP_Z*MAP_SCALE*2.5f,

				  CloudVertices[4][3] = { minX, 0.f, minZ, 
										  minX, 0.f, maxZ, 
										  maxX, 0.f, maxZ, 
										  maxX, 0.f, minZ },

				  CloudTexCoords[4][2] = { 0.f,			  0.f,
										   0.f,			  textureSpread,
										   textureSpread, textureSpread,
										   textureSpread, 0.f },

				  CloudColors[4][4];

		// ustawienie aktualnego rozciagniecia tekstury chmur

			textureCurrentSpreadX = textureSpread;
			textureCurrentSpreadZ = textureSpread;

		// pobranie kolorow z kontrolek

			cloudR = (float)GetDlgItemInt(cloud, IDC_CLOUD_RED,   FALSE, FALSE) / 255.f;
			cloudG = (float)GetDlgItemInt(cloud, IDC_CLOUD_GREEN, FALSE, FALSE) / 255.f;
			cloudB = (float)GetDlgItemInt(cloud, IDC_CLOUD_BLUE,  FALSE, FALSE) / 255.f;
			cloudA = (float)GetDlgItemInt(cloud, IDC_CLOUD_ALPHA, FALSE, FALSE) / 255.f;

		// wypelnienie tablicy kolorow powyzszymi wartosciami

			for(int i = 0; i < 4; i++)
			{
				CloudColors[i][0] = cloudR;
				CloudColors[i][1] = cloudG;
				CloudColors[i][2] = cloudB;
				CloudColors[i][3] = cloudA;
			}

		// generowanie VBO cubemap

			glGenBuffersARB(1, &cloudVBOv);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, cloudVBOv);

			glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*3*sizeof(float), CloudVertices, GL_STATIC_DRAW_ARB);


			glGenBuffersARB(1, &cloudVBOvt);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, cloudVBOvt);

			glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*2*sizeof(float), CloudTexCoords, GL_DYNAMIC_DRAW_ARB);


			glGenBuffersARB(1, &cloudVBOvc);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, cloudVBOvc);

			glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*4*sizeof(float), CloudColors, GL_STATIC_DRAW_ARB);
	

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	}



	void changeCloudTexCoords()
	{
		// czyszczenie VBO
			
			if( cloudVBOvt != 0 )
			{
				glDeleteBuffersARB(1, &cloudVBOvt);
			}

		// "przesuniecie" tekstury

			float textureBeginningX = textureCurrentSpreadX - textureSpread,
				  textureBeginningZ = textureCurrentSpreadZ - textureSpread;
				  

			if( directionX )
			{
				textureCurrentSpreadX += (textureSpeed * directionX);

				if( textureCurrentSpreadX == (textureSpread*2)-1 )
					textureCurrentSpreadX = textureSpread;
				else if( textureCurrentSpreadX == -(textureSpread*2)-1 )
					textureCurrentSpreadX = textureSpread;
			}

			if( directionZ )
			{
				textureCurrentSpreadZ += (textureSpeed * directionZ);

				if( textureCurrentSpreadZ == (textureSpread*2)-1 )
					textureCurrentSpreadZ = textureSpread;
				else if( textureCurrentSpreadZ == -(textureSpread*2)-1 )
					textureCurrentSpreadZ = textureSpread;
			}

		// generowanie nowego VBO tex coord'ow chmur

			float  CloudTexCoords[4][2] = { textureBeginningX,     textureBeginningZ,
										    textureBeginningX,	   textureCurrentSpreadZ,
										    textureCurrentSpreadX, textureCurrentSpreadZ,
										    textureCurrentSpreadX, textureBeginningZ };

			glGenBuffersARB(1, &cloudVBOvt);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, cloudVBOvt);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*2*sizeof(float), CloudTexCoords, GL_DYNAMIC_DRAW_ARB);

			
			glGenBuffersARB(1, 0);
	}


	void changeTextureSpread()
	{
		textureSpread = GetDlgItemInt(cloud, IDC_CLOUD_SPREAD, FALSE, FALSE);
		changeCloudTexCoords();
	}
	void changeCloudColors()
	{

			if( cloudVBOvc != 0 )
				glDeleteBuffersARB(1, &cloudVBOvc);

			float CloudColors[4][4];

		// pobranie kolorow z kontrolek

			cloudR = (float)GetDlgItemInt(cloud, IDC_CLOUD_RED,   FALSE, FALSE) / 255.f;
			cloudG = (float)GetDlgItemInt(cloud, IDC_CLOUD_GREEN, FALSE, FALSE) / 255.f;
			cloudB = (float)GetDlgItemInt(cloud, IDC_CLOUD_BLUE,  FALSE, FALSE) / 255.f;
			cloudA = (float)GetDlgItemInt(cloud, IDC_CLOUD_ALPHA, FALSE, FALSE) / 255.f;

		// wypelnienie tablicy kolorow powyzszymi wartosciami

			for(int i = 0; i < 4; i++)
			{
				CloudColors[i][0] = cloudR;
				CloudColors[i][1] = cloudG;
				CloudColors[i][2] = cloudB;
				CloudColors[i][3] = cloudA;
			}

		// wygenerowanie VBO kolorow dla cubemap

			glGenBuffersARB(1, &cloudVBOvc);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, cloudVBOvc);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, 4*4*sizeof(float), CloudColors, GL_STATIC_DRAW_ARB);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
	void changeCloudHeight()
	{
		cloudHeight = GetDlgItemInt(cloud, IDC_CLOUD_HEIGHT, FALSE, FALSE) * MAP_SCALE;
	}

	void changeCloudSpeed()
	{
		textureSpeed = (float)GetDlgItemInt(cloud, IDC_CLOUD_SPEED, FALSE, FALSE) / 100000.f;
	}

	void changeCloudDirection()
	{
		directionX = SendMessageA(GetDlgItem(cloud, IDC_CLOUD_X_DIR_SLIDER), TBM_GETPOS, 0, 0);
		directionZ = SendMessageA(GetDlgItem(cloud, IDC_CLOUD_Z_DIR_SLIDER), TBM_GETPOS, 0, 0);

		directionX -= 1;
		directionZ -= 1;
	}

	void updateCloud()
	{
		changeTextureSpread();
		changeCloudColors();
		changeCloudHeight();
		changeCloudSpeed();
		changeCloudDirection();
	}
	void drawCloud()
	{
	// terrainKey jest ustawiane, jezeli zaszla jakas zmiana w kontrolkach, w zakladce "Terrain"

		if( terrainKey )
		{
			cleanCloud();
			generateCloud();

			terrainKey = NULL;
		}

		if( showCloud )
		{
			glEnable(GL_TEXTURE_2D);

			glEnable(GL_BLEND);
			glDepthMask(GL_FALSE);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


			glDisable(GL_CULL_FACE);

			if( enableLighting )
				glDisable(GL_LIGHTING);

				
				glBindTexture(GL_TEXTURE_2D, cloudBrush);


				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cloudVBOv);
				glVertexPointer(3, GL_FLOAT, 0, 0);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cloudVBOvt);
				glTexCoordPointer(2, GL_FLOAT, 0, 0);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cloudVBOvc);
				glColorPointer(4, GL_FLOAT, 0, 0);


				glPushMatrix();

					glTranslatef(0.f, cloudHeight, 0.f);
					glDrawArrays(GL_QUADS, 0, 4);

				glPopMatrix();


				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);


				glBindTexture(GL_TEXTURE_2D, 0);
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


			if( enableLighting )
				glEnable(GL_LIGHTING);


			glEnable(GL_CULL_FACE);
			glDepthMask(GL_TRUE);
			glDisable(GL_TEXTURE_2D);


		// zmiana koordynatow tekstur chmur (jezeli sie poruszaja)

			if( textureSpeed && ( directionX || directionZ ) )
			{
				changeCloudTexCoords();
			}

		}// if( showCloud )
	}

	void cleanCloud()
	{
		// czyszczenie VBO chmur

			if( cloudVBOv != 0 )
				glDeleteBuffersARB(1, &cloudVBOv);

			if( cloudVBOvt != 0 )
				glDeleteBuffersARB(1, &cloudVBOvt);

			if( cloudVBOvc != 0 )
				glDeleteBuffersARB(1, &cloudVBOvc);

		// zerowanie identyfikatorow

			cloudVBOv = cloudVBOvt = cloudVBOvc = 0;
	}

//
// ------------------------------

	unsigned int generateCloudTexture(unsigned char *bits, unsigned int &width, unsigned int &height)
	{

		//wygeneruj nowy obiekt tekstury

			glGenTextures(1, &cloudBrush);
			glBindTexture(GL_TEXTURE_2D, cloudBrush);
		
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

			// Don't forget to use GL_RGBA for our new image data... we support Alpha transparency now!
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
						  GL_RGBA, GL_UNSIGNED_BYTE, bits );


			glBindTexture(GL_TEXTURE_2D, 0);


			return cloudBrush;
	}



	bool loadCloudTexture(char *filename)
	{
			bool originalTexture;   //nie ma sensu generowac "nowej" tekstury

		// sprawdzenie "oryginalnosci" tekstury

			if( cloudTextureArray.empty() )
				originalTexture = true;
			else
			{
				originalTexture = true;

				for(int i = 0; i < cloudTextureArray.size(); i++)
				{
					if( strcmp(filename, cloudTextureArray[i].filename) == 0 )
					{
						originalTexture = false;
						break;
					}
				}
			}
			
		// nowa tekstura? no to dodajemy...

			if( originalTexture )
			{
				
			// ladowanie i generacja tekstury

				try
				{
					LoadTexture(filename, TRUE, generateCloudTexture);
				}
				catch(std::string error)
				{
					MessageBox(g_hwnd, error.c_str(), "Error!", MB_OK);
					return false;
				}


			// dodaj nowy obiekt tekstury + sciezke do wektora tekstur

				cloudTextureObj newCloudTexture;
				strcpy_s(newCloudTexture.filename, filename);
				newCloudTexture.textureID = cloudBrush;

				cloudTextureArray.push_back(newCloudTexture);


			// dodaj nowa pozycje do IDC_DETAIL_COMBO

				std::string newItemName;
				newItemName.insert(0, filename, 0, MAX_PATH);

				bool trigger = false;
				std::string::size_type result;

				while(!trigger)
				{
					result = newItemName.find('\\');

					if(result != std::string::npos)
					{
						newItemName.erase(0, result+1);
					}
					else
					{
						result = newItemName.rfind('.');
						newItemName.erase(result, std::string::npos);

						trigger = true;
					}
				}

			// dodanie do combobox'a

				HWND comboBox = GetDlgItem(cloud, IDC_CLOUD_COMBO);

				ComboBox_AddString(comboBox, newItemName.c_str());

			// wyzerowanie brusha

				cloudBrush = 0;

			}


			return originalTexture;
	}



	int  loadCloudTextures()
	{

		// lista nazw plikow do zaladowania
			std::vector < std::string > files;

		// uchwyt do folderu

			HANDLE directory;

		// struktura z danymi pliku/folderu

			WIN32_FIND_DATAA data;

		// bool okreslajacy czy znaleziono plik

			bool FoundFile;

		// Ladowanie BMP

			// otworzenie folderu i pobranie pierwszego pliku

				directory = FindFirstFileA("TerrainData\\Textures\\Clouds\\*.bmp", &data);

			// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

				if( directory != INVALID_HANDLE_VALUE )
				{
					files.push_back(data.cFileName);

					FoundFile = true;

				// petla pobierajaca i zapisujaca nazwy plikow w folderze
					while( FoundFile )
					{
						FoundFile = FindNextFileA(directory, &data);

						if( FoundFile )
							files.push_back(data.cFileName);
					}
				}

		// Ladowanie JPG

			// otworzenie folderu i pobranie pierwszego pliku

				directory = FindFirstFileA("TerrainData\\Textures\\Clouds\\*.jpg", &data);

			// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

				if( directory != INVALID_HANDLE_VALUE )
				{
					files.push_back(data.cFileName);

					FoundFile = true;

				// petla pobierajaca i zapisujaca nazwy plikow w folderze
					while( FoundFile )
					{
						FoundFile = FindNextFileA(directory, &data);

						if( FoundFile )
							files.push_back(data.cFileName);
					}
				}

		// Ladowanie TGA

			// otworzenie folderu i pobranie pierwszego pliku

				directory = FindFirstFileA("TerrainData\\Textures\\Clouds\\*.TGA", &data);

			// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

				if( directory != INVALID_HANDLE_VALUE )
				{
					files.push_back(data.cFileName);

					FoundFile = true;

				// petla pobierajaca i zapisujaca nazwy plikow w folderze
					while( FoundFile )
					{
						FoundFile = FindNextFileA(directory, &data);

						if( FoundFile )
							files.push_back(data.cFileName);
					}
				}
				

		// przygotowanie stringa z sciezka do aplikacji

			int argCount;
		    LPWSTR *szArgList = CommandLineToArgvW((LPCWSTR)GetCommandLineA(), &argCount);

			std::string programFolder;
			programFolder.insert(0, (const char*)szArgList[0], 0, MAX_PATH);

			programFolder.erase( programFolder.rfind('\\')+1, std::string::npos );  // usuniecie nazwy aplikacji
			programFolder.erase( programFolder.begin() );							// usuniecie pierwszego znaku (cudzyslow)

		// ladowanie tekstur na podstawie wektora nazw

			int counter = 0;

			for(int i = 0; i < files.size(); i++)
			{

			// zmienna ktora bedzie przechowywac sciezke do pliku z tekstura

				char szFileName[MAX_PATH] = "";

			// przygotowanie nazwy pliku z tekstura

				strcpy_s( szFileName, programFolder.c_str() );
				strcat_s( szFileName, "TerrainData\\Textures\\Clouds\\" );
				strcat_s( szFileName, files[i].c_str() );


			// Ladowanie tekstury o nazwie szFileName

				if( loadCloudTexture(szFileName) )
				{
					counter++;
				}
			}

			return counter++;
	}


	void reloadCloudTextures()
	{
		int newTextures = loadCloudTextures();

		char buffer[MAX_PATH] = "";

		_itoa_s( newTextures, buffer, 10 );

		if( newTextures == 1 )
			strcat_s( buffer, " new texture loaded!" );
		else
			strcat_s( buffer, " new textures loaded!" );

		MessageBox(g_hwnd, buffer, "Reloading finished.", MB_OK);
	}
	void changeCloudTexture()
	{
		
			std::string cbIndexText;
			char buffer[MAX_PATH] = "";

			HWND comboBox = GetDlgItem(cloud, IDC_CLOUD_COMBO);
			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			cbIndexText.insert(0, buffer);

			if( !cbIndexText.empty() )
			{
				for(int i = 0; i < cloudTextureArray.size(); i++)
				{
					std::string helper;
					helper.insert(0, cloudTextureArray[i].filename);

					bool trigger = false;
					std::string::size_type result;

					while( !trigger )
					{
						result = helper.find('\\');

						if(result != std::string::npos)
						{
							helper.erase(0, result+1);
						}
						else
						{
							result = helper.rfind('.');
							helper.erase(result, std::string::npos);

							trigger = true;
						}
					}

					if( cbIndexText == helper )
					{
						cloudBrush = cloudTextureArray[i].textureID;

						// ustaw obrazek ...
						HBITMAP hBitmap;
						hBitmap = (HBITMAP)LoadImage(NULL, cloudTextureArray[i].filename, IMAGE_BITMAP, 196, 128,
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

						HWND hPicture = GetDlgItem(cloud, IDC_CLOUD);
						SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); 

						break;
					}
				}


			}// !cbIndexText.empty()
			else
			{
				HWND hPicture = GetDlgItem(cloud, IDC_CLOUD);
				SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, NULL); 
			}

	}



	void removeCloudTextures()
	{
		for(int i = 0; i < cloudTextureArray.size(); i++)
		{
			glDeleteTextures(1, &cloudTextureArray[i].textureID);
		}
	}

//
// ------------------------------

	void saveCloud(char *filename)
	{
			std::fstream file;

		// otworzenie pliku w trybie "nadpisz" + binarny

			file.open( filename, std::ios::out | std::ios::app | std::ios::binary );

		// ustawienie markera poczatkowego

			file << "\nClouds\n";

		// zapisanie tekstury chmur

			if( cloudBrush )
			{
				bool TextureFound = false;

				for(int i = 0; i < cloudTextureArray.size(); i++)
				{
					if( cloudBrush == cloudTextureArray[i].textureID )
					{
						file << "\t" << StripName( cloudTextureArray[i].filename, false ) << "\n";
						TextureFound = true;
						break;
					}
				}

				assert( TextureFound );
			}
			else
			{
				file << "\tNone\n";
			}

		// zapisanie koloru chmur

			file << "\t" << cloudR << " " 
				         << cloudG << " "
						 << cloudB << " "
						 << cloudA << "\n";

		// zapisanie wysokosci chmur

			file << "\t" << cloudHeight << "\n";

		// zapisanie wartosci okreslajacych teksturowanie chmur

			file << "\t" <<  textureSpread		    << "\n"
				 << "\t" <<  textureSpeed			<< "\n"
				 << "\t" <<  directionX				<< "\n"
				 << "\t" <<  directionZ				<< "\n";

		// ustawienie markera koncowego

			file << "End\n\n";

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();
	}

	bool loadCloud(char *filename)
	{
			std::fstream      file;

		// otworzenie pliku w trybie binarnym

			file.open( filename, std::ios::in | std::ios::binary );

			if( !file )
			{
				return false;
			}

		// podstawowe flagi informujace o stanie loader'a

			bool markerFound      = false, 
				 loadingCommenced = false,
				 loadingCorrect   = true;


			char buffer[MAX_PATH];

			strcpy_s( buffer, "Save file \"" );
			strcat_s( buffer, filename );
			strcat_s( buffer, "\" is corrupted!\n\n" );

			std::string errorInfo = buffer;

			std::string line;

		// ladowanie danych z pliku

			while( !file.eof() )
			{
				file >> line;

				if( line == "Clouds" )
				{
					if( !markerFound )
					{
						markerFound = true;
					}
					else
					{
						loadingCorrect = false;
						errorInfo += "There are two or more cloud info markers (should be one).";
						break;
					}
				}

				if( markerFound && !loadingCommenced )
				{
				// ustawienie falgi

					loadingCommenced = true;

				// pobranie tekstury chmur

					file >> line;

					if( line == "End"  )
					{
						loadingCorrect = false;
						errorInfo += "\"End\" marker found too early, while loading Cloud values.";
						break;
					}
					else
					{
						if( line == "None" )
						{
							cloudBrush = 0;
						}
						else
						{
						// przeszukiwanie nazwy tekstury w rejestrze

							bool TextureFound = false;

							for(int i = 0; i < cloudTextureArray.size(); i++)
							{
								if( line == StripName( cloudTextureArray[i].filename, false ) )
								{
									cloudBrush = cloudTextureArray[i].textureID;
									TextureFound = true;
									break;
								}
							}

							if( !TextureFound )
							{
								loadingCorrect = false;

								char buffer[MAX_PATH];

								strcpy_s( buffer, "Haven't found loaded texture name in cloudTextureArray register: " );
								strcat_s( buffer, line.c_str() );

								errorInfo += buffer;
								break;
							}
						}
					}

				// pobranie wartosci chmur

					for(int i = 0; i < 9; i++)
					{
						file >> line;


						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading Cloud values.";
							break;
						}
						else if( !IsNumber(line, false) )
						{
							loadingCorrect = false;
							errorInfo += "Word where Cloud value should be, isn't a number!\n\n";
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
									cloudR = atof(line.c_str());
								break;

								case 1:
									cloudG = atof(line.c_str());
								break;

								case 2:
									cloudB = atof(line.c_str());
								break;

								case 3:
									cloudA = atof(line.c_str());
								break;

								case 4:
									cloudHeight = atof(line.c_str());
								break;

								case 5:
									textureSpread = atof(line.c_str());
								break;

								break;

								case 6:
									textureSpeed = atof(line.c_str());
								break;

								case 7:
									directionX = atof(line.c_str());
								break;

								case 8:
									directionZ = atof(line.c_str());
								break;
							}
						}
					}

				
				}// if( markerFound && !loadingCommenced )
			}

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();


			if( !markerFound )
			{
				loadingCorrect = false;
				errorInfo += "\"Clouds\" marker hasn't been found!";
			}


			if( loadingCorrect )
			{
			// ustawienie wartosci chmur w kontrolkach

				SetDlgItemInt(cloud, IDC_CLOUD_RED,   cloudR * 255, FALSE);
				SetDlgItemInt(cloud, IDC_CLOUD_GREEN, cloudG * 255, FALSE);
				SetDlgItemInt(cloud, IDC_CLOUD_BLUE,  cloudB * 255, FALSE);
				SetDlgItemInt(cloud, IDC_CLOUD_ALPHA, cloudA * 255, FALSE);

				SetDlgItemInt(cloud, IDC_CLOUD_SPREAD, textureSpread, FALSE);

				SetDlgItemInt(cloud, IDC_CLOUD_HEIGHT, cloudHeight / MAP_SCALE, FALSE);


				SetDlgItemInt(cloud, IDC_CLOUD_SPEED, textureSpeed * 100000.f, FALSE);

				SetDlgItemInt(cloud, IDC_CLOUD_X_DIR_SLIDER, directionX+1, FALSE);
				SetDlgItemInt(cloud, IDC_CLOUD_Z_DIR_SLIDER, directionZ+1, FALSE);

			// odswiezenie chmur

				updateCloud();
			}


			return loadingCorrect;
	}

//
// ------------------------------

	void ResetCloud()
	{
		cleanCloud();
		generateCloud();

		cloudBrush = 0;
	}

	void CloseCloud()
	{
		removeCloudTextures();
		cleanCloud();
	}