#pragma once


// zewnetrzne zmienne
	
	extern  HWND cubemap,
		         cloud;


/*****************************************************************************

 Procedury okienkowe poszczegolnych zakladek ToolBox'a

*****************************************************************************/

BOOL CALLBACK lightingProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		{	
			int index = IDC_AMB_R;

			for(int i = 0; i < 6; i++)
				SetDlgItemInt(hwnd, index++, 0, FALSE);


			// przyszykowanie slider'ow

				HWND helper = GetDlgItem(hwnd, IDC_AMB_R_SLIDER);

				DWORD word;   // zakres <max, min> slider'ow
				word = ((WORD)255 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);


				helper = GetDlgItem(hwnd, IDC_AMB_G_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);


				helper = GetDlgItem(hwnd, IDC_AMB_B_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);




				helper = GetDlgItem(hwnd, IDC_DIFF_R_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);


				helper = GetDlgItem(hwnd, IDC_DIFF_G_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);


				helper = GetDlgItem(hwnd, IDC_DIFF_B_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);


			return TRUE;
		}
		break;


		case WM_COMMAND:
		{

			switch(LOWORD(wParam))
			{

			// korygowanie edit kontrolek....

				case IDC_AMB_R:
				{
					if( GetDlgItemInt(hwnd, IDC_AMB_R, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_AMB_R, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_AMB_R_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_AMB_R, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;
				case IDC_AMB_G:
				{
					if( GetDlgItemInt(hwnd, IDC_AMB_G, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_AMB_G, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_AMB_G_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_AMB_G, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;
				case IDC_AMB_B:
				{
					if( GetDlgItemInt(hwnd, IDC_AMB_B, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_AMB_B, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_AMB_B_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_AMB_B, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_DIFF_R:
				{
					if( GetDlgItemInt(hwnd, IDC_DIFF_R, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_DIFF_R, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_DIFF_R_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_DIFF_R, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;
				case IDC_DIFF_G:
				{
					if( GetDlgItemInt(hwnd, IDC_DIFF_G, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_DIFF_G, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_DIFF_G_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_DIFF_G, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;
				case IDC_DIFF_B:
				{
					if( GetDlgItemInt(hwnd, IDC_DIFF_B, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_DIFF_B, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_DIFF_B_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_DIFF_B, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


			// koniec korygowania... (uff!)


				case IDC_ADD_LIGHT:
					Action = PLACE_LIGHT;
					SetFocus(g_hwnd);
				break;

				case IDC_REMOVE_LIGHT:
					Action = REMOVE_LIGHTS;
					SetFocus(g_hwnd);
				break;

				case IDC_ENABLE_LIGHT_CHECK:
				{
					enableLighting = !enableLighting;

					if( enableLighting )
					{
						glEnable(GL_LIGHTING);
						cgSetParameter1f(terrainEnableLighting, 1);
						cgSetParameter1f(liquidEnableLighting,  1);
					}
					else
					{
						glDisable(GL_LIGHTING);
						cgSetParameter1f(terrainEnableLighting, 0);
						cgSetParameter1f(liquidEnableLighting,  0);
					}

					SetFocus(g_hwnd);
				}
				break;

				case IDC_LIGHT_CHECK:
					showLights = !showLights;
					SetFocus(g_hwnd);
				break;


				default:
				break;
			}

		}
		break; //(LOWORD(wParam))


	// odswiezenie slider'ow

		case WM_HSCROLL:
		{
					
					if( (HWND)lParam == GetDlgItem(hwnd, IDC_AMB_R_SLIDER) )
						
						SetDlgItemInt(hwnd, IDC_AMB_R, SendMessageA(GetDlgItem(hwnd, IDC_AMB_R_SLIDER), TBM_GETPOS, 0, 0), FALSE);

					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_AMB_G_SLIDER) )
						
						SetDlgItemInt(hwnd, IDC_AMB_G, SendMessageA(GetDlgItem(hwnd, IDC_AMB_G_SLIDER), TBM_GETPOS, 0, 0), FALSE);

					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_AMB_B_SLIDER) )
						
						SetDlgItemInt(hwnd, IDC_AMB_B, SendMessageA(GetDlgItem(hwnd, IDC_AMB_B_SLIDER), TBM_GETPOS, 0, 0), FALSE);



					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_DIFF_R_SLIDER) )
						
						SetDlgItemInt(hwnd, IDC_DIFF_R, SendMessageA(GetDlgItem(hwnd, IDC_DIFF_R_SLIDER), TBM_GETPOS, 0, 0), FALSE);

					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_DIFF_G_SLIDER) )
						
						SetDlgItemInt(hwnd, IDC_DIFF_G, SendMessageA(GetDlgItem(hwnd, IDC_DIFF_G_SLIDER), TBM_GETPOS, 0, 0), FALSE);

					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_DIFF_B_SLIDER) )
					
						SetDlgItemInt(hwnd, IDC_DIFF_B, SendMessageA(GetDlgItem(hwnd, IDC_DIFF_B_SLIDER), TBM_GETPOS, 0, 0), FALSE);

		}
		break;


		default:
		   return FALSE;
	}

	return TRUE;
}






BOOL CALLBACK texturesProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		{	

		// edits'y

			SetDlgItemInt(hwnd, IDC_TERRAIN_ALPHA,     255, FALSE);

		// combo box'y

			HWND helper = GetDlgItem(hwnd, IDC_TERRAIN_COMBO);
			ComboBox_SetItemHeight(helper, 0, 15);

			helper = GetDlgItem(hwnd, IDC_TERRAIN_LAYER_COMBO);
			ComboBox_SetItemHeight(helper, 0, 15);


			SetDlgItemInt(hwnd, IDC_TEXT_X_EDIT,  textureRangeX,     FALSE);
			SetDlgItemInt(hwnd, IDC_TEXT_Z_EDIT,  textureRangeZ,     FALSE);

		// przyszykowanie slider'ow

			helper = GetDlgItem(hwnd, IDC_TERRAIN_ALPHA_SLIDER);

			DWORD word;   // zakres <max, min> slider'ow
			word = ((WORD)255 << 16) | (WORD)0;

				SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
				SendMessageA(helper, TBM_SETRANGE, 0, word);

				SendMessageA(helper, TBM_SETPOS, TRUE, 255);

			return TRUE;
		}
		break;


		case WM_COMMAND:
		{

			switch( LOWORD(wParam) )
			{

				case IDC_TERRAIN_ALPHA:
				{
					if( GetDlgItemInt(hwnd, IDC_TERRAIN_ALPHA, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_TERRAIN_ALPHA, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_TERRAIN_ALPHA_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_TERRAIN_ALPHA, FALSE, FALSE));
				}
				break;



				case IDC_TEXT_X_EDIT:
				{
					if( GetDlgItemInt(hwnd, IDC_TEXT_X_EDIT, FALSE, FALSE) > MAP_X/2 )
						SetDlgItemInt(hwnd, IDC_TEXT_X_EDIT, MAP_Z/2, FALSE);

					else if( GetDlgItemInt(hwnd, IDC_TEXT_X_EDIT, FALSE, FALSE) == 0 )
						SetDlgItemInt(hwnd, IDC_TEXT_X_EDIT, 1, FALSE);
				}
				break;
				case IDC_TEXT_Z_EDIT:
				{
					if( GetDlgItemInt(hwnd, IDC_TEXT_Z_EDIT, FALSE, FALSE) > MAP_Z/2 )
						SetDlgItemInt(hwnd, IDC_TEXT_Z_EDIT, MAP_Z/2, FALSE);

					else if( GetDlgItemInt(hwnd, IDC_TEXT_Z_EDIT, FALSE, FALSE) == 0 )
						SetDlgItemInt(hwnd, IDC_TEXT_Z_EDIT, 1, FALSE);
				}
				break;



				case IDC_TERRAIN_COMBO:
					Action = CHANGE_TEXTURE;
				break;

				case IDC_TERRAIN_RELOAD: 
					Action = RELOAD_TEXTURES;
					SetFocus(g_hwnd);
				break;

				case IDC_CLEAR: 
					Action = CLEAR_LAND;
					SetFocus(g_hwnd);
				break;

				case IDC_SET_LAYER: 
					Action = SET_LAYER;
					SetFocus(g_hwnd);
				break;

				case IDC_REMOVE_LAYER:
					Action = REMOVE_LAYER;
					SetFocus(g_hwnd);
				break;

				case IDC_LAYER_UP: 
					Action = LAYER_UP;
					SetFocus(g_hwnd);
				break;

				case IDC_LAYER_DOWN:
					Action = LAYER_DOWN;
					SetFocus(g_hwnd);
				break;


				case IDC_TEXTURE_CHECK:
					OnlyTexture = !OnlyTexture;
					SetFocus(g_hwnd);
				break;

				case IDC_HIDE_CHECK:
					RenderTextures = !RenderTextures;
					SetFocus(g_hwnd);
				break;

				case IDC_SOLID_BRUSH_CHECK:
					SolidBrush = !SolidBrush;
					SetFocus(g_hwnd);
				break;


				default:
				break;
			}

		}
		break; //(LOWORD(wParam))



		// odswiezenie slider'ow

		case WM_HSCROLL:
		{

				if( (HWND)lParam == GetDlgItem(hwnd, IDC_TERRAIN_ALPHA_SLIDER) )

					SetDlgItemInt(hwnd, IDC_TERRAIN_ALPHA, SendMessageA(GetDlgItem(hwnd, IDC_TERRAIN_ALPHA_SLIDER), TBM_GETPOS, 0, 0), FALSE);
		}
		break;



		default:
		   return FALSE;
	}

	return TRUE;
}








BOOL CALLBACK detailsProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		{	

			// edits'y

				SetDlgItemInt(hwnd, IDC_DETAIL_RED,   255, FALSE);
				SetDlgItemInt(hwnd, IDC_DETAIL_GREEN, 255, FALSE);
				SetDlgItemInt(hwnd, IDC_DETAIL_BLUE,  255, FALSE);
				SetDlgItemInt(hwnd, IDC_DETAIL_ALPHA, 255, FALSE);


			// combo box

				HWND helper = GetDlgItem(hwnd, IDC_DETAIL_COMBO);
				ComboBox_SetItemHeight(helper, 0, 15);


			// przyszykowanie slider'ow

				helper = GetDlgItem(hwnd, IDC_DETAIL_RED_SLIDER);

				DWORD word;   // zakres <max, min> slider'ow
				word = ((WORD)255 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);


				helper = GetDlgItem(hwnd, IDC_DETAIL_GREEN_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);



				helper = GetDlgItem(hwnd, IDC_DETAIL_BLUE_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);


				helper = GetDlgItem(hwnd, IDC_DETAIL_ALPHA_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);


			return TRUE;
		}
		break;


		case WM_COMMAND:
		{

			switch( LOWORD(wParam) )
			{

				case IDC_DETAIL_RED:
				{
					if( GetDlgItemInt(hwnd, IDC_DETAIL_RED, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_DETAIL_RED, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_DETAIL_RED_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_DETAIL_RED, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_DETAIL_GREEN:
				{
					if( GetDlgItemInt(hwnd, IDC_DETAIL_GREEN, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_DETAIL_GREEN, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_DETAIL_GREEN_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_DETAIL_GREEN, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_DETAIL_BLUE:
				{
					if( GetDlgItemInt(hwnd, IDC_DETAIL_BLUE, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_DETAIL_BLUE, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_DETAIL_BLUE_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_DETAIL_BLUE, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_DETAIL_ALPHA:
				{
					if( GetDlgItemInt(hwnd, IDC_DETAIL_ALPHA, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_DETAIL_ALPHA, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_DETAIL_ALPHA_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_DETAIL_ALPHA, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;



				case IDC_DETAIL_CHECK:
					showDetails = !showDetails;
					SetFocus(g_hwnd);
				break;

				case IDC_FLAT_CHECK:
					Flat = !Flat;
					SetFocus(g_hwnd);
				break;




				case IDC_PLACE_DETAIL:
					Action = PLACE_DETAIL;
					SetFocus(g_hwnd);
				break;

				case IDC_REMOVE_ALL_DETAILS:
					Action = REMOVE_DETAILS;
					SetFocus(g_hwnd);
				break;


				case IDC_DETAIL_RELOAD: 
					Action = RELOAD_DETAILS;
					SetFocus(g_hwnd);
				break;


				case IDC_DETAIL_COMBO:
					Action = CHANGE_DETAIL;
				break;


				default:
				break;
			}

		}
		break; //(LOWORD(wParam))



		// odswiezenie slider'ow

		case WM_HSCROLL:
		{

				if( (HWND)lParam == GetDlgItem(hwnd, IDC_DETAIL_RED_SLIDER) )

					SetDlgItemInt(hwnd, IDC_DETAIL_RED, SendMessageA(GetDlgItem(hwnd, IDC_DETAIL_RED_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_DETAIL_GREEN_SLIDER) )

					SetDlgItemInt(hwnd, IDC_DETAIL_GREEN, SendMessageA(GetDlgItem(hwnd, IDC_DETAIL_GREEN_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_DETAIL_BLUE_SLIDER) )

					SetDlgItemInt(hwnd, IDC_DETAIL_BLUE, SendMessageA(GetDlgItem(hwnd, IDC_DETAIL_BLUE_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_DETAIL_ALPHA_SLIDER) )

					SetDlgItemInt(hwnd, IDC_DETAIL_ALPHA, SendMessageA(GetDlgItem(hwnd, IDC_DETAIL_ALPHA_SLIDER), TBM_GETPOS, 0, 0), FALSE);
		}
		break;



		default:
		   return FALSE;
	}

	return TRUE;
}








BOOL CALLBACK morphingProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		{	

			HWND helper = GetDlgItem(hwnd, IDC_TYPE_COMBO);
			ComboBox_SetItemHeight(helper, 0, 15);

			ComboBox_AddString(helper, "Convex");
			ComboBox_AddString(helper, "Concave");
			ComboBox_AddString(helper, "Flat");
			ComboBox_AddString(helper, "Simple");

			ComboBox_SetCurSel(helper, ComboBox_FindString(helper, 0, "Simple") );


			SetDlgItemInt(hwnd, IDC_SPEED_EDIT,   morphingSpeed,     FALSE);
			SetDlgItemInt(hwnd, IDC_X_EDIT,	      morphRangeX,       FALSE);
			SetDlgItemInt(hwnd, IDC_Z_EDIT,	      morphRangeZ,       FALSE);
			SetDlgItemInt(hwnd, IDC_RAND_EDIT,    morphRandGradient, FALSE);


			HWND edit = GetDlgItem(hwnd, IDC_RAND_EDIT);
			EnableWindow(edit, FALSE);

			return TRUE;
		}
		break;


		case WM_COMMAND:
		{

			switch(LOWORD(wParam))
			{

				case IDC_X_EDIT:
				{
					if( GetDlgItemInt(hwnd, IDC_X_EDIT, FALSE, FALSE) == 0 )
						SetDlgItemInt(hwnd, IDC_X_EDIT, 1, FALSE);

					else if( GetDlgItemInt(hwnd, IDC_X_EDIT, FALSE, FALSE) > MAP_X/2 )
						SetDlgItemInt(hwnd, IDC_X_EDIT, MAP_Z/2, FALSE);
				}
				break;
				case IDC_Z_EDIT:
				{
					if( GetDlgItemInt(hwnd, IDC_Z_EDIT, FALSE, FALSE) == 0 )
						SetDlgItemInt(hwnd, IDC_Z_EDIT, 1, FALSE);

					else if( GetDlgItemInt(hwnd, IDC_Z_EDIT, FALSE, FALSE) > MAP_Z/2 )
						SetDlgItemInt(hwnd, IDC_Z_EDIT, MAP_Z/2, FALSE);
				}
				break;


				case IDC_SPEED_EDIT:
				{
					if( GetDlgItemInt(hwnd, IDC_SPEED_EDIT, FALSE, FALSE) > 1000 )
						SetDlgItemInt(hwnd, IDC_SPEED_EDIT, 1000, FALSE);
				}
				break;




				case IDC_RAND_CHECK:
				{
					morphRandom = !morphRandom;

					HWND edit = GetDlgItem(hwnd, IDC_RAND_EDIT);

					if(morphRandom)
						EnableWindow(edit, TRUE);
					else
						EnableWindow(edit, FALSE);

					SetFocus(g_hwnd);
				}
				break;

				default:
				break;
			}

		}
		break; //(LOWORD(wParam))


		default:
		   return FALSE;
	}

	return TRUE;
}





BOOL CALLBACK terrainProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		{	

			SetDlgItemInt(hwnd, IDC_TERRAIN_HEIGHT,  		terrainHeight,			FALSE);
			SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_SCALE,		  MAP_SCALE,   			FALSE);
			SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_X,				MAP_X,   			FALSE);
			SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_Z,				MAP_Z,   			FALSE);
			SetDlgItemInt(hwnd, IDC_TERRAIN_MAX_HEIGHT,		MAX_TERRAIN_HEIGHT,   	FALSE);
			SetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT,		MIN_TERRAIN_HEIGHT,   	FALSE);



			// przyszykowanie slider'ow

				HWND helper = GetDlgItem(hwnd, IDC_TERRAIN_HEIGHT_SLIDER);

				DWORD word;   // zakres <max, min> slider'ow
				word = ((WORD)MAX_HEIGHT << 16) | (WORD)MIN_HEIGHT;
			

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);



				helper = GetDlgItem(hwnd, IDC_TERRAIN_MAP_SCALE_SLIDER);

				word = ((WORD)MAX_MAP_SCALE << 16) | (WORD)MIN_MAP_SCALE;
			

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, MAP_SCALE);



				helper = GetDlgItem(hwnd, IDC_TERRAIN_MAP_X_SLIDER);

				word = ((WORD)MAX_MAP_X << 16) | (WORD)MIN_MAP_X;
			

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, MAP_X);



				helper = GetDlgItem(hwnd, IDC_TERRAIN_MAP_Z_SLIDER);

				word = ((WORD)MAX_MAP_Z << 16) | (WORD)MIN_MAP_Z;
			

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, MAP_Z);



				helper = GetDlgItem(hwnd, IDC_TERRAIN_MAX_HEIGHT_SLIDER);

				word = ((WORD)OVERALL_MAX_HEIGHT << 16) | (WORD)OVERALL_MIN_HEIGHT;
			

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, MAX_TERRAIN_HEIGHT);




				helper = GetDlgItem(hwnd, IDC_TERRAIN_MIN_HEIGHT_SLIDER);

				word = ((WORD)OVERALL_MAX_HEIGHT << 16) | (WORD)OVERALL_MIN_HEIGHT;
			

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, MIN_TERRAIN_HEIGHT);
					


			return TRUE;
		}
		break;


		case WM_COMMAND:
		{

			switch( LOWORD(wParam) )
			{

				case IDC_TERRAIN_HEIGHT:
				{
					if( GetDlgItemInt(hwnd, IDC_TERRAIN_HEIGHT, FALSE, FALSE) > MAX_HEIGHT )
						SetDlgItemInt(hwnd, IDC_TERRAIN_HEIGHT, MAX_HEIGHT, FALSE);

					if( GetDlgItemInt(hwnd, IDC_TERRAIN_HEIGHT, FALSE, FALSE) < MIN_HEIGHT )
						SetDlgItemInt(hwnd, IDC_TERRAIN_HEIGHT, MIN_HEIGHT, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_TERRAIN_HEIGHT_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_TERRAIN_HEIGHT, FALSE, FALSE));

					terrainKey = 'H';
				}
				break;


				case IDC_TERRAIN_MAP_SCALE:
				{
					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MAP_SCALE, FALSE, FALSE) > MAX_MAP_SCALE )
						SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_SCALE, MAX_MAP_SCALE, FALSE);

					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MAP_SCALE, FALSE, FALSE) < MIN_MAP_SCALE )
						SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_SCALE, MIN_MAP_SCALE, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_TERRAIN_MAP_SCALE_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_TERRAIN_MAP_SCALE, FALSE, FALSE));
				
					terrainKey = 'S';
				}
				break;


				case IDC_TERRAIN_MAP_X:
				{
					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MAP_X, FALSE, FALSE) > MAX_MAP_X )
						SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_X, MAX_MAP_X, FALSE);

					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MAP_X, FALSE, FALSE) < MIN_MAP_X )
						SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_X, MIN_MAP_X, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_TERRAIN_MAP_X_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_TERRAIN_MAP_X, FALSE, FALSE));
				
					terrainKey = 'X';
				}
				break;


				case IDC_TERRAIN_MAP_Z:
				{
					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MAP_Z, FALSE, FALSE) > MAX_MAP_Z )
						SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_Z, MAX_MAP_Z, FALSE);

					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MAP_Z, FALSE, FALSE) < MIN_MAP_Z )
						SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_Z, MIN_MAP_Z, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_TERRAIN_MAP_Z_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_TERRAIN_MAP_Z, FALSE, FALSE));
				
					terrainKey = 'Z';
				}
				break;



				case IDC_TERRAIN_MAX_HEIGHT:
				{
					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MAX_HEIGHT, FALSE, FALSE) > OVERALL_MAX_HEIGHT )

						SetDlgItemInt(hwnd, IDC_TERRAIN_MAX_HEIGHT, OVERALL_MAX_HEIGHT, FALSE);


					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MAX_HEIGHT, FALSE, FALSE) < GetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT, FALSE, FALSE) )

						SetDlgItemInt(hwnd, IDC_TERRAIN_MAX_HEIGHT, GetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT, FALSE, FALSE), FALSE);


					SendMessageA(GetDlgItem(hwnd, IDC_TERRAIN_MAX_HEIGHT_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_TERRAIN_MAX_HEIGHT, FALSE, FALSE));
				
				}
				break;



				case IDC_TERRAIN_MIN_HEIGHT:
				{
					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT, FALSE, FALSE) > GetDlgItemInt(hwnd, IDC_TERRAIN_MAX_HEIGHT, FALSE, FALSE) )

						SetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT, GetDlgItemInt(hwnd, IDC_TERRAIN_MAX_HEIGHT, FALSE, FALSE), FALSE);


					if( GetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT, FALSE, FALSE) < OVERALL_MIN_HEIGHT )

						SetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT, OVERALL_MIN_HEIGHT, FALSE);


					SendMessageA(GetDlgItem(hwnd, IDC_TERRAIN_MIN_HEIGHT_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT, FALSE, FALSE));
				
				}
				break;






			
				case IDC_TERRAIN_AFFECT_CHECK:
				{
					affectWholeTerrain = !affectWholeTerrain;
					SetFocus(g_hwnd);
				}
				break;


				case IDC_TERRAIN_DROP_CHECK:
				{
					droppingObjects = !droppingObjects;
					SetFocus(g_hwnd);
				}
				break;





				case IDC_TERRAIN_EQUILIZE:
				{
					Action = EQUILIZE_TERRAIN;
					SetFocus(g_hwnd);
				}
				break;


				case IDC_TERRAIN_ADJUST:
				{
					Action = ADJUST_TERRAIN;
					SetFocus(g_hwnd);
				}
				break;


				default:
				break;

			}//(LOWORD(wParam))

		}
		break; 




		// odswiezenie slider'ow

		case WM_HSCROLL:
		{

				if( (HWND)lParam == GetDlgItem(hwnd, IDC_TERRAIN_HEIGHT_SLIDER) )
				{
					SetDlgItemInt(hwnd, IDC_TERRAIN_HEIGHT,		SendMessageA( GetDlgItem(hwnd, IDC_TERRAIN_HEIGHT_SLIDER),	    TBM_GETPOS, 0, 0), FALSE);

					terrainKey = 'H';

				}
				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_TERRAIN_MAP_SCALE_SLIDER) )
				{
					SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_SCALE,	SendMessageA( GetDlgItem(hwnd, IDC_TERRAIN_MAP_SCALE_SLIDER),	TBM_GETPOS, 0, 0), FALSE);

					terrainKey = 'S';

				}
				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_TERRAIN_MAP_X_SLIDER) )
				{
					SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_X,		SendMessageA( GetDlgItem(hwnd, IDC_TERRAIN_MAP_X_SLIDER),		TBM_GETPOS, 0, 0), FALSE);

					terrainKey = 'X';
				}
				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_TERRAIN_MAP_Z_SLIDER) )
				{
					SetDlgItemInt(hwnd, IDC_TERRAIN_MAP_Z,		SendMessageA( GetDlgItem(hwnd, IDC_TERRAIN_MAP_Z_SLIDER),		TBM_GETPOS, 0, 0), FALSE);

					terrainKey = 'Z';
				}
				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_TERRAIN_MAX_HEIGHT_SLIDER) )
				{
					SetDlgItemInt(hwnd, IDC_TERRAIN_MAX_HEIGHT,		SendMessageA( GetDlgItem(hwnd, IDC_TERRAIN_MAX_HEIGHT_SLIDER),		TBM_GETPOS, 0, 0), FALSE);
				}
				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_TERRAIN_MIN_HEIGHT_SLIDER) )
				{
					SetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT,		SendMessageA( GetDlgItem(hwnd, IDC_TERRAIN_MIN_HEIGHT_SLIDER),		TBM_GETPOS, 0, 0), FALSE);
				}		

		}
		break;


		default:
		   return FALSE;
	}

	return TRUE;
}





BOOL CALLBACK liquidsProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		{	

			SetDlgItemInt(hwnd, IDC_LIQUID_MAX,     terrainHeight,      FALSE);
			SetDlgItemInt(hwnd, IDC_LIQUID_MIN,		terrainHeight,      FALSE);
	
			SetDlgItemInt(hwnd, IDC_LIQUID_STORMY,			  0,		FALSE);
			SetDlgItemInt(hwnd, IDC_LIQUID_WAVE,			  0,		FALSE);
			SetDlgItemInt(hwnd, IDC_LIQUID_STRETCH,			  1,		FALSE);
	
			SetDlgItemInt(hwnd, IDC_LIQUID_SPEED,			  1,		FALSE);
			SetDlgItemInt(hwnd, IDC_LIQUID_ALPHA,		    200,		FALSE);

			SetDlgItemInt(hwnd, IDC_LIQUID_X,				   1,       FALSE);
			SetDlgItemInt(hwnd, IDC_LIQUID_Z,				   1,       FALSE);


			// przyszykowanie slider'ow

				HWND helper = GetDlgItem(hwnd, IDC_LIQUID_MAX_SLIDER);

				DWORD word;   // zakres <max, min> slider'ow
				word = ((WORD)MAX_TERRAIN_HEIGHT << 16) | (WORD)MIN_TERRAIN_HEIGHT;
			

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, terrainHeight);


				helper = GetDlgItem(hwnd, IDC_LIQUID_MIN_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, terrainHeight);


				helper = GetDlgItem(hwnd, IDC_LIQUID_SPEED_SLIDER);

				word = ((WORD)2500 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 1);


				helper = GetDlgItem(hwnd, IDC_LIQUID_STORMY_SLIDER);

				word = ((WORD)2500 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);



				helper = GetDlgItem(hwnd, IDC_LIQUID_WAVE_SLIDER);

				word = ((WORD)100 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);


				helper = GetDlgItem(hwnd, IDC_LIQUID_STRETCH_SLIDER);

				word = ((WORD)25 << 16) | (WORD)1;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 0);



				helper = GetDlgItem(hwnd, IDC_LIQUID_ALPHA_SLIDER);

				word = ((WORD)200 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 200);



				helper = GetDlgItem(hwnd, IDC_LIQUID_X_DIR_SLIDER);

				word = ((WORD)2 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 1);


				helper = GetDlgItem(hwnd, IDC_LIQUID_Z_DIR_SLIDER);

				word = ((WORD)2 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 1);



			return TRUE;
		}
		break;


		case WM_COMMAND:
		{

			switch(LOWORD(wParam))
			{

			// obsluga editow

				case IDC_LIQUID_ALPHA:
				{
					if( GetDlgItemInt(hwnd, IDC_LIQUID_ALPHA, FALSE, FALSE) > 200 )
						SetDlgItemInt(hwnd, IDC_LIQUID_ALPHA, 200, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_LIQUID_ALPHA_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_LIQUID_ALPHA, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_LIQUID_SPEED:
				{
					if( GetDlgItemInt(hwnd, IDC_LIQUID_SPEED, FALSE, FALSE) > 2500 )
						SetDlgItemInt(hwnd, IDC_LIQUID_SPEED, 2500, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_LIQUID_SPEED_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_LIQUID_SPEED, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_LIQUID_MAX:
				{	
					if( GetDlgItemInt(hwnd, IDC_LIQUID_MAX, FALSE, FALSE) > MAX_TERRAIN_HEIGHT )
						SetDlgItemInt(hwnd, IDC_LIQUID_MAX, MAX_TERRAIN_HEIGHT, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_LIQUID_MAX_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_LIQUID_MAX, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_LIQUID_MIN:
				{
					if( GetDlgItemInt(hwnd, IDC_LIQUID_MIN, FALSE, FALSE) < MIN_TERRAIN_HEIGHT )
						SetDlgItemInt(hwnd, IDC_LIQUID_MIN, MIN_TERRAIN_HEIGHT, FALSE);

					else if( GetDlgItemInt(hwnd, IDC_LIQUID_MIN, FALSE, FALSE) > MAX_TERRAIN_HEIGHT )
						SetDlgItemInt(hwnd, IDC_LIQUID_MIN, MAX_TERRAIN_HEIGHT, FALSE);


					SendMessageA(GetDlgItem(hwnd, IDC_LIQUID_MIN_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_LIQUID_MIN, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_LIQUID_STORMY:
				{
					if( GetDlgItemInt(hwnd, IDC_LIQUID_STORMY, FALSE, FALSE) > 2500 )
						SetDlgItemInt(hwnd, IDC_LIQUID_STORMY, 2500, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_LIQUID_STORMY_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_LIQUID_STORMY, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_LIQUID_WAVE:
				{
					if( GetDlgItemInt(hwnd, IDC_LIQUID_WAVE, FALSE, FALSE) > 100 )
						SetDlgItemInt(hwnd, IDC_LIQUID_WAVE, 100, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_LIQUID_WAVE_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_LIQUID_WAVE, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_LIQUID_STRETCH:
				{
					if( GetDlgItemInt(hwnd, IDC_LIQUID_STRETCH, FALSE, FALSE) > 25 )
						SetDlgItemInt(hwnd, IDC_LIQUID_STRETCH, 25, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_LIQUID_STRETCH_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_LIQUID_STRETCH, FALSE, FALSE));

					objectOperator.keyPressed = 'L';
				}
				break;



				case IDC_LIQUID_X:
				{
					if( GetDlgItemInt(hwnd, IDC_LIQUID_X, FALSE, FALSE) > MAP_X/2 )
						SetDlgItemInt(hwnd, IDC_LIQUID_X, MAP_X/2, FALSE);
					else if( GetDlgItemInt(hwnd, IDC_LIQUID_X, FALSE, FALSE) < 1 )
						SetDlgItemInt(hwnd, IDC_LIQUID_X, 1, FALSE);

					objectOperator.keyPressed = 'L';
				}
				break;


				case IDC_LIQUID_Z:
				{
					if( GetDlgItemInt(hwnd, IDC_LIQUID_Z, FALSE, FALSE) > MAP_Z/2 )
						SetDlgItemInt(hwnd, IDC_LIQUID_Z, MAP_Z/2, FALSE);
					else if( GetDlgItemInt(hwnd, IDC_LIQUID_Z, FALSE, FALSE) < 1 )
						SetDlgItemInt(hwnd, IDC_LIQUID_Z, 1, FALSE);

					objectOperator.keyPressed = 'L';
				}
				break;


			// check...

				case IDC_LIQUID_CHECK:
					showLiquids = !showLiquids;
					SetFocus(g_hwnd);
				break;


			// obsluga buttonow

				case IDC_LIQUID_RELOAD:
					Action = RELOAD_LIQUIDS;
					SetFocus(g_hwnd);
				break;

				case IDC_LIQUID_COMBO:
					Action = CHANGE_LIQUID;
				break;

				case IDC_LIQUID_PLACE:
					Action = PLACE_LIQUID;
					SetFocus(g_hwnd);
				break;

				case IDC_LIQUID_CLEAR:
					Action = REMOVE_LIQUIDS;
					SetFocus(g_hwnd);
				break;


				default:
				break;
			}

		}
		break; //(LOWORD(wParam))


	// odswiezenie slider'ow

		case WM_HSCROLL:
		{
			
				if( (HWND)lParam == GetDlgItem(hwnd, IDC_LIQUID_MAX_SLIDER) )

					SetDlgItemInt(hwnd, IDC_LIQUID_MAX,		SendMessageA( GetDlgItem(hwnd, IDC_LIQUID_MAX_SLIDER),		TBM_GETPOS, 0, 0), FALSE);


				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_LIQUID_MIN_SLIDER) )
				
					SetDlgItemInt(hwnd, IDC_LIQUID_MIN,		SendMessageA( GetDlgItem(hwnd, IDC_LIQUID_MIN_SLIDER),		TBM_GETPOS, 0, 0), FALSE);


				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_LIQUID_SPEED_SLIDER) )
				
					SetDlgItemInt(hwnd, IDC_LIQUID_SPEED,	SendMessageA( GetDlgItem(hwnd, IDC_LIQUID_SPEED_SLIDER),	TBM_GETPOS, 0, 0), FALSE);


				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_LIQUID_STORMY_SLIDER) )
				
					SetDlgItemInt(hwnd, IDC_LIQUID_STORMY,	SendMessageA( GetDlgItem(hwnd, IDC_LIQUID_STORMY_SLIDER),	TBM_GETPOS, 0, 0), FALSE);


				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_LIQUID_WAVE_SLIDER) )
				
					SetDlgItemInt(hwnd, IDC_LIQUID_WAVE,	SendMessageA( GetDlgItem(hwnd, IDC_LIQUID_WAVE_SLIDER),	    TBM_GETPOS, 0, 0), FALSE);


				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_LIQUID_STRETCH_SLIDER) )
				
					SetDlgItemInt(hwnd, IDC_LIQUID_STRETCH,	SendMessageA( GetDlgItem(hwnd, IDC_LIQUID_STRETCH_SLIDER),	TBM_GETPOS, 0, 0), FALSE);


				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_LIQUID_ALPHA_SLIDER) )
				
					SetDlgItemInt(hwnd, IDC_LIQUID_ALPHA,	SendMessageA( GetDlgItem(hwnd, IDC_LIQUID_ALPHA_SLIDER),	TBM_GETPOS, 0, 0), FALSE);


				objectOperator.keyPressed = 'L';

		}
		break;


		default:
		   return FALSE;
	}

	return TRUE;
}







BOOL CALLBACK grassProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		{	

			SetDlgItemInt(hwnd, IDC_GRASS_X_EDIT,  grassRangeX,     FALSE);
			SetDlgItemInt(hwnd, IDC_GRASS_Z_EDIT,  grassRangeZ,     FALSE);

			SetDlgItemInt(hwnd, IDC_GRASS_SIZE,			  1,     FALSE);
			SetDlgItemInt(hwnd, IDC_GRASS_HEIGHT,		  1,     FALSE);
			SetDlgItemInt(hwnd, IDC_GRASS_DENSITY,		  1,     FALSE);

			SetDlgItemInt(hwnd, IDC_GRASS_SIZE2,		  1,     FALSE);
			SetDlgItemInt(hwnd, IDC_GRASS_HEIGHT2,		  1,     FALSE);
			SetDlgItemInt(hwnd, IDC_GRASS_DENSITY2,		  1,     FALSE);

			SetDlgItemInt(hwnd, IDC_GRASS_RED,		      255,   FALSE);
			SetDlgItemInt(hwnd, IDC_GRASS_GREEN,		  255,   FALSE);
			SetDlgItemInt(hwnd, IDC_GRASS_BLUE,			  255,   FALSE);
			SetDlgItemInt(hwnd, IDC_GRASS_ALPHA,		  255,   FALSE);

		// przyszykowanie slider'ow

			HWND helper = GetDlgItem(hwnd, IDC_GRASS_RED_SLIDER);

			DWORD word;   // zakres <max, min> slider'ow
			word = ((WORD)255 << 16) | (WORD)0;
		

				SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
				SendMessageA(helper, TBM_SETRANGE, 0, word);

				SendMessageA(helper, TBM_SETPOS, TRUE, 255);


			helper = GetDlgItem(hwnd, IDC_GRASS_GREEN_SLIDER);

				SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
				SendMessageA(helper, TBM_SETRANGE, 0, word);

				SendMessageA(helper, TBM_SETPOS, TRUE, 255);


			helper = GetDlgItem(hwnd, IDC_GRASS_BLUE_SLIDER);

				SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
				SendMessageA(helper, TBM_SETRANGE, 0, word);

				SendMessageA(helper, TBM_SETPOS, TRUE, 255);


			helper = GetDlgItem(hwnd, IDC_GRASS_ALPHA_SLIDER);

				SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
				SendMessageA(helper, TBM_SETRANGE, 0, word);

				SendMessageA(helper, TBM_SETPOS, TRUE, 255);
	
			return TRUE;
		}
		break;


		case WM_COMMAND:
		{

			switch(LOWORD(wParam))
			{

				case IDC_GRASS_X_EDIT:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_X_EDIT, FALSE, FALSE) > MAP_X/2 )
						SetDlgItemInt(hwnd, IDC_GRASS_X_EDIT, MAP_Z/2, FALSE);

					else if( GetDlgItemInt(hwnd, IDC_GRASS_X_EDIT, FALSE, FALSE) == 0 )
						SetDlgItemInt(hwnd, IDC_GRASS_X_EDIT, 1, FALSE);
				}
				break;
				case IDC_GRASS_Z_EDIT:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_Z_EDIT, FALSE, FALSE) > MAP_Z/2 )
						SetDlgItemInt(hwnd, IDC_GRASS_Z_EDIT, MAP_Z/2, FALSE);

					else if( GetDlgItemInt(hwnd, IDC_GRASS_Z_EDIT, FALSE, FALSE) == 0 )
						SetDlgItemInt(hwnd, IDC_GRASS_Z_EDIT, 1, FALSE);
				}
				break;



				case IDC_GRASS_SIZE:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_SIZE, FALSE, FALSE) > GetDlgItemInt(hwnd, IDC_GRASS_SIZE2, FALSE, FALSE) )
						SetDlgItemInt(hwnd, IDC_GRASS_SIZE, GetDlgItemInt(hwnd, IDC_GRASS_SIZE2, FALSE, FALSE), FALSE);
				}
				break;

				case IDC_GRASS_HEIGHT:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_HEIGHT, FALSE, FALSE) > GetDlgItemInt(hwnd, IDC_GRASS_HEIGHT2, FALSE, FALSE) )
						SetDlgItemInt(hwnd, IDC_GRASS_HEIGHT, GetDlgItemInt(hwnd, IDC_GRASS_HEIGHT2, FALSE, FALSE), FALSE);
				}
				break;

				case IDC_GRASS_DENSITY:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_DENSITY, FALSE, FALSE) > GetDlgItemInt(hwnd, IDC_GRASS_DENSITY2, FALSE, FALSE) )
						SetDlgItemInt(hwnd, IDC_GRASS_DENSITY, GetDlgItemInt(hwnd, IDC_GRASS_DENSITY2, FALSE, FALSE), FALSE);
				}
				break;



				case IDC_GRASS_SIZE2:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_SIZE2, FALSE, FALSE) > 100 )
						SetDlgItemInt(hwnd, IDC_GRASS_SIZE2, 100, FALSE);
				}
				break;

				case IDC_GRASS_HEIGHT2:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_HEIGHT2, FALSE, FALSE) > 100 )
						SetDlgItemInt(hwnd, IDC_GRASS_HEIGHT2, 100, FALSE);
				}
				break;

				case IDC_GRASS_DENSITY2:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_DENSITY2, FALSE, FALSE) > 100 )
						SetDlgItemInt(hwnd, IDC_GRASS_DENSITY2, 100, FALSE);
				}
				break;




				case IDC_GRASS_RED:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_RED, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_GRASS_RED, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_GRASS_RED_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_GRASS_RED, FALSE, FALSE));
				}
				break;


				case IDC_GRASS_GREEN:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_GREEN, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_GRASS_GREEN, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_GRASS_GREEN_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_GRASS_GREEN, FALSE, FALSE));
				}
				break;


				case IDC_GRASS_BLUE:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_BLUE, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_GRASS_BLUE, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_GRASS_BLUE_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_GRASS_BLUE, FALSE, FALSE));
				}
				break;


				case IDC_GRASS_ALPHA:
				{
					if( GetDlgItemInt(hwnd, IDC_GRASS_ALPHA, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_GRASS_ALPHA, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_GRASS_ALPHA_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_GRASS_ALPHA, FALSE, FALSE));
				}
				break;




				case IDC_GRASS_CHECK:
				{
					ShowGrass = !ShowGrass;
					adjustGrassHeight();
					SetFocus(g_hwnd);
				}
				break;




				case IDC_GRASS_RELOAD:
					Action = RELOAD_GRASS;
					SetFocus(g_hwnd);
				break;

				case IDC_GRASS_COMBO:
					Action = CHANGE_GRASS;
				break;


				//case IDC_PLANT:
				//	Action = 19;
				//	SetFocus(g_hwnd);
				//break;

				//case IDC_WEED:
				//	Action = 20;
				//	SetFocus(g_hwnd);
				//break;


				case IDC_GRASS_FILL:
					Action = FILL_GRASS;
					SetFocus(g_hwnd);
				break;

				case IDC_GRASS_CLEAR:
					Action = REMOVE_GRASS;
					SetFocus(g_hwnd);
				break;



				default:
				break;
			}

		}
		break; //(LOWORD(wParam))


	// odswiezenie slider'ow

		case WM_HSCROLL:
		{
			
					if( (HWND)lParam == GetDlgItem(hwnd, IDC_GRASS_RED_SLIDER) )

						SetDlgItemInt(hwnd, IDC_GRASS_RED,		SendMessageA( GetDlgItem(hwnd, IDC_GRASS_RED_SLIDER),		TBM_GETPOS, 0, 0), FALSE);


					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_GRASS_GREEN_SLIDER) )
					
						SetDlgItemInt(hwnd, IDC_GRASS_GREEN,		SendMessageA( GetDlgItem(hwnd, IDC_GRASS_GREEN_SLIDER),		TBM_GETPOS, 0, 0), FALSE);


					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_GRASS_BLUE_SLIDER) )
					
						SetDlgItemInt(hwnd, IDC_GRASS_BLUE,	SendMessageA( GetDlgItem(hwnd, IDC_GRASS_BLUE_SLIDER),	TBM_GETPOS, 0, 0), FALSE);


					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_GRASS_ALPHA_SLIDER) )
					
						SetDlgItemInt(hwnd, IDC_GRASS_ALPHA,	SendMessageA( GetDlgItem(hwnd, IDC_GRASS_ALPHA_SLIDER),	TBM_GETPOS, 0, 0), FALSE);


		}
		break;


		default:
		   return FALSE;
	}

	return TRUE;
}





BOOL CALLBACK modelsProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

		switch( Message )
		{

			case WM_INITDIALOG:
			{	
				
				// ustawienie edit controls

					SetDlgItemInt(hwnd, IDC_MODEL_SCALE,  10000,   FALSE);

					SetDlgItemInt(hwnd, IDC_MODEL_RED,    255,     FALSE);
					SetDlgItemInt(hwnd, IDC_MODEL_GREEN,  255,     FALSE);
					SetDlgItemInt(hwnd, IDC_MODEL_BLUE,   255,     FALSE);
					SetDlgItemInt(hwnd, IDC_MODEL_ALPHA,  255,     FALSE);


				// ustawienie slider'ow

					HWND helper = GetDlgItem(hwnd, IDC_MODEL_SCALE_SLIDER);

					DWORD word;   // zakres <max, min> slider'ow
					word = ((WORD)20000 << 16) | (WORD)1;


						SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
						SendMessageA(helper, TBM_SETRANGE, 0, word);

						SendMessageA(helper, TBM_SETPOS, TRUE, 9999);




					helper = GetDlgItem(hwnd, IDC_MODEL_RED_SLIDER);

					word = ((WORD)255 << 16) | (WORD)0;


						SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
						SendMessageA(helper, TBM_SETRANGE, 0, word);

						SendMessageA(helper, TBM_SETPOS, TRUE, 255);



					helper = GetDlgItem(hwnd, IDC_MODEL_GREEN_SLIDER);

						SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
						SendMessageA(helper, TBM_SETRANGE, 0, word);

						SendMessageA(helper, TBM_SETPOS, TRUE, 255);



					helper = GetDlgItem(hwnd, IDC_MODEL_BLUE_SLIDER);

						SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
						SendMessageA(helper, TBM_SETRANGE, 0, word);

						SendMessageA(helper, TBM_SETPOS, TRUE, 255);


					helper = GetDlgItem(hwnd, IDC_MODEL_ALPHA_SLIDER);

						SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
						SendMessageA(helper, TBM_SETRANGE, 0, word);

						SendMessageA(helper, TBM_SETPOS, TRUE, 255);


				// combo box'y

					helper = GetDlgItem(hwnd, IDC_MODEL_COMBO);
					ComboBox_SetItemHeight(helper, 0, 15);

					helper = GetDlgItem(hwnd, IDC_MODEL_TEXTURE_COMBO);
					ComboBox_SetItemHeight(helper, 0, 15);


				return TRUE;
			}
			break;


			case WM_COMMAND:
			{

				switch( LOWORD(wParam) )
				{


				// obsluga edit controls

					case IDC_MODEL_SCALE:
					{
						if( GetDlgItemInt(hwnd, IDC_MODEL_SCALE, FALSE, FALSE) > 20000 )

							SetDlgItemInt(hwnd, IDC_MODEL_SCALE, 20000, FALSE);


						if( GetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT, FALSE, FALSE) < 1 )

							SetDlgItemInt(hwnd, IDC_TERRAIN_MIN_HEIGHT, 1, FALSE);


						SendMessageA(GetDlgItem(hwnd, IDC_MODEL_SCALE_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_MODEL_SCALE, FALSE, FALSE));

						objectOperator.keyPressed = 'L';
					}
					break;



					case IDC_MODEL_RED:
					{
						if( GetDlgItemInt(hwnd, IDC_MODEL_RED, FALSE, FALSE) > 255 )

							SetDlgItemInt(hwnd, IDC_MODEL_RED, 255, FALSE);


						SendMessageA(GetDlgItem(hwnd, IDC_MODEL_RED_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_MODEL_RED, FALSE, FALSE));

						objectOperator.keyPressed = 'L';

					}
					break;



					case IDC_MODEL_GREEN:
					{
						if( GetDlgItemInt(hwnd, IDC_MODEL_GREEN, FALSE, FALSE) > 255 )

							SetDlgItemInt(hwnd, IDC_MODEL_GREEN, 255, FALSE);


						SendMessageA(GetDlgItem(hwnd, IDC_MODEL_GREEN_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_MODEL_GREEN, FALSE, FALSE));

						objectOperator.keyPressed = 'L';
					}
					break;



					case IDC_MODEL_BLUE:
					{
						if( GetDlgItemInt(hwnd, IDC_MODEL_BLUE, FALSE, FALSE) > 255 )

							SetDlgItemInt(hwnd, IDC_MODEL_BLUE, 255, FALSE);


						SendMessageA(GetDlgItem(hwnd, IDC_MODEL_BLUE_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_MODEL_BLUE, FALSE, FALSE));

						objectOperator.keyPressed = 'L';
					}
					break;



					case IDC_MODEL_ALPHA:
					{
						if( GetDlgItemInt(hwnd, IDC_MODEL_ALPHA, FALSE, FALSE) > 255 )

							SetDlgItemInt(hwnd, IDC_MODEL_ALPHA, 255, FALSE);


						SendMessageA(GetDlgItem(hwnd, IDC_MODEL_ALPHA_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_MODEL_ALPHA, FALSE, FALSE));

						objectOperator.keyPressed = 'L';
					}
					break;



				// haczyk

					case IDC_MODEL_OBSTACLE_CHECK:

						modelsOperator.ObstacleCheck = !modelsOperator.ObstacleCheck;
						objectOperator.keyPressed = 'L';

						SetFocus(g_hwnd);
					break;



				// buttony

					case IDC_MODEL_PLACE_MODEL:
						Action = PLACE_MODEL;
						SetFocus(g_hwnd);
					break;


					case IDC_MODEL_RELOAD:
						Action = RELOAD_MODELS;
						SetFocus(g_hwnd);
					break;

					
					case IDC_MODEL_COMBO:
						Action = CHANGE_MODEL;
					break;


					case IDC_MODEL_REMOVE:
						Action = REMOVE_MODELS;
						SetFocus(g_hwnd);
					break;


					case IDC_MODEL_RELOAD_TEXTURE:
						Action = RELOAD_MODEL_TEXTURES;
						SetFocus(g_hwnd);
					break;


					case IDC_MODEL_TEXTURE_COMBO:
						Action = CHANGE_MODEL_TEXTURE;
					break;


					default:
					break;
				}

			}
			break; //(LOWORD(wParam))




		// odswiezenie slider'ow

			case WM_HSCROLL:
			{

					if( (HWND)lParam == GetDlgItem(hwnd, IDC_MODEL_SCALE_SLIDER) )
					{
						SetDlgItemInt(hwnd, IDC_MODEL_SCALE,	SendMessageA( GetDlgItem(hwnd, IDC_MODEL_SCALE_SLIDER),		TBM_GETPOS, 0, 0), FALSE);
					}


					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_MODEL_RED_SLIDER) )
					{
						SetDlgItemInt(hwnd, IDC_MODEL_RED,		SendMessageA( GetDlgItem(hwnd, IDC_MODEL_RED_SLIDER),		TBM_GETPOS, 0, 0), FALSE);
						objectOperator.keyPressed = 'L';
					}

					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_MODEL_GREEN_SLIDER) )
					{
						SetDlgItemInt(hwnd, IDC_MODEL_GREEN,	SendMessageA( GetDlgItem(hwnd, IDC_MODEL_GREEN_SLIDER),		TBM_GETPOS, 0, 0), FALSE);
						objectOperator.keyPressed = 'L';
					}

					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_MODEL_BLUE_SLIDER) )
					{
						SetDlgItemInt(hwnd, IDC_MODEL_BLUE,		SendMessageA( GetDlgItem(hwnd, IDC_MODEL_BLUE_SLIDER),		TBM_GETPOS, 0, 0), FALSE);
						objectOperator.keyPressed = 'L';
					}

					else if( (HWND)lParam == GetDlgItem(hwnd, IDC_MODEL_ALPHA_SLIDER) )
					{
						SetDlgItemInt(hwnd, IDC_MODEL_ALPHA,	SendMessageA( GetDlgItem(hwnd, IDC_MODEL_ALPHA_SLIDER),		TBM_GETPOS, 0, 0), FALSE);
						objectOperator.keyPressed = 'L';
					}

			}
			break;



			default:
			return FALSE;
		}

		return TRUE;
}



BOOL CALLBACK cubemapProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		{	

			// edits'y

				SetDlgItemInt(hwnd, IDC_CUBEMAP_RED,   255, FALSE);
				SetDlgItemInt(hwnd, IDC_CUBEMAP_GREEN, 255, FALSE);
				SetDlgItemInt(hwnd, IDC_CUBEMAP_BLUE,  255, FALSE);
				SetDlgItemInt(hwnd, IDC_CUBEMAP_ALPHA, 255, FALSE);


			// combo box

				HWND helper = GetDlgItem(hwnd, IDC_CUBEMAP_COMBO);
				ComboBox_SetItemHeight(helper, 0, 15);


			// przyszykowanie slider'ow

				helper = GetDlgItem(hwnd, IDC_CUBEMAP_RED_SLIDER);

				DWORD word;   // zakres <max, min> slider'ow
				word = ((WORD)255 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);


				helper = GetDlgItem(hwnd, IDC_CUBEMAP_GREEN_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);



				helper = GetDlgItem(hwnd, IDC_CUBEMAP_BLUE_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);


				helper = GetDlgItem(hwnd, IDC_CUBEMAP_ALPHA_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);


			return TRUE;
		}
		break;


		case WM_COMMAND:
		{

			switch( LOWORD(wParam) )
			{

				case IDC_CUBEMAP_RED:
				{
					if( GetDlgItemInt(hwnd, IDC_CUBEMAP_RED, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_CUBEMAP_RED, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CUBEMAP_RED_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CUBEMAP_RED, FALSE, FALSE));

					Action = CHANGE_CUBEMAP_COLORS;
				}
				break;


				case IDC_CUBEMAP_GREEN:
				{
					if( GetDlgItemInt(hwnd, IDC_CUBEMAP_GREEN, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_CUBEMAP_GREEN, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CUBEMAP_GREEN_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CUBEMAP_GREEN, FALSE, FALSE));

					Action = CHANGE_CUBEMAP_COLORS;
				}
				break;


				case IDC_CUBEMAP_BLUE:
				{
					if( GetDlgItemInt(hwnd, IDC_CUBEMAP_BLUE, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_CUBEMAP_BLUE, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CUBEMAP_BLUE_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CUBEMAP_BLUE, FALSE, FALSE));

					Action = CHANGE_CUBEMAP_COLORS;
				}
				break;


				case IDC_CUBEMAP_ALPHA:
				{
					if( GetDlgItemInt(hwnd, IDC_CUBEMAP_ALPHA, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_CUBEMAP_ALPHA, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CUBEMAP_ALPHA_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CUBEMAP_ALPHA, FALSE, FALSE));

					Action = CHANGE_CUBEMAP_COLORS;
				}
				break;



				case IDC_CUBEMAP_CHECK:
					showCubemap = !showCubemap;
					SetFocus(g_hwnd);
				break;



				case IDC_CUBEMAP_COMBO:
					Action = CHANGE_CUBEMAP;
				break;


				case IDC_CUBEMAP_RELOAD:
					Action = RELOAD_CUBEMAPS;
					SetFocus(g_hwnd);
				break;


				default:
				break;
			}

		}
		break; //(LOWORD(wParam))



		// odswiezenie slider'ow

		case WM_HSCROLL:
		{

				if( (HWND)lParam == GetDlgItem(hwnd, IDC_CUBEMAP_RED_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CUBEMAP_RED, SendMessageA(GetDlgItem(hwnd, IDC_CUBEMAP_RED_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CUBEMAP_GREEN_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CUBEMAP_GREEN, SendMessageA(GetDlgItem(hwnd, IDC_CUBEMAP_GREEN_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CUBEMAP_BLUE_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CUBEMAP_BLUE, SendMessageA(GetDlgItem(hwnd, IDC_CUBEMAP_BLUE_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				if( (HWND)lParam == GetDlgItem(hwnd, IDC_CUBEMAP_ALPHA_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CUBEMAP_ALPHA, SendMessageA(GetDlgItem(hwnd, IDC_CUBEMAP_ALPHA_SLIDER), TBM_GETPOS, 0, 0), FALSE);
		}
		break;



		default:
		   return FALSE;
	}

	return TRUE;
}
BOOL CALLBACK cloudProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch(Message)
	{
		case WM_INITDIALOG:
		{	

			// edits'y

				SetDlgItemInt(hwnd, IDC_CLOUD_HEIGHT, 150, FALSE);

				SetDlgItemInt(hwnd, IDC_CLOUD_SPEED,   1,  FALSE);

				SetDlgItemInt(hwnd, IDC_CLOUD_SPREAD,  1,  FALSE);

				SetDlgItemInt(hwnd, IDC_CLOUD_RED,    255, FALSE);
				SetDlgItemInt(hwnd, IDC_CLOUD_GREEN,  255, FALSE);
				SetDlgItemInt(hwnd, IDC_CLOUD_BLUE,   255, FALSE);
				SetDlgItemInt(hwnd, IDC_CLOUD_ALPHA,  255, FALSE);


			// combo box

				HWND helper = GetDlgItem(hwnd, IDC_CLOUD_COMBO);
				ComboBox_SetItemHeight(helper, 0, 15);


			// przyszykowanie slider'ow

				helper = GetDlgItem(hwnd, IDC_CLOUD_HEIGHT_SLIDER);

				DWORD word;   // zakres <max, min> slider'ow
				word = ((WORD)OVERALL_MAX_HEIGHT << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 150);


				helper = GetDlgItem(hwnd, IDC_CLOUD_SPEED_SLIDER);

				word = ((WORD)1000 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 1);


				helper = GetDlgItem(hwnd, IDC_CLOUD_SPREAD_SLIDER);

				word = ((WORD)50 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 1);


				helper = GetDlgItem(hwnd, IDC_CLOUD_X_DIR_SLIDER);

				word = ((WORD)2 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 1);


				helper = GetDlgItem(hwnd, IDC_CLOUD_Z_DIR_SLIDER);

				word = ((WORD)2 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 1);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 1);


				helper = GetDlgItem(hwnd, IDC_CLOUD_RED_SLIDER);

				word = ((WORD)255 << 16) | (WORD)0;

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);


				helper = GetDlgItem(hwnd, IDC_CLOUD_GREEN_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);



				helper = GetDlgItem(hwnd, IDC_CLOUD_BLUE_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);


				helper = GetDlgItem(hwnd, IDC_CLOUD_ALPHA_SLIDER);

					SendMessageA(helper, TBM_SETLINESIZE, 0, 255);
					SendMessageA(helper, TBM_SETRANGE, 0, word);

					SendMessageA(helper, TBM_SETPOS, TRUE, 255);


			return TRUE;
		}
		break;


		case WM_COMMAND:
		{

			switch( LOWORD(wParam) )
			{

				case IDC_CLOUD_HEIGHT:
				{
					if( GetDlgItemInt(hwnd, IDC_CLOUD_HEIGHT, FALSE, FALSE) > OVERALL_MAX_HEIGHT )
						SetDlgItemInt(hwnd, IDC_CLOUD_HEIGHT, OVERALL_MAX_HEIGHT, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_HEIGHT_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CLOUD_HEIGHT, FALSE, FALSE));

					Action = UPDATE_CLOUD;
				}
				break;


				case IDC_CLOUD_SPEED:
				{
					if( GetDlgItemInt(hwnd, IDC_CLOUD_SPEED, FALSE, FALSE) > 1000 )
						SetDlgItemInt(hwnd, IDC_CLOUD_SPEED, 1000, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_SPEED_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CLOUD_SPEED, FALSE, FALSE));

					Action = UPDATE_CLOUD;
				}
				break;


				case IDC_CLOUD_SPREAD:
				{
					if( GetDlgItemInt(hwnd, IDC_CLOUD_SPREAD, FALSE, FALSE) > 50 )
						SetDlgItemInt(hwnd, IDC_CLOUD_SPREAD, 50, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_SPREAD_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CLOUD_SPREAD, FALSE, FALSE));

					Action = UPDATE_CLOUD;
				}
				break;


				case IDC_CLOUD_RED:
				{
					if( GetDlgItemInt(hwnd, IDC_CLOUD_RED, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_CLOUD_RED, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_RED_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CLOUD_RED, FALSE, FALSE));

					Action = UPDATE_CLOUD;
				}
				break;


				case IDC_CLOUD_GREEN:
				{
					if( GetDlgItemInt(hwnd, IDC_CLOUD_GREEN, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_CLOUD_GREEN, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_GREEN_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CLOUD_GREEN, FALSE, FALSE));

					Action = UPDATE_CLOUD;
				}
				break;


				case IDC_CLOUD_BLUE:
				{
					if( GetDlgItemInt(hwnd, IDC_CLOUD_BLUE, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_CLOUD_BLUE, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_BLUE_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CLOUD_BLUE, FALSE, FALSE));

					Action = UPDATE_CLOUD;
				}
				break;


				case IDC_CLOUD_ALPHA:
				{
					if( GetDlgItemInt(hwnd, IDC_CLOUD_ALPHA, FALSE, FALSE) > 255 )
						SetDlgItemInt(hwnd, IDC_CLOUD_ALPHA, 255, FALSE);

					SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_ALPHA_SLIDER), TBM_SETPOS, TRUE, GetDlgItemInt(hwnd, IDC_CLOUD_ALPHA, FALSE, FALSE));

					Action = UPDATE_CLOUD;
				}
				break;



				case IDC_CLOUD_CHECK:
					showCloud = !showCloud;
					SetFocus(g_hwnd);
				break;



				case IDC_CLOUD_COMBO:
					Action = CHANGE_CLOUD;
				break;


				case IDC_CLOUD_RELOAD:
					Action = RELOAD_CLOUDS;
					SetFocus(g_hwnd);
				break;


				default:
				break;
			}

		}
		break; //(LOWORD(wParam))



		// odswiezenie slider'ow

		case WM_HSCROLL:
		{

				if( (HWND)lParam == GetDlgItem(hwnd, IDC_CLOUD_HEIGHT_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CLOUD_HEIGHT, SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_HEIGHT_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CLOUD_SPEED_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CLOUD_SPEED, SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_SPEED_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CLOUD_SPREAD_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CLOUD_SPREAD, SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_SPREAD_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CLOUD_X_DIR_SLIDER) )

					Action = UPDATE_CLOUD;

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CLOUD_Z_DIR_SLIDER) )

					Action = UPDATE_CLOUD;

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CLOUD_RED_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CLOUD_RED, SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_RED_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CLOUD_GREEN_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CLOUD_GREEN, SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_GREEN_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CLOUD_BLUE_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CLOUD_BLUE, SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_BLUE_SLIDER), TBM_GETPOS, 0, 0), FALSE);

				else if( (HWND)lParam == GetDlgItem(hwnd, IDC_CLOUD_ALPHA_SLIDER) )

					SetDlgItemInt(hwnd, IDC_CLOUD_ALPHA, SendMessageA(GetDlgItem(hwnd, IDC_CLOUD_ALPHA_SLIDER), TBM_GETPOS, 0, 0), FALSE);
		}
		break;



		default:
		   return FALSE;
	}

	return TRUE;
}
/*****************************************************************************
 ToolDlgProc()

 procedura okienkowa ToolBox'a
*****************************************************************************/
BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	static int xClient, yClient,  //rozmiary client area
			   xPos, yPos;        //aktualne pozycje scroll bar'ow
		 
	static int tabIndex;


	switch(Message)
	{
		case WM_INITDIALOG:
		{			

			// ustawienie tab'ow

				HWND helper = GetDlgItem(hwnd, IDC_TAB);
				
				TCITEM tie;
				tie.mask = TCIF_TEXT | TCIF_IMAGE; 
				tie.iImage = -1; 

				tie.pszText = "Lighting"; 
				TabCtrl_InsertItem(helper, 0, &tie);

				tie.pszText = "Textures"; 
				TabCtrl_InsertItem(helper, 1, &tie);

				tie.pszText = "Details"; 
				TabCtrl_InsertItem(helper, 2, &tie);

				tie.pszText = "Morphing"; 
				TabCtrl_InsertItem(helper, 3, &tie);

				tie.pszText = "Terrain"; 
				TabCtrl_InsertItem(helper, 4, &tie);

				tie.pszText = "Liquids"; 
				TabCtrl_InsertItem(helper, 5, &tie);

				tie.pszText = "Grass"; 
				TabCtrl_InsertItem(helper, 6, &tie);

				tie.pszText = "Models"; 
				TabCtrl_InsertItem(helper, 7, &tie);

				tie.pszText = "Cubemap"; 
				TabCtrl_InsertItem(helper, 8, &tie);

				tie.pszText = "Clouds"; 
				TabCtrl_InsertItem(helper, 9, &tie);

			// inicjalizacja obecnego indeksu

				tabIndex = 0;
				ShowWindow(lighting, SW_SHOW);

			return TRUE;
		}
		break;


		case WM_COMMAND:

			switch(LOWORD(wParam))
			{

				case IDC_PLANT:
					Action = PLANT_GRASS;
					SetFocus(GetParent(hwnd));
				break;

				case IDC_WEED:
					Action = WEED_OUT;
					SetFocus(GetParent(hwnd));
				break;


				case IDC_MORPH:
					Action = MORPH;
					SetFocus(GetParent(hwnd));
				break;


				case IDC_PAINT: 
					Action = PAINT_LAND;
					SetFocus(GetParent(hwnd));
				break;


	
				case IDC_TEXT_CHECK:
					render2Dtext = !render2Dtext;
					SetFocus(GetParent(hwnd));
				break;


				case IDC_NET_CHECK:

					drawingNet = !drawingNet;

					if( drawingNet )
						computeNet();

					SetFocus(GetParent(hwnd));

				break;


				case IDC_ROUND_CHECK:
					roundCursor = !roundCursor;
					SetFocus(GetParent(hwnd));
				break;



				default:
				break;
			}

		break; //(LOWORD(wParam))



		// obsluga tab'ow
		case WM_NOTIFY:
		{
			
			switch (((LPNMHDR)lParam)->code)
            {
				// zmieniono zakladke
				case TCN_SELCHANGE:
                { 

					// pobieramy identyfikator taba
						HWND tab = GetDlgItem(hwnd, ((LPNMHDR)lParam)->idFrom);


					// chowanie poprzedniej zakladki

						switch( tabIndex )
						{
							case 0:		ShowWindow(lighting,				 SW_HIDE);	break;

							case 1:		ShowWindow(textures,				 SW_HIDE);	break;

							case 2:		ShowWindow(details,					 SW_HIDE);	break;

							case 3:		ShowWindow(morphing,				 SW_HIDE);	break;

							case 4:		ShowWindow(terrain,					 SW_HIDE);	break;

							case 5:		ShowWindow(liquids,					 SW_HIDE);	break;

							case 6:		ShowWindow(grass,					 SW_HIDE);	break;

							case 7:		ShowWindow(modelsOperator.models,    SW_HIDE);	break;

							case 8:		ShowWindow(cubemap,					 SW_HIDE);	break;

							case 9:	ShowWindow(cloud,					 SW_HIDE);	break;
						}

					// sprawdzenie obecnie zaznaczonego indeksu
						tabIndex = TabCtrl_GetCurSel(tab);



					// ukazanie nowej zakladki

						switch( tabIndex )
						{
							case 0:		ShowWindow(lighting,				 SW_SHOW);	break;

							case 1:		ShowWindow(textures,				 SW_SHOW);	break;

							case 2:		ShowWindow(details,					 SW_SHOW);	break;

							case 3:		ShowWindow(morphing,				 SW_SHOW);	break;

							case 4:		ShowWindow(terrain,					 SW_SHOW);	break;

							case 5:		ShowWindow(liquids,					 SW_SHOW);	break;

							case 6:		ShowWindow(grass,					 SW_SHOW);	break;

							case 7:		ShowWindow(modelsOperator.models,    SW_SHOW);	break;

							case 8:		ShowWindow(cubemap,					 SW_SHOW);	break;

							case 9:	ShowWindow(cloud,					 SW_SHOW);	break;
						}

                } 
				break;

            }//switch (((LPNMHDR)lParam)->code)


			SetWindowRedraw(dialog, TRUE);
			SetFocus(GetParent(hwnd));
            return TRUE;
		}
		break;


		//// kiedys moze to zrobie...
		//case WM_KEYDOWN:
		//{
		//	switch(toupper(wParam))
		//	{
		//		case VK_RETURN:
		//			SetFocus(GetParent(hwnd));
		//			MessageBox(g_hwnd, "sfasfas", "sffasfs", MB_OK);
		//		break;
		//	}
		//}
		//break;


		// mooooze kiedys zrobie resizowanie toolboxa
		//case WM_SIZING:
		//	
		//	// do tego czasu, jest on statyczny

		//	//GetClientRect(hwnd, &rcClient);
		//	//GetClientRect(GetParent(hwnd), &rcParent);

		//	////modyfikacja toolsModifier
		//	//toolsModifier = ( (float)rcParent.right - (float)rcClient.right ) / (float)rcParent.right;

		//	//DWORD slowo;

		//	//slowo = ((WORD)rcParent.bottom<<16) | (WORD)rcParent.right;

		//	//SendMessage(GetParent(hwnd), WM_SIZE, 0, slowo);

		//	return 0;

		//break;

		//case WM_SIZE:

		//	// Retrieve the dimensions of the client area. 
		//	GetClientRect(hwnd, &rcClient);

		//	WINDOWINFO wi;
		//	GetWindowInfo(hwnd, &wi);

		//	yClient = rcClient.bottom; 
		//	xClient = rcClient.right; 
	 //
		//	// Set the vertical scrolling range and page size
		//	si.cbSize = sizeof(si); 
		//	si.fMask  = SIF_RANGE | SIF_PAGE; 
		//	si.nMin   = 0; 
		//	si.nMax   = 10; 
		//	si.nPage  = (yClient / wi.rcWindow.bottom)  / 10; 
		//	SetScrollInfo(hwnd, SB_VERT, &si, TRUE); 
	 //
		//	// Set the horizontal scrolling range and page size. 
		//	si.cbSize = sizeof(si); 
		//	si.fMask  = SIF_RANGE | SIF_PAGE; 
		//	si.nMin   = 0; 
		//	si.nMax   = 10; 
		//	si.nPage  = (xClient / wi.rcWindow.right) / 10; 
		//	SetScrollInfo(hwnd, SB_HORZ, &si, TRUE); 
	 //
		//	return 0; 

		//break;

		//case WM_HSCROLL:
		//
		//	si.cbSize = sizeof(SCROLLINFO);
		//	si.fMask  = SIF_ALL;

		//	// Save the position for comparison later on
		//	GetScrollInfo (hwnd, SB_HORZ, &si);
		//	xPos = si.nPos;

		//	switch (LOWORD (wParam))
		//	{
		//		// user clicked left arrow
		//		case SB_LINELEFT: 
		//		  si.nPos -= 1;
		//		  break;
		//		  
		//		// user clicked right arrow
		//		case SB_LINERIGHT: 
		//		  si.nPos += 1;
		//		  break;
		//		  
		//		// user clicked the scroll bar shaft left of the scroll box
		//		case SB_PAGELEFT:
		//		  si.nPos -= si.nPage;
		//		  break;
		//		  
		//		// user clicked the scroll bar shaft right of the scroll box
		//		case SB_PAGERIGHT:
		//		  si.nPos += si.nPage;
		//		  break;
		//		  
		//		// user dragged the scroll box
		//		case SB_THUMBTRACK: 
		//		  si.nPos = si.nTrackPos;
		//		  break;
		//		  
		//		default :
		//		  break;
		//	}
		//	// Set the position and then retrieve it.  Due to adjustments
		//	//   by Windows it may not be the same as the value set.
		//	si.fMask = SIF_POS;
		//	SetScrollInfo (hwnd, SB_HORZ, &si, TRUE);
		//	GetScrollInfo (hwnd, SB_HORZ, &si);

		//	// If the position has changed, scroll the window 
		//	if (si.nPos != xPos)
		//	{
		//		ScrollWindow(hwnd, 10 * (xPos - si.nPos), 0, NULL, NULL);
		//		UpdateWindow(hwnd);
		//	}
		//	return 0;
		//break;


		//case WM_VSCROLL:

		//	 // Get all the vertial scroll bar information
		//	 si.cbSize = sizeof (si);
		//	 si.fMask  = SIF_ALL;

		//	 // Save the position for comparison later on
		//	 GetScrollInfo (hwnd, SB_VERT, &si);
		//	 yPos = si.nPos;

		//	 switch (LOWORD (wParam))
		//	 {
		//		 // user clicked the HOME keyboard key
		//		 case SB_TOP:
		//			  si.nPos = si.nMin;
		//			  break;
		//              
		//		 // user clicked the END keyboard key
		//		 case SB_BOTTOM:
		//			  si.nPos = si.nMax;
		//			  break;
		//              
		//		 // user clicked the top arrow
		//		 case SB_LINEUP:
		//			  si.nPos -= 1;
		//			  break;
		//              
		//		 // user clicked the bottom arrow
		//		 case SB_LINEDOWN:
		//			  si.nPos += 1;
		//			  break;
		//              
		//		 // user clicked the scroll bar shaft above the scroll box
		//		 case SB_PAGEUP:
		//			  si.nPos -= si.nPage;
		//			  break;
		//              
		//		 // user clicked the scroll bar shaft below the scroll box
		//		 case SB_PAGEDOWN:
		//			  si.nPos += si.nPage;
		//			  break;
		//              
		//		 // user dragged the scroll box
		//		 case SB_THUMBTRACK:
		//			  si.nPos = si.nTrackPos;
		//			  break;
		//              
		//		 default:
		//			  break; 
		//	}

  //       // Set the position and then retrieve it.  Due to adjustments
  //       //   by Windows it may not be the same as the value set.
  //       si.fMask = SIF_POS;
  //       SetScrollInfo (hwnd, SB_VERT, &si, TRUE);
  //       GetScrollInfo (hwnd, SB_VERT, &si);

  //       // If the position has changed, scroll window and update it
  //       if (si.nPos != yPos)
  //       {                    
  //          ScrollWindow(hwnd, 0, 10 * (yPos - si.nPos), NULL, NULL);
  //          UpdateWindow (hwnd);
  //       }
  //       return 0;


		case WM_MOUSEMOVE:
			cameraRotate = lButtonDown = rButtonDown = false;
		break;


		////w momencie opuszczenia tool box'a (minimalizacja, zmiana okna etc.)
		//case WM_NCMOUSELEAVE:

		//	//altPressed = lButtonDown = rButtonDown = false;

		//	SetFocus(g_hwnd);


		//	/*RECT rcClient;
		//	GetClientRect(hwnd, &rcClient);
		//	g_mouseX = (rcClient.right*toolsModifier)/2.f;
		//	g_mouseY = rcClient.bottom/2.f;*/

		//break;


		default:
		   return FALSE;
	}


	return TRUE;
}






/*****************************************************************************
 WndProc()

 procedura okienkowa - dla glownego okna
*****************************************************************************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch(message)
	{
		case WM_CREATE:
		{
		// generowanie glownego dialog box'a

			dialog = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_TOOLBOX), hwnd, (DLGPROC)ToolDlgProc);

			if(!dialog)
				MessageBox(hwnd, "Nie zaladowano dialogu ToolBox!", "Ops!", MB_OK);
			else
				ShowWindow(dialog, SW_SHOW);

		// generowanie nizszych dialog box'ow

			lighting = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_LIGHTING), dialog, (DLGPROC)lightingProc);

			if( !lighting )
				MessageBox(hwnd, "Nie zaladowano dialogu Lighting!", "Ops!", MB_OK);
			else
				ShowWindow(lighting, SW_SHOW);


			liquids = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_LIQUIDS), dialog, (DLGPROC)liquidsProc);

			if( !liquids )
				MessageBox(hwnd, "Nie zaladowano dialogu Liquids!", "Ops!", MB_OK);
			else
				ShowWindow(liquids, SW_HIDE);


			morphing = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_MORPHING), dialog, (DLGPROC)morphingProc);

			if( !morphing )
				MessageBox(hwnd, "Nie zaladowano dialogu Morphing!", "Ops!", MB_OK);
			else
				ShowWindow(morphing, SW_HIDE);


			terrain = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_TERRAIN), dialog, (DLGPROC)terrainProc);

			if( !terrain )
				MessageBox(hwnd, "Nie zaladowano dialogu Terrain!", "Ops!", MB_OK);
			else
				ShowWindow(terrain, SW_HIDE);


			textures = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_TEXTURES), dialog, (DLGPROC)texturesProc);

			if( !textures )
				MessageBox(hwnd, "Nie zaladowano dialogu Textures!", "Ops!", MB_OK);
			else
				ShowWindow(textures, SW_HIDE);


			details = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DETAILS), dialog, (DLGPROC)detailsProc);

			if( !details )
				MessageBox(hwnd, "Nie zaladowano dialogu Details!", "Ops!", MB_OK);
			else
				ShowWindow(details, SW_HIDE);


			grass = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_GRASS), dialog, (DLGPROC)grassProc);

			if( !grass )
				MessageBox(hwnd, "Nie zaladowano dialogu Grass!", "Ops!", MB_OK);
			else
				ShowWindow(grass, SW_HIDE);


			modelsOperator.models = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_MODELS), dialog, (DLGPROC)modelsProc);

			if( !modelsOperator.models )
				MessageBox(hwnd, "Nie zaladowano dialogu Models!", "Ops!", MB_OK);
			else
				ShowWindow(modelsOperator.models, SW_HIDE);


			cubemap = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_CUBEMAP), dialog, (DLGPROC)cubemapProc);

			if( !cubemap )
				MessageBox(hwnd, "Nie zaladowano dialogu Cubemap!", "Ops!", MB_OK);
			else
				ShowWindow(cubemap, SW_HIDE);


			cloud = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_CLOUD), dialog, (DLGPROC)cloudProc);

			if( !cloud )
				MessageBox(hwnd, "Nie zaladowano dialogu Cloud!", "Ops!", MB_OK);
			else
				ShowWindow(cloud, SW_HIDE);


		// sygnalizacja, ze przybornik zostal w pelni zainicjalizowany

			toolBoxReady = true;

			return 0;
		}
		break;

		case WM_ACTIVATE:  // okno jest zwijane do ikony b¹dŸ rozwijane
		{
			if (!HIWORD(wParam))
			{
			// okno zosta³o rozwiniête
				g_isActive = TRUE;


				DWORD resolution;

				RECT clientArea;
				GetClientRect(hwnd, &clientArea);

				int   width  = (clientArea.right  - clientArea.left),
					  height = (clientArea.bottom - clientArea.top);

				resolution = height << 16 | width;

				SendMessageA(hwnd, WM_SIZE, 0, resolution);
			}
			else
			{
			// okno zosta³o zwiniête do ikony
				g_isActive=FALSE;
			}

			return 0;
		}

		case WM_SYSCOMMAND:  // wygaszacz ekranu b¹dŸ tryb oszczêdny monitora
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:     // uruchamiany jest wygaszacz ekranu 
				case SC_MONITORPOWER:   // monitor prze³¹cza siê w tryb oszczêdny
				// zwrócenie wartoœci 0 zapobiega obu akcjom
				return 0;

				default:
				break;
			}
		} 
		break;

		case WM_COMMAND:
		{
		// obsluga menu

			switch(LOWORD(wParam))
			{
			// ponizsze funkcje pochodza z MenuOperator.h

				case IDM_NEW: 
					New();
				break;

				case IDM_SAVEAS:  
					SaveAs(); 
				break;

				case IDM_SAVE:  
					Save();
				break;

				case IDM_LOAD:  
					Load();
				break;

				case IDM_EXIT:  
					Exit();
				break;

				default:
				break;
			}
		}
		break;

		case WM_CLOSE:    // okno jest zamykane
		{
			DestroyWindow(dialog);

		// wstawia komunikat WM_QUIT do kolejki

			PostQuitMessage(0);
			return 0;
		}

		case WM_SIZE:
		{
			RECT rcClient;
			GetClientRect(hwnd, &rcClient); 


			MoveWindow(dialog, 
			rcClient.right * toolsModifier,
			rcClient.top,
			rcClient.right - rcClient.right * toolsModifier, 
			rcClient.bottom,
			TRUE); 

		// aktualizuje szerokoœæ i wysokoœæ sceny

			ResizeScene(LOWORD(lParam), HIWORD(lParam));

			return 0;
		}

		case WM_KEYDOWN:
		{
			switch(toupper(wParam))
			{

				case VK_CONTROL:
					ctrlPressed = true;
				break;


				case VK_DELETE:
					deletePressed = true;
				break;

				case VK_ESCAPE:
				{
					if( Action )
						Action = NONE;

					SetFocus(g_hwnd);
				}
				break;


				case VK_SPACE:
				{
					roundCursor = !roundCursor;

					if( roundCursor )
						SendMessageA(GetDlgItem(dialog, IDC_ROUND_CHECK), BM_SETCHECK, 1, 0);
					else
						SendMessageA(GetDlgItem(dialog, IDC_ROUND_CHECK), BM_SETCHECK, 0, 0);

					SetFocus(g_hwnd);
				}
				break;


				case 'W':
				{
					if( !ctrlPressed )
					{
						kamera.GoForward();
						cameraPosChanged = true;
					}

					else if( Action == MORPH )  // zmiana obszaru morphing'u
					{
						SetDlgItemInt(morphing, IDC_X_EDIT, GetDlgItemInt(morphing, IDC_X_EDIT, FALSE, FALSE)+1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else if( Action == PAINT_LAND )  // zmiana obszaru teksturowania (lub mazania gumka)
					{					
						SetDlgItemInt(textures, IDC_TEXT_X_EDIT, GetDlgItemInt(textures, IDC_TEXT_X_EDIT, FALSE, FALSE)+1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else if( Action == PLANT_GRASS || Action == WEED_OUT )
					{
						SetDlgItemInt(grass, IDC_GRASS_X_EDIT, GetDlgItemInt(grass, IDC_GRASS_X_EDIT, FALSE, FALSE)+1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else
						objectOperator.keyPressed = 'W';  // < --- keyPressed = char znajdujacy sie w ObjectOperator.h

				}
				break;

				case 'S':
				{
					if(!ctrlPressed)
					{
						kamera.GoBackward();
						cameraPosChanged = true;
					}

					else if( Action == MORPH )
					{
						SetDlgItemInt(morphing, IDC_X_EDIT, GetDlgItemInt(morphing, IDC_X_EDIT, FALSE, FALSE)-1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else if( Action == PAINT_LAND )
					{
						SetDlgItemInt(textures, IDC_TEXT_X_EDIT, GetDlgItemInt(textures, IDC_TEXT_X_EDIT, FALSE, FALSE)-1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else if( Action == PLANT_GRASS || Action == WEED_OUT )
					{
						SetDlgItemInt(grass, IDC_GRASS_X_EDIT, GetDlgItemInt(grass, IDC_GRASS_X_EDIT, FALSE, FALSE)-1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else
						objectOperator.keyPressed = 'S';

				}
				break;

				case 'A':
				{
					if( !ctrlPressed )
					{
						kamera.GoLeft();
						cameraPosChanged = true;
					}

					else if( Action == MORPH )
					{
						SetDlgItemInt(morphing, IDC_Z_EDIT, GetDlgItemInt(morphing, IDC_Z_EDIT, FALSE, FALSE)-1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else if( Action == PAINT_LAND )
					{
						SetDlgItemInt(textures, IDC_TEXT_Z_EDIT, GetDlgItemInt(textures, IDC_TEXT_Z_EDIT, FALSE, FALSE)-1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else if( Action == PLANT_GRASS || Action == WEED_OUT )
					{
						SetDlgItemInt(grass, IDC_GRASS_Z_EDIT, GetDlgItemInt(grass, IDC_GRASS_Z_EDIT, FALSE, FALSE)-1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else
						objectOperator.keyPressed = 'A';

				}
				break;

				case 'D':
				{
					if( !ctrlPressed )
					{
						kamera.GoRight();
						cameraPosChanged = true;
					}

					else if( Action == MORPH )
					{
						SetDlgItemInt(morphing, IDC_Z_EDIT, GetDlgItemInt(morphing, IDC_Z_EDIT, FALSE, FALSE)+1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else if( Action == PAINT_LAND  )
					{
						SetDlgItemInt(textures, IDC_TEXT_Z_EDIT, GetDlgItemInt(textures, IDC_TEXT_Z_EDIT, FALSE, FALSE)+1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else if( Action == PLANT_GRASS || Action == WEED_OUT )
					{
						SetDlgItemInt(grass, IDC_GRASS_Z_EDIT, GetDlgItemInt(grass, IDC_GRASS_Z_EDIT, FALSE, FALSE)+1, FALSE);

					// sygnalizuje potrzebe odswiezenia kursora
						mouseMoved = true;
					}

					else
						objectOperator.keyPressed = 'D';

				}
				break;


				case 'N':
				{
					objectOperator.keyPressed = 'N';
				}
				break;


				case 'M':
				{
					objectOperator.keyPressed = 'M';
				}
				break;


			
				case '1':
				{
				// Zmiana typow morphowania, poprzez klikanie cyferek.
				// ( wystarczy ustawic odpowiednia wartosc ComboBox'a. morphing sie zajmie reszta)

					if( Action == MORPH )
					{
						HWND helper = GetDlgItem(morphing, IDC_TYPE_COMBO);
						ComboBox_SetCurSel(helper, ComboBox_FindString(helper, 0, "Convex") );
					}
					else if( Action == PAINT_LAND )
					{
						textureKey = '1';
					}
				}
				break;


				case '2':
				{
					if( Action == MORPH )
					{
						HWND helper = GetDlgItem(morphing, IDC_TYPE_COMBO);
						ComboBox_SetCurSel(helper, ComboBox_FindString(helper, 0, "Concave") );
					}
					else if( Action == PAINT_LAND )
					{
						textureKey = '2';
					}
				}
				break;


				case '3':
				{
					if( Action == MORPH )
					{
						HWND helper = GetDlgItem(morphing, IDC_TYPE_COMBO);
						ComboBox_SetCurSel(helper, ComboBox_FindString(helper, 0, "Flat") );
					}
					else if( Action == PAINT_LAND )
					{
						textureKey = '3';
					}
				}
				break;


				case '4':
				{
					if( Action == MORPH )
					{
						HWND helper = GetDlgItem(morphing, IDC_TYPE_COMBO);
						ComboBox_SetCurSel(helper, ComboBox_FindString(helper, 0, "Simple") );
					}
					else if( Action == PAINT_LAND )
					{
						textureKey = '4';
					}
				}
				break;


				case '5':
				{
					if( Action == PAINT_LAND )
					{
						textureKey = '5';
					}
				}
				break;


				case VK_OEM_MINUS:
				{
					if( Action == PAINT_LAND )
					{
						textureKey = '-';
					}
				}
				break;


				case VK_OEM_PLUS:
				{
					if( Action == PAINT_LAND )
					{
						textureKey = '+';
					}
				}
				break;

			// do np. obracania detalami

				case 'Q':
				{
					if( ctrlPressed )
						objectOperator.keyPressed = 'Q';
				}
				break;

				case 'E':
				{
					if( ctrlPressed )
						objectOperator.keyPressed = 'E';
				}
				break;


				case 'R':
				{
					if( ctrlPressed )
						objectOperator.keyPressed = 'R';
				}
				break;

				case 'T':
				{
					if( ctrlPressed )
						objectOperator.keyPressed = 'T';
				}
				break;


				case 'F':
				{
					if( ctrlPressed )
						objectOperator.keyPressed = 'F';
				}
				break;

				case 'G':
				{
					if( ctrlPressed )
						objectOperator.keyPressed = 'G';
				}
				break;


				default:					  
				break;
			}

		}
		break;

		case WM_KEYUP:
		{
			switch(toupper(wParam))
			{
				case VK_CONTROL:
					ctrlPressed = false;
				break;

				case VK_DELETE:
					deletePressed = false;
				break;
			}
		}
		break;



		case WM_LBUTTONDOWN:
		{
			lButtonDown = true;

			if( mouseMoved )
				mouseMoved = false;

			return 0; 
		}
		case WM_LBUTTONUP:
		{
			lButtonDown = false;
			return 0;
		} 

		case WM_RBUTTONDOWN:
		{
			rButtonDown = true;

			if( mouseMoved )
				mouseMoved = false;

			return 0;
		}
		case WM_RBUTTONUP:
		{
			rButtonDown = false;
			return 0;  
		}

		case WM_SYSKEYDOWN:
		{
			altPressed = true;
			return 0;
		}
		case WM_SYSKEYUP:
		{
			altPressed = false;
			return 0;
		}


		case WM_MOUSEWHEEL:
		{
			wheelMovement = GET_WHEEL_DELTA_WPARAM(wParam);
			mouseWheelSteps = wheelMovement / WHEEL_DELTA;	// < --- dla terrain editor'a

			objectOperator.keyPressed = 'L';	// ctrl + scroll daje mozliwosc skalowania zaznaczonego modelu

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			mouseMoved = true;

		// zapamiêtuje poprzednie wspó³rzêdne myszy

			oldMouseX = g_mouseX;
			oldMouseY = g_mouseY;

		// pobiera bie¿¹ce wspó³rzêdne myszy

			g_mouseX = LOWORD(lParam);
			g_mouseY = HIWORD(lParam);


		// pierdolki kontrolne

			//Vector3 lol;

			//lol = GetOGLPos(g_mouseX, g_mouseY);

			//strum << "X:_" << lol.posX << "_Y:_" << lol.posY << "_Z:_" << lol.posZ;
			//strum >> wiersz;
			//strum.clear();
			//SetWindowText(hwnd, wiersz.c_str());

			return 0;
		}

		//w momencie opuszczenia client area (minimalizacja, zmiana okna etc.)
		case WM_NCMOUSELEAVE:

			altPressed = lButtonDown = rButtonDown = false;


			RECT rcClient;
			GetClientRect(hwnd, &rcClient);
			g_mouseX = (rcClient.right*toolsModifier)/2.f;
			g_mouseY = rcClient.bottom/2.f;

		break;

		default:
		break;

	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
} // WndProc()

