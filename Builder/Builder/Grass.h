#pragma once



// Zmienne, struktury, klasy


	HWND	grass    = NULL;

	unsigned int grassBrush = 0;



	// rozmiary pedzelka "trawiastego"
	int			 grassRangeX = 1,
				 grassRangeZ = 1;


	bool		ShowGrass = true;	// wlacza/wylacza trawe


	// do zapisu / odczytu poszczegolnych tekstur
		struct grassTextureObj
		{
			char	        filename[MAX_PATH];
			unsigned int    textureID;
		};

	std::vector <grassTextureObj> grassTextureArray;



	// kepka trawy


	// przygotowanie sobie w tablicy coord'ow textur (do rysowania pojedynczej kepki), zeby sobie ulatwic zycie
		float grassTexCoords[5][2] = { {-0.25f, 0.f}, {1.25f,0.f}, 
											  {.5f, 1.f}, 
									 {-0.25f, 0.f}, {1.25f,0.f} };

		struct grassTuft
		{

				float vertexArray[5][3],  // kepka trawy bedzie posiadac 2 skrzyzowane trojkaty (z jednym wspolnym wierzcholkiem, ergo 5 vertexow)

					  size, height;


				grassTuft()
				{}

				grassTuft(float xPos, float yPos, float zPos, float Size, float Height): size(Size), height(Height)
				{

					// wylosowanie kata, o ktory wszystkie plane'y zostana obrocone
						   float yAngle = (rand() % 360) + 1;

						
							// kolejnosc punktow trojkata
							//
							//		 0
							//	     /\   
							//	    /  \   
							//	   /    \ 
							//	  /      \
							//	 /________\
							//  2          1


					// pierwszy trojkat

						vertexArray[0][0] = xPos + size;
						vertexArray[0][1] = yPos - height/2.f;
						vertexArray[0][2] = zPos;

						vertexArray[1][0] = xPos - size;
						vertexArray[1][1] = yPos - height/2.f;
						vertexArray[1][2] = zPos;


					// wierzcholek wspolny

						vertexArray[2][0] = xPos;
						vertexArray[2][1] = yPos + height * 2.f;
						vertexArray[2][2] = zPos;


					// drugi trojkat

						vertexArray[3][0] = xPos;
						vertexArray[3][1] = yPos - height/2.f;
						vertexArray[3][2] = zPos + size;

						vertexArray[4][0] = xPos;
						vertexArray[4][1] = yPos - height/2.f;
						vertexArray[4][2] = zPos - size;




				// jak juz all trojkaty sa przygotowane, mozna zaczac je rotowac o yAngle...

					for(int point = 0; point < 5; point++)
					{
						RotatePoint(xPos, yPos, zPos,
									vertexArray[point][0], vertexArray[point][1], vertexArray[point][2],
									0.f, yAngle, 0.f);
					}
					
				}


				~grassTuft()
				{}



			// funkcja rysujaca juz chyba nie bedzie potrzebna (grassPatch biedzie rysowac wszystkie kepki)
				/*void Draw()
				{

						glBegin(GL_TRIANGLES);

							for(int point = 0; point < 3; point++)
							{

								glTexCoord2f(grassTexCoords[point][0], grassTexCoords[point][1]);
								glVertex3f(vertexArray[point][0], vertexArray[point][1], vertexArray[point][2]);

							}

							for(int point = 2; point < 5; point++)
							{

								glTexCoord2f(grassTexCoords[point][0], grassTexCoords[point][1]);
								glVertex3f(vertexArray[point][0], vertexArray[point][1], vertexArray[point][2]);

							}

						glEnd();

				}*/

		};




// ------------------------------------------------------------------------
// ------------------------------------------------------------------------


	// poletko trawy, skladajace sie z kepek

		class grassPatch
		{
			public:

				unsigned int   textureID;		// id obiektu tekstury

				float		   R, G, B, A,		// wartosci koloru

							   points[4][3];	// koordynaty wierzcholkow (x,y,z) pola trawy

								// 0 ____ 1
								//  |   /|
								//  |  / |
								//  | /  |
								//  |/___|
								// 3      2

			// wektor kepek trawy

				std::vector <grassTuft>	   tuftVector; 


				grassPatch()
				{
				}

				grassPatch(float x, float y, float z, unsigned int texID, float red, float green, float blue, float alpha, float size, float height, float density): 
						   textureID(texID), R(red), G(green), B(blue), A(alpha)
				{	
						
					// ustawienie coord'ow poletka
						points[0][0] = x;
						points[0][1] = y+0.1f;
						points[0][2] = z;


						int index = (x/MAP_SCALE + 1) + MAP_X * (-z/MAP_SCALE);

						points[1][0] = x + MAP_SCALE;
						points[1][1] = g_terrain[index][1] + 0.1f;
						points[1][2] = z;


						index = (x/MAP_SCALE + 1) + MAP_X * (-z/MAP_SCALE + 1);

						points[2][0] = x + MAP_SCALE;
						points[2][1] = g_terrain[index][1] + 0.1f;
						points[2][2] = z + MAP_SCALE;


						index = (x/MAP_SCALE) + MAP_X * (-z/MAP_SCALE + 1);

						points[3][0] = x;
						points[3][1] = g_terrain[index][1] + 0.1f;
						points[3][2] = z + MAP_SCALE;



					// generowanie kepek trawy

						float xModifier,
							  zModifier;

						for(int i = 0; i < density; i++)
						{

								xModifier = (float)( rand() % (MAP_SCALE * 100) ) / 100.f;
								zModifier = (float)( rand() % (MAP_SCALE * 100) ) / 100.f;
				
							// nad Y trza popracowac
								tuftVector.push_back( grassTuft(x + xModifier, y, z - zModifier, size, height) );

						}

				}


				~grassPatch()
				{}



				//void Draw()
				//{

				//		glColor4f(1,1,1,1);
				//		//glColor4f(R, G, B, A);

				//		glBindTexture(GL_TEXTURE_2D, textureID);

				//		for(int i = 0; i < tuftVector.size(); i++)
				//			tuftVector[i].Draw();

				//		glBindTexture(GL_TEXTURE_2D, 0);

				//}



				void Morph()
				{

					// odswiezenie wysokosci wierzcholkow pola trawy

						int index = (points[0][0]/MAP_SCALE) + MAP_X * (-points[0][2]/MAP_SCALE);

							points[0][1] = g_terrain[index][1] + 0.1f;



						index = (points[0][0]/MAP_SCALE + 1) + MAP_X * (-points[0][2]/MAP_SCALE);

							points[1][1] = g_terrain[index][1] + 0.1f;



						index = (points[0][0]/MAP_SCALE + 1) + MAP_X * (-points[0][2]/MAP_SCALE + 1);

							points[2][1] = g_terrain[index][1] + 0.1f;



						index = (points[0][0]/MAP_SCALE) + MAP_X * (-points[0][2]/MAP_SCALE + 1);

							points[3][1] = g_terrain[index][1] + 0.1f;





						// wysokosci granicznych wierzcholkow
							float maxY1, maxY2,
								  minY1, minY2;

						// lewa krawedz...
							if( points[0][1] > points[1][1] )
							{
								maxY1 = points[0][1];
								minY1 = points[1][1];
							}
							else if( points[0][1] < points[1][1] )
							{
								maxY1 = points[1][1];
								minY1 = points[0][1];
							}
							else
							{
								maxY1 = minY1 = points[0][1];
							}



						// prawa krawedz
							if( points[3][1] > points[2][1] )
							{
								maxY2 = points[3][1];
								minY2 = points[2][1];
							}
							else if( points[3][1] < points[2][1] )
							{
								maxY2 = points[2][1];
								minY2 = points[3][1];
							}
							else
							{
								maxY2 = minY2 = points[3][1];
							}



						for(int i = 0; i < tuftVector.size(); i++)
						{

							// obliczenie jaka wysokosc miala by kepka ( o coord'ach (X,Z) ), jesli znajdowala by sie na jednej ze skrajnych krawedzi

								  // Y jaki mialaby kepka, gdyby znajdowala sie na lewej/prawej krawedzi
								float leftY, rightY;

				
								// prymitywne dzielenie modulo na floatach...

									float result,
										  search = 0.f;

									while( search < tuftVector[i].vertexArray[3][0] )
										search += MAP_SCALE;

									search -= MAP_SCALE;

									result = tuftVector[i].vertexArray[3][0] - search;



							// posiada wartosci od 0.0 do 1.0, bedzie sluzyc jako modyfikator przy interpolacji wzdluz krawedzi
								float Modifier = result / (float)MAP_SCALE;


							// liczenie dwoch przecinych Y'ekow, ktore posiadala by kepka, gdyby znajdowala sie na krawedzi

								leftY =  ( (maxY1 - minY1) * Modifier ) + minY1;

								rightY = ( (maxY2 - minY2) * Modifier ) + minY2;


								
								// kolejne prymitywne dzielenie modulo na floatach...

									search = 0.f;

									while( search < -tuftVector[i].vertexArray[3][2] )
										search += MAP_SCALE;

									search -= MAP_SCALE;

									result = -tuftVector[i].vertexArray[3][2] - search;


								Modifier = result / (float)MAP_SCALE;


							// uzywajac juz maxY1 i minY1, tym razem do okreslenia ktore z Y'ekow,
							// lezacych na krawedzi jest mniejsze, ktore wieksze

							if( leftY > rightY )
							{
								maxY1 = leftY;
								minY1 = rightY;
							}
							else if( leftY < rightY )
							{
								maxY1 = rightY;
								minY1 = leftY;
							}

							// jezeli oba sa takie same, to mozna przerwac petle!
							// nie ma sensu interpolowac!
							else
							{

								// pierwszy trojkat

									tuftVector[i].vertexArray[0][1] = maxY1 - tuftVector[i].height/2.f;
			
									tuftVector[i].vertexArray[1][1] = maxY1 - tuftVector[i].height/2.f;
					

								// wierzcholek wspolny

									tuftVector[i].vertexArray[2][1] = maxY1 + tuftVector[i].height * 2.f;

								// drugi trojkat

									tuftVector[i].vertexArray[3][1] = maxY1 - tuftVector[i].height/2.f;
								
									tuftVector[i].vertexArray[4][1] = maxY1 - tuftVector[i].height/2.f;


								return;
							}

								

							// teraz mozna interpolowac miedzy leftY a rightY

								// pierwszy trojkat

									tuftVector[i].vertexArray[0][1] = ( ( (maxY1 - minY1) * Modifier ) + minY1 ) - tuftVector[i].height/2.f;
			
									tuftVector[i].vertexArray[1][1] = ( ( (maxY1 - minY1) * Modifier ) + minY1 ) - tuftVector[i].height/2.f;
					

								// wierzcholek wspolny

									tuftVector[i].vertexArray[2][1] = ( ( (maxY1 - minY1) * Modifier ) + minY1 ) + tuftVector[i].height * 2.f;

								// drugi trojkat

									tuftVector[i].vertexArray[3][1] = ( ( (maxY1 - minY1) * Modifier ) + minY1 ) - tuftVector[i].height/2.f;
								
									tuftVector[i].vertexArray[4][1] = ( ( (maxY1 - minY1) * Modifier ) + minY1 ) - tuftVector[i].height/2.f;


						}// for...

				}// Morph

		};// GrassPatch



	std::vector <grassPatch> grassVector;		// wektor przechowujacy wszystkie obiekty traw



	// struktura zawierajaca VBO trawy ( w zaleznosci od ID tekstury jaka posiada grassPatch )

		class grassVBO
		{
			public:

				unsigned int	VBOv,
								VBOvt,
								VBOvn,
								VBOvc,

								textureID,
								
								indexCounter;

				grassVBO(): VBOv(0), VBOvt(0), VBOvn(0), VBOvc(0), textureID(0)
				{}
				~grassVBO()
				{}

		};

	std::vector <grassVBO> grassVBOarray;


// Funkcje


	void updateGrassVBOs()
	{

		// czyszczenie poprzednich VBO

			for(int i = 0; i < grassVBOarray.size(); i++)
			{
				if( grassVBOarray[i].VBOv != 0 )
					glDeleteBuffersARB(1, &grassVBOarray[i].VBOv);

				if( grassVBOarray[i].VBOvt != 0 )
					glDeleteBuffersARB(1, &grassVBOarray[i].VBOvt);

				if( grassVBOarray[i].VBOvn != 0 )
					glDeleteBuffersARB(1, &grassVBOarray[i].VBOvn);

				if( grassVBOarray[i].VBOvc != 0 )
					glDeleteBuffersARB(1, &grassVBOarray[i].VBOvc);
			}

		// jak juz VBO zostaly usuniete z pamieci, mozna usunac caly wektor VBO

			grassVBOarray.erase( grassVBOarray.begin(), grassVBOarray.end() );


		// patrzac sie na identyfikatory zaladowanych tekstur trawy, przeglada sie grassVector.
		// jezeli jakis grassPatch (lub wiele pol trawy) wykorzystuje jakies id tekstury, dodaje sie ich wierzcholki 
		// do nowej tablicy wierzcholkow

			float (*vertices)[3]  = NULL,
				  (*texCoords)[2] = NULL, 
				  (*normals)[3]   = NULL, 
				  (*colors)[4]    = NULL;

			bool TextureIsUsed = false;


			for(int texture = 0; texture < grassTextureArray.size(); texture++)
			{

			// odszukanie czy tekstura o ID z wektora tekstur trawy, jest wykorzystywana przez ktores z pol trawy

				for(int patch = 0; patch < grassVector.size(); patch++)
				{
					if( grassVector[patch].textureID == grassTextureArray[texture].textureID )
					{
						TextureIsUsed = true;
						break;
					}
				}

			// jezeli tak, to nalezy obliczyc ilosc wierzcholkow jakie posiadaja pola trawy korzystajace z tej tekstury.
			// nastepnie wyczyscic i zarezerwowac pamiec dla tablic, potrzebnych do utworzenia nowego VBO trawy.

				if( TextureIsUsed )
				{
				// zliczenie wymaganego rozmiaru nowych tablic

					unsigned int size = 0;

					for(int patch = 0; patch < grassVector.size(); patch++)
					{
						if( grassVector[patch].textureID == grassTextureArray[texture].textureID )
						{
						// inkrementacja wymaganego rozmiaru o ilosc kepek trawy na polu trawy razy 6 (jedna kepka = 2 trojkaty 5 wierzcholkow, z czego trzeci jest wspolny dla obu trojkatow)

							size += grassVector[patch].tuftVector.size() * 6;
						}
					}

				// rezerwacja pamieci
 
					vertices  = new float [size][3];
					texCoords = new float [size][2];
					normals   = new float [size][3];
					colors    = new float [size][4];


				// wypelnianie tablic

					int index = 0;

					for(int patch = 0; patch < grassVector.size(); patch++)
					{
						if( grassVector[patch].textureID == grassTextureArray[texture].textureID )
						{
							for(int tuft = 0;  tuft  < grassVector[patch].tuftVector.size(); tuft++)
							for(int point = 0; point < 5; point ++)
							{
								
								vertices[index][0] = grassVector[patch].tuftVector[tuft].vertexArray[point][0];
								vertices[index][1] = grassVector[patch].tuftVector[tuft].vertexArray[point][1];
								vertices[index][2] = grassVector[patch].tuftVector[tuft].vertexArray[point][2];
								
								texCoords[index][0] = grassTexCoords[point][0];
								texCoords[index][1] = grassTexCoords[point][1];

								normals[index][0] = 0.f;
								normals[index][1] = 1.f;
								normals[index][2] = 0.f;

								colors[index][0] = grassVector[patch].R;
								colors[index][1] = grassVector[patch].G;
								colors[index][2] = grassVector[patch].B;
								colors[index][3] = grassVector[patch].A;

								index++;

							// jezeli jest to srodkowy punkt miedzy dwoma trojkatami (VBO przechowuja dwa trojkaty a nie jeden pieciokat)

								if( point == 2 )
								{
									vertices[index][0] = grassVector[patch].tuftVector[tuft].vertexArray[point][0];
									vertices[index][1] = grassVector[patch].tuftVector[tuft].vertexArray[point][1];
									vertices[index][2] = grassVector[patch].tuftVector[tuft].vertexArray[point][2];
									
									texCoords[index][0] = grassTexCoords[point][0];
									texCoords[index][1] = grassTexCoords[point][1];

									normals[index][0] = 0.f;
									normals[index][1] = 1.f;
									normals[index][2] = 0.f;

									colors[index][0] = grassVector[patch].R;
									colors[index][1] = grassVector[patch].G;
									colors[index][2] = grassVector[patch].B;
									colors[index][3] = grassVector[patch].A;

									index++;
								}

							}
						}
					}


				// nowy obiekt VBO trawy

					grassVBO newGrassVBO;

				// generowanie VBO

					glGenBuffersARB( 1, &newGrassVBO.VBOv );		

					glBindBufferARB( GL_ARRAY_BUFFER_ARB, newGrassVBO.VBOv );			
					glBufferDataARB( GL_ARRAY_BUFFER_ARB, size*3*sizeof(float), vertices,	GL_DYNAMIC_DRAW_ARB );

			
					glGenBuffersARB( 1, &newGrassVBO.VBOvt );

					glBindBufferARB( GL_ARRAY_BUFFER_ARB, newGrassVBO.VBOvt );
					glBufferDataARB( GL_ARRAY_BUFFER_ARB, size*2*sizeof(float), texCoords,  GL_DYNAMIC_DRAW_ARB );


					glGenBuffersARB( 1, &newGrassVBO.VBOvn );

					glBindBufferARB( GL_ARRAY_BUFFER_ARB, newGrassVBO.VBOvn );
					glBufferDataARB( GL_ARRAY_BUFFER_ARB, size*3*sizeof(float), normals,	GL_DYNAMIC_DRAW_ARB );


					glGenBuffersARB( 1, &newGrassVBO.VBOvc );

					glBindBufferARB( GL_ARRAY_BUFFER_ARB, newGrassVBO.VBOvc );
					glBufferDataARB( GL_ARRAY_BUFFER_ARB, size*4*sizeof(float), colors,		GL_DYNAMIC_DRAW_ARB );
					
				// zbindowanie zerowego VBO (safety reasons)

					glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

				// przypisanie ID tekstury dla VBO trawy

					newGrassVBO.textureID = grassTextureArray[texture].textureID;

				// ustawienie licznika indeksow

					newGrassVBO.indexCounter = size;


				// dodanie nowego VBO trawy do wektora VBO

					grassVBOarray.push_back( newGrassVBO );


				// wyczyszczenie tablic

					if( vertices != NULL )
						delete [] vertices;

					if( texCoords != NULL )
						delete [] texCoords;

					if( normals != NULL )
						delete [] normals;

					if( colors != NULL )
						delete [] colors;

				// reset flagi

					TextureIsUsed = false;

				}// if( TextureIsUsed )

			}// for(int texture = 0; texture < grassTextureArray.size(); texture++)

	}



	void clearGrassVBOs()
	{
		// czyszczenie VBO

			for(int i = 0; i < grassVBOarray.size(); i++)
			{
				if( grassVBOarray[i].VBOv != 0 )
					glDeleteBuffersARB(1, &grassVBOarray[i].VBOv);

				if( grassVBOarray[i].VBOvt != 0 )
					glDeleteBuffersARB(1, &grassVBOarray[i].VBOvt);

				if( grassVBOarray[i].VBOvn != 0 )
					glDeleteBuffersARB(1, &grassVBOarray[i].VBOvn);

				if( grassVBOarray[i].VBOvc != 0 )
					glDeleteBuffersARB(1, &grassVBOarray[i].VBOvc);
			}

			grassVBOarray.erase( grassVBOarray.begin(), grassVBOarray.end() );

	}
// --------------------------------------------
	void dropGrass()
	{

		if( !grassVector.empty() )
		{

					std::vector <grassPatch> grassVectorCopy;	// do niego beda kopiowane te pola trawy, ktore nie wystaja poza granice terenu

					bool crossedBorder;	 // w trakcie petli bedzie on ustawiany na true, jesli chociaz jedna kepka pola trawy wystaje poza granice terenu

				// granice terenu

					unsigned int xBorder = MAP_X*MAP_SCALE,
								 zBorder = MAP_Z*MAP_SCALE;


					
					unsigned int workTime = grassVector.size();

					ProgressDialog progressDlg(g_hInstance, terrain, "Deleting grass...",
											   0, workTime, 1);

					for(int patch = 0; patch < grassVector.size(); patch++)
					{

						// resetowanie flagi
							
							crossedBorder = false;



						// skanowanie kepek trawy

							for(int tuft = 0;  tuft < grassVector[patch].tuftVector.size(); tuft++)
							{
								
								if( grassVector[patch].tuftVector[tuft].vertexArray[2][0] > xBorder
						/* Z jest ujemne */							||
								   -grassVector[patch].tuftVector[tuft].vertexArray[2][2] > zBorder )

								{
									// zasygnalizuj potrzebe usuniecia pola trawy
										crossedBorder = true;

									// jezeli chociaz jedna kepka wystaje, 
									// nie ma sensu dalej sprawdzac reszty kepek w danym polu trawy
										break;
								}
							}	


						// skopiowanie tych pol trawy, ktore nie wystaja za teren

							if( !crossedBorder )

								grassVectorCopy.push_back( grassVector[patch] );



							progressDlg.Increment();

					}// for( patches... )


				// wyczyszczenie wektora z trawy i skopiowanie tego, co sie ostalo

					grassVector.erase( grassVector.begin(), grassVector.end() );

					grassVector = grassVectorCopy;



				// update VBO trawy

					updateGrassVBOs();


		}// if( !grassVector.empty() )

	}




	void adjustGrassHeight(float modifier)
	{

			for(int patch = 0; patch < grassVector.size(); patch++)
			for(int tuft = 0; tuft < grassVector[patch].tuftVector.size(); tuft++)
			for(int point = 0; point < 5; point++)
			{
				grassVector[patch].tuftVector[tuft].vertexArray[point][1] += modifier;
			}

		// update VBO trawy

			updateGrassVBOs();
	}


	void rescaleGrass(float newScale, float oldScale)
	{
		// obliczenie modyfikatora skali

			float scale = newScale / oldScale;

		// przemnozenie wektora trawy przez ten modyfikator

			for(int patch = 0; patch < grassVector.size(); patch++)
			for(int tuft = 0; tuft < grassVector[patch].tuftVector.size(); tuft++)
			for(int point = 0; point < 5; point++)
			{
				grassVector[patch].tuftVector[tuft].vertexArray[point][0] *= scale;
				grassVector[patch].tuftVector[tuft].vertexArray[point][1] *= scale;
				grassVector[patch].tuftVector[tuft].vertexArray[point][2] *= scale;
			}

		// update VBO trawy

			updateGrassVBOs();
	}
	void updateGrass()
	{
			grassRangeX = GetDlgItemInt(grass, IDC_GRASS_X_EDIT, FALSE, FALSE) * MAP_SCALE;
			grassRangeZ = GetDlgItemInt(grass, IDC_GRASS_Z_EDIT, FALSE, FALSE) * MAP_SCALE;
	}


	void plantGrass()
	{

			updateGrass();

		// pobranie modyfikatorow koloru trawy

			float   red     =  GetDlgItemInt(grass, IDC_GRASS_RED,     FALSE, FALSE) / 255.f,
				    green   =  GetDlgItemInt(grass, IDC_GRASS_GREEN,   FALSE, FALSE) / 255.f,
					blue    =  GetDlgItemInt(grass, IDC_GRASS_BLUE,    FALSE, FALSE) / 255.f,
					alpha   =  GetDlgItemInt(grass, IDC_GRASS_ALPHA,   FALSE, FALSE) / 255.f;



			// kliknieto przycisk myszki oraz wybrano trawiasty pedzelek :)
			if( lButtonDown && grassBrush != 0 )
			{

					Vector3 wektor = GetOGLPos(g_mouseX, g_mouseY);

					wektor.posZ *= -1;

					int X, Z;

					if( MAP_SCALE > 1)
					{
						X = (int)wektor.posX - ( (int)wektor.posX % MAP_SCALE );
						Z = (int)wektor.posZ - ( (int)wektor.posZ % MAP_SCALE );
					}

					X = ((int)wektor.posX / MAP_SCALE);
					Z = ((int)wektor.posZ / MAP_SCALE);
				

					// zeby zmniejszyc ilosc iteracji sprawdzania
					int lastIndex = 0;
				
					for(int x = X - grassRangeX/MAP_SCALE; x < X + grassRangeX/MAP_SCALE; x++)
					for(int z = Z - grassRangeZ/MAP_SCALE; z < Z + grassRangeZ/MAP_SCALE; z++)
					{

						// sprawdzenie czy istnieje, juz poletko trawy o identycznej teksturze, na coord'ach (x,z)
						for(int i = 0; i < grassVector.size(); i++)
						{

							if( grassVector[i].textureID == grassBrush 
												&& 
								grassVector[i].points[0][0] == x*MAP_SCALE
												&& 
								grassVector[i].points[0][2] == -z*MAP_SCALE )
							{

									grassVector.erase(grassVector.begin() + i--);

								// nie jest mozliwe istnienie dwoch identycznych poletek na tych samych koordach,
								// a wiec mozna przerwac petle
									break;
							}

						}

						if(x < MAP_X-1 && x > 0 && z < (MAP_Z-1) && z >= 0)
						{


							// minimalne wartosci

								int	  minSize    =  GetDlgItemInt(grass, IDC_GRASS_SIZE,    FALSE, FALSE),
									  minHeight  =  GetDlgItemInt(grass, IDC_GRASS_HEIGHT,  FALSE, FALSE),
									  minDensity =  GetDlgItemInt(grass, IDC_GRASS_DENSITY, FALSE, FALSE),

							// maksymalne wartosci

									  maxSize    =  GetDlgItemInt(grass, IDC_GRASS_SIZE2,    FALSE, FALSE),
									  maxHeight  =  GetDlgItemInt(grass, IDC_GRASS_HEIGHT2,  FALSE, FALSE),
									  maxDensity =  GetDlgItemInt(grass, IDC_GRASS_DENSITY2, FALSE, FALSE),

							// obliczenie wartosci finalnych

									  finalSize    = minSize    + rand() % ( maxSize    - minSize + 1),
									  finalHeight  = minHeight  + rand() % ( maxHeight  - minHeight + 1),
									  finalDensity = minDensity + rand() % ( maxDensity - minDensity + 1 );



								float	size    =  ( (float)finalSize   / 100.f ) * MAP_SCALE,
										height  =  ( (float)finalHeight / 100.f ) * MAP_SCALE;

								int		density =  (float)finalDensity / 10.f;



						// ne ma sensu plantowac, jezeli density == 0

							if( density )
							{

								grassVector.push_back( grassPatch((x)*MAP_SCALE, g_terrain[x + MAP_X*z][1], (-z)*MAP_SCALE, grassBrush, red, green, blue, alpha, size, height, density) ); 

							// skorygowanie wysokosci kepek trawy
								grassVector.back().Morph();

							}

						}
					}


					lButtonDown = false;


				// update VBO trawy

					updateGrassVBOs();


					Saved = false;

			}
			else if( lButtonDown )
			{
					MessageBox(g_hwnd, "Grass texture hasn't been chosen!", "Error!", MB_OK);
					lButtonDown = false;
			}

	}






	void weedOut()
	{

			updateGrass();


			if( lButtonDown )
			{

				Vector3 wektor = GetOGLPos(g_mouseX, g_mouseY);

				wektor.posZ *= -1;

				int X, Z;

				if( MAP_SCALE > 1)
				{
					X = (int)wektor.posX - ( (int)wektor.posX % MAP_SCALE );
					Z = (int)wektor.posZ - ( (int)wektor.posZ % MAP_SCALE );
				}

				X = ((int)wektor.posX / MAP_SCALE);
				Z = ((int)wektor.posZ / MAP_SCALE);
				
			
				for(int x = X - grassRangeX/MAP_SCALE; x < X + grassRangeX/MAP_SCALE; x++)
				for(int z = Z - grassRangeZ/MAP_SCALE; z < Z + grassRangeZ/MAP_SCALE; z++)
				{
					for(int i = 0; i < grassVector.size(); i++)
					{
						if( grassVector[i].points[0][0] == x*MAP_SCALE && grassVector[i].points[0][2] == -z*MAP_SCALE )
							grassVector.erase(grassVector.begin() + i--);
					}
				}


			// update VBO trawy

				updateGrassVBOs();


				Saved = false;

			}// lButtonDown...

	}



	void plantEverywhere()
	{

		// pobranie modyfikatorow koloru trawy

			float   red     =  GetDlgItemInt(grass, IDC_GRASS_RED,     FALSE, FALSE) / 255.f,
				    green   =  GetDlgItemInt(grass, IDC_GRASS_GREEN,   FALSE, FALSE) / 255.f,
					blue    =  GetDlgItemInt(grass, IDC_GRASS_BLUE,    FALSE, FALSE) / 255.f,
					alpha   =  GetDlgItemInt(grass, IDC_GRASS_ALPHA,   FALSE, FALSE) / 255.f;



			// wybrano trawiasty pedzelek :)
			if( grassBrush != 0 )
			{		

				// obliczenie dlugosci trwania pracy

					unsigned int workTime = grassVector.size() + (MAP_X-1)*(MAP_Z-1);

				// stworzenie pop up'a z paskiem postepu pracy

					ProgressDialog progressDlg(g_hInstance, grass, "Filling terrain with grass...",
											   0, workTime, 1);
					
					




				// sprawdzenie czy istnieje, juz poletko trawy o identycznej teksturze, na coord'ach (x,z)

					for(int i = 0; i < grassVector.size(); i++)
					{

						if( grassVector[i].textureID == grassBrush )
						{
							grassVector.erase(grassVector.begin() + i--);
						}


					// zaznaczenie progresu
						progressDlg.Increment();

					}
			


				// wypelnianie terenu trawa

					for(int x = 0; x < MAP_X-1; x++)
					for(int z = 0; z < MAP_Z-1; z++)
					{

						// minimalne wartosci

								int	  minSize    =  GetDlgItemInt(grass, IDC_GRASS_SIZE,    FALSE, FALSE),
									  minHeight  =  GetDlgItemInt(grass, IDC_GRASS_HEIGHT,  FALSE, FALSE),
									  minDensity =  GetDlgItemInt(grass, IDC_GRASS_DENSITY, FALSE, FALSE),

							// maksymalne wartosci

									  maxSize    =  GetDlgItemInt(grass, IDC_GRASS_SIZE2,    FALSE, FALSE),
									  maxHeight  =  GetDlgItemInt(grass, IDC_GRASS_HEIGHT2,  FALSE, FALSE),
									  maxDensity =  GetDlgItemInt(grass, IDC_GRASS_DENSITY2, FALSE, FALSE),

							// obliczenie wartosci finalnych

									  finalSize    = minSize    + rand() % ( maxSize    - minSize + 1),
									  finalHeight  = minHeight  + rand() % ( maxHeight  - minHeight + 1),
									  finalDensity = minDensity + rand() % ( maxDensity - minDensity + 1 );



								float	size    =  ( (float)finalSize   / 100.f ) * MAP_SCALE,
										height  =  ( (float)finalHeight / 100.f ) * MAP_SCALE;

								int		density =  (float)finalDensity / 10.f;



						// ne ma sensu plantowac, jezeli density == 0

							if( density )
							{

								grassVector.push_back( grassPatch((x)*MAP_SCALE, g_terrain[x + MAP_X*z][1], (-z)*MAP_SCALE, grassBrush, red, green, blue, alpha, size, height, density) ); 

								grassVector.back().Morph();

							}


						// zaznaczenie progresu
							progressDlg.Increment();


					}// 2 x for lecacy wzdluz MAP_X i MAP_Z


				// update VBO trawy

					updateGrassVBOs();


			}// if ( grassBrush )
			else
			{
				MessageBox(g_hwnd, "Detail texture hasn't been chosen.", "Error!", MB_OK);
			}

	}


	void removeAllWeeds()
	{
		// ustawienie stanu aplikacji w pasku
					
			SetWindowText(g_hwnd, "Clearing terrain from grass...");


		// czyszczenie wektora trawy

			grassVector.erase(grassVector.begin(), grassVector.end());


		// update VBO trawy

			updateGrassVBOs();
	}





	void drawGrass()
	{

			glEnable(GL_TEXTURE_2D);
			glDisable(GL_CULL_FACE);
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER,0.1f);


			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


				/*for(int i = 0; i < grassVector.size(); i++)
					grassVector[i].Draw();*/

				
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);


				// rysowanie VBO
					
					for(int i = 0; i < grassVBOarray.size(); i++)
					{
						glBindTexture(GL_TEXTURE_2D, grassVBOarray[i].textureID);
						
						glBindBufferARB(GL_ARRAY_BUFFER_ARB, grassVBOarray[i].VBOv);
						glVertexPointer(3, GL_FLOAT, 0, 0);

						glBindBufferARB(GL_ARRAY_BUFFER_ARB, grassVBOarray[i].VBOvt);
						glTexCoordPointer(2, GL_FLOAT, 0, 0);

						glBindBufferARB(GL_ARRAY_BUFFER_ARB, grassVBOarray[i].VBOvn);
						glNormalPointer(GL_FLOAT, 0, 0);

						glBindBufferARB(GL_ARRAY_BUFFER_ARB, grassVBOarray[i].VBOvc);
						glColorPointer(4, GL_FLOAT, 0, 0);

						glDrawArrays(GL_TRIANGLES, 0, grassVBOarray[i].indexCounter);

					}


					glBindTexture(GL_TEXTURE_2D, 0);



				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
					
			
			glDisable(GL_ALPHA_TEST);

			glDisable(GL_BLEND);

			glEnable(GL_CULL_FACE);
			glDisable(GL_TEXTURE_2D);
			

	}





	void adjustGrassHeight()
	{
		// odswiezenie wysokosci wierzcholkow trawy

			for(int i = 0; i < grassVector.size(); i++)
				grassVector[i].Morph();

		// update VBO trawy

			updateGrassVBOs();
	}





// --------------------------------------------

	unsigned int generateGrassTexture(unsigned char *bits, unsigned int &width, unsigned int &height)
	{
	//wygeneruj nowy obiekt tekstury

		glGenTextures(1, &grassBrush);
		glBindTexture(GL_TEXTURE_2D, grassBrush);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height,
					  GL_RGBA, GL_UNSIGNED_BYTE, bits );


		glBindTexture(GL_TEXTURE_2D, 0);


		return grassBrush;
	}

	bool loadGrassTexture(char *filename)
	{

			bool originalTexture;   //nie ma sensu generowac "nowej" tekstury

		// sprawdzenie "oryginalnosci" tekstury

			if(grassTextureArray.empty())
				originalTexture = true;
			else
			{
				originalTexture = true;

				for(int i = 0; i < grassTextureArray.size(); i++)
				{
					if( strcmp(filename, grassTextureArray[i].filename) == 0 )
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
					LoadTexture(filename, TRUE, generateGrassTexture);
				}
				catch(std::string error)
				{
					MessageBox(g_hwnd, error.c_str(), "Error!", MB_OK);
					return false;
				}


			// dodaj nowy obiekt tekstury + sciezke do wektora tekstur

				grassTextureObj newGrassTexture;
				strcpy_s(newGrassTexture.filename, filename);
				newGrassTexture.textureID = grassBrush;

				grassTextureArray.push_back(newGrassTexture);

			// dodaj nowa pozycje do IDC_GRASS_COMBO

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

				HWND comboBox = GetDlgItem(grass, IDC_GRASS_COMBO);

				ComboBox_AddString(comboBox, newItemName.c_str());

			// zerowanie brusha

				grassBrush = 0;
			}


			return originalTexture;
	}


	int  loadGrassTextures()
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

				directory = FindFirstFileA("TerrainData\\Textures\\Grass\\*.bmp", &data);

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

				directory = FindFirstFileA("Textures\\Grass\\*.jpg", &data);

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

				directory = FindFirstFileA("Textures\\Grass\\*.TGA", &data);

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
				strcat_s( szFileName, "TerrainData\\Textures\\Grass\\" );
				strcat_s( szFileName, files[i].c_str() );


			// Ladowanie tekstury o nazwie szFileName

				if( loadGrassTexture(szFileName) )
				{
					counter++;
				}
			}


			return counter;
	}


	void reloadGrassTextures()
	{
		int newTextures = loadGrassTextures();

		char buffer[MAX_PATH] = "";

		_itoa_s( newTextures, buffer, 10 );

		if( newTextures == 1 )
		{
			strcat_s( buffer, " new texture loaded!" );
		}
		else
			strcat_s( buffer, " new textures loaded!" );

		MessageBox(g_hwnd, buffer, "Reloading finished.", MB_OK);
	}


	void changeGrassTexture()
	{

			std::string cbIndexText;
			char buffer[MAX_PATH] = "";

			HWND comboBox = GetDlgItem(grass, IDC_GRASS_COMBO);
			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			cbIndexText.insert(0, buffer);

			if(!cbIndexText.empty())
			{
				for(int i=0; i<grassTextureArray.size(); i++)
				{
					std::string helper;
					helper.insert(0, grassTextureArray[i].filename);

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
						grassBrush = grassTextureArray[i].textureID;

						// ustaw obrazek ...
						HBITMAP hBitmap;
						hBitmap = (HBITMAP)LoadImage(NULL, grassTextureArray[i].filename, IMAGE_BITMAP, 128, 128,
						LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

						HWND hPicture = GetDlgItem(grass, IDC_GRASS_PIC);
						SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); 

						break;
					}
				}
			}// !cbIndexText.empty()
			else
			{
				HWND hPicture = GetDlgItem(grass, IDC_GRASS_PIC);
				SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, NULL); 
			}

	}

	void removeGrassTextures()
	{
		for(int i = 0; i < grassTextureArray.size(); i++)
		{
			glDeleteTextures(1, &grassTextureArray[i].textureID);
		}
	}
//
// --------------------------------------------
	
	void saveGrass(char *filename)
	{
			std::fstream file;

		// otworzenie pliku w trybie "nadpisz" + binarny

			file.open( filename, std::ios::out | std::ios::app | std::ios::binary );

		// ustawienie markera poczatkowego

			file << "\nGrass\n";

		// zapis liczby pol trawy

			file << "\t" << grassVector.size() << "\n";

		// zapis danych poszczegolnych pol trawy

			for(int patch = 0; patch < grassVector.size(); patch++)
			{
			// zapisanie nazwy tekstury na podstawie ID tekstury
				
				int index = -1;

				for(int i = 0; i < grassTextureArray.size(); i++)
				{
					if( grassVector[patch].textureID == grassTextureArray[i].textureID )
					{
						index = i;
						break;
					}
				}

				assert( index != -1 );
				file << "\t" << StripName(grassTextureArray[index].filename, false) << "\n";

			// zapisanie wartosci RGBA

				file << "\t" << grassVector[patch].R << "\n";
				file << "\t" << grassVector[patch].G << "\n";
				file << "\t" << grassVector[patch].B << "\n";
				file << "\t" << grassVector[patch].A << "\n";

			// zapisanie wartosci wierzcholkow pola trawy

				for(int point = 0; point < 4; point++)
				{
					file << "\t" << grassVector[patch].points[point][0] << " "
					     << "\t" << grassVector[patch].points[point][1] << " "
						 << "\t" << grassVector[patch].points[point][2] << "\n";
				}

			// zapisanie gestosci pola trawy (ilosc kepek (kepka = "ang. tuft")
		
				file << "\t" << grassVector[patch].tuftVector.size() << "\n";

			// zapisanie wartosci kepek

				for(int tuft = 0; tuft < grassVector[patch].tuftVector.size(); tuft++)
				{
				// zapis wierzcholkow kepki

					for(int vertex = 0; vertex < 5; vertex++)
					{
						file << "\t" << grassVector[patch].tuftVector[tuft].vertexArray[vertex][0] << " "
							 << "\t" << grassVector[patch].tuftVector[tuft].vertexArray[vertex][1] << " "
							 << "\t" << grassVector[patch].tuftVector[tuft].vertexArray[vertex][2] << "\n";
					}

				// zapisanie szerokosci i wysokosci kepki

					file << "\t" << grassVector[patch].tuftVector[tuft].size   << "\n";
					file << "\t" << grassVector[patch].tuftVector[tuft].height << "\n";
				}

			}

		// ustawienie markera koncowego

			file << "End\n\n";

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();
	}

	bool loadGrass(char *filename)
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

		// kopia zapasowa wektora trawy

			std::vector <grassPatch> grassCopy(grassVector);

			char buffer[MAX_PATH];

			strcpy_s( buffer, "Save file \"" );
			strcat_s( buffer, filename );
			strcat_s( buffer, "\" is corrupted!\n\n" );

			std::string errorInfo = buffer;

			std::string line;

		// wyczyszczenie wektora trawy

			grassVector.erase( grassVector.begin(), grassVector.end() );

		// ladowanie danych z pliku

			while( !file.eof() )
			{
				file >> line;

				if( line == "Grass" )
				{
					if( !markerFound )
					{
						markerFound = true;
					}
					else
					{
						loadingCorrect = false;
						errorInfo += "There are two or more grass info markers (should be one).";
						break;
					}
				}

				if( markerFound && !loadingCommenced )
				{
				// ustawienie falgi

					loadingCommenced = true;

				// pobranie ilosci pol trawy

					int patchCount;

					file >> line;

					if( line == "End"  )
					{
						loadingCorrect = false;
						errorInfo += "\"End\" marker found too early, while loading grass vertices.";
						break;
					}
					else if( !IsNumber(line, true) )
					{
						loadingCorrect = false;
						errorInfo += "Word where grassPatch count value should be, isn't a number!\n\n";
						errorInfo += "\"";
						errorInfo += line;
						errorInfo += "\"";
						break;
					}
					else
					{
						patchCount = atoi(line.c_str());
					}

				// ladowanie poszczegolnych pol trawy

					std::string lastWrongTexture("");  // nazwa ostatniej zlej tekstury (przydatne nizej) 

					for(int patch = 0; patch < patchCount; patch++)
					{
					// struktura nowego pola trawy

						grassPatch newPatch;

					// pobranie nazwy tekstury

						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading grass vertices.";
							break;
						}

					// sprawdzenie w rejestrze tekstur, czy istnieje tekstura o takiej nazwie.
					// jezeli tak, to przydziela ID tekstury do tego pola trawy

						bool TextureFound = false;

						for(int i = 0; i < grassTextureArray.size(); i++)
						{
							if( line == StripName( grassTextureArray[i].filename, FALSE ) )
							{
								newPatch.textureID = grassTextureArray[i].textureID;
								TextureFound = true;
								break;
							}
						}

					// jezeli nie znaleziono tekstury, poinformuj

						if( !TextureFound && lastWrongTexture != line )
						{
							char buffer[MAX_PATH];

							strcpy_s( buffer, "Certain grass patches have texture name \"" );
							strcat_s( buffer, line.c_str() );
							strcat_s( buffer, "\", that doesn't exist in grassTextureArray register.\n\nDo you want to continue loading process without those certain grass patches?" );

							int result = MessageBox(g_hwnd, buffer, "Error!", MB_YESNO);

							if( result == IDYES )
							{
								lastWrongTexture = line;
							}
							else
							{
								loadingCorrect = false;
								errorInfo += "Grass loading canceled, due to invalid texture name.";
								break;
							}
						}

					// pobranie wartosci RGBA

						float color[4];

						for(int i = 0; i < 4; i++)
						{
							file >> line;

							if( line == "End"  )
							{
								loadingCorrect = false;
								errorInfo += "\"End\" marker found too early, while loading grass vertices.";
								break;
							}
							else if( !IsNumber(line, false) )
							{
								loadingCorrect = false;
								errorInfo += "Word where grassPatch color value should be, isn't a number!\n\n";
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

						newPatch.R = color[0];
						newPatch.G = color[1];
						newPatch.B = color[2];
						newPatch.A = color[3];

					// pobranie wartosci wierzcholkow pola trawy
	
						for(int point = 0; point < 4; point++)
						for(int     i = 0;     i < 3;     i++)
						{
							file >> line;

							if( line == "End"  )
							{
								loadingCorrect = false;
								errorInfo += "\"End\" marker found too early, while loading grass vertices.";
								break;
							}
							else if( !IsNumber(line, false) )
							{
								loadingCorrect = false;
								errorInfo += "Word where grassPatch point value should be, isn't a number!\n\n";
								errorInfo += "\"";
								errorInfo += line;
								errorInfo += "\"";
								break;
							}
							else
							{
								newPatch.points[point][i] = atof(line.c_str());
							}
						}
					
					// pobranie liczby kepek

						int tuftCount;

						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;
							errorInfo += "\"End\" marker found too early, while loading grass vertices.";
							break;
						}
						else if( !IsNumber(line, true) )
						{
							loadingCorrect = false;
							errorInfo += "Word where grassPatch tuftCount value should be, isn't a number!\n\n";
							errorInfo += "\"";
							errorInfo += line;
							errorInfo += "\"";
							break;
						}
						else
						{
							tuftCount = atoi(line.c_str());
						}

					// zabezpieczenie (liczba pol trawy moze byc zerowa, lecz liczba pol kepek na danym polu nie ma prawa byc zerowa!)

						if( tuftCount == 0 )
						{
							loadingCorrect = false;
							errorInfo += "tuftCount value equals zero!";
							break;
						}

					// pobranie wartosci kepek

						for(int tuft = 0; tuft < tuftCount; tuft++)
						{
							grassTuft newTuft;

						// ladowanie vertex'ow dwoch trojkatow (ze wspolnym gornym wierzcholkiem) skladajacych sie na kepke trawy

							for(int vertex = 0; vertex < 5; vertex++)
							for(int  point = 0;  point < 3;  point++)
							{
								file >> line;
			
								if( line == "End"  )
								{
									loadingCorrect = false;
									errorInfo += "\"End\" marker found too early, while loading grass vertices.";
									break;
								}
								else if( !IsNumber(line, false) )
								{
									loadingCorrect = false;
									errorInfo += "Word where grassTuft vertex value should be, isn't a number!\n\n";
									errorInfo += "\"";
									errorInfo += line;
									errorInfo += "\"";
									break;
								}
								else
								{
									newTuft.vertexArray[vertex][point] = atof(line.c_str());
								}
							}

						// pobranie szerokosci i wysokosci kepki

							file >> line;
			
							if( line == "End"  )
							{
								loadingCorrect = false;
								errorInfo += "\"End\" marker found too early, while loading grass vertices.";
								break;
							}
							else if( !IsNumber(line, false) )
							{
								loadingCorrect = false;
								errorInfo += "Word where grassTuft size value should be, isn't a number!\n\n";
								errorInfo += "\"";
								errorInfo += line;
								errorInfo += "\"";
								break;
							}
							else
							{
								newTuft.size = atof(line.c_str());
							}


							file >> line;
			
							if( line == "End"  )
							{
								loadingCorrect = false;
								errorInfo += "\"End\" marker found too early, while loading grass vertices.";
								break;
							}
							else if( !IsNumber(line, false) )
							{
								loadingCorrect = false;
								errorInfo += "Word where grassTuft height value should be, isn't a number!\n\n";
								errorInfo += "\"";
								errorInfo += line;
								errorInfo += "\"";
								break;
							}
							else
							{
								newTuft.height = atof(line.c_str());
							}

						// dodanie kepki do wektora kepek

							newPatch.tuftVector.push_back( newTuft );

						}// koniec petli ladujacej kepki trawy


					// dodanie nowego pola trawy do wektora

						// pomimo zlej tekstury, trza przejsc wszystkie powyzsze linijki w pliku
						// i po prostu je zignorowac :)

						if( TextureFound )
							grassVector.push_back( newPatch );
					
					}

				}
			}

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();


			if( !markerFound )
			{
				loadingCorrect = false;
				errorInfo += "\"Grass\" marker hasn't been found!";
			}

			if( loadingCorrect )
			{
			// utworzenie nowych VBO trawy

				updateGrassVBOs();
			}
			else
			{
			// przywrocenie danych trawy, za pomoca kopii zapasowej

				grassVector = grassCopy;

			// poinformowanie user'a o naturze bledu

				MessageBox(g_hwnd, errorInfo.c_str(), "Error!", MB_OK);
			}

			return loadingCorrect;
	}

// --------------------------------------------

	void ResetGrass()
	{
		grassVector.erase( grassVector.begin(), grassVector.end() );
		clearGrassVBOs();
	}
	void CloseGrass()
	{
		removeGrassTextures();
		clearGrassVBOs();
	}