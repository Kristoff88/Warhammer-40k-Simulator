#pragma once



// -----------------------------------------------------------------------------------


// Function declarations
	//  to have a quick look at all the used functions (and get a heart attack)


	// ObjectOperator.h

#if 0

	// declarations below are inside a singleton class named ObjectOperator

			static ObjectOperator& Singleton();


		// deklaracje funkcji


			void objectsText();	// ustawia tekst 2D, ktory ma byc wypluty  

			void Sort();	// sortuje wektor obiektow

			void Add(Object& newObject);

			void dropObjects();

			void objectScan();

			void unselectObjects();

			void moveObjects();

			void drawObjects();

			void removeObjects();


			void adjustObjectsHeight(float modifier);

			void rescaleObjects(float newScale, float oldScale);

			bool removeSpecificObjects(std::string typeName);

			void manageKeys();

			void morphAdjustment();

			void executeSpecificFunctions(std::string typeName);


			void Clear(std::string name = "All");



		// * --- glowna funkcja --- * //

			void ObjectOperatorRun();



		ObjectOperator &objectOperator = ObjectOperator::Singleton();


#endif

	// Lights.h

		void updateCGlightParams();

		void corrigateLight(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

		void lightKeyManager(char key, void *object, float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ);

		void lightText(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);
	
		void moveLight(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

		void removeLight(void *object);
		
		void removeAllLights();

		bool preLight();
		
		void drawLightBox(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object, bool Selected);
		
		void postLight();
		
		void placeLight();
		

		void saveLights(char *filename);
		
		bool loadLights(char *filename);
		

		void generateLightBulbTexture(unsigned char *bits, unsigned int &width, unsigned int &height, FIBITMAP *dib);
	

		void InitLighting();
		
		void CloseLighting();
		

	// Details.h

		void computeDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

		void adjustDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

		bool preDetail();

		void drawDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ,  void *object, bool Selected);

		void drawFlatDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ,  void *object, bool Selected);

		void postDetail();

		void moveDetail(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

		void detailKeyManager(char key, void *object, float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ);

		void detailText(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

		void removeDetail(void *object);

		void placeDetail();

		void removeAllDetails();


		unsigned int generateDetailTexture(unsigned char *bits, unsigned int &width, unsigned int &height);

		bool loadDetailTexture(char *filename);

		int  loadDetailTextures();

		void reloadDetailTextures();

		void changeDetailTexture();

		void removeDetailTextures();


		void saveDetails(char *filename);

		bool loadDetails(char *filename);

		
		void CloseDetails();

	// Textures.h

		void resizeColorArrays(int oldX, int oldZ);

		void updateTextures();

		void paintLand();

		void setLayer();

		void removeLayer();

		void layerUp();

		void layerDown();

		void clearLand();

		void textureKeyManager(char key);

		void textureText();


		void saveTextures(char *filename, void *object);
	
		bool loadTextures(char *filename, void *object);


		unsigned int generateTerrainTexture(unsigned char *bits, unsigned int &width, unsigned int &height);

		bool loadTerrainTexture(char *filename);

		int  loadTerrainTextures();

		void reloadTerrainTextures();

		void changeTerrainTexture();

		void removeTerrainTextures();


		void ResetTerrainTextures(int oldMAP_X, int oldMAP_Z);

		bool InitTerrainTextures();

		void CloseTerrainTextures();

	// Liquids.h

		void computeLiquid(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

		bool preLiquid();

		void drawLiquid(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ,  void *object, bool Selected);

		void postLiquid();

		void moveLiquid(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

		void liquidKeyManager(char key, void *object, float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ);

		void liquidText(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

		void removeLiquid(void *object);

		void placeLiquid();

		void removeAllLiquids();


		unsigned int generateLiquidTexture(unsigned char *bits, unsigned int &width, unsigned int &height);

		bool loadLiquidTextures(char *filename);

		int  loadLiquidTextures();

		void reloadLiquidTextures();

		void changeLiquidTexture();

		void removeLiquidTextures();


		void saveLiquids(char *filename);

		bool loadLiquids(char *filename);


		bool InitLiquids();

		void CloseLiquids();

	// Grass.h

		void updateGrassVBOs();

		void clearGrassVBOs();


		void dropGrass();   // usuwa trawe wystajaca poza teren, jezeli droppingObjects = true

		void adjustGrassHeight(float modifier);

		void rescaleGrass(float newScale, float oldScale);

		void updateGrass();

		void plantGrass();

		void weedOut();

		void plantEverywhere();

		void removeAllWeeds();

		void drawGrass();

		void adjustGrassHeight();


		unsigned int generateGrassTexture(unsigned char *bits, unsigned int &width, unsigned int &height);

		bool loadGrassTexture(char *filename);

		int  loadGrassTextures();

		void reloadGrassTextures();

		void changeGrassTexture();

		void removeGrassTextures();


		void saveGrass(char *filename);

		bool loadGrass(char *filename);


		void ResetGrass();

		void CloseGrass();

	// Morphing.h

		void updateMorphing();

		void morphingText();

		void landMorph();


	// Cursor.h

		void computeCursor();

		void paintCursor();

		void CloseCursor();

	// Cubemap.h

		void generateCubeMap();

		void changeCubeMapColors();

		void drawCubeMap();

		void cleanCubeMap();


		void generateCubeMapTexture(unsigned char *bits, unsigned int &width, unsigned int &height, FIBITMAP *dib);

		bool loadCubeMapTexture(char *filename);

		int  loadCubeMapTextures();

		void reloadCubeMapTextures();

		void changeCubeMapTexture();

		void removeCubeMapTextures();


		void saveCubeMap(char *filename);

		bool loadCubeMap(char *filename);


		void ResetCubeMap();

		void CloseCubeMap();

	// Clouds.h

		void generateCloud();

		void changeCloudColors();

		void drawCloud();

		void cleanCloud();


		void generateCloudTexture(unsigned char *bits, unsigned int &width, unsigned int &height, FIBITMAP *dib);

		bool loadCloudTexture(char *filename);

		int  loadCloudTextures();

		void reloadCloudTextures();

		void changeCloudTexture();

		void removeCloudTextures();


		void saveCloud(char *filename);

		bool loadCloud(char *filename);


		void ResetCloud();

		void CloseCloud();


	// Terrain.h

		void equilaizeTerrain();

		void adjustTerrain();

		void updateTerrain(char key);

		void computeNet();

		void drawNet();

		void drawTerrain();

		void computeNormals();


		void saveTerrain(char *filename);

		bool loadTerrain(char *filename);


		void ResetTerrain();

		bool InitTerrain();

		void CloseTerrain();


// adres do operatora modeli

	//OBJ_Loader::Singleton();


	// Models.h

#if 0

			static ModelsOperator& Singleton();

			static void generateModelTexture(unsigned char *bits, unsigned int &width, unsigned int &height, FIBITMAP *dib)
			

			void placeModel();
			void removeModel();


			bool loadModel(char *filename) throw(std::string);
			int  loadModels();
			void reloadModels();
			void changeModel();
			void removeModelObjects();

		
			static void scaleModel(float &, float &, float &, float &sizeX, float &sizeY, float &sizeZ, void *object);
		
			static bool preModel();

			static void drawModel(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ,  void *object, bool Selected);

			static void postModel();

			static void moveModel(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);

			static void modelKeyManager(char key, void *object, float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ);

			static void removeModel(void *object);

			static void modelText(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object);
			
			void removeAllModels();


			void saveModels(char *filename);
		
			bool loadModels(char *filename);


			bool loadModelTexture(char *filename);
			int  loadModelTextures();
			void reloadModelTextures();
			void changeModelTexture();
			void removeModelTextures();

#endif


	// TextOperator.h

		void TextOperator(unsigned int Action);

		bool InitTextOperator();

		void CloseTextOperator();

	// MenuOperator.h

		void New();

		bool SaveAs();

		void Save();

		void Load();

		void Exit();


	// *** Core Editor Functions ***

		bool ResetEditor();

		bool InitEditor();

		void CloseEditor();


		void ActionOperator();

		void editRun();

// -----------------------------------------------------------------------------------

	#include "TerrainEditorHeaders.h"





	bool ResetEditor()
	{

	// reset terenu

		ResetTerrain();

	// reset cubemap

		ResetCubeMap();

	// reset chmur

		ResetCloud();

	// ponowna inicjalizacja kamery

		InitCameraOperator();

	// wyczyszczenie bufora obiektow

		objectOperator.Clear();

	// reset operatora trawy

		ResetGrass();

		return true;
	}


	bool InitEditor()
	{
		// pierdoly
		glLineWidth(2.f);
		glPointSize(4.f);


		if( !OBJ_Loader::Singleton().Init() )
		{
			MessageBox(g_hwnd, "Nieudana inicjalizacja loader'a plikow OBJ.", "Error", MB_OK);
			return false;
		}

		InitCameraOperator();


	// ustawienie obrazkow tekstur

		// poczatkowo landBrush, detailBrush, liquidBrush, grassBrush, etc. == 0; 
		// jezeli sa one == 0, to change*Texture() ustawia brak obrazka.

		changeTerrainTexture();
		changeDetailTexture();
		changeLiquidTexture();
		changeGrassTexture();
		modelsOperator.changeModelTexture();
		changeCubeMapTexture();
		changeCloudTexture();


	// przygotowanie progress bar'a

		ProgressDialog progressDlg(g_hInstance, dialog, "",
								   0, 9, 1);

	// Ladowanie tekstur oraz modeli

		progressDlg.SetCaption("Loading terrain textures...");
		loadTerrainTextures();
		progressDlg.Increment();

		progressDlg.SetCaption("Loading detail textures...");
		loadDetailTextures();
		progressDlg.Increment();

		progressDlg.SetCaption("Loading liquid textures...");
		loadLiquidTextures();
		progressDlg.Increment();

		progressDlg.SetCaption("Loading grass textures...");
		loadGrassTextures();
		progressDlg.Increment();

		progressDlg.SetCaption("Loading model textures...");
		modelsOperator.loadModelTextures();
		progressDlg.Increment();

		progressDlg.SetCaption("Loading cubemap textures...");
		loadCubeMapTextures();
		progressDlg.Increment();

		progressDlg.SetCaption("Loading cloud textures...");
		loadCloudTextures();
		progressDlg.Increment();



		progressDlg.SetCaption("Loading models...");
		modelsOperator.loadModels();
		progressDlg.Increment();



		generateCubeMap();
		generateCloud();


		InitLighting();
	
		if( !InitTerrain() )
			return false;

		if( !InitLiquids() )
			return false;

		if( !InitTextOperator() )
			return false;

		return true;
	}




	void CloseEditor()
	{
		objectOperator.Clear();

		OBJ_Loader::Singleton().Close();

		CloseLighting();	// < --- musi byc po Clear() (kasuje ono tablice dostepnych swiatelek, ktora moze byc uzyta w Remove() swiatelka)

		CloseDetails();

		CloseTerrain();

		CloseLiquids();

		CloseGrass();

		CloseCubeMap();

		CloseCloud();

		CloseTextOperator();

		CloseCursor();
	}




// obsluguje akcje na podstawie zmiennej Action
void ActionOperator()
{
	// obsluga roznego rodzaju akcji

		if( Action )
		{
				switch( Action )
				{

				// morfowanie terenu
					case MORPH:
					{
					
						if( mouseMoved )
						{
							computeCursor();

							// Rzeczy zalezne od poruszenia myszka (odswiezenie pozycji kursora, zmiana pozycji morphownia etc.),
							// sa zalezne od zmiany pozycji myszy. Funkcje wykonujace te rzeczy, sprawdzaja if(mouseMoved).
							// Tutaj ta flaga jest ustawiana na FALSE, w celu poprawnej pracy. Jest ona ustawiana na TRUE w 
							// WndProc() w evencie WM_MOUSEMOVE.
							mouseMoved = false;
						}


						landMorph();

						paintCursor();

						
						objectOperator.unselectObjects();

						objectOperator.morphAdjustment(); // <--- funkcja z ObjectOperator.h (poprawia ona wysokosci niektorych obiektow, w trakcie morphowanai terenu)

						objectOperator.scanReady = false;
					}
					break;


				// umieszczanie zrodla swiatla
					case PLACE_LIGHT:
					{
						if(lButtonDown)
						{
							 placeLight();
							 Action = 0;

							 objectOperator.scanReady = true;

							 Saved = false;
						}
						else
							objectOperator.scanReady = false;
					}
					break;

				// usuwanie wszystkich swiatel ze sceny
					case REMOVE_LIGHTS:
					{
						removeAllLights();
						Action = 0;
					}
					break;



				// malowanie tekstur ladu
					case PAINT_LAND:
					{
						if( mouseMoved )
						{
							computeCursor();

							// Rzeczy zalezne od poruszenia myszka (odswiezenie pozycji kursora, zmiana pozycji morphownia etc.),
							// sa zalezne od zmiany pozycji myszy. Funkcje wykonujace te rzeczy, sprawdzaja if(mouseMoved).
							// Tutaj ta flaga jest ustawiana na FALSE, w celu poprawnej pracy. Jest ona ustawiana na TRUE w 
							// WndProc() w evencie WM_MOUSEMOVE.
							mouseMoved = false;
						}

						objectOperator.unselectObjects();

						paintLand();
						paintCursor();

						objectOperator.scanReady = false;
					}
					break;


				// "czyszczenie" warstw terenu
					case CLEAR_LAND:
						clearLand();
						Action = 0;
					break;

				// dodanie warstwy tekstury
					case SET_LAYER:
						setLayer();
						Action = 0;
					break;

				// usuniecie warstwu tekstury
					case REMOVE_LAYER:
						removeLayer();
						Action = 0;
					break;

				// ustawienie drugorzednej tekstury
					case LAYER_UP:
						layerUp();
						Action = 0;
					break;

				// usuniecie drugorzednej tekstury
					case LAYER_DOWN:
						layerDown();
						Action = 0;
					break;



				// ladowanie tekstur terenu
					case RELOAD_TEXTURES:
						reloadTerrainTextures();
						Action = 0;
					break;

				// zmiana tekstury ladu
					case CHANGE_TEXTURE:
						changeTerrainTexture();
						Action = 0;
					break;



				// umieszczanie detalu
					case PLACE_DETAIL:
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

						if( lButtonDown )
						{
							placeDetail();

							Action = 0;

							objectOperator.scanReady = true;

							Saved = false;
						}
						else
							objectOperator.scanReady = false;
					}
					break;

				// usuwanie wszystkich detali
					case REMOVE_DETAILS:
					{
						removeAllDetails();
						Action = 0;
						Saved = false;
					}
					break;


				// reload tekstur detali
					case RELOAD_DETAILS:
						reloadDetailTextures();
						Action = 0;
					break;

				// zmiana tekstury detali
					case CHANGE_DETAIL:
						changeDetailTexture();
						Action = 0;
					break;




				// plantowanie trawy pedzelkiem
					case PLANT_GRASS:
					{
						if( mouseMoved )
						{
							computeCursor();

							mouseMoved = false;
						}

						objectOperator.unselectObjects();

						plantGrass();
						paintCursor();

						objectOperator.scanReady = false;
					}	
					break;

				// usuwanie trawy pedzelkiem
					case WEED_OUT:
					{
						if( mouseMoved )
						{
							computeCursor();

							mouseMoved = false;
						}

						objectOperator.unselectObjects();

						weedOut();
						paintCursor();

						objectOperator.scanReady = false;
					}	
					break;

				// pokrycie calego terenu sciolka
					case FILL_GRASS:
					{
						plantEverywhere();
						Action = 0;
						Saved = false;
					}
					break;

				// calkowite usuwanie sciolki
					case REMOVE_GRASS:
					{
						removeAllWeeds();
						Action = 0;
						Saved = false;
					}
					break;

				// reload tekstur trawy
					case RELOAD_GRASS:
						reloadGrassTextures();
						Action = 0;
					break;

				// zmiana tekstury trawy
					case CHANGE_GRASS:
						changeGrassTexture();
						Action = 0;
					break;


					

				// umieszczanie wody
					case PLACE_LIQUID:
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

						if( lButtonDown )
						{
							placeLiquid();
							Action = 0;

							objectOperator.scanReady = true;

							Saved = false;
						}
						else
							objectOperator.scanReady = false;
					}
					break;


				// usuwanie wszystkich wod
					case REMOVE_LIQUIDS:
					{
						removeAllLiquids();
						Action = 0;
						Saved = false;
					}
					break;

				// ladowanie tekstury wody
					case RELOAD_LIQUIDS:
					{
						reloadLiquidTextures();
						Action = 0;
					}
					break;


				// zmiana tekstury wody
					case CHANGE_LIQUID:
					{
						changeLiquidTexture();
						Action = 0;
					}
					break;


				
				
				// wyrownywanie terenu wobec terrainHeight
					case EQUILIZE_TERRAIN:
					{
						equilaizeTerrain();
						Action = 0;
					}
					break;

				// dostosowanie terenu do nowych ogranicznikow wysokosci
					case ADJUST_TERRAIN:
					{
						adjustTerrain();
						Action = 0;
					}
					break;


				
				// umieszczanie modelu na mapie
					case PLACE_MODEL:
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

						if( lButtonDown )
						{
							modelsOperator.placeModel();

							Action = 0;
							objectOperator.scanReady = true;
							Saved = false;
						}
						else
							objectOperator.scanReady = false;
					}
					break;

				// usuwanie wszystkich modeli
					case REMOVE_MODELS:
					{
						modelsOperator.removeAllModels();
						Action = 0;
					}
					break;

				// reload tekstur modeli
					case RELOAD_MODEL_TEXTURES:
					{
						modelsOperator.reloadModelTextures();
						Action = 0;
					}
					break;

				// zmiana tekstury modelu
					case CHANGE_MODEL_TEXTURE:
					{
						modelsOperator.changeModelTexture();
						Action = 0;
					}
					break;

				// reload model object'ow
					case RELOAD_MODELS:
					{
						modelsOperator.reloadModels();
						Action = 0;
					}
					break;

				// zmiana aktualnego model object'a 
					case CHANGE_MODEL:
					{
						modelsOperator.changeModel();
						Action = 0;
					}
					break;

				

				// zmiana kolorow cubemap
					case CHANGE_CUBEMAP_COLORS:
					{
						changeCubeMapColors();
						Action = 0;
						Saved = false;
					}
					break;

				// reload tekstur cubemap
					case RELOAD_CUBEMAPS:
					{
						reloadCubeMapTextures();
						Action = 0;
					}
					break;

				// zmiana tekstury cubemap
					case CHANGE_CUBEMAP:
					{
						changeCubeMapTexture();
						Action = 0;
						Saved = false;
					}
					break;


				// zmiana kolorow chmur
					case UPDATE_CLOUD:
					{
						updateCloud();
						Action = 0;
						Saved = false;
					}
					break;

				// reload tekstur chmur
					case RELOAD_CLOUDS:
					{
						reloadCloudTextures();
						Action = 0;
					}
					break;

				// zmiana tekstury chmur
					case CHANGE_CLOUD:
					{
						changeCloudTexture();
						Action = 0;
						Saved = false;
					}
					break;


					default:
					break;
				}

		}// Action
		else
		{
			objectOperator.scanReady = true;
		}

}

// glowna funkcja edytora
void editRun()
{

		// napierdalaj kamerzysto!

			cameraRun();

		// korygacja swiatel

			if( cameraPosChanged ) // < -- ustawiane w cameraRun()
			{
				objectOperator.executeSpecificFunctions(LIGHT_BULB_NAME);
				cameraPosChanged = false;
			}


		// obsluga klawisza tekstur
			
			if( textureKey )
			{
				textureKeyManager(textureKey);
				textureKey = NULL;
			}

		// rysowanie powierzchni terenu

			drawTerrain();

		// rysowanie siatki terenu

			drawNet();

		// rysowanie cubemap

			drawCubeMap();

		// rysowanie chmur

			drawCloud();

		// obsluga obiektow na scenie

			objectOperator.ObjectOperatorRun();

		// trawka

			if( ShowGrass )
				drawGrass();

		
		

		// zmienna pomocnicza

			unsigned int TextAction = Action;

		// obsluga akcji

			ActionOperator();

		// wyswietlanie tekstu 2D

			if( render2Dtext )
			{
				objectOperator.objectsText();
				TextOperator(TextAction);
			}
}