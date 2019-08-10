#pragma once


// Zmienne


	HWND	terrain  = NULL;


	// CG

		#define TERRAIN_SHADER_PATH		  "Utilities\\CG shaders\\Terrain shader\\Multitexture.cg"
		#define TERRAIN_VERTEX_SHADER     "passVertex"
		#define TERRAIN_FRAGMENT_SHADER   "passFragment"

		CGcontext	terrainContext;			
		CGprogram	vertexShader, RenderMultitextures;	
		CGprofile	terrainVP, terrainFP;				

		CGparameter cgTexture,
					cgModelViewMatrix,
					cgScale;

	bool	affectWholeTerrain = false,  // przy zmianie terrainHeight, okresla czy cala scena sie zmienia, czy tylko brzegi

			drawingNet = false;   // decyduje o rysowaniu siatki terenu
	

//  TABLICZKI

	float	(*netArray)[3];		// tablica przechowujaca coord'y siatki terenu

	unsigned int netVBOv = 0;

	GLuint	 *g_indexArray = NULL;		// tablica indeksow do ponizszych tablic, uzywanych przy rysowaniu terenu

	// g_terrain jest globalne w Builder.cpp
	float	(*g_normals)[3]       = NULL;  // normale terenu
	float   (*g_texcoordArray)[2] = NULL;  // koordynaty tekstur terenu



// identyfikatory VBO dla terenu

	unsigned int terrainVBOv  = 0,   // vertexy
		         terrainVBOvt = 0,	 // texCoords
				 terrainVBOvn = 0;   // normale


	char    terrainKey = NULL;	// jest on wykorzystywany przy zmianach wartosci terenu.
							// jego wartosc ma wplyw na to, ktora wartosc terenu nalezy zmienic.



	#include "Morphing.h"
	#include "Textures.h"


// Funkcje

	void equilaizeTerrain()
	{

			int result = MessageBox(g_hwnd, "Do you want to equalize the whole terrain?", "Warning!", MB_YESNO);

			if( result == IDYES )
			{
				int index;

				for(int z = 0; z < MAP_Z-1; z++)
				for(int x = 0; x < MAP_X;   x++)
				{
					index = x + MAP_X * z;

					g_terrain[index][1] = terrainHeight;
				}

			// odswiezenie siatki

				computeNet();

			// odswiezenie normali

				computeNormals();

			// czyszczenie VBO

				glDeleteBuffersARB(1, &terrainVBOv);
				glDeleteBuffersARB(1, &terrainVBOvn);

			// utworzenie nowych VBO

				glGenBuffersARB( 1, &terrainVBOv );		

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOv );			
				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_terrain,		GL_DYNAMIC_DRAW_ARB );

				glGenBuffersARB( 1, &terrainVBOvn );

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvn );
				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_normals,		GL_DYNAMIC_DRAW_ARB );
				

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


				Saved = false;
			}
	}




	void adjustTerrain()
	{
		
		// odswiezenie aktualnych ogranicznikow terenu

			MAX_TERRAIN_HEIGHT = GetDlgItemInt(terrain, IDC_TERRAIN_MAX_HEIGHT, FALSE, FALSE);
			MIN_TERRAIN_HEIGHT = GetDlgItemInt(terrain, IDC_TERRAIN_MIN_HEIGHT, FALSE, FALSE); 

		// przelecenie przez macierz terenu i ewentualne korygowanie wysokosci

			int index;

			for(int z = 0; z < MAP_Z; z++)
			for(int x = 0; x < MAP_X;   x++)
			{
				index = x + MAP_X * z;

				if( g_terrain[index][1] > MAX_TERRAIN_HEIGHT )
					g_terrain[index][1] = MAX_TERRAIN_HEIGHT;

				if( g_terrain[index][1] < MIN_TERRAIN_HEIGHT )
					g_terrain[index][1] = MIN_TERRAIN_HEIGHT;
			}


		// korygacja siatki

			computeNet();

		// korygacja normali

			computeNormals();

		// czyszczenie VBO

			glDeleteBuffersARB(1, &terrainVBOv);
			glDeleteBuffersARB(1, &terrainVBOvn);

		// utworzenie nowych VBO

			glGenBuffersARB( 1, &terrainVBOv );		

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOv );			
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_terrain,		GL_DYNAMIC_DRAW_ARB );

			glGenBuffersARB( 1, &terrainVBOvn );

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvn );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_normals,		GL_DYNAMIC_DRAW_ARB );
			

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	}



	void updateTerrain(char key)
	{

		if( toolBoxReady )
		{
			switch( key )
			{
				// zmienna ta bedzie okreslac indeks do tablicy g_terrain

					int index;

				// terrainHeight

					case 'H':
					{

						if( affectWholeTerrain )
						{
						// zapamietanie starej wysokosci

							int oldTerrainHeight = terrainHeight;

						// pobranie nowej wysokosci terenu

							terrainHeight = GetDlgItemInt(terrain, IDC_TERRAIN_HEIGHT, FALSE, FALSE);


						// drobne zabezpieczenie 

							if( oldTerrainHeight == terrainHeight )
								break;


						// dodawanie roznicy nowej i starej wysokosci do ogolnej wysokosci terenu

							for(int x = 0; x < MAP_X; x++)
							for(int z = 0; z < MAP_Z; z++)
							{
								index = x + MAP_X * z;

								g_terrain[index][1] += terrainHeight - oldTerrainHeight;
							}


						// modyfikowanie wysokosci trawy

							adjustGrassHeight(terrainHeight - oldTerrainHeight);


						// modyfikowanie wysokosci obiektow

							objectOperator.adjustObjectsHeight(terrainHeight - oldTerrainHeight);



						// wykonanie dodatkowych obliczen na detalach terenu oraz cieczach

						if( detailCounter )
							objectOperator.executeSpecificFunctions(DETAIL_NAME);

						if( liquidCounter )
							objectOperator.executeSpecificFunctions(LIQUID_NAME);


						}// if( affectWholeTerrain )
						else
						{

							// pobranie nowej wysokosci terenu

								terrainHeight = GetDlgItemInt(terrain, IDC_TERRAIN_HEIGHT, FALSE, FALSE);


							// aktualizacja krawedzi wzdluz osi X

								for(int x = 0; x < MAP_X; x++)
								{
									index = x + MAP_X * 0;

									g_terrain[index][1] = terrainHeight;
								}

								for(int x = 0; x < MAP_X; x++)
								{
									index = x + MAP_X * (MAP_Z-1);

									g_terrain[index][1] = terrainHeight;
								}

							// aktualizacja krawedzi wzdluz osi Z

								for(int z = 0; z < MAP_Z; z++)
								{
									index = 0 + MAP_X * z;

									g_terrain[index][1] = terrainHeight;
								}

								for(int z = 0; z < MAP_Z; z++)
								{
									index = (MAP_X-1) + MAP_X * z;

									g_terrain[index][1] = terrainHeight;
								}
						}
					}
					break;


				// MAP_SCALE

					case 'S':
					{
						
					// zapamietaj stara wartosc MAP_SCALE

						int oldMapScale = MAP_SCALE;

					// pobierz nowa wartosc skali mapy

						MAP_SCALE = GetDlgItemInt(terrain, IDC_TERRAIN_MAP_SCALE, FALSE, FALSE);


					// drobne zabezpieczenie 

						if( oldMapScale == MAP_SCALE )
							break;

						else if( MAP_SCALE < 1 )
							break;


					// ustawia na nowo X i Z wszystkich wierzcholkow terenu

						for(int z = 0; z < MAP_Z; z++)
						for(int x = 0; x < MAP_X; x++)
						{
							index = x + MAP_X * z;	

							g_terrain[index][0] =  float(x) * MAP_SCALE;
							g_terrain[index][2] = -float(z) * MAP_SCALE;

							g_terrain[index][1] *= (float)MAP_SCALE / (float)oldMapScale;
						}



					// przeskalowywuje pozycje wszystkich obiektow na mapie

						objectOperator.rescaleObjects((float)MAP_SCALE, (float)oldMapScale); // < --- funkcja z ObjectOperator.h

					// korygacja detali (jezeli sa)

						if( detailCounter )
							objectOperator.executeSpecificFunctions(DETAIL_NAME);

					// korygacja cieczy (jezeli sa)

						if( liquidCounter )
							objectOperator.executeSpecificFunctions(LIQUID_NAME);

					// korygacja skali modeli

						if( modelsOperator.modelCounter )
							objectOperator.executeSpecificFunctions(MODEL_NAME);

					// korygacja trawy

						rescaleGrass(MAP_SCALE, oldMapScale);


					// przeskalowanie pozycji kamery oraz jej focusa

						kamera.RescaleCamera( (float)MAP_SCALE / (float)oldMapScale );

					// aktualizacja kamery

						kamera.SetFocusPosition((MAP_X)/2.0,  10.f, -(MAP_Z)/2.0);
						kamera.SetBorders(-250*MAP_SCALE, 250 + MAP_X*MAP_SCALE, -250 -MAP_Z*MAP_SCALE, 250*MAP_SCALE);
						kamera.SetZoom(100.f);

					// odswiezenie zmiennej shader'a

						cgSetParameter1f(cgScale, MAP_SCALE);

					}
					break;



				// MAP_X

					case 'X':
					{
		
					// zapamietanie starego MAP_X

						int old_MAP_X = MAP_X;

					// pobranie nowego MAP_X

						MAP_X = GetDlgItemInt(terrain, IDC_TERRAIN_MAP_X, FALSE, FALSE);

					// drobne zabezpieczenia 

						if( old_MAP_X == MAP_X )
							break;

						else if( MAP_X < 4 )
							break;

					// zarezerwowanie pamieci dla pomocniczych tablic
							
						float *oldTerrain  = new float [old_MAP_X * MAP_Z];

					// skopiowanie starych danych terenu

						for(int z = 0; z < MAP_Z;     z++)
						for(int x = 0; x < old_MAP_X; x++)
						{
							index = x + old_MAP_X * z;

							oldTerrain[index] = g_terrain[index][1];
						}


					// czyszczenie starych tablic

						delete [] netArray;

						delete [] g_indexArray;
						
						delete [] g_terrain;
						delete [] g_normals;
						delete [] g_texcoordArray;


					// rezerwacja pamieci na nowe, wieksze tablice

						netArray =			new float [MAP_X * MAP_Z * 2][3];

						g_indexArray =		new GLuint [MAP_X * MAP_Z * 2];

						g_terrain =			new float [MAP_X * MAP_Z][3];
						g_normals =			new float [MAP_X * MAP_Z][3];
						g_texcoordArray =	new float [MAP_X * MAP_Z][2];


						// przegl¹da mapê wysokoœci i wyznacza wspó³rzêdne punktów

							for(int z = 0; z < MAP_Z; z++)
							for(int x = 0; x < MAP_X; x++)
							{
								index = x + MAP_X * z;

								int helperIndex = x + old_MAP_X * z;

								g_terrain[index][0] =  float(x) * MAP_SCALE;
								g_terrain[index][2] = -float(z) * MAP_SCALE;

								if( x >= old_MAP_X )
									g_terrain[index][1] = terrainHeight;
								else
									g_terrain[index][1] = oldTerrain[helperIndex];

							}

						// zwolnienie pomocniczej tablicy

							delete [] oldTerrain;


						// przegl¹da w pêtli wszystkie wierzcho³ki mapy terenu

							for (int z = 0; z < MAP_Z; z++)
							for (int x = 0; x < MAP_X; x++)
							{
								// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³

									index = z * MAP_X + x;

								// umieszcza wartoœci w tablicy wspó³rzêdnych tekstury

									g_texcoordArray[index][0] = (float) x;
									g_texcoordArray[index][1] = (float) z;

							}
					

					// g_indexArray
							
							index = 0;

							int currentVertex;

							for (int z = 0; z < MAP_Z - 1; z++)
							for (int x = 0; x < MAP_X; x++)
							{
								currentVertex = z * MAP_X + x;

								g_indexArray[index++] = currentVertex + MAP_X;
								g_indexArray[index++] = currentVertex;

							}
						

					// wyzerowanie dwoch nastepnych pozycji, zeby glDrawElements nie powodowalo "access violation"

						g_indexArray[index++] = 0;
						g_indexArray[index]	  = 0;


					// usuniecie wystajacych obiektow (jezeli sa takie)

						if( droppingObjects )
						{
							objectOperator.dropObjects();
							dropGrass();
						}

					// aktualizacja rozmiarow tablic kolorow (i kopiowanie ich wartosci)

						resizeColorArrays(old_MAP_X, 0);

					// aktualizacja kamery

						kamera.SetFocusPosition((MAP_X)/2.0,  10.f, -(MAP_Z)/2.0);
						kamera.SetBorders(-250*MAP_SCALE, 250 + MAP_X*MAP_SCALE, -250 -MAP_Z*MAP_SCALE, 250*MAP_SCALE);

					}
					break;


				// MAP_Z

					case 'Z':
					{
					// zapamietanie starego MAP_Z

						int old_MAP_Z = MAP_Z;

					// pobranie nowego MAP_X

						MAP_Z = GetDlgItemInt(terrain, IDC_TERRAIN_MAP_Z, FALSE, FALSE);


					// drobne zabezpieczenie 

						if( old_MAP_Z == MAP_Z )
							break;

						else if( MAP_Z < 4 )
							break;


					// zarezerwowanie pamieci dla pomocniczych tablic
							
						float *oldTerrain  = new float [MAP_X * old_MAP_Z];


					// skopiowanie starych danych terenu

						
						for(int z = 0; z < old_MAP_Z;     z++)
						for(int x = 0; x < MAP_X; x++)
						{
							index = x + MAP_X * z;

							oldTerrain[index] = g_terrain[index][1];
						}


					// czyszczenie starych tablic

						delete [] netArray;

						delete [] g_indexArray;
						
						delete [] g_terrain;
						delete [] g_normals;
						delete [] g_texcoordArray;


					// rezerwacja pamieci na nowe, wieksze tablice

						netArray =			new float [MAP_X * MAP_Z * 2][3];

						g_indexArray =		new GLuint [MAP_X * MAP_Z * 2];

						g_terrain =			new float [MAP_X * MAP_Z][3];
						g_normals =			new float [MAP_X * MAP_Z][3];
						g_texcoordArray =	new float [MAP_X * MAP_Z][2];


						// przegl¹da mapê wysokoœci i wyznacza wspó³rzêdne punktów

							for(int z = 0; z < MAP_Z; z++)
							for(int x = 0; x < MAP_X; x++)
							{
								index = x + MAP_X * z;

								//int helperIndex = x + old_MAP_Z * z;

								g_terrain[index][0] =  float(x) * MAP_SCALE;
								g_terrain[index][2] = -float(z) * MAP_SCALE;

								if( z >= old_MAP_Z )
									g_terrain[index][1] = terrainHeight;
								else
									g_terrain[index][1] = oldTerrain[index];
							}


						// zwolnienie pomocniczej tablicy

							delete [] oldTerrain;


						// przegl¹da w pêtli wszystkie wierzcho³ki mapy terenu

							for (int z = 0; z < MAP_Z; z++)
							for (int x = 0; x < MAP_X; x++)
							{
								// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³

									index = z * MAP_X + x;

								// umieszcza wartoœci w tablicy wspó³rzêdnych tekstury

									g_texcoordArray[index][0] = (float) x;
									g_texcoordArray[index][1] = (float) z;
							}
					

					// g_indexArray
							
							index = 0;

							int currentVertex;

							for (int z = 0; z < MAP_Z - 1; z++)
							for (int x = 0; x < MAP_X; x++)
							{
								currentVertex = z * MAP_X + x;

								g_indexArray[index++] = currentVertex + MAP_X;
								g_indexArray[index++] = currentVertex;
							}
						

					// wyzerowanie dwoch nastepnych pozycji, zeby glDrawElements nie powodowalo "access violation"

						g_indexArray[index++] = 0;
						g_indexArray[index]	  = 0;


					// usuniecie wystajacych obiektow (jezeli sa takie)

						if( droppingObjects )
						{
							objectOperator.dropObjects();
							dropGrass();
						}

					// aktualizacja rozmiarow tablic kolorow (i kopiowanie ich wartosci)

						resizeColorArrays(0, old_MAP_Z);

					// aktualizacja kamery

						kamera.SetFocusPosition((MAP_X)/2.0,  10.f, -(MAP_Z)/2.0);
						kamera.SetBorders(-250*MAP_SCALE, 250 + MAP_X*MAP_SCALE, -250 -MAP_Z*MAP_SCALE, 250*MAP_SCALE);

					}
					break;

			}// switch( key )


			// odswiezenie siatki, po zmianie terenu

				computeNet();

			// ponowne obliczenie normali

				computeNormals();

			// korygacja oswietlenia

				objectOperator.executeSpecificFunctions(LIGHT_BULB_NAME);

			// czyszczenie VBO terenu

				glDeleteBuffersARB(1, &terrainVBOv);
				glDeleteBuffersARB(1, &terrainVBOvt);
				glDeleteBuffersARB(1, &terrainVBOvn);

			// utworzenie nowych VBO

				glGenBuffersARB( 1, &terrainVBOv );		

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOv );			
				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_terrain,		GL_DYNAMIC_DRAW_ARB );

			
				glGenBuffersARB( 1, &terrainVBOvt );

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvt );
				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*2*sizeof(float), g_texcoordArray, GL_DYNAMIC_DRAW_ARB );


				glGenBuffersARB( 1, &terrainVBOvn );

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvn );
				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_normals,		GL_DYNAMIC_DRAW_ARB );
				

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			 // drobne zabezpieczenie

				if( MAP_SCALE == 0 )
					MAP_SCALE = 1;

		}// if( toolBoxRead )
	}

	void computeNet()
	{
			int index, index2 = 0;

			for(int x = 0, x2 = 0; x < MAP_X; x++, x2 += 2)
			for(int z = 0;         z < MAP_Z; z++)
			{
				index  = x + MAP_X*z;

				netArray[index2][0] = g_terrain[index][0];
				netArray[index2][1] = g_terrain[index][1] + .2f;
				netArray[index2++][2] = g_terrain[index][2];

				index  = (x+1) + MAP_X*z;

				netArray[index2][0] = g_terrain[index][0];
				netArray[index2][1] = g_terrain[index][1] + .2f;
				netArray[index2++][2] = g_terrain[index][2];
			}

			if( netVBOv != 0 )
				glDeleteBuffersARB(1, &netVBOv);

			glGenBuffersARB(1, &netVBOv);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, netVBOv);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*2*sizeof(float), netArray, GL_DYNAMIC_DRAW_ARB);


			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}


	void drawNet()
	{

			if(drawingNet)
			{
				glLineWidth(1.f);

				if( enableLighting )
					glDisable(GL_LIGHTING);

				glColor3f(1.f, 1.f, 1.f);

				glEnableClientState(GL_VERTEX_ARRAY);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, netVBOv);
				glVertexPointer(3, GL_FLOAT, 0, 0);


					glDrawArrays(GL_LINES, 0, (MAP_X-1)*MAP_Z*2);


				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
				glDisableClientState(GL_VERTEX_ARRAY);


			/*	unsigned int index;

				for(int z = 0; z < (MAP_Z*2)-1; z++)
				{
					glBegin(GL_LINES);

						for(int x = 0; x < MAP_X; x++)
						{
							index = x + MAP_X*z;
							glVertex3f(netArray[index][0], netArray[index][1], netArray[index][2]);

							index = x + MAP_X*(z+1);
							glVertex3f(netArray[index][0], netArray[index][1], netArray[index][2]);
						}
						
					glEnd();
				}*/


				if( enableLighting )
					glEnable(GL_LIGHTING);

				glLineWidth(2.0f);

			}// if(drawNet)
	}



	

	void drawTerrain()
	{
		// terrainKey jest ustawiane, jezeli zaszla jakas zmiana w kontrolkach, w zakladce "Terrain"

			if( terrainKey )
			{
				updateTerrain(terrainKey);
				
				// terrainKey = NULL; jest ustawiane w drawCloud(),
				// ktore jest wywolywane po drawTerrain();
			}


			glEnable(GL_TEXTURE_2D);

			glEnable(GL_BLEND);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


				cgGLEnableProfile(terrainVP);
				cgGLBindProgram(vertexShader);

				
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, terrainVBOv);         
				glVertexPointer(3, GL_FLOAT, 0, 0);             

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, terrainVBOvt);         
				glTexCoordPointer(2, GL_FLOAT, 0, 0);               

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, terrainVBOvn);         
				glNormalPointer(GL_FLOAT, 0, 0);              


			// przyszykowanie parametrow shader'a

				cgGLSetStateMatrixParameter(cgModelViewMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
				

				if( layerCount && RenderTextures )
				{
					cgGLEnableProfile(terrainFP);
					cgGLBindProgram(RenderMultitextures);

					for(int i = 0; i < layerCount; i++)
					{
						glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[i].VBOvc);        
						glColorPointer(4, GL_FLOAT, 0, 0);

						cgGLSetTextureParameter( cgTexture, layers[i].textureID );
						cgGLEnableTextureParameter( cgTexture );

							glDrawElements(GL_TRIANGLE_STRIP, (MAP_Z-1)*MAP_X*2, GL_UNSIGNED_INT, &g_indexArray[0]);
					}

					cgGLDisableTextureParameter( cgTexture );
					cgGLDisableProfile(terrainFP);
				}
				else
				{
					glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[0].VBOvc);        
					glColorPointer(4, GL_FLOAT, 0, 0);

						glDrawElements(GL_TRIANGLE_STRIP, (MAP_Z-1)*MAP_X*2, GL_UNSIGNED_INT, &g_indexArray[0]);

				}

				glBindTexture(GL_TEXTURE_2D, 0);
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);   
				

				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);

			  cgGLDisableProfile(terrainVP);

			glDisable(GL_TEXTURE_2D);	
	}




	void computeNormals()
	{

			try
			{

				for(int z = 0; z < MAP_Z; z++)
				for(int x = 0; x < MAP_X; x++)
				{
				// crossProduct(...) z crossProduct.h (includowany w "GlobalHeaders.h")

					if( x == MAP_X-1 && z == MAP_Z-1 )
					{
						float point2[3] = {
											g_terrain[x + MAP_X*z][0] + MAP_SCALE,
											g_terrain[x + MAP_X*z][1],
											g_terrain[x + MAP_X*z][2]
										  },

							  point3[3] = {
											g_terrain[x + MAP_X*z][0],
											g_terrain[x + MAP_X*z][1],
											g_terrain[x + MAP_X*z][2] - MAP_SCALE
										  };

						crossProduct(g_terrain[x + MAP_X*z], point2, point3,
									 g_normals[x + MAP_X*z]);
					}
					else if( x == MAP_X-1 )
					{
						float point2[3] = {
											g_terrain[x + MAP_X*z][0] + MAP_SCALE,
											g_terrain[x + MAP_X*z][1],
											g_terrain[x + MAP_X*z][2]
										  };

						crossProduct(g_terrain[x + MAP_X*z], point2, g_terrain[x + MAP_X*(z+1)],
									 g_normals[x + MAP_X*z]);
					}
					else if( z == MAP_Z-1 )
					{
						float point3[3] = {
											g_terrain[x + MAP_X*z][0],
											g_terrain[x + MAP_X*z][1],
											g_terrain[x + MAP_X*z][2] - MAP_SCALE
										  };

						crossProduct(g_terrain[x + MAP_X*z], g_terrain[x+1 + MAP_X*z], point3,
									 g_normals[x + MAP_X*z]);
					}
					else
					{
						crossProduct(g_terrain[x + MAP_X*z], g_terrain[x+1 + MAP_X*z], g_terrain[x + MAP_X*(z+1)],
									 g_normals[x + MAP_X*z]);
					}
				}
			}
			catch(...)
			{
				MessageBox(g_hwnd, "Blad przy obliczaniu normali terenu!", "Error", MB_OK);
			}
	}






	


	void saveTerrain(char *filename)
	{
			std::fstream file;

		// otworzenie pliku w trybie "nadpisz" + binarny

			file.open( filename, std::ios::out | std::ios::binary );


		// sprawdzenie, czy w pliku juz wczesniej dane ternu byly zapisywane.
		// jezeli tak, usuwa je (a dokladniej przepisuje reszte danych).

			bool markerFound = false, 
				 rewrite = false;

			std::string line;
			std::vector <std::string> lines;

			while( !file.eof() )
			{
				file >> line;

				if( line == "Terrain" )
				{
					markerFound = true;
					rewrite     = true;
				}

				if( markerFound )
				{
					if( line == "End" )
					{
						markerFound = false;
					}
				}
				else
				{
					lines.push_back( line );
				}
			}

			file.clear();

		// przepisuje wartosci "nieusuniete"

			if( rewrite )
			{
			// ustawienie nadpisywania

				file.flags( std::ios::trunc );

				for(int i = 0; i < lines.size(); i++)
				{
					file << lines[i];
				}
			}

			file.clear();


		// ustawienie dopisywania

			file.flags( std::ios::app );

		// ustawienie markera poczatkowego

			file << "\nTerrain\n";

		// zapis danych terenu

			// zapisanie rozmiarow terenu
				
				file << "\t" << MAP_X		  << "\n" 
					 << "\t" << MAP_Z		  << "\n"
					 << "\t" << MAP_SCALE	  << "\n"
					 << "\t" << terrainHeight << "\n";

			// petla zapisujaca tablice wierzcholkow, tex coords, normali, kolorow

				int currentVertex;

				for(int z = 0; z < MAP_Z; z++)
				{
					for(int x = 0; x < MAP_X; x++)
					{
						currentVertex = x + MAP_X*z;

						file << "\t" << g_terrain[currentVertex][1] << " ";
					}

					file << "\n";
				}

		// ustawienie markera koncowego

			file << "End\n\n";

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();

		// zapisanie tekstur

			saveTextures(filename);
	}

	bool loadTerrain(char *filename)
	{
			std::fstream      file;

		// otworzenie pliku w trybie binarnym

			file.open( filename, std::ios::in | std::ios::binary );

			if( !file )
			{
				return false;
			}

		// przyszykowanie glownych zmiennych

			int oldMAP_X			= MAP_X, 
				oldMAP_Z			= MAP_Z,
				oldMAP_SCALE		= MAP_SCALE,
				oldTerrainHeight    = terrainHeight;

			float *newTerrain = NULL;

			bool markerFound      = false, 
				 loadingCommenced = false,
				 loadingCorrect   = true;

			char buffer[MAX_PATH];

			strcpy_s( buffer, "Save file \"" );
			strcat_s( buffer, filename );
			strcat_s( buffer, "\" is corrupted!\n\n" );

			std::string errorInfo = buffer;

			std::string line;

		// czytanie pliku

			while( !file.eof() )
			{
			// pobranie wiersza ze strumienia

				file >> line;

			// marker check

				if( line == "Terrain" )
				{
					if( !markerFound )
					{
						markerFound = true;
					}
					else
					{
						loadingCorrect = false;
						errorInfo += "There are two or more terrain info markers (should be one).";
						break;
					}
				}

			// jezeli znaleziono marker, mozna odczytywac dane

				if( markerFound && !loadingCommenced )
				{
				// ustawienie flagi

					loadingCommenced = true;

				// licznik zaladowanych slow

					int wordCounter = 0;

				// MAP_X

					file >> line;

					if( line == "End" )
					{
						loadingCorrect = false;

						char buffer[MAX_PATH], buffer2[MAX_PATH];

						strcpy_s( buffer, "\"End\" marker found too early, while loading terrain vertices.\nWords expected: " );
						_itoa_s( 4+(MAP_X*MAP_Z), buffer2, 10 );
						strcat_s( buffer2, "\n" );
						strcat_s( buffer, buffer2 );
						strcat_s( buffer, "Words registered: " );
						_itoa_s( wordCounter, buffer2, 10 );
						strcat_s( buffer, buffer2 );

						errorInfo +=  buffer;
											
						break;
					}
					else if( !IsNumber(line, true) )
					{
						loadingCorrect = false;
						errorInfo += "Word where MAP_X value should be, isn't a number!\n\n";
						errorInfo += "\"";
						errorInfo += line;
						errorInfo += "\"";
						break;
					}
					else
					{
						MAP_X = atoi(line.c_str());
						wordCounter++;
					}

				// MAP_Z

					file >> line;

					if( line == "End"  )
					{
						loadingCorrect = false;

						char buffer[MAX_PATH], buffer2[MAX_PATH];

						strcpy_s( buffer, "\"End\" marker found too early, while loading terrain vertices.\nWords expected: " );
						_itoa_s( 4+(MAP_X*MAP_Z), buffer2, 10 );
						strcat_s( buffer2, "\n" );
						strcat_s( buffer, buffer2 );
						strcat_s( buffer, "Words registered: " );
						_itoa_s( wordCounter, buffer2, 10 );
						strcat_s( buffer, buffer2 );

						errorInfo +=  buffer;
											
						break;
					}
					else if( !IsNumber(line, true) )
					{
						loadingCorrect = false;
						errorInfo += "Word where MAP_Z value should be, isn't a number!\n\n";
						errorInfo += "\"";
						errorInfo += line;
						errorInfo += "\"";
						break;
					}
					else
					{
						MAP_Z = atoi(line.c_str());
						wordCounter++;
					}

				// MAP_SCALE

					file >> line;

					if( line == "End"  )
					{
						loadingCorrect = false;

						char buffer[MAX_PATH], buffer2[MAX_PATH];

						strcpy_s( buffer, "\"End\" marker found too early, while loading terrain vertices.\nWords expected: " );
						_itoa_s( 4+(MAP_X*MAP_Z), buffer2, 10 );
						strcat_s( buffer2, "\n" );
						strcat_s( buffer, buffer2 );
						strcat_s( buffer, "Words registered: " );
						_itoa_s( wordCounter, buffer2, 10 );
						strcat_s( buffer, buffer2 );

						errorInfo +=  buffer;
											
						break;
					}
					else if( !IsNumber(line, true) )
					{
						loadingCorrect = false;
						errorInfo += "Word where MAP_SCALE value should be, isn't a number!\n\n";
						errorInfo += "\"";
						errorInfo += line;
						errorInfo += "\"";
						break;
					}
					else
					{
						MAP_SCALE = atoi(line.c_str());
						wordCounter++;
					}

				// terrainHeight

					file >> line;

					if( line == "End"  )
					{
						loadingCorrect = false;

						char buffer[MAX_PATH], buffer2[MAX_PATH];

						strcpy_s( buffer, "\"End\" marker found too early, while loading terrain vertices.\nWords expected: " );
						_itoa_s( 4+(MAP_X*MAP_Z), buffer2, 10 );
						strcat_s( buffer2, "\n" );
						strcat_s( buffer, buffer2 );
						strcat_s( buffer, "Words registered: " );
						_itoa_s( wordCounter, buffer2, 10 );
						strcat_s( buffer, buffer2 );

						errorInfo +=  buffer;
											
						break;
					}
					else if( !IsNumber(line, true) )
					{
						loadingCorrect = false;
						errorInfo += "Word where terrainHeight value should be, isn't a number!\n\n";
						errorInfo += "\"";
						errorInfo += line;
						errorInfo += "\"";
						break;
					}
					else
					{
						terrainHeight = atoi(line.c_str());
						wordCounter++;
					}

				// sprawdzenie czy zaladowanie zmienne terenu sa poprawne

					if( MAP_X < MIN_MAP_X  || MAP_X > MAX_MAP_X )
					{
						loadingCorrect = false;

						char buffer[MAX_PATH], buffer2[MAX_PATH];

						strcpy_s( buffer, "Incorrect MAP_X value: " );
						_itoa_s( MAP_X, buffer2, 10 );
						strcat_s( buffer, buffer2 );

						errorInfo +=  buffer;

						break;
					}
					else if( MAP_Z < MIN_MAP_Z || MAP_Z > MAX_MAP_Z )  
					{
						loadingCorrect = false;

						char buffer[MAX_PATH], buffer2[MAX_PATH];

						strcpy_s( buffer, "Incorrect MAP_Z value: " );
						_itoa_s( MAP_Z, buffer2, 10 );
						strcat_s( buffer, buffer2 );

						errorInfo +=  buffer;

						break;
					}
					else if( MAP_SCALE < MIN_MAP_SCALE || MAP_SCALE > MAX_MAP_SCALE )
					{
						loadingCorrect = false;

						char buffer[MAX_PATH], buffer2[MAX_PATH];

						strcpy_s( buffer, "Incorrect MAP_SCALE value: " );
						_itoa_s( MAP_SCALE, buffer2, 10 );
						strcat_s( buffer, buffer2 );

						errorInfo +=  buffer;

						break;
					}
					else if( terrainHeight < MIN_HEIGHT || terrainHeight > MAX_HEIGHT )
					{
						loadingCorrect = false;

						char buffer[MAX_PATH], buffer2[MAX_PATH];

						strcpy_s( buffer, "Incorrect terrainHeight value: " );
						_itoa_s( terrainHeight, buffer2, 10 );
						strcat_s( buffer, buffer2 );

						errorInfo +=  buffer;

						break;
					}

				// skopiowanie wierzcholkow z pliku

					newTerrain = new float [MAP_X * MAP_Z];

					int currentVertex;

					for(int z = 0; z < MAP_Z; z++)
					for(int x = 0; x < MAP_X; x++)
					{
					// zabezpieczenie 

						if( !loadingCorrect )
							break;

						currentVertex = x + MAP_X*z;

						
						file >> line;

						if( line == "End"  )
						{
							loadingCorrect = false;

							char buffer[MAX_PATH], buffer2[MAX_PATH];

							strcpy_s( buffer, "\"End\" marker found too early, while loading terrain vertices.\nWords expected: " );
							_itoa_s( 4+(MAP_X*MAP_Z), buffer2, 10 );
							strcat_s( buffer2, "\n" );
							strcat_s( buffer, buffer2 );
							strcat_s( buffer, "Words registered: " );
							_itoa_s( wordCounter, buffer2, 10 );
							strcat_s( buffer, buffer2 );

							errorInfo +=  buffer;
												
							break;
						}
						else if( !IsNumber(line, false) )
						{
							loadingCorrect = false;
							errorInfo += "Word where vertex value should be, isn't a number: ";
							errorInfo += "\"";
							errorInfo += line;
							errorInfo += "\"";
							break;
						}
						else
						{
							newTerrain[currentVertex] = atof(line.c_str());
							wordCounter++;
						}
						
					}// podwojny for...

				// sprawdzenie czy nastepny wiersz to "End"

					if( loadingCorrect )
					{
						file >> line;

						if( line != "End" )
						{
							loadingCorrect = false;
							errorInfo +=  "Didn't found \"End\" marker after loading vertices (no marker or superflous characters).";				
							break;
						}
					}

				}// if( markerFound  && !loadingCorrect  )

			}// while( !file.eof() )

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();


			if( !markerFound )
			{
				loadingCorrect = false;
				errorInfo += "\"Terrain\" marker hasn't been found!";
			}


		// wstepne ladowanie tekstur (nie ma sensu wykonywac dalszych instrukcji, jezeli ladowanie tekstur sie nie uda)

			if( loadingCorrect )
			{
				float helper = MAP_Z;
				MAP_Z = oldMAP_Z;

				resizeColorArrays(oldMAP_X, 0);

				MAP_Z = helper;

				resizeColorArrays(0, oldMAP_Z);

				if( !loadTextures(filename) )
				{
					loadingCorrect = false;
					errorInfo += "Texture loading failed.";
				}
			}
			

		// jezeli ladowanie odbylo sie pomyslnie...

			if( loadingCorrect )
			{
			// czyszczenie i rezerwacja tablic terenu

				if( netArray != NULL )
					delete [] netArray;

				if( g_indexArray != NULL )
					delete [] g_indexArray;

				if( g_terrain != NULL )
					delete [] g_terrain;

				if( g_texcoordArray != NULL )
					delete [] g_texcoordArray;

				if( g_normals != NULL )
					delete [] g_normals;


				netArray =			new float [MAP_X * MAP_Z * 2][3];

				g_indexArray =		new GLuint [MAP_X * MAP_Z * 2];

				g_terrain =			new float [MAP_X * MAP_Z][3];
				g_texcoordArray =	new float [MAP_X * MAP_Z][2];
				g_normals =			new float [MAP_X * MAP_Z][3];


				int currentVertex, index = 0;

				for (int z = 0; z < MAP_Z; z++)
				for (int x = 0; x < MAP_X; x++)
				{
				// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³

					currentVertex = x + MAP_X*z;

					g_terrain[currentVertex][0] =  float(x) * MAP_SCALE;
					g_terrain[currentVertex][1] =  newTerrain[currentVertex];
					g_terrain[currentVertex][2] = -float(z) * MAP_SCALE;

				// umieszcza wartoœci w tablicy wspó³rzêdnych tekstury

					g_texcoordArray[currentVertex][0] = (float) x;
					g_texcoordArray[currentVertex][1] = (float) z;
				}


				for (int z = 0; z < MAP_Z - 1; z++)
				for (int x = 0; x < MAP_X; x++)
				{
					currentVertex = x + MAP_X*z;

					g_indexArray[index++] = currentVertex + MAP_X;
					g_indexArray[index++] = currentVertex;
				}
			
			// wyzerowanie dwoch nastepnych pozycji, zeby glDrawElements nie powodowalo "access violation"

				g_indexArray[index++] = 0;
				g_indexArray[index]	  = 0;

			// obliczenie nowej siatki terenu

				computeNet();

			// obliczenie normali

				computeNormals();

			// czyszczenie VBO (nigdy nie wiadomo)

				if( terrainVBOv != 0 )
					glDeleteBuffersARB(1, &terrainVBOv);

				if( terrainVBOvt != 0 )
					glDeleteBuffersARB(1, &terrainVBOvt);

				if( terrainVBOvn != 0 )
					glDeleteBuffersARB(1, &terrainVBOvn);

			// generowanie VBO terenu
				
				glGenBuffersARB( 1, &terrainVBOv );		

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOv );			
				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_terrain,		GL_DYNAMIC_DRAW_ARB );

				glGenBuffersARB( 1, &terrainVBOvt );

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvt );
				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*2*sizeof(float), g_texcoordArray, GL_DYNAMIC_DRAW_ARB );

				glGenBuffersARB( 1, &terrainVBOvn );

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvn );
				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_normals,		GL_DYNAMIC_DRAW_ARB );
				

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			// ustawienie kontrolek

				SetDlgItemInt(terrain, IDC_TERRAIN_MAP_X,      MAP_X,          FALSE);
				SetDlgItemInt(terrain, IDC_TERRAIN_MAP_Z,      MAP_Z,          FALSE);
				SetDlgItemInt(terrain, IDC_TERRAIN_MAP_SCALE,  MAP_SCALE,      FALSE);
				SetDlgItemInt(terrain, IDC_TERRAIN_HEIGHT,     terrainHeight,  FALSE);

			// aktualizacja kamery

				kamera.SetFocusPosition((MAP_X)/2.0,  10.f, -(MAP_Z)/2.0);
				kamera.SetBorders(-250*MAP_SCALE, 250 + MAP_X*MAP_SCALE, -250 -MAP_Z*MAP_SCALE, 250*MAP_SCALE);
			}
			else
			{

			// jezeli ladowanie sie nie udalo, przywraca poprzednie rozmiary terenu

				/*int helperX = MAP_X,
					helperZ = MAP_Z;*/

				//if( MAP_X != oldMAP_X )
				//{
				//	MAP_X = oldMAP_X;
				//	resizeColorArrays(MAP_X, 0);
				//	//resizeColorArrays(helperX, 0);
				//}

				//if( MAP_Z != oldMAP_Z )
				//{
				//	MAP_Z = oldMAP_Z;
				//	resizeColorArrays(0, MAP_Z);
				//	//resizeColorArrays(0, helperZ);
				//}

				MAP_X = oldMAP_X;
				MAP_Z = oldMAP_Z;

				MAP_SCALE     = oldMAP_SCALE;
				terrainHeight = oldTerrainHeight;

			// poinformowanie user'a o naturze bledu

				MessageBox(g_hwnd, errorInfo.c_str(), "Error!", MB_OK);
			}

		// wyczyszczenie pomocniczej tablicy wierzcholkow

			if( newTerrain != NULL )
				delete [] newTerrain;

			return loadingCorrect;
	}


	
	void ResetTerrain()
	{
		// czyszczenie tablic

			delete [] netArray;			    netArray		= NULL;

			delete [] g_indexArray;			g_indexArray	= NULL;
			
			delete [] g_terrain;			g_terrain		= NULL;
			delete [] g_normals;			g_normals		= NULL;
			delete [] g_texcoordArray;		g_texcoordArray = NULL;

		// czyszczenie VBO siatki

			if( netVBOv != 0 )
				glDeleteBuffersARB(1, &netVBOv);

		// czyszczenie VBO terenu

			if( terrainVBOv != 0 )
				glDeleteBuffersARB(1, &terrainVBOv);

			if( terrainVBOvt != 0 )
				glDeleteBuffersARB(1, &terrainVBOvt);

			if( terrainVBOvn != 0 )
				glDeleteBuffersARB(1, &terrainVBOvn);


		// beda potrzebne do resetu tekstur (resizeColorArrays(...))

			int oldMAP_X = MAP_X,
				oldMAP_Z = MAP_Z;

		// reset zmiennych terenu

			MAP_X = 128;
			MAP_Z = 128;    
			MAP_SCALE = 1;     
			terrainHeight = 1;

		// ustawienie kontrolek

			SetDlgItemInt(terrain, IDC_TERRAIN_MAP_X,      MAP_X,          FALSE);
			SetDlgItemInt(terrain, IDC_TERRAIN_MAP_Z,      MAP_Z,          FALSE);
			SetDlgItemInt(terrain, IDC_TERRAIN_MAP_SCALE,  MAP_SCALE,      FALSE);
			SetDlgItemInt(terrain, IDC_TERRAIN_HEIGHT,     terrainHeight,  FALSE);

		// zmienne wskazuj¹ce bie¿¹c¹ pozycjê tablicy indeksów

			int index = 0,
				currentVertex;

		// rezerwacja pamieci dla tablic

			netArray =			new float [MAP_X * MAP_Z * 2][3];

			g_indexArray =		new GLuint [MAP_X * MAP_Z * 2];

			g_terrain =			new float [MAP_X * MAP_Z][3];
			g_texcoordArray =	new float [MAP_X * MAP_Z][2];
			g_normals =			new float [MAP_X * MAP_Z][3];
			
		// g_terrain

			// przegl¹da mapê wysokoœci i wyznacza wspó³rzêdne punktów

				for(int z = 0; z < MAP_Z; z++)
				for(int x = 0; x < MAP_X; x++)
				{
				   g_terrain[x + MAP_X * z][0] = float(x) * MAP_SCALE;
				   g_terrain[x + MAP_X * z][1] = 1;
				   g_terrain[x + MAP_X * z][2] = -float(z) * MAP_SCALE;
				}

		// g_texcoordArray

			// przegl¹da w pêtli wszystkie wierzcho³ki mapy terenu

				for (int z = 0; z < MAP_Z; z++)
				for (int x = 0; x < MAP_X; x++)
				{
				// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³

					currentVertex = z * MAP_X + x;

				// umieszcza wartoœci w tablicy wspó³rzêdnych tekstury

					g_texcoordArray[currentVertex][0] = (float) x;
					g_texcoordArray[currentVertex][1] = (float) z;
				}

		// obliczenie normali terenu

			computeNormals();

		// obliczenie siatki terenu

			computeNet();
		
		// g_indexArray

				for (int z = 0; z < MAP_Z - 1; z++)
				for (int x = 0; x < MAP_X; x++)
				{
					currentVertex = z * MAP_X + x;

					g_indexArray[index++] = currentVertex + MAP_X;
					g_indexArray[index++] = currentVertex;
				}
			
		// wyzerowanie dwoch nastepnych pozycji, zeby glDrawElements nie powodowalo "access violation"

			g_indexArray[index++] = 0;
			g_indexArray[index]	  = 0;

		// generowanie VBO terenu
			
			glGenBuffersARB( 1, &terrainVBOv );		

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOv );			
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_terrain,		GL_DYNAMIC_DRAW_ARB );

			glGenBuffersARB( 1, &terrainVBOvt );

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvt );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*2*sizeof(float), g_texcoordArray, GL_DYNAMIC_DRAW_ARB );

			glGenBuffersARB( 1, &terrainVBOvn );

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvn );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_normals,		GL_DYNAMIC_DRAW_ARB );
			

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


		// reset tekstur

			ResetTerrainTextures(oldMAP_X, oldMAP_Z);

	}

	bool InitTerrain()
	{

		// inicjalizacja tekstur terenu

			if( !InitTerrainTextures() )
				return false;

			terrainContext = cgCreateContext();	

			if (terrainContext == NULL)
			{
				MessageBox(NULL, "Failed To Create Cg Context (terrain initialization)", "Error", MB_OK);
				return false;			
			}


			terrainVP = cgGLGetLatestProfile(CG_GL_VERTEX);

			if (terrainVP == CG_PROFILE_UNKNOWN)
			{
				MessageBox(NULL, "Invalid profile type (terrain initialization)", "Error", MB_OK);
				return false;	
			}

			cgGLSetOptimalOptions(terrainVP);


			vertexShader = cgCreateProgramFromFile(terrainContext,
												   CG_SOURCE,
												   TERRAIN_SHADER_PATH,
												   terrainVP,
												   TERRAIN_VERTEX_SHADER,
												   0);

			if (vertexShader == NULL)
			{
				CGerror Error = cgGetError();

				MessageBox(NULL, cgGetLastListing(terrainContext),
					//cgGetErrorString(Error),
						   "Error", MB_OK);
				return false;	
			}

			cgGLLoadProgram(vertexShader);


			terrainFP = cgGLGetLatestProfile(CG_GL_FRAGMENT);	

			if (terrainFP == CG_PROFILE_UNKNOWN)
			{
				MessageBox(NULL, "Invalid profile type (terrain initialization)", "Error", MB_OK);
				return false;	
			}

			cgGLSetOptimalOptions(terrainFP);		

			RenderMultitextures = cgCreateProgramFromFile(terrainContext, 
													      CG_SOURCE, 
														  TERRAIN_SHADER_PATH, 
														  terrainFP, 
														  TERRAIN_FRAGMENT_SHADER, 
														  0);

			if (RenderMultitextures == NULL)
			{
				CGerror Error = cgGetError();

				MessageBox(NULL, cgGetLastListing(terrainContext),
					//cgGetErrorString(Error),
						   "Error", MB_OK);
				return false;	
			}

			cgGLLoadProgram(RenderMultitextures);

		// ustawienie parametrow cg

			cgModelViewMatrix = cgGetNamedParameter(vertexShader, "ModelViewMatrix");

			// te zmienne beda ustawiane w Light.h (funkcja updateCGlightParams())
			// oraz lightingProc() w LIGHT_CHECK:

				terrainLightPosition[0]  = cgGetNamedParameter(vertexShader, "lightPosition[0]");
				terrainLightPosition[1]  = cgGetNamedParameter(vertexShader, "lightPosition[1]");
				terrainLightPosition[2]  = cgGetNamedParameter(vertexShader, "lightPosition[2]");
				terrainLightPosition[3]  = cgGetNamedParameter(vertexShader, "lightPosition[3]");
				terrainLightPosition[4]  = cgGetNamedParameter(vertexShader, "lightPosition[4]");
				terrainLightPosition[5]  = cgGetNamedParameter(vertexShader, "lightPosition[5]");
				terrainLightPosition[6]  = cgGetNamedParameter(vertexShader, "lightPosition[6]");
				terrainLightPosition[7]  = cgGetNamedParameter(vertexShader, "lightPosition[7]");
				terrainEyePosition	     = cgGetNamedParameter(vertexShader, "eyePosition");
				terrainKa			     = cgGetNamedParameter(vertexShader, "Ka");
				terrainKd[0]			 = cgGetNamedParameter(vertexShader, "Kd[0]");
				terrainKd[1]			 = cgGetNamedParameter(vertexShader, "Kd[1]");
				terrainKd[2]			 = cgGetNamedParameter(vertexShader, "Kd[2]");
				terrainKd[3]			 = cgGetNamedParameter(vertexShader, "Kd[3]");
				terrainKd[4]			 = cgGetNamedParameter(vertexShader, "Kd[4]");
				terrainKd[5]			 = cgGetNamedParameter(vertexShader, "Kd[5]");
				terrainKd[6]			 = cgGetNamedParameter(vertexShader, "Kd[6]");
				terrainKd[7]			 = cgGetNamedParameter(vertexShader, "Kd[7]");
				terrainLightCount        = cgGetNamedParameter(vertexShader, "lightCount");
				terrainEnableLighting    = cgGetNamedParameter(vertexShader, "enableLighting");


				cgScale					 = cgGetNamedParameter(vertexShader, "scale");
				cgSetParameter1f(cgScale, MAP_SCALE);
			
			cgTexture = cgGetNamedParameter(RenderMultitextures, "texture");


		// zmienne wskazuj¹ce bie¿¹c¹ pozycjê tablicy indeksów

			int index = 0,
				currentVertex;


		// zabezpieczenie
	
			if( netArray != NULL )
				delete [] netArray;

			if( g_indexArray != NULL )
				delete [] g_indexArray;

			if( g_terrain != NULL )
				delete [] g_terrain;

			if( g_texcoordArray != NULL )
				delete [] g_texcoordArray;

			if( g_normals != NULL )
				delete [] g_normals;
			
		// rezerwacja pamieci dla tablic

			netArray =			new float [MAP_X * MAP_Z * 2][3];

			g_indexArray =		new GLuint [MAP_X * MAP_Z * 2];

			g_terrain =			new float [MAP_X * MAP_Z][3];
			g_texcoordArray =	new float [MAP_X * MAP_Z][2];
			g_normals =			new float [MAP_X * MAP_Z][3];
			
		// g_terrain

			// przegl¹da mapê wysokoœci i wyznacza wspó³rzêdne punktów

				for(int z = 0; z < MAP_Z; z++)
				for(int x = 0; x < MAP_X; x++)
				{
				   g_terrain[x + MAP_X * z][0] = float(x) * MAP_SCALE;
				   g_terrain[x + MAP_X * z][1] = 1;
				   g_terrain[x + MAP_X * z][2] = -float(z) * MAP_SCALE;
				}

		// g_texcoordArray

			// przegl¹da w pêtli wszystkie wierzcho³ki mapy terenu

				for (int z = 0; z < MAP_Z; z++)
				for (int x = 0; x < MAP_X; x++)
				{
				// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³

					currentVertex = z * MAP_X + x;

				// umieszcza wartoœci w tablicy wspó³rzêdnych tekstury

					g_texcoordArray[currentVertex][0] = (float) x;
					g_texcoordArray[currentVertex][1] = (float) z;
				}

		// obliczenie normali terenu

			computeNormals();
		
		// g_indexArray

				for (int z = 0; z < MAP_Z - 1; z++)
				for (int x = 0; x < MAP_X; x++)
				{
					currentVertex = z * MAP_X + x;

					g_indexArray[index++] = currentVertex + MAP_X;
					g_indexArray[index++] = currentVertex;
				}
			
		// wyzerowanie dwoch nastepnych pozycji, zeby glDrawElements nie powodowalo "access violation"

			g_indexArray[index++] = 0;
			g_indexArray[index]	  = 0;


		// czyszczenie VBO (nigdy nie wiadomo)

			if( terrainVBOv != 0 )
				glDeleteBuffersARB(1, &terrainVBOv);

			if( terrainVBOvt != 0 )
				glDeleteBuffersARB(1, &terrainVBOvt);

			if( terrainVBOvn != 0 )
				glDeleteBuffersARB(1, &terrainVBOvn);

		// generowanie VBO terenu
			
			glGenBuffersARB( 1, &terrainVBOv );		

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOv );			
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_terrain,		GL_DYNAMIC_DRAW_ARB );

			glGenBuffersARB( 1, &terrainVBOvt );

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvt );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*2*sizeof(float), g_texcoordArray, GL_DYNAMIC_DRAW_ARB );

			glGenBuffersARB( 1, &terrainVBOvn );

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvn );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_normals,		GL_DYNAMIC_DRAW_ARB );
			

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			return true;
	}



	void CloseTerrain()
	{
		// usuniecie kontekstu cg

			cgDestroyContext(terrainContext);


		// czyszczenie tablic

			delete [] netArray;			    netArray		= NULL;

			delete [] g_indexArray;			g_indexArray	= NULL;
			
			delete [] g_terrain;			g_terrain		= NULL;
			delete [] g_normals;			g_normals		= NULL;
			delete [] g_texcoordArray;		g_texcoordArray = NULL;

		// czyszczenie VBO siatki

			if( netVBOv != 0 )
				glDeleteBuffersARB(1, &netVBOv);

		// czyszczenie VBO terenu

			if( terrainVBOv != 0 )
				glDeleteBuffersARB(1, &terrainVBOv);

			if( terrainVBOvt != 0 )
				glDeleteBuffersARB(1, &terrainVBOvt);

			if( terrainVBOvn != 0 )
				glDeleteBuffersARB(1, &terrainVBOvn);


		// zamkniecie tekstur terenu

			CloseTerrainTextures();

	}

