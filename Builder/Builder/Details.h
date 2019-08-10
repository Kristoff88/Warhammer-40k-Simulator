#pragma once


// Zmienne, struktury

	HWND details = NULL;
	
	unsigned int detailBrush = 0;
	
	// do zapisu / odczytu poszczegolnych tekstur
		struct detailTextureObj
		{
			char	        filename[MAX_PATH];
			unsigned int    textureID;
		};

	std::vector <detailTextureObj> detailTextureArray;

	bool	Flat = false; // indykuje, jaki rodzaj detalu nalezy wygenerowac


		struct detailObject
		{
				unsigned int  textureID;

				float		  R, G, B, A,
							 *rotationPtr,
							  
							 (*surface)[3],
							 (*texCoords)[2],
							 (*normals)[3];

						// licznik uzywany do tablic powyzej
				int			  indexCounter;

				bool		  Flat;

				detailObject(): surface(NULL), texCoords(NULL), normals(NULL)
				{	
						rotationPtr = new float [3];	
				}

				~detailObject()
				{	

						if( rotationPtr != NULL )
							delete [] rotationPtr;


						if( surface != NULL )
							delete [] surface;

						if( texCoords != NULL )
							delete [] texCoords;

						if( normals != NULL )
							delete [] normals;

				}
		};


	float	 yAdditive = 0.0001f; // podnosi lekko wysokosc kazdego detalu (zaklada sie, ze ne bedzie na scenie wiecej detali niz pare set)
								// dzieki temu detale nie nakladaja sie



// Funkcje


	void computeDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
	{

			detailObject *detail = reinterpret_cast < detailObject* > (object);


			if( !detail->Flat )
			{

			// dla zwiekszenia czytelnosci kodu i zmniejszeniu ilosci bazgrania...
				#define sizeX (sizeX/MAP_SCALE)
				#define sizeZ (sizeZ/MAP_SCALE)

					
					// czyszczenie tablic

						if( detail->surface != NULL )
							delete [] detail->surface;

						if( detail->texCoords != NULL )
							delete [] detail->texCoords;

						if( detail->normals != NULL )
							delete [] detail->normals;


					// obliczenie potrzebnego rozmiaru tablic

						detail->indexCounter = 0;

						for(int posZ = 0; posZ < sizeZ*2; posZ++)
						for(int posX = 0; posX < sizeX*2; posX++)
						{
							detail->indexCounter += 4;
						}


					// rezerwacja pamieci dla potrzebnych tablic

						detail->surface =	new float [detail->indexCounter][3];
						detail->texCoords = new float [detail->indexCounter][2];
						detail->normals =   new float [detail->indexCounter][3];

					// obliczenie powierzchni oraz coord'ow tekstury

						int terrainIndex, X, Z,
							arraysIndex = 0;

						float yModifier = y - terrainHeight;


						for(int posZ = 0; posZ < sizeZ*2; posZ++)
						for(int posX = 0; posX < sizeX*2; posX++)
						{

							// (0,0)
								X =  ( x  / MAP_SCALE ) - sizeX + posX;
								Z =  (-z  / MAP_SCALE ) - sizeZ + posZ;

								terrainIndex = X + MAP_X*Z;
								
								
								detail->texCoords[arraysIndex][0] = (float)posX / (sizeX*2);
								detail->texCoords[arraysIndex][1] = (float)posZ / (sizeZ*2);
									
									
								detail->surface[arraysIndex][0] = g_terrain[terrainIndex][0];
								detail->surface[arraysIndex][1] = g_terrain[terrainIndex][1]+.1f + yModifier;
								detail->surface[arraysIndex][2] = g_terrain[terrainIndex][2];

								arraysIndex++;


							// (1,0)
								X =  ( x  / MAP_SCALE ) - sizeX + posX + 1;
								Z =  (-z  / MAP_SCALE ) - sizeZ + posZ;

								terrainIndex = X + MAP_X*Z;

								
								detail->texCoords[arraysIndex][0] = ((float)posX + 1.f) / (sizeX*2);
								detail->texCoords[arraysIndex][1] = ((float)posZ ) / (sizeZ*2);


								detail->surface[arraysIndex][0] = g_terrain[terrainIndex][0];
								detail->surface[arraysIndex][1] = g_terrain[terrainIndex][1]+.1f + yModifier;
								detail->surface[arraysIndex][2] = g_terrain[terrainIndex][2];

								arraysIndex++;


							// (1,1)
								X =  ( x  / MAP_SCALE ) - sizeX + posX + 1;
								Z =  (-z  / MAP_SCALE ) - sizeZ + posZ + 1;

								terrainIndex = X + MAP_X*Z;

								
								detail->texCoords[arraysIndex][0] = ((float)posX + 1.f) / (sizeX*2);
								detail->texCoords[arraysIndex][1] = ((float)posZ + 1.f) / (sizeZ*2);


								detail->surface[arraysIndex][0] = g_terrain[terrainIndex][0];
								detail->surface[arraysIndex][1] = g_terrain[terrainIndex][1]+.1f + yModifier;
								detail->surface[arraysIndex][2] = g_terrain[terrainIndex][2];

								arraysIndex++;


							// (0,1)
								X =  ( x  / MAP_SCALE ) - sizeX + posX;
								Z =  (-z  / MAP_SCALE ) - sizeZ + posZ + 1;

								terrainIndex = X + MAP_X*Z;

								
								detail->texCoords[arraysIndex][0] = ((float)posX ) / (sizeX*2);
								detail->texCoords[arraysIndex][1] = ((float)posZ + 1.f) / (sizeZ*2);
								

								detail->surface[arraysIndex][0] = g_terrain[terrainIndex][0];
								detail->surface[arraysIndex][1] = g_terrain[terrainIndex][1]+.1f + yModifier;
								detail->surface[arraysIndex][2] = g_terrain[terrainIndex][2];

								

							// inkrementacja uniwerslanego indeksu
								arraysIndex++;
						}
			

				// sprzatanie...
				#undef sizeX
				#undef sizeZ


			// petla "kradnaca" normale terenu

				int index;

				for(int i = 0; i < detail->indexCounter; i++)
				{
						index = (detail->surface[i][0] / MAP_SCALE) + MAP_X * (-detail->surface[i][2] / MAP_SCALE);

						detail->normals[i][0] = g_normals[index][0]; 
						detail->normals[i][1] = g_normals[index][1];
						detail->normals[i][2] = g_normals[index][2];	
				}

			}// if (!Flat)
	}






	void adjustDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
	{
		// funkcja ta bedzie wykonywana, przy evencie morphingu

			detailObject *detail = reinterpret_cast < detailObject* > (object);

			if( !detail->Flat )
			{

				// korygacja detalu

					computeDetail(x, y, z, sizeX, sizeY, sizeZ, object);

				
				// na podstawie roznicy miedzy najmniejszym, a najwiekszym punktem detalu,
				// zostanie obliczone sizeY ( w celu umozliwienia poprawnego Select'owania detalu )

					float minY = MAX_TERRAIN_HEIGHT, 
						  maxY = MIN_TERRAIN_HEIGHT;


					for(int i = 0; i < detail->indexCounter; i++)
					{

						if( detail->surface[i][1] < minY )
							minY = detail->surface[i][1];

						if( detail->surface[i][1] > maxY )
							maxY = detail->surface[i][1];

					}

			}// if( !detail->Flat )
	}



	bool preDetail()
	{
		
			if( showDetails )
			{
				glEnable(GL_TEXTURE_2D);
				glDisable(GL_CULL_FACE);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.f);

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				return true;
			}
			else
				return false;

	}
	void drawDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ,  void *object = NULL, bool Selected = false)
	{
		
			detailObject *detail = reinterpret_cast < detailObject* > (object);

			glBindTexture(GL_TEXTURE_2D, detail->textureID);

			if( Selected )
				glColor4f(detail->R * 0.35f, detail->G * 0.8f, detail->B * 0.35f, detail->A);
			else
				glColor4f(detail->R, detail->G, detail->B, detail->A);


			glBegin(GL_QUADS);

				//float yModifier = y - terrainHeight;

				for(int i = 0; i < detail->indexCounter; i++)
				{	
					glTexCoord2fv( detail->texCoords[i] );
					glNormal3fv( detail->normals[i] );
					glVertex3fv( detail->surface[i] );
				}
			
			glEnd();

	}


	void drawFlatDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ,  void *object = NULL, bool Selected = false)
	{

			detailObject *detail = reinterpret_cast <detailObject*> (object);

			glBindTexture(GL_TEXTURE_2D, detail->textureID);

			glPushMatrix();

				if( Selected )
					glColor4f(.35f, .8f, .35f, detail->A);
				else
					glColor4f(detail->R, detail->G, detail->B, detail->A);

				glTranslatef( x, y, z );


				glRotatef(*detail->rotationPtr, 1.f, 0.f, 0.f);
				detail->rotationPtr++;

				glRotatef(*detail->rotationPtr, 0.f, 1.f, 0.f);
				detail->rotationPtr++;

				glRotatef(*detail->rotationPtr, 0.f, 0.f, 1.f);
		

				detail->rotationPtr -= 2;
			

				glBegin(GL_QUADS);

					glTexCoord2f(1.f, 1.f);
					glVertex3f( sizeX, 0.1f,  sizeZ);


					glTexCoord2f(1.f, 0.f);
					glVertex3f( sizeX, 0.1f, -sizeZ);


					glTexCoord2f(0.f, 0.f);
					glVertex3f(-sizeX, 0.1f, -sizeZ);


					glTexCoord2f(0.f, 1.f);
					glVertex3f(-sizeX, 0.1f,  sizeZ);

				glEnd();

			glPopMatrix();
				
	}




	void postDetail()
	{
			glBindTexture(GL_TEXTURE_2D, 0);

			glDisable(GL_BLEND);

			glDisable(GL_TEXTURE_2D);
			glEnable(GL_CULL_FACE);
	}
	void moveDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
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


			if( y < MIN_TERRAIN_HEIGHT )
				y = MIN_TERRAIN_HEIGHT;


		// korygacja roznorakich wartosci detalu
	
			detailObject *detail = reinterpret_cast < detailObject* > (object);

			adjustDetail(x, y, z, sizeX, sizeY, sizeZ, object);


	}




	void detailKeyManager(char key, void *object, float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ)
	{

			detailObject *detail = reinterpret_cast < detailObject* > (object);

			switch( toupper(key) )
			{
				// zmiana rozmiarow detalu

				case 'W':

					if( 
						sizeX/MAP_SCALE < MAP_X/2 
									&& 
						(x/MAP_SCALE - sizeX/MAP_SCALE) > 1
									&&
						(x/MAP_SCALE + sizeX/MAP_SCALE) < MAP_X-1 
					  )

						sizeX += MAP_SCALE;

					Saved = false;

				break;


				case 'S':

					if( sizeX/MAP_SCALE > 1 )
						sizeX -= MAP_SCALE;

					Saved = false;

				break;


				case 'D':

					if( 
						sizeZ/MAP_SCALE < MAP_Z/2 
									 &&
						(-z/MAP_SCALE - sizeZ/MAP_SCALE) > 1
									 &&
						(-z/MAP_SCALE + sizeZ/MAP_SCALE) < MAP_Z-1 
					  )

						sizeZ += MAP_SCALE;

					Saved = false;

				break;


				case 'A':

					if( sizeZ/MAP_SCALE > 1 )
						sizeZ -= MAP_SCALE;

					Saved = false;

				break;



			// rotacja plaskiego detalu

				// Y
				case 'Q':
				{

					if( detail->Flat)
					{
						detail->rotationPtr++;
						*detail->rotationPtr += 2.f;

						if( *detail->rotationPtr >= 360.f )
							*detail->rotationPtr = 0.f;

						detail->rotationPtr--;

						Saved = false;
					}
				}
				break;


				case 'E':
				{
					
					if( detail->Flat)
					{
						detail->rotationPtr++;
						*detail->rotationPtr -= 2.f;

						if( *detail->rotationPtr <= -360.f )
							*detail->rotationPtr = 0.f;

						detail->rotationPtr--;

						Saved = false;
					}
				}
				break;



				// X

				case 'R':
				{

					if( detail->Flat)
					{
						*detail->rotationPtr += 2.f;

						if( *detail->rotationPtr >= 360.f )
							*detail->rotationPtr = 0.f;

						Saved = false;
					}
				}
				break;


				case 'T':
				{
					
					if( detail->Flat)
					{
						*detail->rotationPtr -= 2.f;

						if( *detail->rotationPtr <= -360.f )
							*detail->rotationPtr = 0.f;

						Saved = false;
					}
				}
				break;



				// Z

				case 'F':
				{

					if( detail->Flat)
					{
						detail->rotationPtr += 2;
						*detail->rotationPtr += 2.f;

						if( *detail->rotationPtr >= 360.f )
							*detail->rotationPtr = 0.f;

						detail->rotationPtr -= 2;

						Saved = false;
					}
				}
				break;


				case 'G':
				{

					if( detail->Flat)
					{
						detail->rotationPtr += 2;
						*detail->rotationPtr -= 2.f;

						if( *detail->rotationPtr <= -360.f )
							*detail->rotationPtr = 0.f;

						detail->rotationPtr -= 2;

						Saved = false;
					}
				}
				break;


				case 'L':
				
					detail->R = (float)GetDlgItemInt(details, IDC_DETAIL_RED,    FALSE, FALSE) / 255.f;
					detail->G = (float)GetDlgItemInt(details, IDC_DETAIL_GREEN,  FALSE, FALSE) / 255.f;
					detail->B = (float)GetDlgItemInt(details, IDC_DETAIL_BLUE,   FALSE, FALSE) / 255.f;
					detail->A = (float)GetDlgItemInt(details, IDC_DETAIL_ALPHA,  FALSE, FALSE) / 255.f;

					detail->textureID = detailBrush;

					Saved = false;

				break;

			}


		// korygacja detalu
			computeDetail(x, y, z, sizeX, sizeY, sizeZ, object);
	}




	void detailText(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
	{

			detailObject *detail = reinterpret_cast < detailObject* > (object);

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
			static char CurrentScale[50];

			sprintf_s( CurrentAction, "Action : Moving detail" ); 
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

			if( detail->Flat )
			{
				static char CurrentRotateX[50];
				static char CurrentRotateY[50];
				static char CurrentRotateZ[50];

				sprintf_s( CurrentRotateX, "Rotate X: %8.2f", *detail->rotationPtr++ );
				textOutput.push_back( CurrentRotateX );

				sprintf_s( CurrentRotateY, "Rotate Y: %8.2f", *detail->rotationPtr++ );
				textOutput.push_back( CurrentRotateY );

				sprintf_s( CurrentRotateZ, "Rotate Z: %8.2f", *detail->rotationPtr );
				textOutput.push_back( CurrentRotateZ );

				detail->rotationPtr -= 2;
			}

	}
	void removeDetail(void *object)
	{
			detailCounter--;
	}




	void placeDetail()
	{

			if( detailBrush != 0 )
			{

				detailObject *newDetail = new detailObject;

				newDetail->R = (float)GetDlgItemInt(details, IDC_DETAIL_RED,    FALSE, FALSE) / 255.f;
				newDetail->G = (float)GetDlgItemInt(details, IDC_DETAIL_GREEN,  FALSE, FALSE) / 255.f;
				newDetail->B = (float)GetDlgItemInt(details, IDC_DETAIL_BLUE,   FALSE, FALSE) / 255.f;
				newDetail->A = (float)GetDlgItemInt(details, IDC_DETAIL_ALPHA,  FALSE, FALSE) / 255.f;

				newDetail->textureID = detailBrush;


				if( Flat )
				{
					*newDetail->rotationPtr = 0.f;
					newDetail->rotationPtr++;

					*newDetail->rotationPtr = 0.f;
					newDetail->rotationPtr++;

					*newDetail->rotationPtr = 0.f;

					newDetail->rotationPtr -= 2;
				}
				else
				{
				// dla tego wskaznika jest juz rezerwowana pamiec w konstruktorze detalu.
				// dlatego jezeli jest on niepotrzebny (detal nieplaski), nalezy zwolnic jego pamiec.

					delete [] newDetail->rotationPtr;
				}


				newDetail->Flat = Flat;


			// przygotowanie wlasciwosci detalu

				void (*ptrDrawDetail) (float &, float &, float &, float &, float &, float &, void *, bool);

				bool Scaled, morphAdjust;

				if(Flat)
				{
					ptrDrawDetail = drawFlatDetail;
					Scaled = false;
					morphAdjust = true;
				}
				else
				{
					ptrDrawDetail = drawDetail;
					Scaled = true;
					morphAdjust = false;
				}


				int index = cursorX/MAP_SCALE + MAP_X * -cursorZ/MAP_SCALE;


				ObjectOperator::Object newObject(cursorX, g_terrain[index][1] + yAdditive, cursorZ,

								 MAP_SCALE, MAP_SCALE * 100.f, MAP_SCALE,

								 newDetail,
								 preDetail,
								 ptrDrawDetail,
								 postDetail,
								 moveDetail,
								 removeDetail,
								 detailKeyManager,
								 detailText,
								 adjustDetail,
								 DETAIL_NAME,
								 Scaled,
								 morphAdjust,
								 FALSE);


			// wstepne obliczenie stanu detalu (zeby w ogole bylo widoczne)

				float x = cursorX,
					  y = g_terrain[index][1] + yAdditive,
					  z = cursorZ,

					  size = MAP_SCALE;
				
				void *helper = newDetail;

				adjustDetail(x, y, z, size, size, size, helper);

			// dodanie nowego obiektu

				objectOperator.Add(newObject);

			// inkrementacja licznika detali

				detailCounter++;

			// yAdditive jest uzywane do lekkiego podnoszenia kolejnych obiektow detali.
			// jako ze zaklada sie niewielka ich ilosc, nie powinno to dac negatywnego efektu
			// a zapewni to "przezroczystosc" detali" (nie beda sie nakladac)

				yAdditive += 0.0001f;

			}
			else
				MessageBox(g_hwnd, "Detail texture hasn't been chosen.", "Error!", MB_OK);

	}



	void removeAllDetails()
	{

			if ( !objectOperator.removeSpecificObjects(DETAIL_NAME) )
				MessageBox(g_hwnd, "There are no details on the map.", "Error!", MB_OK);

	}













// --------------------------------------------

	unsigned int generateDetailTexture(unsigned char *bits, unsigned int &width, unsigned int &height)
	{
	//wygeneruj nowy obiekt tekstury

		glGenTextures(1, &detailBrush);
		glBindTexture(GL_TEXTURE_2D, detailBrush);
	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		// Don't forget to use GL_RGBA for our new image data... we support Alpha transparency now!
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
					  GL_RGBA, GL_UNSIGNED_BYTE, bits );


		glBindTexture(GL_TEXTURE_2D, 0);


		return detailBrush;
	}



	bool loadDetailTexture(char *filename)
	{

			bool originalTexture;   //nie ma sensu generowac "nowej" tekstury

		// sprawdzenie "oryginalnosci" tekstury

			if(detailTextureArray.empty())
				originalTexture = true;
			else
			{
				originalTexture = true;

				for(int i = 0; i < detailTextureArray.size(); i++)
				{
					if( strcmp(filename, detailTextureArray[i].filename) == 0 )
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
					LoadTexture(filename, TRUE, generateDetailTexture);
				}
				catch(std::string error)
				{
					MessageBox(g_hwnd, error.c_str(), "Error!", MB_OK);
					return false;
				}

			// dodaj nowy obiekt tekstury + sciezke do wektora tekstur

				detailTextureObj newDetailTexture;
				strcpy_s(newDetailTexture.filename, filename);
				newDetailTexture.textureID = detailBrush;

				detailTextureArray.push_back(newDetailTexture);

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

				HWND comboBox = GetDlgItem(details, IDC_DETAIL_COMBO);

				ComboBox_AddString(comboBox, newItemName.c_str());

			// wyzerowanie brusha

				detailBrush = 0;
			}


			return originalTexture;
	}





	int  loadDetailTextures()
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

				directory = FindFirstFileA("TerrainData\\Textures\\Detail\\*.bmp", &data);

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

				directory = FindFirstFileA("TerrainData\\Textures\\Detail\\*.jpg", &data);

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

				directory = FindFirstFileA("TerrainData\\Textures\\Detail\\*.TGA", &data);

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
				strcat_s( szFileName, "TerrainData\\Textures\\Detail\\" );
				strcat_s( szFileName, files[i].c_str() );


			// Ladowanie tekstury o nazwie szFileName

				if( loadDetailTexture(szFileName) )
				{
					counter++;
				}
			}


			return counter;
	}


	void reloadDetailTextures()
	{
		int newTextures = loadDetailTextures();

		char buffer[MAX_PATH] = "";

		_itoa_s( newTextures, buffer, 10 );

		if( newTextures == 1 )
			strcat_s( buffer, " new texture loaded!" );
		else
			strcat_s( buffer, " new textures loaded!" );

		MessageBox(g_hwnd, buffer, "Reloading finished.", MB_OK);
	}
	void changeDetailTexture()
	{
		
			std::string cbIndexText;
			char buffer[MAX_PATH] = "";

			HWND comboBox = GetDlgItem(details, IDC_DETAIL_COMBO);
			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			cbIndexText.insert(0, buffer);

			if( !cbIndexText.empty() )
			{
				for(int i = 0; i < detailTextureArray.size(); i++)
				{
					std::string helper;
					helper.insert(0, detailTextureArray[i].filename);

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
						detailBrush = detailTextureArray[i].textureID;

						// ustaw obrazek ...
						HBITMAP hBitmap;
						hBitmap = (HBITMAP)LoadImage(NULL, detailTextureArray[i].filename, IMAGE_BITMAP, 128, 128,
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

						HWND hPicture = GetDlgItem(details, IDC_DETAIL);
						SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); 

						break;
					}
				}


				// sygnalizacja potrzeby odswiezenia tekstury

					objectOperator.keyPressed = 'L';

			}// !cbIndexText.empty()
			else
			{
				HWND hPicture = GetDlgItem(details, IDC_DETAIL);
				SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, NULL); 
			}

	}







	void removeDetailTextures()
	{
		for(int i = 0; i < detailTextureArray.size(); i++)
		{
			glDeleteTextures(1, &detailTextureArray[i].textureID);
		}
	}
// --------------------------------------------

	void saveDetails(char *filename)
	{
			
			std::fstream file;

		// otworzenie pliku w trybie "nadpisz" + binarny

			file.open( filename, std::ios::out | std::ios::app | std::ios::binary );

		// ustawienie markera poczatkowego

			file << "\nDetails\n";

		// zapisanie licznika detali

			file << "\t" << detailCounter << "\n";

		// przeszukiwanie wektora obiektow w poszukiwaniu obiektow detalu

			int counter = 0;

			for(int obj = 0; obj < objectOperator.objectArray.size(); obj++)
			{
			// znaleziono detal!

				if( objectOperator.objectArray[obj].GetType() == DETAIL_NAME )
				{
				// inkrementacja kontrolnej zmiennej

					counter++;

				// pobranie obiektu detalu

					detailObject *detail = reinterpret_cast <detailObject*> ( objectOperator.objectArray[obj].GetObject() );

				// zapis danych obiektu (pozycja, rozmiar)

					Vector3 position = objectOperator.objectArray[obj].GetPosition(), 
						    size     = objectOperator.objectArray[obj].GetSize();

					file << "\t" << position.posX << " "
						         << position.posY << " "
								 << position.posZ << "\n";

					file << "\t" << size.posX << " "
						         << size.posY << " "
								 << size.posZ << "\n";
				
				// zapis tekstury detalu

					bool TextureFound = false;

					for(int i = 0; i < detailTextureArray.size(); i++)
					{
						if( detail->textureID == detailTextureArray[i].textureID )
						{
							file << "\t" << StripName( detailTextureArray[i].filename, false ) << "\n";
							
							TextureFound = true;
							break;
						}
					}

					assert( TextureFound );

				// zapis wartosci opisujacych kolor detalu

					file << "\t" << detail->R << " "
						         << detail->G << " "
								 << detail->B << " "
								 << detail->A << "\n";

				// zapis "plaskosci" detalu

					file << "\t" << detail->Flat << "\n";

				// jezeli jest plaski, to nalezy zapisac stan tablicy obrotu (wokol osi X, Y, Z)

					if( detail->Flat )
					{
						for(int i = 0; i < 3; i++)
						{
							file << "\t" << detail->rotationPtr[i] << "\n";
						}
					}
				}

			}

		// kontrolny assert

			assert( detailCounter == counter );

		// ustawienie markera koncowego

			file << "End\n\n";

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();
	}

	bool loadDetails(char *filename)
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

				if( line == "Details" )
				{
					if( !markerFound )
					{
						markerFound = true;
					}
					else
					{
						loadingCorrect = false;
						errorInfo += "There are two or more detail info markers (should be one).";
						break;
					}
				}

				if( markerFound && !loadingCommenced )
				{
				// ustawienie falgi

					loadingCommenced = true;

				// pobranie licznika detali

					file >> line;

					if( line == "End"  )
					{
						loadingCorrect = false;
						errorInfo += "\"End\" marker found too early, while loading detailObject values.";
						break;
					}
					else if( !IsNumber(line, true) )
					{
						loadingCorrect = false;
						errorInfo += "Word where detailCounter value should be, isn't a number!\n\n";
						errorInfo += "\"";
						errorInfo += line;
						errorInfo += "\"";
						break;
					}
					else
					{
						detailCounter = atoi(line.c_str());
					}

					for(int d = 0; d < detailCounter; d++)
					{
					// zmienne, ktore beda przechowywac dane detalu

						Vector3		  position, size;
						float		  color[4], rotation[3];
						bool		  Flat;
						unsigned int  textureID;

					// pobranie pozycji i rozmiaru detalu

						for(int i = 0; i < 6; i++)
						{
							file >> line;

							if( line == "End"  )
							{
								loadingCorrect = false;
								errorInfo += "\"End\" marker found too early, while loading detailObject values.";
								break;
							}
							else if( !IsNumber(line, false) )
							{
								loadingCorrect = false;
								errorInfo += "Word where detailObject position/size value should be, isn't a number!\n\n";
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
							errorInfo += "\"End\" marker found too early, while loading detailObject values.";
							break;
						}

						bool TextureFound = false;

						for(int i = 0; i < detailTextureArray.size(); i++)
						{
							if( line == StripName( detailTextureArray[i].filename, false ) )
							{
								textureID = detailTextureArray[i].textureID;
								TextureFound = true;
								break;
							}
						}

						if( !TextureFound )
						{
							loadingCorrect = false;
							errorInfo += "Texture name hasn't been found in detailTextureArray register!";
							break;
						}

					// pobranie kolorow

						for(int i = 0; i < 4; i ++)
						{
							file >> line;

							if( line == "End"  )
							{
								loadingCorrect = false;
								errorInfo += "\"End\" marker found too early, while loading detailObject values.";
								break;
							}
							else if( !IsNumber( line, false ) )
							{
								loadingCorrect = false;
								errorInfo += "Word where detailObject color value should be, isn't a number!\n\n";
								errorInfo += "\"";
								errorInfo += line;
								errorInfo += "\"";
								break;
							}
							else
							{
								color[i] = atof(line.c_str());
							}
						}

					// pobranie "plaskosci"

						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading detailObject values.";
							break;
						}
						else if( !IsNumber( line, true ) )
						{
							loadingCorrect = false;
							errorInfo += "Word where detailObject Flat value should be, isn't a number!\n\n";
							errorInfo += "\"";
							errorInfo += line;
							errorInfo += "\"";
							break;
						}
						else
						{
							Flat = atoi(line.c_str());
						}

					// jezeli jest plaski, nalezy pobrac tablice rotacji

						if( Flat )
						{
							for(int i = 0; i < 3; i++)
							{
								file >> line;
			
								if( line == "End"  )
								{
									loadingCorrect = false;
									errorInfo += "\"End\" marker found too early, while loading detailObject values.";
									break;
								}
								else if( !IsNumber( line, false ) )
								{
									loadingCorrect = false;
									errorInfo += "Word where detailObject rotation value should be, isn't a number!\n\n";
									errorInfo += "\"";
									errorInfo += line;
									errorInfo += "\"";
									break;
								}
								else
								{
									rotation[i] = atof(line.c_str());
								}
							}
						}

					// przygotowanie nowego detalu do umieszczenia go w wektorze obiektow

						detailObject *newDetail = new detailObject;

						newDetail->R = color[0];
						newDetail->G = color[1];
						newDetail->B = color[2];
						newDetail->A = color[3];

						newDetail->textureID = textureID;

						if( Flat )
						{
							*newDetail->rotationPtr = rotation[0];
							newDetail->rotationPtr++;

							*newDetail->rotationPtr = rotation[1];
							newDetail->rotationPtr++;

							*newDetail->rotationPtr = rotation[2];

							newDetail->rotationPtr -= 2;
						}
						else
						{
						// dla tego wskaznika jest juz rezerwowana pamiec w konstruktorze detalu.
						// dlatego jezeli jest on niepotrzebny (detal nieplaski), nalezy zwolnic jego pamiec.

							delete [] newDetail->rotationPtr;
						}


						newDetail->Flat = Flat;


					// przygotowanie wlasciwosci detalu

						void (*ptrDrawDetail) (float &, float &, float &, float &, float &, float &, void *, bool);

						bool Scaled, morphAdjust;

						if(Flat)
						{
							ptrDrawDetail = drawFlatDetail;
							Scaled      = false;
							morphAdjust = true;
						}
						else
						{
							ptrDrawDetail = drawDetail;
							Scaled      = true;
							morphAdjust = false;
						}


						ObjectOperator::Object newObject(position.posX, position.posY, position.posZ,

														 size.posX, size.posY, size.posZ,

														 newDetail,
														 preDetail,
														 ptrDrawDetail,
														 postDetail,
														 moveDetail,
														 removeDetail,
														 detailKeyManager,
														 detailText,
														 adjustDetail,
														 DETAIL_NAME,
														 Scaled,
														 morphAdjust,
														 FALSE);


					// wstepne obliczenie stanu detalu (zeby w ogole bylo widoczne)
						
						float x     = position.posX,
							  y     = position.posY,
							  z     = position.posZ,
							  sizeX = size.posX,
							  sizeY = size.posY,
							  sizeZ = size.posZ;

						void *helper = newDetail;

						adjustDetail( x, y, z, sizeX, sizeY, sizeZ, helper );

						computeDetail( x, y, z, sizeX, sizeY, sizeZ, helper );

					// dodanie nowego obiektu

						objectOperator.Add(newObject);

					}// petla ladujaca detale
				
				}// if( markerFound && !loadingCommenced )
			}

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();


			if( !markerFound )
			{
				loadingCorrect = false;
				errorInfo += "\"Details\" marker hasn't been found!";
			}


			return loadingCorrect;
	}


// --------------------------------------------

	void CloseDetails()
	{
		removeDetailTextures();
	}