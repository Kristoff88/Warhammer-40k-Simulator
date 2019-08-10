
	#define MAP_FOLDER "TerrainData\\Maps"
	

	// funkcje

		void New()
		{
			int result;
			
		// sprawdza czy obecny stan jest zapisany

			if( Saved )
				result = IDNO;
			else
				result = MessageBox(g_hwnd, "Current work hasn't been saved. Do you want to save it now?", "Warning.", MB_YESNO);

		// jezeli nie (i uzytkownik tego sobie zyczy), nalezy zapisac obecny stan

			bool cancelPressed = false;

			if( result == IDYES )
			{
				if( strcmp( lastSave, "" ) == 0 )
					
					if( !SaveAs() )
						cancelPressed = true;
				else
					Save();
			}

			if( !cancelPressed )
			{
			// reset edytora

				ResetEditor();

			// reset flagi oraz tablicy z nazwa ostatniego zapisu

				Saved = true;
				strcpy_s( lastSave, UNNAMED );
			}
		}

		bool SaveAs()
		{
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_hwnd;
			ofn.lpstrFilter = "Saved files (*.sav)\0*.sav"; 
			ofn.lpstrFile = szFileName;
			ofn.lpstrInitialDir = MAP_FOLDER;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;// | OFN_FILEMUSTEXIST;
			ofn.lpstrDefExt = ".sav";

			if( GetSaveFileName(&ofn) )
			{
				ProgressDialog progressDlg(g_hInstance, g_hwnd, "", 1, 9, 1);


				progressDlg.SetCaption("Saving terrain data...");
				saveTerrain( szFileName );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving grass data...");
				saveGrass( szFileName );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving details data...");
				saveDetails( szFileName );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving liquids data...");
				saveLiquids( szFileName );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving models data...");
				modelsOperator.saveModels( szFileName );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving lights data...");
				saveLights( szFileName );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving clouds data...");
				saveCloud( szFileName );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving cubemap data...");
				saveCubeMap( szFileName );
				progressDlg.Increment();

				Saved = true;
				strcpy_s( lastSave, szFileName );

				return true;
			}
			else
				return false;

		}

		void Save()
		{

			if( strcmp( lastSave, UNNAMED ) == 0 )
			{
				SaveAs();
			}
			else
			{
				ProgressDialog progressDlg(g_hInstance, g_hwnd, "", 1, 9, 1);

				progressDlg.SetCaption("Saving terrain data...");
				saveTerrain( lastSave );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving grass data...");
				saveGrass( lastSave );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving details data...");
				saveDetails( lastSave );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving liquids data...");
				saveLiquids( lastSave );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving models data...");
				modelsOperator.saveModels( lastSave );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving lights data...");
				saveLights( lastSave );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving clouds data...");
				saveCloud( lastSave );
				progressDlg.Increment();

				progressDlg.SetCaption("Saving cubemap data...");
				saveCubeMap( lastSave );
				progressDlg.Increment();


				Saved = true;
			}

		}


		void Load()
		{
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_hwnd;
			ofn.lpstrFilter = "Saved files (*.sav)\0*.sav"; 
			ofn.lpstrFile = szFileName;
			ofn.lpstrInitialDir = MAP_FOLDER;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
			ofn.lpstrDefExt = "sav";

			if( GetOpenFileName(&ofn) )
			{
			// ladowanie terenu

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

			// reset flagi oraz tablicy z nazwa ostatniego zapisu

				Saved = true;
				strcpy_s( lastSave, szFileName );

				Action = NONE;
			}
		}


		void Exit()
		{
			int result = 0;

			if( !Saved )
			{
				result = MessageBox(g_hwnd, "Do you want to save the state of work, before exiting the application?", "Warning.", MB_YESNOCANCEL);

				if( result == IDYES )
				{
					if( strcmp( lastSave, UNNAMED ) == 0 )
					{	
						if( !SaveAs() )
							result = IDCANCEL;
					}
					else
					{
						Save();
					}
				}
			}
			
			if( result != IDCANCEL )
				isDone = TRUE;
		}