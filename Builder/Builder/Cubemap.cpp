#include "GlobalHeaders.h"

#include "Cubemap.h"


// Zmienne, stale zewnetrzne

	extern  const int 	OVERALL_MIN_HEIGHT,
						OVERALL_MAX_HEIGHT;

	extern  int		    MAP_X, MAP_Z, MAP_SCALE;


	extern  char terrainKey;
	extern  bool toolBoxReady,
				 enableLighting,

				 showCubemap;

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

	HWND cubemap = NULL;

	unsigned  int cubemapBrush = 0,
				 
				  cubeMapVBOv  = 0,
				  cubeMapVBOvt = 0,
				  cubeMapVBOvc = 0;

	float		  cubemapR, cubemapG, cubemapB, cubemapA;

	// do zapisu / odczytu poszczegolnych tekstur
		struct cubemapTextureObj
		{
			char	        filename[MAX_PATH];
			unsigned int    textureID;
		};

	std::vector <cubemapTextureObj> cubemapTextureArray;



// Definicje funkcji

	void generateCubeMap()
	{

		// zeby skapowac, zobacz "cubemap_sample.jpg" w Textures\\Cubemap

			float CubeMapTexCoords[24][2] = { 
										
									// lewy kwadracik

										0.f,        (0.5f - (0.5f * 0.3f)),
										0.f,  	    (0.5f + (0.5f * 0.3f)),

										(1.f/4.f),  (0.5f + (0.5f * 0.3f)),
										(1.f/4.f),  (0.5f - (0.5f * 0.3f)),
										
									// gorny kwadracik

										(1.f/4.f),  (0.5f + (0.5f * 0.3f)),
										(1.f/4.f),  1.f,

										(2.f/4.f),  1.f,
										(2.f/4.f),  (0.5f + (0.5f * 0.3f)),

									//srodkowy kwadracik

										(1.f/4.f),  (0.5f - (0.5f * 0.3f)),
										(1.f/4.f),  (0.5f + (0.5f * 0.3f)),

										(2.f/4.f),  (0.5f + (0.5f * 0.3f)),
										(2.f/4.f),  (0.5f - (0.5f * 0.3f)), 

									// dolny kwadracik

										(1.f/4.f),  0.f,
										(1.f/4.f),  (0.5f - (0.5f * 0.3f)),

										(2.f/4.f),  (0.5f - (0.5f * 0.3f)), 
										(2.f/4.f),  0.f,

									// prawy kwadracik

										(2.f/4.f),  (0.5f - (0.5f * 0.3f)), 
										(2.f/4.f),  (0.5f + (0.5f * 0.3f)),

										(3.f/4.f),  (0.5f + (0.5f * 0.3f)),
										(3.f/4.f),  (0.5f - (0.5f * 0.3f)), 


									// dalszy prawy kwadracik
										
										(3.f/4.f),  (0.5f - (0.5f * 0.3f)),
										(3.f/4.f),  (0.5f + (0.5f * 0.3f)),

										1.f,  (0.5f + (0.5f * 0.3f)),
										1.f,  (0.5f - (0.5f * 0.3f)), 

										},


				  minX = -MAP_X*MAP_SCALE*2.5f,
				  maxX =  MAP_X*MAP_SCALE*5.f,
				  
				  minZ = -MAP_Z*MAP_SCALE*5.f,
				  maxZ =  MAP_Z*MAP_SCALE*2.5f,

				  minY =  0.f,
				  maxY =  (OVERALL_MAX_HEIGHT*MAP_SCALE)+1, // + 1 zeby nie kolidowalo z chmurami (ktore moga byc na wysokosci OVERALL_MAX_HEIGHT*MAP_SCALE, co psuloby efekt)


			 // przygotowanie wierzcholkow (podpisane odpowiednio na "cubemap_sample.jpg")

				  A[3] = {	maxX,	minY,	maxZ  },
				  B[3] = {	maxX,	maxY,	maxZ  }, 

				  C[3] = {	minX,	minY,	maxZ  },
				  D[3] = {	minX,	maxY,	maxZ  },

				  E[3] = {	minX,	minY,	minZ  },
				  F[3] = {	minX,	maxY,	minZ  },

				  G[3] = {	maxX,	minY,	minZ  },
				  H[3] = {	maxX,	maxY,	minZ  },


				 CubeMapVertices[24][3] = { 
										
									// lewy kwadrat

										A[0], A[1], A[2], 
										B[0], B[1], B[2],
										D[0], D[1], D[2], 
										C[0], C[1], C[2], 

									// gorny kwadrat

										D[0], D[1], D[2], 
										B[0], B[1], B[2], 
										H[0], H[1], H[2], 
										F[0], F[1], F[2],

									// srodkowy kwadrat
									
										C[0], C[1], C[2], 
										D[0], D[1], D[2], 
										F[0], F[1], F[2], 
										E[0], E[1], E[2], 

									// dolny blizszy

										A[0], A[1], A[2],
										C[0], C[1], C[2], 
										E[0], E[1], E[2], 
										G[0], G[1], G[2], 
									
									// prawy blizszy

										E[0], E[1], E[2], 
										F[0], F[1], F[2], 
										H[0], H[1], H[2], 
										G[0], G[1], G[2], 

									// prawy dalszy
										
										G[0], G[1], G[2], 
										H[0], H[1], H[2],
										B[0], B[1], B[2],
										A[0], A[1], A[2],  

									 },

				 CubeMapColors[24][4];

		// pobranie kolorow z kontrolek

			cubemapR = (float)GetDlgItemInt(cubemap, IDC_CUBEMAP_RED,   FALSE, FALSE) / 255.f;
			cubemapG = (float)GetDlgItemInt(cubemap, IDC_CUBEMAP_GREEN, FALSE, FALSE) / 255.f;
			cubemapB = (float)GetDlgItemInt(cubemap, IDC_CUBEMAP_BLUE,  FALSE, FALSE) / 255.f;
			cubemapA = (float)GetDlgItemInt(cubemap, IDC_CUBEMAP_ALPHA, FALSE, FALSE) / 255.f;

		// wypelnienie tablicy kolorow powyzszymi wartosciami

			for(int i = 0; i < 24; i++)
			{
				CubeMapColors[i][0] = cubemapR;
				CubeMapColors[i][1] = cubemapG;
				CubeMapColors[i][2] = cubemapB;
				CubeMapColors[i][3] = cubemapA;
			}

		// generowanie VBO cubemap

			glGenBuffersARB(1, &cubeMapVBOv);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeMapVBOv);

			glBufferDataARB(GL_ARRAY_BUFFER_ARB, 24*3*sizeof(float), CubeMapVertices, GL_STATIC_DRAW_ARB);


			glGenBuffersARB(1, &cubeMapVBOvt);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeMapVBOvt);

			glBufferDataARB(GL_ARRAY_BUFFER_ARB, 24*2*sizeof(float), CubeMapTexCoords, GL_STATIC_DRAW_ARB);


			glGenBuffersARB(1, &cubeMapVBOvc);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeMapVBOvc);

			glBufferDataARB(GL_ARRAY_BUFFER_ARB, 24*4*sizeof(float), CubeMapColors, GL_STATIC_DRAW_ARB);
	

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	}




	void changeCubeMapColors()
	{
			if( cubeMapVBOvc != 0 )
				glDeleteBuffersARB(1, &cubeMapVBOvc);

			float CubeMapColors[24][4];

		// pobranie kolorow z kontrolek

			cubemapR = (float)GetDlgItemInt(cubemap, IDC_CUBEMAP_RED,   FALSE, FALSE) / 255.f;
			cubemapG = (float)GetDlgItemInt(cubemap, IDC_CUBEMAP_GREEN, FALSE, FALSE) / 255.f;
			cubemapB = (float)GetDlgItemInt(cubemap, IDC_CUBEMAP_BLUE,  FALSE, FALSE) / 255.f;
			cubemapA = (float)GetDlgItemInt(cubemap, IDC_CUBEMAP_ALPHA, FALSE, FALSE) / 255.f;

		// wypelnienie tablicy kolorow powyzszymi wartosciami

			for(int i = 0; i < 24; i++)
			{
				CubeMapColors[i][0] = cubemapR;
				CubeMapColors[i][1] = cubemapG;
				CubeMapColors[i][2] = cubemapB;
				CubeMapColors[i][3] = cubemapA;
			}

		// wygenerowanie VBO kolorow dla cubemap

			glGenBuffersARB(1, &cubeMapVBOvc);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeMapVBOvc);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, 24*4*sizeof(float), CubeMapColors, GL_STATIC_DRAW_ARB);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	}

	void drawCubeMap()
	{

		// terrainKey jest ustawiane, jezeli zaszla jakas zmiana w kontrolkach, w zakladce "Terrain"

			if( terrainKey )
			{
				cleanCubeMap();
				generateCubeMap();

				// terrainKey = NULL; jest ustawiane w drawCloud(),
				// ktore jest wywolywane po drawCubeMap();
			}

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glDisable(GL_CULL_FACE);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			if( enableLighting )
				glDisable(GL_LIGHTING);

				if( showCubemap )
					glBindTexture(GL_TEXTURE_2D, cubemapBrush);


				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeMapVBOv);
				glVertexPointer(3, GL_FLOAT, 0, 0);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeMapVBOvt);
				glTexCoordPointer(2, GL_FLOAT, 0, 0);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cubeMapVBOvc);
				glColorPointer(4, GL_FLOAT, 0, 0);


					glDrawArrays(GL_QUADS, 0, 24);



				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);


				glBindTexture(GL_TEXTURE_2D, 0);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


			if( enableLighting )
				glEnable(GL_LIGHTING);

			glEnable(GL_CULL_FACE);

			glDisable(GL_TEXTURE_2D);
	}



	void cleanCubeMap()
	{
		// czyszczenie VBO cubemap

			if( cubeMapVBOv != 0 )
				glDeleteBuffersARB(1, &cubeMapVBOv);

			if( cubeMapVBOvt != 0 )
				glDeleteBuffersARB(1, &cubeMapVBOvt);

			if( cubeMapVBOvc != 0 )
				glDeleteBuffersARB(1, &cubeMapVBOvc);

		// zerowanie identyfikatorow

			cubeMapVBOv = cubeMapVBOvt = cubeMapVBOvc = 0;
	}

// -------------------------------------------------------

	unsigned int generateCubeMapTexture(unsigned char *bits, unsigned int &width, unsigned int &height)
	{
		//wygeneruj nowy obiekt tekstury

			glGenTextures(1, &cubemapBrush);
			glBindTexture(GL_TEXTURE_2D, cubemapBrush);
		
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

			// Don't forget to use GL_RGBA for our new image data... we support Alpha transparency now!
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
						  GL_RGBA, GL_UNSIGNED_BYTE, bits );


			glBindTexture(GL_TEXTURE_2D, 0);


			return cubemapBrush;
	}



	bool loadCubeMapTexture(char *filename)
	{
			bool originalTexture;   //nie ma sensu generowac "nowej" tekstury

		// sprawdzenie "oryginalnosci" tekstury

			if( cubemapTextureArray.empty() )
				originalTexture = true;
			else
			{
				originalTexture = true;

				for(int i = 0; i < cubemapTextureArray.size(); i++)
				{
					if( strcmp(filename, cubemapTextureArray[i].filename) == 0 )
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
					LoadTexture(filename, TRUE, generateCubeMapTexture);
				}
				catch(std::string error)
				{
					MessageBox(g_hwnd, error.c_str(), "Error!", MB_OK);
					return false;
				}


			// dodaj nowy obiekt tekstury + sciezke do wektora tekstur

				cubemapTextureObj newCubemapTexture;
				strcpy_s(newCubemapTexture.filename, filename);
				newCubemapTexture.textureID = cubemapBrush;

				cubemapTextureArray.push_back(newCubemapTexture);


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

				HWND comboBox = GetDlgItem(cubemap, IDC_CUBEMAP_COMBO);

				ComboBox_AddString(comboBox, newItemName.c_str());

			// wyzerowanie brusha

				cubemapBrush = 0;

			}


			return originalTexture;
	}



	int  loadCubeMapTextures()
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

				directory = FindFirstFileA("TerrainData\\Textures\\Cubemap\\*.bmp", &data);

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

				directory = FindFirstFileA("TerrainData\\Textures\\Cubemap\\*.jpg", &data);

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

				directory = FindFirstFileA("TerrainData\\Textures\\Cubemap\\*.TGA", &data);

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
				strcat_s( szFileName, "TerrainData\\Textures\\Cubemap\\" );
				strcat_s( szFileName, files[i].c_str() );


			// Ladowanie tekstury o nazwie szFileName

				if( loadCubeMapTexture(szFileName) )
				{
					counter++;
				}
			}

			return counter;
	}


	void reloadCubeMapTextures()
	{
		int newTextures = loadCubeMapTextures();

		char buffer[MAX_PATH] = "";

		_itoa_s( newTextures, buffer, 10 );

		if( newTextures == 1 )
			strcat_s( buffer, " new texture loaded!" );
		else
			strcat_s( buffer, " new textures loaded!" );

		MessageBox(g_hwnd, buffer, "Reloading finished.", MB_OK);
	}
	void changeCubeMapTexture()
	{
		
			std::string cbIndexText;
			char buffer[MAX_PATH] = "";

			HWND comboBox = GetDlgItem(cubemap, IDC_CUBEMAP_COMBO);
			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			cbIndexText.insert(0, buffer);

			if( !cbIndexText.empty() )
			{
				for(int i = 0; i < cubemapTextureArray.size(); i++)
				{
					std::string helper;
					helper.insert(0, cubemapTextureArray[i].filename);

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
						cubemapBrush = cubemapTextureArray[i].textureID;

						// ustaw obrazek ...
						HBITMAP hBitmap;
						hBitmap = (HBITMAP)LoadImage(NULL, cubemapTextureArray[i].filename, IMAGE_BITMAP, 196, 128,
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

						HWND hPicture = GetDlgItem(cubemap, IDC_CUBEMAP);
						SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); 

						break;
					}
				}


			}// !cbIndexText.empty()
			else
			{
				HWND hPicture = GetDlgItem(cubemap, IDC_CUBEMAP);
				SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, NULL); 
			}

	}



	void removeCubeMapTextures()
	{
		for(int i = 0; i < cubemapTextureArray.size(); i++)
		{
			glDeleteTextures(1, &cubemapTextureArray[i].textureID);
		}
	}
// -------------------------------------------------------

	void saveCubeMap(char *filename)
	{
			std::fstream file;

		// otworzenie pliku w trybie "dopisz" + binarny

			file.open( filename, std::ios::out | std::ios::app | std::ios::binary );

		// ustawienie markera poczatkowego

			file << "\nCubemap\n";

		// zapis nazwy tekstury

			if( cubemapBrush )
			{
			// odszukanie nazwy tekstury w rejestrze

				bool TextureFound = false;

				for(int i = 0; i < cubemapTextureArray.size(); i++)
				{
					if( cubemapBrush == cubemapTextureArray[i].textureID )
					{
						file << "\t" << StripName( cubemapTextureArray[i].filename, false  ) << "\n";
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

		// zapis koloru cubemapy'

			file << "\t" << cubemapR << " "
				 << "\t" << cubemapG << " "
				 << "\t" << cubemapB << " "
				 << "\t" << cubemapA << "\n";

		// ustawienie markera koncowego

			file << "End\n\n";

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();
	}


	bool loadCubeMap(char *filename)
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

				if( line == "Cubemap" )
				{
					if( !markerFound )
					{
						markerFound = true;
					}
					else
					{
						loadingCorrect = false;
						errorInfo += "There are two or more cubemap info markers (should be one).";
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
						errorInfo += "\"End\" marker found too early, while loading Cubemap values.";
						break;
					}
					else
					{
						if( line == "None" )
						{
							cubemapBrush = 0;
						}
						else
						{
						// przeszukiwanie nazwy tekstury w rejestrze

							bool TextureFound = false;

							for(int i = 0; i < cubemapTextureArray.size(); i++)
							{
								if( line == StripName( cubemapTextureArray[i].filename, false ) )
								{
									cubemapBrush = cubemapTextureArray[i].textureID;
									TextureFound = true;
									break;
								}
							}

							if( !TextureFound )
							{
								loadingCorrect = false;

								char buffer[MAX_PATH];

								strcpy_s( buffer, "Haven't found loaded texture name in cubemapTextureArray register: " );
								strcat_s( buffer, line.c_str() );

								errorInfo += buffer;
								break;
							}
						}
					}

				// pobranie wartosci chmur

					for(int i = 0; i < 4; i++)
					{
						file >> line;


						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading Cubemap color values.";
							break;
						}
						else if( !IsNumber(line, false) )
						{
							loadingCorrect = false;
							errorInfo += "Word where Cubemap color value should be, isn't a number!\n\n";
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
									cubemapR = atof(line.c_str());
								break;

								case 1:
									cubemapG = atof(line.c_str());
								break;

								case 2:
									cubemapB = atof(line.c_str());
								break;

								case 3:
									cubemapA = atof(line.c_str());
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
				errorInfo += "\"Cubemap\" marker hasn't been found!";
			}


			if( loadingCorrect )
			{
				generateCubeMap();
			}


			return loadingCorrect;
	}

// -------------------------------------------------------

	void ResetCubeMap()
	{
		cleanCubeMap();
		generateCubeMap();

		cubemapBrush = 0;
	}

	void CloseCubeMap()
	{
		cleanCubeMap();
		removeCubeMapTextures();
	}