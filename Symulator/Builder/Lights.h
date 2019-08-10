#pragma once




	#define LIGHT_BULB_PATH				"Utilities\\LightBulb\\light_bulb.obj"	// sciezka do pliku z modelem zarowki
	#define LIGHT_BULB_TEXTURE_PATH		"Utilities\\LightBulb\\light_bulb.jpg"	

// Zmienne, struktury

	HWND	lighting = NULL;

	unsigned int lightBulbTextureID;	// identyfikator tekstury zarowki
	Model  lightBulb;				// struktura przechowujaca potrzebne info modelu zarowki




	struct LightSource
	{
		float ambient[4], diff[4], // wlasciwosci swiatla
			  pos[3];

		unsigned int   ID;		   // identyfikator swiatla
	};
//

	int	   lightCounter = 0,	// licznik swiatel
		   maxLights;			// zmienna przechowujaca max. liczbe swiatel (pobierana wartosc do niej w inicjalizacji swiatla)

	bool   *availableLights;  // wskaznik, ktory bedzie pokazywac na tablice dostepnych swiatel (ich liczba moze sie roznic w zaleznosci od implementacji OpenGL)




// Funkcje
	
	void updateCGlightParams()
	{
	// ustawienie pozycji kamery w shaderze

		cgSetParameter3f(terrainEyePosition, kamera.GetCameraPos().x, kamera.GetCameraPos().y, kamera.GetCameraPos().z);
		cgSetParameter3f(liquidEyePosition, kamera.GetCameraPos().x, kamera.GetCameraPos().y, kamera.GetCameraPos().z);

	// licznik swiatel

		int counter = 0;

	// tablica, do ktorej beda sumowane wartosci ambient light

		float ambientSum[3] = { 0, 0, 0 };

	// petla wyszukujaca obiekty swiatel w wektorze obiektow

		for(int i = 0; i < objectOperator.objectArray.size() && counter < lightCounter; i++)
		{
			if( objectOperator.objectArray[i].GetType() == LIGHT_BULB_NAME )
			{
				LightSource *light = reinterpret_cast <LightSource*> (objectOperator.objectArray[i].GetObject());

			// sumowanie wartosci ambient light

				ambientSum[0] += light->ambient[0];
				ambientSum[1] += light->ambient[1];
				ambientSum[2] += light->ambient[2];

			// ustawienie parametrow pozycji i diffuse light w tablicach shadera

				cgSetParameter3f(terrainLightPosition[counter], light->pos[0],  light->pos[1],  light->pos[2]);
				cgSetParameter3f(terrainKd[counter],            light->diff[0], light->diff[1], light->diff[2]);

				cgSetParameter3f(liquidLightPosition[counter], light->pos[0],  light->pos[1],  light->pos[2]);
				cgSetParameter3f(liquidKd[counter],            light->diff[0], light->diff[1], light->diff[2]);

			// inkrementacja licznika znalezionych swiatel

				counter++;
			}
		}

	// ustawienie sumy ambient lights w Ke w shaderze

		cgSetParameter3f(terrainKa, ambientSum[0], ambientSum[1], ambientSum[2]);
		cgSetParameter3f(liquidKa, ambientSum[0], ambientSum[1], ambientSum[2]);

	// ustawienie liczby swiatel

		cgSetParameter1f(terrainLightCount, lightCounter);
		cgSetParameter1f(liquidLightCount, lightCounter);
	}

	void corrigateLight(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
	{
		float lightPosition[] = {  x, y, z, 1.f  };

		LightSource *light;  
		light = reinterpret_cast < LightSource * > (object);	

		light->pos[0] = x;
		light->pos[1] = y;
		light->pos[2] = z;

		glLightfv(light->ID, GL_POSITION, lightPosition);

	// aktualizacja parametrow oswietlenia CG

		updateCGlightParams();
	}


	//void moveLight(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
	//{

	//		int index, X, Z;

	//		X =  x / MAP_SCALE;
	//		Z = -z / MAP_SCALE;

	//		index = X + MAP_X*Z;

	//		if( index < (MAP_X + MAP_X*MAP_Z) && index > 0)

	//			// detal nie moze znajdowac sie pod terenem...
	//			if( y <= g_terrain[index][1] )
	//				y = g_terrain[index][1]+0.1f;
	//			
	//			if( y < MIN_TERRAIN_HEIGHT )
	//				y = MIN_TERRAIN_HEIGHT;

	//		float lightPosition[] = {  x, y, z, 1.f  };

	//		LightSource *light;  
	//		light = reinterpret_cast < LightSource * > (object);

	//		glLightfv(light->ID, GL_AMBIENT,  light->ambient);
	//		glLightfv(light->ID, GL_DIFFUSE,  light->diff);

	//		light->pos[0] = x;
	//		light->pos[1] = y;
	//		light->pos[2] = z;

	//		glLightfv(light->ID, GL_POSITION, lightPosition);

	//	// aktualizacja parametrow oswietlenia CG

	//		updateCGlightParams();
	//}
	void removeLight(void *object)
	{
		LightSource *light;  
		light = reinterpret_cast < LightSource * > (object);

		glDisable(light->ID);

		int index = light->ID - GL_LIGHT0;
		availableLights[index] = true;

		lightCounter--;

	// aktualizacja parametrow oswietlenia CG

		updateCGlightParams();
	}


	bool preLight()
	{
		if( showLights )
		{
			glEnable(GL_TEXTURE_2D);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.f);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			return true;
		}
		else
			return false;
	}

	void drawLightBox(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object = NULL, bool Selected = false)
	{
		if( enableLighting )
			glDisable(GL_LIGHTING);

		glPushMatrix();
			
			glColor4f( 1.f, 1.f, 1.f, 1.f );

			glTranslatef(x, y + sizeY/2.f, z);

			glScalef( MAP_SCALE, MAP_SCALE, MAP_SCALE );

			// rysowanie VBO
			
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, lightBulb.vboIDv);
				glVertexPointer(3, GL_FLOAT, 0, 0);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, lightBulb.vboIDvt);
				glTexCoordPointer(2, GL_FLOAT, 0, 0);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, lightBulb.vboIDvn);
				glNormalPointer(GL_FLOAT, 0, 0);


				
				glBindTexture(GL_TEXTURE_2D, lightBulb.meshVector[0].textureID);
				glDrawArrays(GL_TRIANGLES, 0, lightBulb.meshVector[0].indexCounter*3);
				

		glPopMatrix();	

		if( enableLighting )
			glEnable(GL_LIGHTING);
	}



	void postLight()
	{
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glDisable(GL_BLEND);

		glDisable(GL_TEXTURE_2D);
	}


//
// --------------------------------------------

	bool loadLights(char *filename)
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

			if( line == "Lights" )
			{
				if( !markerFound )
				{
					markerFound = true;
				}
				else
				{
					loadingCorrect = false;
					errorInfo += "There are two or more light info markers (should be one).";
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
					errorInfo += "\"End\" marker found too early, while loading LightSource values.";
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
					lightCounter = atoi(line.c_str());
				}

				for(int l = 0; l < lightCounter; l++)
				{
				// zmienne, ktore beda przechowywac dane detalu

					Vector3		  position, size;
					float         pos[3], amb[4], diff[4];

				// pobranie pozycji i rozmiaru detalu

					for(int i = 0; i < 6; i++)
					{
						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading LightSource values.";
							break;
						}
						else if( !IsNumber(line, false) )
						{
							loadingCorrect = false;
							errorInfo += "Word where LightSource position/size value should be, isn't a number!\n\n";
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
					}// for ladujacy pozycje i rozmiar obiektu
				

				// pobranie wlasnosci swiatelka

					for(int i = 0; i < 3; i ++)
					{
						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading LightSource values.";
							break;
						}
						else if( !IsNumber( line, false ) )
						{
							loadingCorrect = false;
							errorInfo += "Word where LightSource value should be, isn't a number!\n\n";
							errorInfo += "\"";
							errorInfo += line;
							errorInfo += "\"";
							break;
						}
						else
						{
							pos[i] = atof(line.c_str());
						}
					}

					for(int i = 0; i < 4; i ++)
					{
						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading LightSource values.";
							break;
						}
						else if( !IsNumber( line, false ) )
						{
							loadingCorrect = false;
							errorInfo += "Word where LightSource value should be, isn't a number!\n\n";
							errorInfo += "\"";
							errorInfo += line;
							errorInfo += "\"";
							break;
						}
						else
						{
							amb[i] = atof(line.c_str());
						}
					}

					for(int i = 0; i < 4; i ++)
					{
						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading LightSource values.";
							break;
						}
						else if( !IsNumber( line, false ) )
						{
							loadingCorrect = false;
							errorInfo += "Word where LightSource value should be, isn't a number!\n\n";
							errorInfo += "\"";
							errorInfo += line;
							errorInfo += "\"";
							break;
						}
						else
						{
							diff[i] = atof(line.c_str());
						}
					}

				// struktura nowego swiatelka

					LightSource *newLight = new LightSource;
				
				// odszukanie ID dostepnego swiatla w OpenGL

					int index = 0;
					while( availableLights[index] != true )
						index++;

					newLight->ID = GL_LIGHT0 + index;
					availableLights[index] = false;

				// pozycjonowanie

					float lightPosition[] = { position.x, position.y, position.z, 1.f };
					glLightfv(newLight->ID, GL_POSITION, lightPosition);

					newLight->pos[0] = lightPosition[0];
					newLight->pos[1] = lightPosition[1];
					newLight->pos[2] = lightPosition[2];

				// ustawienie wartosci oswietlenia

					newLight->ambient[0] = amb[0];
					newLight->ambient[1] = amb[1];
					newLight->ambient[2] = amb[2];
					newLight->ambient[3] = amb[3];

					newLight->diff[0] = diff[0];
					newLight->diff[1] = diff[1];
					newLight->diff[2] = diff[2];
					newLight->diff[3] = diff[3];

				// wlacz swiatelko

					glEnable(newLight->ID);

					glLightfv(newLight->ID, GL_AMBIENT,  newLight->ambient);
					glLightfv(newLight->ID, GL_DIFFUSE,  newLight->diff);


					ObjectOperator::Object newObject(position.x, position.y, position.z,
													 size.x,     size.y,     size.z,

													 newLight,
													 preLight,
													 drawLightBox,
													 postLight,
													 NULL,//moveLight,
													 removeLight,
													 NULL,
													 NULL,
													 corrigateLight,
													 LIGHT_BULB_NAME,
													 FALSE,
													 TRUE,
													 TRUE);

				// dodanie nowego obiektu

					objectOperator.Add(newObject);

				// aktualizacja parametrow oswietlenia CG

					updateCGlightParams();

				}// petla ladujaca swiatelka
			
			}// if( markerFound && !loadingCommenced )
		}

	// zamkniecie i wyczyszczenie strumienia

		file.close();
		file.clear();


		if( !markerFound )
		{
			loadingCorrect = false;
			errorInfo += "\"Lights\" marker hasn't been found!";
		}


		return loadingCorrect;
	}
//
// --------------------------------------------

	unsigned int generateLightBulbTexture(unsigned char *bits, unsigned int &width, unsigned int &height)
	{
		glGenTextures(1, &lightBulbTextureID);
		glBindTexture(GL_TEXTURE_2D, lightBulbTextureID);
	

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height,
					  GL_RGBA, GL_UNSIGNED_BYTE, bits );

		glBindTexture(GL_TEXTURE_2D, 0);


		return lightBulbTextureID;
	}



//
	void InitLighting()
	{
	// pobranie oraz ustawienie  modelu zarowki

		OBJstruct *newOBJ;

		try
		{
			newOBJ = OBJ_Loader::Singleton().LoadOBJFile(LIGHT_BULB_PATH);
		}
		catch( std::string error )
		{
			MessageBox(g_hwnd, error.c_str(), "Error! (Light bulb initialization)", MB_OK);
		}

	// pobranie tekstury dla zarowki

		try
		{
			LoadTexture(LIGHT_BULB_TEXTURE_PATH, TRUE, generateLightBulbTexture);
		}
		catch( std::string error )
		{
			MessageBox(g_hwnd, error.c_str(), "Error! (Light bulb initialization)", MB_OK);
		}



		assert( newOBJ->indexCounters.size() == 1 );

		Mesh lightBulbMesh;

		lightBulbMesh.indexCounter = newOBJ->indexCounters[0];
		lightBulbMesh.textureID = lightBulbTextureID;


		lightBulb.meshVector.push_back(lightBulbMesh);


	// generowanie VBO i przypisanie ich do identyfikatorow w modelObj

		glGenBuffersARB(1, &lightBulb.vboIDv);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, lightBulb.vboIDv);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, lightBulb.GetIndexCount()*3*sizeof(float), newOBJ->vertices,  GL_STATIC_DRAW_ARB);

		glGenBuffersARB(1, &lightBulb.vboIDvt);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, lightBulb.vboIDvt);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, lightBulb.GetIndexCount()*2*sizeof(float), newOBJ->texCoords, GL_STATIC_DRAW_ARB);

		glGenBuffersARB(1, &lightBulb.vboIDvn);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, lightBulb.vboIDvn);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, lightBulb.GetIndexCount()*3*sizeof(float), newOBJ->normals,   GL_STATIC_DRAW_ARB);


		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);



	// obliczenie rozmiarow modelu (potrzebne do selekcji i wyswietlania jego ramki)

		float minX, minY, minZ, 
			  maxX, maxY, maxZ,

			  x, y, z;


		minX = maxX = newOBJ->vertices[0][0];
		minY = maxY = newOBJ->vertices[1][0];
		minZ = maxZ = newOBJ->vertices[2][0];



		for(int i = 0; i < newOBJ->vertexCount; i++)
		{
			x = newOBJ->vertices[i][0];
			y = newOBJ->vertices[i][1];
			z = newOBJ->vertices[i][2];

			if( maxX < x )	maxX = x;
			if( minX > x )	minX = x;

			if( maxY < y )	maxY = y;
			if( minY > y )	minY = y;

			if( maxZ < z )	maxZ = z;
			if( minZ > z )	minZ = z;
		}

		lightBulb.sizeX = (maxX - minX);// / 2.f;
		lightBulb.sizeY = (maxY - minY);
		lightBulb.sizeZ = (maxZ - minZ);// / 2.f;


		delete newOBJ;



	// ustawienie materialow

		float matAmb[4] =   { 1.f, 1.f, 1.f, 1.f }, 
			  matDiff[4] =  { 1.f, 1.f, 1.f, 1.f },
			  matEmiss[4] = { 0.f, 0.f, 0.f, 1.f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
		glMaterialfv(GL_FRONT, GL_EMISSION, matEmiss);

	// pobranie maksymalnej liczby swiatel

		glGetIntegerv(GL_MAX_LIGHTS, &maxLights);

		if( maxLights > 32 )
			maxLights = 32;

	// stworzenie tablicy dostepnych swiatelek

		availableLights = new bool[maxLights];

		for(int i = 0; i < maxLights; i++)
		{
			availableLights[i] = true;
		}
	}

	void CloseLighting()
	{
		if( availableLights != NULL )
		{
			delete [] availableLights;
			availableLights = NULL;
		}

	// usuniecie tekstury zarowki

		glDeleteTextures(1, &lightBulbTextureID);

	// wyczyszczenie vbo modelu zarowki

		glDeleteBuffersARB( 1, &lightBulb.vboIDv);
		glDeleteBuffersARB( 1, &lightBulb.vboIDvt);
		glDeleteBuffersARB( 1, &lightBulb.vboIDvn);
	}