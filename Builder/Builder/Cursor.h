#pragma once


	
	int   cursorSize = 0,
		  cursorState;

	unsigned int cursorVBOv = 0;	// identyfikator rodzaju rysowania OpenGL (GL_POINTS || GL_LINE_LOOP)



	void computeCursor()
	{
		// obliczenie pozycji kursora (niepotrzebne odswiezanie pozycji kursora, daje tragiczny efekt przy morphowaniu)

			if( mouseMoved )
			{
				Vector3 wektor = GetOGLPos(g_mouseX, g_mouseY);

			// przeskalowanie coord'ow odpowiednio do skali (zeby nie pokrzaczylo sie, przy indeksowaniu tablicy wysokosci)

				if( MAP_SCALE > 1 )
				{
					cursorX = (int)wektor.posX - ( (int)wektor.posX % MAP_SCALE );
					cursorZ = (int)wektor.posZ - ( (int)wektor.posZ % MAP_SCALE );
				}
				else if ( MAP_SCALE == 1 )
				{
					cursorX = (int)wektor.posX;
					cursorZ = (int)wektor.posZ;
				}
			}

		// tablica z wartosciami wierzcholkow kursora

			float (*cursorArray)[3] = NULL;

		// sprawdzenie czy kursor nie przekroczyl granicy terenu

			if((cursorX/MAP_SCALE) > 0.f && (cursorX/MAP_SCALE) < MAP_X
										 &&
			   (cursorZ/MAP_SCALE) < 0.f && (cursorZ/MAP_SCALE) > -MAP_Z)
			{
			// reset rozmiaru kursora

				cursorSize = 0;

			// zmienne przechowujace rozmiary X, Z kursora

				float RangeX, RangeZ;

				int index, // okresla indeks w tablicy wysokosci terenu

					X =  cursorX/MAP_SCALE, 
					Z = -cursorZ/MAP_SCALE;

			// ustawienie powyzszych zmiennych

				if( Action == PAINT_LAND )
				{
					updateTextures();

					RangeX = textureRangeX;
					RangeZ = textureRangeZ;
				}
				else if( Action == PLANT_GRASS || Action == WEED_OUT )
				{
					updateGrass();

					RangeX = grassRangeX;
					RangeZ = grassRangeZ;
				}
				else if( Action == MORPH )
				{
					updateMorphing();

					RangeX = morphRangeX;
					RangeZ = morphRangeZ;

				}

			// rysowanie punktu	

				cursorState = GL_LINE_LOOP;

				if( !roundCursor || ( RangeX < 2 && RangeZ < 2 ) )
				{
					for(int i = 0; i < RangeZ*2; i++)
					{
						cursorSize++;
					}

					for(int i = 0; i < RangeX*2; i++)
					{
						cursorSize++;
					}

					for(int i = 0; i < RangeZ*2; i++)
					{
						cursorSize++;
					}

					for(int i = 0; i < RangeX*2; i++)
					{
						cursorSize++;
					}
				}
				else
				{
					X =  cursorX/MAP_SCALE;
					Z = -cursorZ/MAP_SCALE;

				// odszukiwanie krawedzi elipsy, cwiartkami

					for(int z = Z - RangeZ - 1; z < Z; z++)
					for(int x = X - RangeX - 1; x < X; x++)
					{

						float pX = (x - X),
							  pZ = (z - Z),

							  elipseEq = pow(pX, 2.f)/pow((float)(RangeX), 2.f)
														+
										 pow(pZ, 2.f)/pow((float)(RangeZ), 2.f);

						if( elipseEq <= 1 )
						{
							cursorSize++;

						// dolny brzeg moze miec wiecej niz 1 punkt na osi Z

							if( z == Z - RangeZ - 1 )
								;
							else
								break;
						}
					}


					for(int z = Z;              z < Z + RangeZ; z++)
					for(int x = X - RangeX + 1; x < X;          x++)
					{
				
						float pX = (x - X),
							  pZ = (z - Z),

							  elipseEq = pow(pX, 2.f)/pow((float)(RangeX), 2.f)
														+
										 pow(pZ, 2.f)/pow((float)(RangeZ), 2.f);

						if( elipseEq <= 1 )
						{
							cursorSize++;

						// tutaj wierzcholki ida akurat wzgledem kierunku rysowania, 
						// a wiec nie trzeba ich przestawiac

							if( z == Z + RangeZ - 1 )
								;
							else
								break;
						}
					}


					for(int z = Z + RangeZ - 1; z > Z; z--)
					for(int x = X + RangeX - 1; x > X; x--)
					{
				
						float pX = (x - X),
							  pZ = (z - Z),

							  elipseEq = pow(pX, 2.f)/pow((float)(RangeX), 2.f)
														+
										 pow(pZ, 2.f)/pow((float)(RangeZ), 2.f);

						if( elipseEq <= 1 )
						{
							cursorSize++;

							if( z == Z + RangeZ )
								;
							else
								break;
						}
					}


					for(int z = Z;              z > Z - RangeZ; z--)
					for(int x = X + RangeX - 1; x > X;          x--)
					{
						float pX = (x - X),
							  pZ = (z - Z),

							  elipseEq = pow(pX, 2.f)/pow((float)(RangeX), 2.f)
														+
										 pow(pZ, 2.f)/pow((float)(RangeZ), 2.f);

						if( elipseEq <= 1 )
						{
							cursorSize++;

							if( z == Z - RangeZ + 1 )
								;
							else
								break;
						}
					}
				}

			// znajac rozmiar kursora, mozna zarezerwowac miejsce dla nowego kursora
				
				assert( cursorArray == NULL );
				cursorArray = new float [cursorSize][3];

			// przeslanie danych do tablicy kursora

				cursorSize = 0; // ta zmienna mozna wykorzystac jako indeks tablicy kursora

			// wypelnienie tablicy wierzcholkow kursora, wierzcholkami heightmapy terenu

				assert( RangeX > 0 && RangeZ > 0 );

			// zabezpieczenie (okragly kursor sie "psuje" przy malym kursorze)

				if( !roundCursor || ( RangeX < 2 && RangeZ < 2 ) )
				{
					for(int i = 0; i < RangeZ*2; i++)
					{
						X = cursorX/MAP_SCALE + RangeX;

						if( X > MAP_X-1)
							X = MAP_X-1;
						else if( X < 0 )
							X = 0;

						Z = -cursorZ/MAP_SCALE + RangeZ - i;

						if( Z > MAP_Z-1 )
							Z = MAP_Z-1;
						else if( Z < 0 )
							Z = 0;

						index = X + MAP_X*Z;

						cursorArray[cursorSize][0] = g_terrain[index][0]; 
						cursorArray[cursorSize][1] = g_terrain[index][1]+.2f; 
						cursorArray[cursorSize][2] = g_terrain[index][2];

						cursorSize++;
					}

					for(int i = 0; i < RangeX*2; i++)
					{
						X = cursorX/MAP_SCALE + RangeX - i; 

						if( X > MAP_X-1)
							X = MAP_X-1;
						else if( X < 0 )
							X = 0;

						Z = -cursorZ/MAP_SCALE - RangeZ;

						if( Z > MAP_Z-1 )
							Z = MAP_Z-1;
						else if( Z < 0 )
							Z = 0;

						index = X + MAP_X*Z;
						
						cursorArray[cursorSize][0] = g_terrain[index][0]; 
						cursorArray[cursorSize][1] = g_terrain[index][1]+.2f; 
						cursorArray[cursorSize][2] = g_terrain[index][2];

						cursorSize++;
					}

					for(int i = 0; i < RangeZ*2; i++)
					{
						X = cursorX/MAP_SCALE - RangeX;

						if( X > MAP_X-1)
							X = MAP_X-1;
						else if( X < 0 )
							X = 0;

						Z = -cursorZ/MAP_SCALE - RangeZ + i;

						if( Z > MAP_Z-1 )
							Z = MAP_Z-1;
						else if( Z < 0 )
							Z = 0;

						index = X + MAP_X*Z;
						
						cursorArray[cursorSize][0] = g_terrain[index][0]; 
						cursorArray[cursorSize][1] = g_terrain[index][1]+.2f; 
						cursorArray[cursorSize][2] = g_terrain[index][2];

						cursorSize++;
					}

					for(int i = 0; i < RangeX*2; i++)
					{
						X = cursorX/MAP_SCALE - RangeX + i;

						if( X > MAP_X-1)
							X = MAP_X-1;
						else if( X < 0 )
							X = 0;

						Z = -cursorZ/MAP_SCALE + RangeZ;

						if( Z > MAP_Z-1 )
							Z = MAP_Z-1;
						else if( Z < 0 )
							Z = 0;

						index = X + MAP_X*Z;
						
						cursorArray[cursorSize][0] = g_terrain[index][0]; 
						cursorArray[cursorSize][1] = g_terrain[index][1]+.2f; 
						cursorArray[cursorSize][2] = g_terrain[index][2];

						cursorSize++;
					}
					
				}// if( !roundCursor )
				else
				{

					X =  cursorX/MAP_SCALE;
					Z = -cursorZ/MAP_SCALE;

				// odszukiwanie krawedzi elipsy, cwiartkami

					for(int z = Z - RangeZ - 1; z < Z; z++)
					for(int x = X - RangeX - 1; x < X; x++)
					{

						float pX = (x - X),
							  pZ = (z - Z),

							  elipseEq = pow(pX, 2.f)/pow((float)(RangeX), 2.f)
														+
										 pow(pZ, 2.f)/pow((float)(RangeZ), 2.f);

						if( elipseEq <= 1 )
						{
						// skopiowanie x i z do zmiennych pomocniczych,
						// aby petla poprawnie dzialala

							int a = x,
								b = z;

							if( a > MAP_X-1)
								a = MAP_X-1;
							else if( a < 0 )
								a = 0;

							if( b > MAP_Z-1 )
								b = MAP_Z-1;
							else if( b < 0 )
								b = 0;

						// skopiowanie danych z tablicy terenu

							index = a + MAP_X*b;

							cursorArray[cursorSize][0] = g_terrain[index][0]; 
							cursorArray[cursorSize][1] = g_terrain[index][1]+.2f; 
							cursorArray[cursorSize][2] = g_terrain[index][2];

						// inkrementacja rozmiaru kursora

							cursorSize++;

						// dolny brzeg moze miec wiecej niz 1 punkt na osi Z

							if( z == Z - RangeZ - 1 )
							{
							// normalnie tablica kursora bierze brzegowe wierzcholki
							// w przypadku dolnych, gornych krawedzi Z, wystepuje wiecej niz 1 wierzcholek,
							// i nie maja one isc od lewej do prawej, lecz odwrotnie, trza je poprzestawiac

								if( cursorSize >= 2 )
								{
									float helper[3];

									helper[0] = cursorArray[cursorSize-1][0];
									helper[1] = cursorArray[cursorSize-1][1];
									helper[2] = cursorArray[cursorSize-1][2];

									cursorArray[cursorSize-1][0] = cursorArray[cursorSize-2][0];
									cursorArray[cursorSize-1][1] = cursorArray[cursorSize-2][1];
									cursorArray[cursorSize-1][2] = cursorArray[cursorSize-2][2];

									cursorArray[cursorSize-2][0] = helper[0];
									cursorArray[cursorSize-2][1] = helper[1];
									cursorArray[cursorSize-2][2] = helper[2];
								}
							}
							else
								break;
						}
					}


					for(int z = Z;              z < Z + RangeZ; z++)
					for(int x = X - RangeX + 1; x < X;          x++)
					{
				
						float pX = (x - X),
							  pZ = (z - Z),

							  elipseEq = pow(pX, 2.f)/pow((float)(RangeX), 2.f)
														+
										 pow(pZ, 2.f)/pow((float)(RangeZ), 2.f);

						if( elipseEq <= 1 )
						{
						// skopiowanie x i z do zmiennych pomocniczych,
						// aby petla poprawnie dzialala

							int a = x,
								b = z;

							if( a > MAP_X-1)
								a = MAP_X-1;
							else if( a < 0 )
								a = 0;

							if( b > MAP_Z-1 )
								b = MAP_Z-1;
							else if( b < 0 )
								b = 0;

							index = a + MAP_X*b;

							cursorArray[cursorSize][0] = g_terrain[index][0]; 
							cursorArray[cursorSize][1] = g_terrain[index][1]+.2f; 
							cursorArray[cursorSize][2] = g_terrain[index][2];

							cursorSize++;

						// tutaj wierzcholki ida akurat wzgledem kierunku rysowania, 
						// a wiec nie trzeba ich przestawiac

							if( z == Z + RangeZ - 1 )
								;
							else
								break;
						}
					}


					for(int z = Z + RangeZ - 1; z > Z; z--)
					for(int x = X + RangeX - 1; x > X; x--)
					{
				
						float pX = (x - X),
							  pZ = (z - Z),

							  elipseEq = pow(pX, 2.f)/pow((float)(RangeX), 2.f)
														+
										 pow(pZ, 2.f)/pow((float)(RangeZ), 2.f);

						if( elipseEq <= 1 )
						{
						// skopiowanie x i z do zmiennych pomocniczych,
						// aby petla poprawnie dzialala

							int a = x,
								b = z;

							if( a > MAP_X-1)
								a = MAP_X-1;
							else if( a < 0 )
								a = 0;

							if( b > MAP_Z-1 )
								b = MAP_Z-1;
							else if( b < 0 )
								b = 0;

							index = a + MAP_X*b;

							cursorArray[cursorSize][0] = g_terrain[index][0]; 
							cursorArray[cursorSize][1] = g_terrain[index][1]+.2f; 
							cursorArray[cursorSize][2] = g_terrain[index][2];

							cursorSize++;

							if( z == Z + RangeZ )
							{
								if( cursorSize >= 2 )
								{
									float helper[3];

									helper[0] = cursorArray[cursorSize-1][0];
									helper[1] = cursorArray[cursorSize-1][1];
									helper[2] = cursorArray[cursorSize-1][2];

									cursorArray[cursorSize-1][0] = cursorArray[cursorSize-2][0];
									cursorArray[cursorSize-1][1] = cursorArray[cursorSize-2][1];
									cursorArray[cursorSize-1][2] = cursorArray[cursorSize-2][2];

									cursorArray[cursorSize-2][0] = helper[0];
									cursorArray[cursorSize-2][1] = helper[1];
									cursorArray[cursorSize-2][2] = helper[2];
								}
							}
							else
								break;
						}
					}


					for(int z = Z;              z > Z - RangeZ; z--)
					for(int x = X + RangeX - 1; x > X;          x--)
					{
						float pX = (x - X),
							  pZ = (z - Z),

							  elipseEq = pow(pX, 2.f)/pow((float)(RangeX), 2.f)
														+
										 pow(pZ, 2.f)/pow((float)(RangeZ), 2.f);

						if( elipseEq <= 1 )
						{
						// skopiowanie x i z do zmiennych pomocniczych,
						// aby petla poprawnie dzialala

							int a = x,
								b = z;

							if( a > MAP_X-1)
								a = MAP_X-1;
							else if( a < 0 )
								a = 0;

							if( b > MAP_Z-1 )
								b = MAP_Z-1;
							else if( b < 0 )
								b = 0;

							index = a + MAP_X*b;

							cursorArray[cursorSize][0] = g_terrain[index][0]; 
							cursorArray[cursorSize][1] = g_terrain[index][1]+.2f; 
							cursorArray[cursorSize][2] = g_terrain[index][2];

							cursorSize++;

							if( z == Z - RangeZ + 1 )
								;
							else
								break;
						}
					}

				}

			// wyczyszczenie i utworzenie VBO wierzcholkow kursora

				if( cursorVBOv != 0 )
					glDeleteBuffersARB(1, &cursorVBOv);

				glGenBuffersARB(1, &cursorVBOv);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cursorVBOv);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, cursorSize*3*sizeof(float), cursorArray, GL_STATIC_DRAW_ARB);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			// czyszczenie tablicy z wierzcholkami

				assert( cursorArray != NULL );
				delete [] cursorArray;

			} // dluuugasny IF sprawdzajacy poprawnosci pozycji kursora...
	}


	void paintCursor()
	{

			if( enableLighting )
				glDisable(GL_LIGHTING);


		// sprawdzenie czy kursor nie przekroczyl granicy terenu

			if((cursorX / MAP_SCALE) > 0.f && (cursorX / MAP_SCALE) < MAP_X
										   &&
			   (cursorZ / MAP_SCALE) < 0.f && (cursorZ / MAP_SCALE) > - MAP_Z
			   
			   && cursorVBOv )
			{
				
				glLineWidth(1.5f);
				glColor3f(0.f, 1.f, 0.f);


				glEnableClientState(GL_VERTEX_ARRAY);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, cursorVBOv);
				glVertexPointer(3, GL_FLOAT, 0, 0);

					glDrawArrays(cursorState, 0, cursorSize);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
				glDisableClientState(GL_VERTEX_ARRAY);
				
			} // dluuugasny IF sprawdzajacy poprawnosci pozycji kursora...


		if( enableLighting )
			glEnable(GL_LIGHTING);
	}


	void CloseCursor()
	{
		SetWindowText(g_hwnd, "Close Cursor");
		if( cursorVBOv != 0 )
			glDeleteBuffersARB(1, &cursorVBOv);
		SetWindowText(g_hwnd, "Close Cursor2");
	}


