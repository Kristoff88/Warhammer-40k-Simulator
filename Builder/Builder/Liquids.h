#pragma once



// Zmienne, struktury


	HWND 	liquids  = NULL;

	unsigned int liquidBrush = 0;

	


	// parametry CG

	#define LIQUID_SHADER_PATH	     "Utilities\\CG shaders\\Liquid shader\\Wave.cg"
	#define LIQUID_VERTEX_SHADER     "makeWave"
	#define LIQUID_FRAGMENT_SHADER	 "passFragment"


		CGcontext	liquidContext;
		CGprogram	MakeWave, passFragment;		
		CGprofile	liquidVP, liquidFP;	

		CGparameter liquidModelViewMatrix, 
					wave,
					level;


	// do zapisu / odczytu poszczegolnych tekstur
		struct liquidTextureObj
		{
			char	        filename[MAX_PATH];
			unsigned int    textureID;
		};

	std::vector <liquidTextureObj> liquidTextureArray;


		struct liquidObject
		{

				unsigned int  textureID;

				float		  alpha,

							// poziom wody
							  minLevel, maxLevel, 

						   // predkosc opadania/wznoszenia sie wody
							  speed,
							  
							// amplituda fal
							  waveLevel, 
							  
						    // czestotliwosc przesuwania sie fal
							  waveFrequency, 
							  
						    // rozciagniecie fal
							  waveStretch,

						   // poruszanie sie fal wzdlu X i Z

							  movementX,	// okreslaja stopien przemieszczenia (ktory sie resetuje co jakis czas)
							  movementZ,

							  directionX,	// okreslaja kierunki
							  directionZ;

				GLuint		  *indexArray;
				unsigned int  indexCounter,  // licznik indeksow powyzszych tablic

							  VBOv,
							  VBOvt,
							  VBOvn,
							  VBOvc;

				bool		  Rising;   	// poziom wody rosnie/spada (true/false)
							  


			// konstruktor, destruktor

				liquidObject(unsigned int ID, float alph, float min, float max, float waveLVL, float waveFREQ, float stretch, int dirX, int dirZ, float sped):
							 textureID(ID), alpha(alph), minLevel(min), maxLevel(max), speed(sped), waveLevel(waveLVL), waveFrequency(waveFREQ), waveStretch(stretch), Rising(true),

							 movementX(0), movementZ(0), directionX(dirX), directionZ(dirZ), indexArray(NULL), indexCounter(0),

							 VBOv(0), VBOvt(0), VBOvn(0), VBOvc(0)
				{}

				~liquidObject()
				{
					
					// czyszczenie VBO
						
						if( VBOv != 0 )
							glDeleteBuffersARB(1, &VBOv);

						if( VBOvt != 0 )
							glDeleteBuffersARB(1, &VBOvt);

						if( VBOvn != 0 )
							glDeleteBuffersARB(1, &VBOvn);

						if( VBOvc != 0 )
							glDeleteBuffersARB(1, &VBOvc);
				}

		};







// Funkcje

	void computeLiquid(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
	{
		
				liquidObject *liquid = reinterpret_cast < liquidObject* > (object);


			// czyszczenie tablicy indeksow

				if( liquid->indexArray != NULL )
					delete [] liquid->indexArray;


			// dla zwiekszenia czytelnosci kodu...

				#define detailLVL	2

				#define sizeX ( (sizeX/MAP_SCALE) * 2 * detailLVL )
				#define sizeZ ( (sizeZ/MAP_SCALE) * 2 * detailLVL )


			// rezerwacja pamieci dla tablic

				liquid->indexArray = new GLuint [sizeX * sizeZ * 2];

				float (*heightMap)[3]	= new float [sizeX * sizeZ][3],
					  (*texCoords)[2]	= new float [sizeX * sizeZ][2],
					  (*normals)[3]		= new float [sizeX * sizeZ][3],
					  (*colors)[4]		= new float [sizeX * sizeZ][4];


				// zmienne wskazuj¹ce bie¿¹c¹ pozycjê tablicy indeksów

					int index = 0,
						currentVertex;

			// obliczenie heightMap i waveMap

					for (int i = 0, posZ = -sizeZ / (2.f / MAP_SCALE);  i < sizeZ;  i++, posZ += MAP_SCALE)
					for (int j = 0, posX = -sizeX / (2.f / MAP_SCALE);  j < sizeX;  j++, posX += MAP_SCALE)
					{
						currentVertex = j + sizeX*i;

						heightMap[currentVertex][0] = x + ((float)posX / (float) detailLVL);
						heightMap[currentVertex][2] = z + ((float)posZ / (float) detailLVL);

						heightMap[currentVertex][1] = 0.f;
		
					}


			// colors, texCoords

				// przegl¹da w pêtli wszystkie wierzcho³ki mapy terenu

					for (int z = 0; z < sizeZ; z++)
					for (int x = 0; x < sizeX; x++)
					{
					// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³
						currentVertex = x + z*sizeX;

					// umieszcza wartoœci w tablicy kolorów
						colors[currentVertex][0] = colors[currentVertex][1] =
						colors[currentVertex][2] = 1.f;
						colors[currentVertex][3] = liquid->alpha;

					// umieszcza wartoœci w tablicy wspó³rzêdnych tekstury
						texCoords[currentVertex][0] = (float) x / (float) detailLVL;
						texCoords[currentVertex][1] = (float) z / (float) detailLVL;
					}

			// normale

					for (int z = 0; z < sizeZ; z++)
					for (int x = 0; x < sizeX; x++)
					{
					// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³
						currentVertex = x + z*sizeX;

						normals[currentVertex][0] = 0.f;
						normals[currentVertex][1] = 1.f;
						normals[currentVertex][2] = 0.f;
					}


			// indexArray

					liquid->indexCounter = 0;			

					for (int z = 0; z < sizeZ - 1; z++)
					for (int x = 0; x < sizeX; x++)
					{
						currentVertex = x + z*sizeX;

						liquid->indexArray[index++] = currentVertex + sizeX;
						liquid->indexArray[index++] = currentVertex;

						liquid->indexCounter += 2;
					}


			// wyzerowanie dwoch nastepnych pozycji, zeby glDrawElements nie powodowalo "access violation"
				liquid->indexArray[index++] = 0;
				liquid->indexArray[index]	= 0;


			// generowanie VBO wody

				if( liquid->VBOv != 0 )
					glDeleteBuffersARB(1, &liquid->VBOv);

				if( liquid->VBOvt != 0 )
					glDeleteBuffersARB(1, &liquid->VBOvt);

				if( liquid->VBOvn != 0 )
					glDeleteBuffersARB(1, &liquid->VBOvn);

				if( liquid->VBOvc != 0 )
					glDeleteBuffersARB(1, &liquid->VBOvc);


				glGenBuffersARB(1, &liquid->VBOv);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, liquid->VBOv);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeX*sizeZ*3*sizeof(float), heightMap, GL_DYNAMIC_DRAW_ARB);


				glGenBuffersARB(1, &liquid->VBOvt);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, liquid->VBOvt);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeX*sizeZ*2*sizeof(float), texCoords, GL_DYNAMIC_DRAW_ARB);


				glGenBuffersARB(1, &liquid->VBOvn);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, liquid->VBOvn);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeX*sizeZ*3*sizeof(float), normals, GL_DYNAMIC_DRAW_ARB);


				glGenBuffersARB(1, &liquid->VBOvc);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, liquid->VBOvc);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeX*sizeZ*4*sizeof(float), colors, GL_DYNAMIC_DRAW_ARB);


				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			// juz starczy preprocesorze...

				#undef detailLVL

				#undef sizeX
				#undef sizeZ

	}


	bool preLiquid()
	{
		
			if( showLiquids )
			{
				glEnable(GL_TEXTURE_2D);

				glFrontFace(GL_CW);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.f);

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);

				return true;
			}
			else
				return false;
	}
	void drawLiquid(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ,  void *object, bool Selected)
	{

			liquidObject *liquid = reinterpret_cast < liquidObject* > (object);
			
		// wyslanie wskaznikow z odpowiednimi tablicami do OpenGL

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, liquid->VBOv);
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, liquid->VBOvt);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, liquid->VBOvn);
			glNormalPointer(GL_FLOAT, 0, 0);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, liquid->VBOvc);
			glColorPointer(4, GL_FLOAT, 0, 0);


			glBindTexture(GL_TEXTURE_2D, liquid->textureID);

		// przyszykowanie shader'ow

			cgGLEnableProfile(liquidVP);
			cgGLBindProgram(MakeWave);

			cgGLEnableProfile(liquidFP);
			cgGLBindProgram(passFragment);

			cgGLSetStateMatrixParameter(liquidModelViewMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

			cgGLSetParameter4f(wave,  liquid->movementX, y, liquid->movementZ, liquid->waveStretch);
			cgGLSetParameter1f(level, liquid->waveLevel);

		// rysowanie tafli
		
			glDrawElements(GL_TRIANGLE_STRIP, liquid->indexCounter, GL_UNSIGNED_INT, &liquid->indexArray[0]);


			cgGLDisableProfile(liquidVP);
			cgGLDisableProfile(liquidFP);

		// jezeli sa nierowne 0, to modyfikacja modyfikatorow przesuniecia fali

			if( liquid->directionX )
			{
				if( liquid->directionX == 1 )
					liquid->movementX += liquid->waveFrequency*100;
				else
					liquid->movementX -= liquid->waveFrequency*100;


			// zabezpieczenie, tj. jezeli przesunie o cala szerokosc, wraca do poczatku

				if( liquid->movementX == PI*2 )
					liquid->movementX = 0;
				else if( liquid->movementX == -(PI*2) )
					liquid->movementX = 0;
			}


			if( liquid->directionZ )
			{
				if( liquid->directionZ == 1 )
					liquid->movementZ += liquid->waveFrequency*100;
				else
					liquid->movementZ -= liquid->waveFrequency*100;


				if( liquid->movementZ == PI*2 )
					liquid->movementZ = 0;
				else if( liquid->movementZ == -(PI*2) )
					liquid->movementZ = 0;
			}


		// korygowanie poziomy wody, etc.

			if( liquid->maxLevel != liquid->minLevel )
			{
				if( liquid->Rising )
				{
					y += liquid->speed;

					if( y > liquid->maxLevel )
						liquid->Rising = false;
				}
				else
				{
					y -= liquid->speed;

					if( y < liquid->minLevel )
						liquid->Rising = true;
				}
			}
	}



	void postLiquid()
	{
		glDisable(GL_TEXTURE_2D);

		glFrontFace(GL_CCW);

		glDisable(GL_BLEND);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glBindTexture(GL_TEXTURE_2D, 0);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}


	void moveLiquid(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
	{

		// korekcja pozycji, w celu uchronienia przed wychodzeniem detalu za granice terenu

			if( x/MAP_SCALE + sizeX/MAP_SCALE > MAP_X )
				x -= MAP_SCALE;

			else if ( x/MAP_SCALE - sizeX/MAP_SCALE < 0 )
				x += MAP_SCALE;


			if( -z/MAP_SCALE + sizeZ/MAP_SCALE > MAP_Z )
				z += MAP_SCALE;

			else if ( -z/MAP_SCALE - sizeZ/MAP_SCALE < 0 )
				z -= MAP_SCALE;


		// korygacja Y i sizeY

			if( y < MIN_TERRAIN_HEIGHT )
				y = MIN_TERRAIN_HEIGHT;

			sizeY = y - MIN_TERRAIN_HEIGHT + 2;


		// korygacja liquid'a

			computeLiquid(x, y, z, sizeX, sizeY, sizeZ, object);

	}




	void liquidKeyManager(char key, void *object, float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ)
	{

			liquidObject *liquid = reinterpret_cast < liquidObject* > (object);

			float oldSizeX = sizeX,
				  oldSizeZ = sizeZ,
				  oldAlpha = liquid->alpha;

			switch( toupper(key) )
			{

				// zmiana rozmiarow detalu

					case 'W':
					{
						if( 
							sizeX/MAP_SCALE < MAP_X/2 
									&& 
							(x/MAP_SCALE - sizeX/MAP_SCALE) > 1
									&&
							(x/MAP_SCALE + sizeX/MAP_SCALE) < MAP_X-1 
						   )

						sizeX += MAP_SCALE;

						SetDlgItemInt(liquids, IDC_LIQUID_X, sizeX, FALSE);

						Saved = false;
					}
					break;


					case 'S':
					{
						if( sizeX/MAP_SCALE > 1 )
							sizeX -= MAP_SCALE;

						SetDlgItemInt(liquids, IDC_LIQUID_X, sizeX, FALSE);

						Saved = false;
					}
					break;


					case 'D':
					{
						if( 
							sizeZ/MAP_SCALE < MAP_Z/2 
										 &&
							(-z/MAP_SCALE - sizeZ/MAP_SCALE) > 1
										 &&
							(-z/MAP_SCALE + sizeZ/MAP_SCALE) < MAP_Z-1 
						  )

						sizeZ += MAP_SCALE;

						SetDlgItemInt(liquids, IDC_LIQUID_Z, sizeZ, FALSE);

						Saved = false;
					}
					break;


					case 'A':
					{
						if( sizeZ/MAP_SCALE > 1 )
							sizeZ -= MAP_SCALE;

						SetDlgItemInt(liquids, IDC_LIQUID_Z, sizeZ, FALSE);

						Saved = false;
					}
					break;


				// ponowne zaladowanie wartosci z kontrolek, do zaznaczonych liquid'ow

					case 'L':
					{

						liquid->speed =     (float)GetDlgItemInt(liquids, IDC_LIQUID_SPEED, FALSE, FALSE) / 10000.f;
						liquid->alpha =		(float)GetDlgItemInt(liquids, IDC_LIQUID_ALPHA, FALSE, FALSE) / 255.f;

						liquid->waveLevel	   = ( GetDlgItemInt(liquids, IDC_LIQUID_STORMY, FALSE, FALSE) / 1000.f ) * (float)MAP_SCALE;
						liquid->waveFrequency  = (float)GetDlgItemInt(liquids, IDC_LIQUID_WAVE, FALSE, FALSE) / 100000.f;
						liquid->waveStretch	   = GetDlgItemInt(liquids, IDC_LIQUID_STRETCH, FALSE, FALSE);


						int   dirX = SendMessageA(GetDlgItem(liquids, IDC_LIQUID_X_DIR_SLIDER), TBM_GETPOS, 0, 0),
							  dirZ = SendMessageA(GetDlgItem(liquids, IDC_LIQUID_Z_DIR_SLIDER), TBM_GETPOS, 0, 0);

						// ssuwaki maja wartosci od 0-2, a potrzeba od -1 do 1

							dirX -= 1;
							dirZ -= 1;

						liquid->directionX = dirX;
						liquid->directionZ = dirZ;

					// reset stanu przesuwania tafli

						liquid->movementX = 0;
						liquid->movementZ = 0;


						liquid->textureID = liquidBrush;
										

						float maxLevel = GetDlgItemInt(liquids, IDC_LIQUID_MAX, FALSE, FALSE),
							  minLevel = GetDlgItemInt(liquids, IDC_LIQUID_MIN, FALSE, FALSE);

						if( minLevel > maxLevel)
						{
							liquid->maxLevel =  minLevel; 
							liquid->minLevel =  maxLevel;
						}
						else
						{
							liquid->maxLevel =  maxLevel;
							liquid->minLevel =  minLevel;
						}

					// pobranie rozmiarow

						sizeX = GetDlgItemInt(liquids, IDC_LIQUID_X, FALSE, FALSE);
						sizeZ = GetDlgItemInt(liquids, IDC_LIQUID_Z, FALSE, FALSE);

						Saved = false;
					}
					break;

			}


		// korygacja Y i sizeY

			if( y < MIN_TERRAIN_HEIGHT )
				y = MIN_TERRAIN_HEIGHT;


			sizeY = y - MIN_TERRAIN_HEIGHT + 2;



		// korygacja liquid'a
			if( oldSizeX != sizeX || oldSizeZ != sizeZ || oldAlpha != liquid->alpha )
			{
				computeLiquid(x, y, z, sizeX, sizeY, sizeZ, object);
				moveLiquid(x, y, z, sizeX, sizeY, sizeZ, object);
			}

	}





	void liquidText(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
	{
		//	liquidObject *liquid = reinterpret_cast < liquidObject* > (object);

		// wyczyszczenie wektora zawierajacego tekst

			textOutput.erase( textOutput.begin(), textOutput.end() );

		// tablice, ktore beda zawierac informacje

			static char CurrentAction[50];
			static char CurrentX[50];
			static char CurrentY[50];
			static char CurrentZ[50];
			static char CurrentSizeX[50];
			static char CurrentSizeY[50];
			static char CurrentSizeZ[50];

			sprintf_s( CurrentAction, "Action : Moving liquid" ); 
			textOutput.push_back( CurrentAction );


			sprintf_s( CurrentX, "X: %8.2f", x );
			textOutput.push_back( CurrentX );

			sprintf_s( CurrentY, "Y: %8.2f", y );
			textOutput.push_back( CurrentY );

			sprintf_s( CurrentZ, "Z: %8.2f", z );
			textOutput.push_back( CurrentZ );


			sprintf_s( CurrentSizeX, "SizeX: %8.2f", sizeX );
			textOutput.push_back( CurrentSizeX );

			sprintf_s( CurrentSizeY, "SizeY: %8.2f", sizeY );
			textOutput.push_back( CurrentSizeY );

			sprintf_s( CurrentSizeZ, "SizeZ: %8.2f", sizeZ );
			textOutput.push_back( CurrentSizeZ );
	}

	void removeLiquid(void *object)
	{
		liquidCounter--;
	}
	void placeLiquid()
	{

			if( liquidBrush != 0 )
			{
				
				// przygotowanie danych, potrzebnych do utworzenia nowej warstwy wody
				
					unsigned int textureID = liquidBrush;

					float alpha =    (float)GetDlgItemInt(liquids, IDC_LIQUID_ALPHA,   FALSE, FALSE) / 100.f,
						
						  minLevel =		GetDlgItemInt(liquids, IDC_LIQUID_MIN,     FALSE, FALSE),
						  maxLevel =		GetDlgItemInt(liquids, IDC_LIQUID_MAX,     FALSE, FALSE),

						  waveLevel =	  ( GetDlgItemInt(liquids, IDC_LIQUID_STORMY,  FALSE, FALSE) / 1000.f ) * (float)MAP_SCALE,
						  waveFrequency =	(float)GetDlgItemInt(liquids, IDC_LIQUID_WAVE,    FALSE, FALSE) / 100000.f,
						  waveStretch	  = GetDlgItemInt(liquids, IDC_LIQUID_STRETCH, FALSE, FALSE),

						  speed =	 (float)GetDlgItemInt(liquids, IDC_LIQUID_SPEED,   FALSE, FALSE) / 10000.f;


					int   dirX = SendMessageA(GetDlgItem(liquids, IDC_LIQUID_X_DIR_SLIDER), TBM_GETPOS, 0, 0),
						  dirZ = SendMessageA(GetDlgItem(liquids, IDC_LIQUID_Z_DIR_SLIDER), TBM_GETPOS, 0, 0);

						// ssuwaki maja wartosci od 0-2, a potrzeba od -1 do 1
							dirX -= 1;
							dirZ -= 1;


					if( minLevel > maxLevel)
					{
						float helper = minLevel;

						minLevel = maxLevel;
						maxLevel = helper;
					}


					liquidObject *newLiquid = new liquidObject(textureID, alpha, minLevel, maxLevel, waveLevel, waveFrequency, waveStretch, dirX, dirZ, speed);

					int index = cursorX/MAP_SCALE + MAP_X * -cursorZ/MAP_SCALE;

					ObjectOperator::Object newObject(cursorX, g_terrain[index][1]+2.f, cursorZ,

									 MAP_SCALE, MAP_SCALE*10.f, MAP_SCALE,

									 newLiquid,
									 preLiquid,
									 drawLiquid,
									 postLiquid,
									 moveLiquid,
									 removeLiquid,
									 liquidKeyManager,
									 liquidText,
									 computeLiquid,
									 LIQUID_NAME,
									 FALSE,
									 FALSE,
									 TRUE);


					objectOperator.Add(newObject);

					liquidCounter++;

					objectOperator.keyPressed = 'L'; // < --- sygnal dla liquidKeyManager(key, ...) (wywoluje on w sobie computeLiquid())

			}
			else
				MessageBox(g_hwnd, "Liquid texture hasn't been chosen.", "Error!", MB_OK);

	}






	void removeAllLiquids()
	{

			if ( !objectOperator.removeSpecificObjects(LIQUID_NAME) )
				MessageBox(g_hwnd, "There are no liquids on the map.", "Error!", MB_OK);
			else
				liquidCounter = 0;

	}















// --------------------------------------------

	unsigned int generateLiquidTexture(unsigned char *bits, unsigned int &width, unsigned int &height)
	{
		glGenTextures(1, &liquidBrush);
		glBindTexture(GL_TEXTURE_2D, liquidBrush);
	

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, width, height,
					  GL_RGB, GL_UNSIGNED_BYTE, bits );


		glBindTexture(GL_TEXTURE_2D, 0);

		
		return liquidBrush;
	}



	bool loadLiquidTexture(char *filename)
	{

			bool originalTexture;   //nie ma sensu generowac "nowej" tekstury

			// sprawdzenie "oryginalnosci" tekstury
			if(liquidTextureArray.empty())
				originalTexture = true;
			else
			{
				originalTexture = true;

				for(int i=0; i<liquidTextureArray.size(); i++)
				{
					if(strcmp(filename, liquidTextureArray[i].filename) == 0)
					{
						originalTexture = false;
						break;
					}
				}
			}
			
			// nowa tekstura? no to dodajemy...
			if(originalTexture)
			{

				// zaladuj i wygeneruj teksture

					try
					{
						LoadTexture(filename, FALSE, generateLiquidTexture);
					}
					catch(std::string error)
					{
						MessageBox(g_hwnd, error.c_str(), "Error!", MB_OK);
						return false;
					}

				// dodaj nowy obiekt tekstury + sciezke do wektora tekstur
					liquidTextureObj newLiquidTexture;
					strcpy_s(newLiquidTexture.filename, filename);
					newLiquidTexture.textureID = liquidBrush;

					liquidTextureArray.push_back(newLiquidTexture);

				// dodaj nowa pozycje do IDC_LIQUID_COMBO
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
					HWND comboBox = GetDlgItem(liquids, IDC_LIQUID_COMBO);

					ComboBox_AddString(comboBox, newItemName.c_str());
			
				// zerowanie brusha

					liquidBrush = 0;
			}


			return originalTexture;
	}



	int  loadLiquidTextures()
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

				directory = FindFirstFileA("TerrainData\\Textures\\Liquids\\*.bmp", &data);

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

				directory = FindFirstFileA("TerrainData\\Textures\\Liquids\\*.jpg", &data);

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

				directory = FindFirstFileA("TerrainData\\Textures\\Liquids\\*.TGA", &data);

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
				strcat_s( szFileName, "TerrainData\\Textures\\Liquids\\" );
				strcat_s( szFileName, files[i].c_str() );


			// Ladowanie tekstury o nazwie szFileName

				if( loadLiquidTexture(szFileName) )
				{
					counter++;
				}
			}

			return counter;
	}


	void reloadLiquidTextures()
	{
		int newTextures = loadLiquidTextures();

		char buffer[MAX_PATH] = "";

		_itoa_s( newTextures, buffer, 10 );

		if( newTextures == 1 )
			strcat_s( buffer, " new texture loaded!" );
		else
			strcat_s( buffer, " new textures loaded!" );

		MessageBox(g_hwnd, buffer, "Reloading finished.", MB_OK);
	}


	void changeLiquidTexture()
	{

			std::string cbIndexText;
			char buffer[MAX_PATH] = "";

			HWND comboBox = GetDlgItem(liquids, IDC_LIQUID_COMBO);
			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			cbIndexText.insert(0, buffer);

			if(!cbIndexText.empty())
			{
				for(int i=0; i<liquidTextureArray.size(); i++)
				{
					std::string helper;
					helper.insert(0, liquidTextureArray[i].filename);

					bool trigger = false;
					std::string::size_type result;

					while(!trigger)
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

					if(cbIndexText == helper)
					{
						liquidBrush = liquidTextureArray[i].textureID;

						// ustaw obrazek ...
						HBITMAP hBitmap;
						hBitmap = (HBITMAP)LoadImage(NULL, liquidTextureArray[i].filename, IMAGE_BITMAP, 128, 128,
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

						HWND hPicture = GetDlgItem(liquids, IDC_LIQUID_PIC);
						SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); 

						break;
					}
				}


			// jezeli jakis liquid jest zaznaczony, to chcemy aby
			// ustawil on nowe ID poprzez liquidKeyManager(...)

				objectOperator.keyPressed = 'L';

			}// !cbIndexText.empty()
			else
			{
				HWND hPicture = GetDlgItem(liquids, IDC_LIQUID_PIC);
				SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, NULL); 
			}

	}


	void removeLiquidTextures()
	{
		for(int i = 0; i < liquidTextureArray.size(); i++)
		{
			glDeleteTextures(1, &liquidTextureArray[i].textureID);
		}
	}
//
// --------------------------------------------

	void saveLiquids(char *filename)
	{

			std::fstream file;

		// otworzenie pliku w trybie "nadpisz" + binarny

			file.open( filename, std::ios::out | std::ios::app | std::ios::binary );

		// ustawienie markera poczatkowego

			file << "\nLiquids\n";

		// zapisanie licznika cieczy

			file << "\t" << liquidCounter << "\n";

		// przeszukiwanie wektora obiektow w poszukiwaniu obiektow detalu

			int counter = 0;

			for(int obj = 0; obj < objectOperator.objectArray.size(); obj++)
			{
			// znaleziono ciecz!

				if( objectOperator.objectArray[obj].GetType() == LIQUID_NAME )
				{
				// inkrementacja kontrolnej zmiennej

					counter++;

				// pobranie obiektu detalu

					liquidObject *liquid = reinterpret_cast <liquidObject*> ( objectOperator.objectArray[obj].GetObject() );

				// zapis danych obiektu (pozycja, rozmiar)

					Vector3 position = objectOperator.objectArray[obj].GetPosition(), 
						    size     = objectOperator.objectArray[obj].GetSize();

					file << "\t" << position.posX << " "
						         << position.posY << " "
								 << position.posZ << "\n";

					file << "\t" << size.posX << " "
						         << size.posY << " "
								 << size.posZ << "\n";
				
				// zapis tekstury cieczy

					bool TextureFound = false;

					for(int i = 0; i < liquidTextureArray.size(); i++)
					{
						if( liquid->textureID == liquidTextureArray[i].textureID )
						{
							file << "\t" << StripName( liquidTextureArray[i].filename, false ) << "\n";
							
							TextureFound = true;
							break;
						}
					}

					assert( TextureFound );

				// zapis wartosci cieczy

					file << "\t" << liquid->alpha << "\n";
					file << "\t" << liquid->minLevel      << "\n";
					file << "\t" << liquid->maxLevel      << "\n";
					file << "\t" << liquid->waveLevel     << "\n";
					file << "\t" << liquid->waveFrequency << "\n";
					file << "\t" << liquid->waveStretch   << "\n";
					file << "\t" << liquid->speed         << "\n";
					file << "\t" << liquid->directionX    << "\n";
					file << "\t" << liquid->directionZ    << "\n";

				}

			}

		// kontrolny assert

			assert( liquidCounter == counter );

		// ustawienie markera koncowego

			file << "End\n\n";

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();
	}

	bool loadLiquids(char *filename)
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

				if( line == "Liquids" )
				{
					if( !markerFound )
					{
						markerFound = true;
					}
					else
					{
						loadingCorrect = false;
						errorInfo += "There are two or more liquid info markers (should be one).";
						break;
					}
				}

				if( markerFound && !loadingCommenced )
				{
				// ustawienie falgi

					loadingCommenced = true;

				// pobranie licznika cieczy

					file >> line;

					if( line == "End"  )
					{
						loadingCorrect = false;
						errorInfo += "\"End\" marker found too early, while loading liquidObject values.";
						break;
					}
					else if( !IsNumber(line, true) )
					{
						loadingCorrect = false;
						errorInfo += "Word where liquidCounter value should be, isn't a number!\n\n";
						errorInfo += "\"";
						errorInfo += line;
						errorInfo += "\"";
						break;
					}
					else
					{
						liquidCounter = atoi(line.c_str());
					}

					for(int l = 0; l < liquidCounter; l++)
					{
					// zmienne, ktore beda przechowywac dane detalu

						Vector3		  position, size;

						unsigned int  textureID;

						float         alpha, 
							          minLevel, maxLevel,
									  waveLevel, waveFrequency, waveStretch,
									  speed,
									  directionX, directionZ;

					// pobranie pozycji i rozmiaru detalu

						for(int i = 0; i < 6; i++)
						{
							file >> line;

							if( line == "End"  )
							{
								loadingCorrect = false;
								errorInfo += "\"End\" marker found too early, while loading liquidObject values.";
								break;
							}
							else if( !IsNumber(line, false) )
							{
								loadingCorrect = false;
								errorInfo += "Word where liquidObject position/size value should be, isn't a number!\n\n";
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
										position.posX = atof(line.c_str());
									break;

									case 1:
										position.posY = atof(line.c_str());
									break;

									case 2:
										position.posZ = atof(line.c_str());
									break;


									case 3:
										size.posX = atof(line.c_str());
									break;

									case 4:
										size.posY = atof(line.c_str());
									break;

									case 5:
										size.posZ = atof(line.c_str());
									break;
								}
							}
						}// for ladujacy pozycje i rozmiar obiektu
					
					// pobranie nazwy tekstury i zidentyfikowanie ID tekstury w rejestrze

						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading liquidObject values.";
							break;
						}

						bool TextureFound = false;

						for(int i = 0; i < liquidTextureArray.size(); i++)
						{
							if( line == StripName( liquidTextureArray[i].filename, false ) )
							{
								textureID = liquidTextureArray[i].textureID;
								TextureFound = true;
								break;
							}
						}

						if( !TextureFound )
						{
							loadingCorrect = false;
							errorInfo += "Texture name hasn't been found in liquidTextureArray register!";
							break;
						}

					// pobranie wlasnosci cieczy

						for(int i = 0; i < 9; i ++)
						{
							file >> line;

							if( line == "End"  )
							{
								loadingCorrect = false;
								errorInfo += "\"End\" marker found too early, while loading liquidObject values.";
								break;
							}
							else if( !IsNumber( line, false ) )
							{
								loadingCorrect = false;
								errorInfo += "Word where liquidObject value should be, isn't a number!\n\n";
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
										alpha = atof(line.c_str());
									break;

									case 1:
										minLevel = atof(line.c_str());
									break;

									case 2:
										maxLevel = atof(line.c_str());
									break;

									case 3:
										waveLevel = atof(line.c_str());
									break;

									case 4:
										waveFrequency = atof(line.c_str());
									break;

									case 5:
										waveStretch = atof(line.c_str());
									break;

									case 6:
										speed = atof(line.c_str());
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


					// przygotowanie nowego obiektu cieczy do umieszczenia w wektorze obiektow

						liquidObject *newLiquid = new liquidObject(textureID, alpha,
																   minLevel, maxLevel, 
																   waveLevel, waveFrequency, waveStretch,
																   directionX, directionZ, speed);


						ObjectOperator::Object newObject(position.posX, position.posY, position.posZ,
														 size.posX,     size.posY,     size.posZ,

														 newLiquid,
														 preLiquid,
														 drawLiquid,
														 postLiquid,
														 moveLiquid,
														 removeLiquid,
														 liquidKeyManager,
														 liquidText,
														 computeLiquid,
														 LIQUID_NAME,
														 FALSE,
														 FALSE,
														 TRUE);

						objectOperator.Add(newObject);

					// wstepne obliczenie stanu detalu (zeby w ogole bylo widoczne)
						
						float x     = position.posX,
							  y     = position.posY,
							  z     = position.posZ,
							  sizeX = size.posX,
							  sizeY = size.posY,
							  sizeZ = size.posZ;

						void *helper = newLiquid;

						computeLiquid( x, y, z, sizeX, sizeY, sizeZ, helper );

					}// petla ladujaca ciecze
				
				}// if( markerFound && !loadingCommenced )
			}

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();


			if( !markerFound )
			{
				loadingCorrect = false;
				errorInfo += "\"Liquids\" marker hasn't been found!";
			}


			return loadingCorrect;
	}

// --------------------------------------------

	bool InitLiquids()
	{
		// Create A New Context For Our Cg Program(s)

			liquidContext = cgCreateContext();	

		// Validate Our Context Generation Was Successful

			if (liquidContext == NULL)
			{
				MessageBox(NULL, "Failed To Create Cg Context (liquids initialization)", "Error", MB_OK);
				return false;			
			}

		// Get The Latest GL Vertex Profile

			liquidVP = cgGLGetLatestProfile(CG_GL_VERTEX);	

		// Validate Our Profile Determination Was Successful

			if (liquidVP == CG_PROFILE_UNKNOWN)
			{
				MessageBox(NULL, "Invalid profile type (liquids initialization)", "Error", MB_OK);
				return false;	
			}

		// Set The Current Profile

			cgGLSetOptimalOptions(liquidVP);		

		// Load And Compile The Vertex Shader From File

			MakeWave = cgCreateProgramFromFile(liquidContext, CG_SOURCE, LIQUID_SHADER_PATH, liquidVP, LIQUID_VERTEX_SHADER, 0);

		// Validate Success

			if (MakeWave == NULL)
			{
			// We Need To Determine What Went Wrong

				CGerror Error = cgGetError();

			// Show A Message Box Explaining What Went Wrong

				MessageBox(NULL, cgGetLastListing(liquidContext),
					//cgGetErrorString(Error),
						   "Error", MB_OK);
				return false;	
			}

		// Load The Program

			cgGLLoadProgram(MakeWave);


		// Get The Latest GL Fragment Profile

			liquidFP = cgGLGetLatestProfile(CG_GL_FRAGMENT);	

		// Validate Our Profile Determination Was Successful

			if (liquidFP == CG_PROFILE_UNKNOWN)
			{
				MessageBox(NULL, "Invalid profile type (liquids initialization)", "Error", MB_OK);
				return false;	
			}

		// Set The Current Profile

			cgGLSetOptimalOptions(liquidFP);		

		// Load And Compile The Vertex Shader From File

			passFragment = cgCreateProgramFromFile(liquidContext, CG_SOURCE, LIQUID_SHADER_PATH, liquidFP, LIQUID_FRAGMENT_SHADER, 0);

		// Validate Success

			if (passFragment == NULL)
			{
			// We Need To Determine What Went Wrong

				CGerror Error = cgGetError();

			// Show A Message Box Explaining What Went Wrong

				MessageBox(NULL, cgGetLastListing(liquidContext),
					//cgGetErrorString(Error),
						   "Error", MB_OK);
				return false;	
			}

		// Load The Program

			cgGLLoadProgram(passFragment);


		// ustawienie parametrow CG
		
			wave					= cgGetNamedParameter(MakeWave, "IN.wave");
			level					= cgGetNamedParameter(MakeWave, "IN.level");
			liquidModelViewMatrix	= cgGetNamedParameter(MakeWave, "ModelViewProj");

			// te zmienne beda ustawiane w Light.h (funkcja updateCGlightParams())
			// oraz lightingProc() w LIGHT_CHECK:

				liquidLightPosition[0]  = cgGetNamedParameter(MakeWave, "lightPosition[0]");
				liquidLightPosition[1]  = cgGetNamedParameter(MakeWave, "lightPosition[1]");
				liquidLightPosition[2]  = cgGetNamedParameter(MakeWave, "lightPosition[2]");
				liquidLightPosition[3]  = cgGetNamedParameter(MakeWave, "lightPosition[3]");
				liquidLightPosition[4]  = cgGetNamedParameter(MakeWave, "lightPosition[4]");
				liquidLightPosition[5]  = cgGetNamedParameter(MakeWave, "lightPosition[5]");
				liquidLightPosition[6]  = cgGetNamedParameter(MakeWave, "lightPosition[6]");
				liquidLightPosition[7]  = cgGetNamedParameter(MakeWave, "lightPosition[7]");
				liquidEyePosition	    = cgGetNamedParameter(MakeWave, "eyePosition");
				liquidKa			    = cgGetNamedParameter(MakeWave, "Ka");
				liquidKd[0]			    = cgGetNamedParameter(MakeWave, "Kd[0]");
				liquidKd[1]			    = cgGetNamedParameter(MakeWave, "Kd[1]");
				liquidKd[2]			    = cgGetNamedParameter(MakeWave, "Kd[2]");
				liquidKd[3]			    = cgGetNamedParameter(MakeWave, "Kd[3]");
				liquidKd[4]			    = cgGetNamedParameter(MakeWave, "Kd[4]");
				liquidKd[5]			    = cgGetNamedParameter(MakeWave, "Kd[5]");
				liquidKd[6]			    = cgGetNamedParameter(MakeWave, "Kd[6]");
				liquidKd[7]			    = cgGetNamedParameter(MakeWave, "Kd[7]");
				liquidLightCount        = cgGetNamedParameter(MakeWave, "lightCount");
				liquidEnableLighting    = cgGetNamedParameter(MakeWave, "enableLighting");
			

			return true;	
	}


	void CloseLiquids()
	{
		// usuniecie wszystkich tekstur z pamieciu karty graficznej

			removeLiquidTextures();

		// usuniecie kontekstu cg

			cgDestroyContext(liquidContext);
	}