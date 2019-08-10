#pragma once


	#include "TerrainEditorHeaders.h"


	#define MAPS_FOLDER ("TerrainData\\Maps\\")




	bool ResetEditor()
	{
	// reset terenu

		ResetTerrain();

	// ponowna inicjalizacja kamery

		InitCameraOperator();


		ResetCubeMap();
		ResetCloud();
	
		objectOperator.Clear();

		ResetGrass();

		return true;
	}


	bool InitEditor()
	{
		// pierdoly
		glLineWidth(2.f);
		glPointSize(4.f);


	
	// przygotowanie progress bar'a

		ProgressDialog progressDlg(g_hInstance, g_hwnd, "",
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
	}
//
/****************************************************************/
	
	float* GetHeightMap ()
	{
		float* heightMap = new float[MAP_X*MAP_Z];

		int index;

		for(int x = 0; x < MAP_X; x++)
		for(int z = 0; z < MAP_Z; z++)
		{
			index = x + MAP_X*z;
			heightMap[index] = g_terrain[index][1];
		}

		return heightMap;
	}
//
/****************************************************************/

	std::vector <std::string> GetMapList ()
	{
		std::vector <std::string> mapList;
		mapList = FolderBrowser::Singleton().GetFileList(MAPS_FOLDER, "*.sav");

		return mapList;
	}

	void LoadMap (char *filename)
	{
		char szFileName[MAX_PATH];

		strcpy_s(szFileName, MAX_PATH, MAPS_FOLDER);
		strcat_s(szFileName, MAX_PATH, filename);


		ProgressDialog progressDlg(g_hInstance, g_hwnd, "", 1, 9, 1);

	// wstepne wyczyszczenie wektora obiektow

		objectOperator.Clear();

		progressDlg.SetCaption("Loading terrain data...");
		if( !loadTerrain( szFileName ) )
			return;
		progressDlg.Increment();

		progressDlg.SetCaption("Loading grass data...");
		if( !loadGrass( szFileName ) )
		{
			ResetGrass();
		}
		progressDlg.Increment();

		progressDlg.SetCaption("Loading details data...");
		if( !loadDetails( szFileName ) )
		{
			objectOperator.Clear(DETAIL_NAME);
		}
		progressDlg.Increment();

		progressDlg.SetCaption("Loading liquids data...");
		if( !loadLiquids( szFileName ) )
		{
			objectOperator.Clear(LIQUID_NAME);
		}
		progressDlg.Increment();

		progressDlg.SetCaption("Loading models data...");
		if( !modelsOperator.loadModels( szFileName ) )
		{
			objectOperator.Clear(MODEL_NAME);
		}
		progressDlg.Increment();

		progressDlg.SetCaption("Loading lights data...");
		if( !loadLights( szFileName ) )
		{
			objectOperator.Clear(LIGHT_BULB_NAME);
		}
		progressDlg.Increment();

		progressDlg.SetCaption("Loading clouds data...");
		if( !loadCloud( szFileName ) )
		{
			ResetCloud();
		}
		progressDlg.Increment();

		progressDlg.SetCaption("Loading cubemap data...");
		if( !loadCubeMap( szFileName ) )
		{
			ResetCubeMap();
		}
		progressDlg.Increment();

		

		InitCameraOperator();
	}
//
/****************************************************************/

	void editRun()
	{
	// korygacja swiatel

		if( cameraPosChanged ) // < -- ustawiane w cameraRun()
		{
			objectOperator.executeSpecificFunctions(LIGHT_BULB_NAME);
			cameraPosChanged = false;
		}

		drawTerrain();
		drawCubeMap();
		drawCloud();

		objectOperator.ObjectOperatorRun();

		if( ShowGrass )
		{
			drawGrass();
		}
	}