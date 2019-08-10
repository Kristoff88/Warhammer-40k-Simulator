
/****************************************************************************
			
	PCH z podstawowymi naglowkami, jak <windows>, <string>, etc.

******************************************************************************/

	#include "GlobalHeaders.h"

/****************************************************************************
			
				Zmiene globalne ( + makro jedno samotne )

******************************************************************************/

	//mali pomocnicy :)

		  std::stringstream strum; 
		  std::string       wiersz;

	/*************************** Sta³e i makra ***************************/

		  const int   SCREEN_WIDTH    = 900;
		  const int	  SCREEN_HEIGHT   = 700;
		  const int   SCREEN_BPP      = 32;
		  const bool  USE_FULLSCREEN  = false; 
		  const char  *APP_TITLE      = "Builder";


	/********************************* Zmienne globalne **********************************/

		  HDC       g_hdc;                   // kontekst urz¹dzenia
		  HGLRC     g_hrc;                   // kontekst tworzenia grafiki
		  BOOL      g_isFullscreen = FALSE;  // prze³¹cza tryb pe³noekranowy i okinekowy
		  BOOL      g_isActive     = TRUE;   // false, gdy okno jest zwiniête
		  HWND      g_hwnd         = NULL;   // uchwyt g³ównego okna aplikacji
		  HINSTANCE g_hInstance;             // instancja aplikacji
 


	 //compiled array extensions

		  PFNGLLOCKARRAYSEXTPROC    glLockArraysEXT = 0;
		  PFNGLUNLOCKARRAYSEXTPROC  glUnlockArraysEXT = 0;

		  PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = 0;


		  char OpenGLversion[MAX_PATH];
	

	// Zmienne myszy, klawiatury i kamery

		int   g_mouseX, g_mouseY,    // wspó³rzêdne myszy
			  oldMouseX = 0,
			  oldMouseY = 0,

			  wheelMovement   = 0,	    //okresla, jak bardzo uzytkownik kreci scrollem
			  mouseWheelSteps = 0;


		 bool	altPressed = false,
				ctrlPressed = false,

				deletePressed = false,

				cameraRotate = false,   //true w przypadku wcisniecia klawisza ALT (WM_SYSKEYDOWN)

			/*	lButtonDown = false,
				rButtonDown = false,
				
				lButtonDblClk,
				rButtonDblClk,*/

				mouseMoved = false,  // potrzebne do:
									//   - morphowanie (tj. pozycja morphingu sie nie zmienia dopoki, nie ruszysz myszka),
									//   - odswiezenia pozycji kursora
									//
									//  jest ono ustawiane na true w WM_MOUSEMOVE w WndProc(),
									//  w CameraOperator, przy zmianie polozenia kamery
									//  oraz w case 'w', case 's', case 'd', case 'a', kiedy sie morphuje, texturuje etc.

				cameraPosChanged = false, // sygnalizuje zmiane pozycji kamery (potrzebne np. do odswiezania pozycji swiatel)

				toolBoxReady = false;	


	// Terrain

		int	MAP_X = 2,        // rozmiar mapy terenu wzd³u¿ osi X
			MAP_Z = 2,        // rozmiar mapy terenu wzd³u¿ osi Z
			MAP_SCALE = 1;    // skala mapy

		//

		//float	(*g_terrain)[3] = NULL;


		//CGparameter terrainLightPosition[8],
  //                  terrainEyePosition,
  //                  terrainKa,
  //                  terrainKd[8],
		//			terrainLightCount,
		//			terrainEnableLighting;


	 //Menu Operator
  //
		//  #define UNNAMED          "Unnamed"
		//  char lastSave[MAX_PATH] = UNNAMED;    // sciezka do pliku ostatnio zapisywanego
		//  bool Saved = true;					// indykuje aktualny stan zapisu

    // zmienna okreslajaca zakonczenie aplikacji

		  BOOL  isDone;
	

	// syf do VBO
		
		PFNGLGENBUFFERSARBPROC	         glGenBuffersARB = 0;                     // VBO Name Generation Procedure
		PFNGLBINDBUFFERARBPROC			 glBindBufferARB = 0;                     // VBO Bind Procedure
		PFNGLBUFFERDATAARBPROC			 glBufferDataARB = 0;                     // VBO Data Loading Procedure
		PFNGLBUFFERSUBDATAARBPROC		 glBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
		PFNGLDELETEBUFFERSARBPROC		 glDeleteBuffersARB = 0;               // VBO Deletion Procedure
		PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = 0; // return various parameters of VBO
		PFNGLMAPBUFFERARBPROC			 glMapBufferARB = 0;                       // map VBO procedure
		PFNGLUNMAPBUFFERARBPROC			 glUnmapBufferARB = 0;                   // unmap VBO procedure



/****************************************************************************
			
 Glowny wezelek laczacy wszystkie naglowki definiujace narzedzia edytora etc.

******************************************************************************/

	#include "MainNode.h"


/****************************************************************************/

	int WINAPI WinMain(HINSTANCE g_hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
	{
		MSG   msg;

		// tworzy okno 
		if( SetupWindow(APP_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, USE_FULLSCREEN) )
		{
			isDone = FALSE;
		}
		else
		{
			isDone = TRUE;
			return 0;
		}



		CHiResTimer timer;
		timer.Init();

		int nFrames = 0;

		// pêtla przetwarzania komunikatów
		while ( !isDone )
		{
			// pompa komunikatow
			if( PeekMessage(&msg, g_hwnd, NULL, NULL, PM_REMOVE) )
			{
				if (msg.message == WM_QUIT)
				{
					isDone = TRUE;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			// nie aktualizuje sceny, jeœli okno jest zwiniête
			if( g_isActive )
			{
				DisplayScene();

				nFrames++;
				if( nFrames > 50 )
				{
					static char Caption[MAX_PATH];

					sprintf_s( Caption, "MiniEditor (FPS: %i) OpenGLver: %s",// Map: %s", 
							   (int)timer.GetFPS(50), OpenGLversion);//, lastSave);

					SetWindowText(g_hwnd, Caption);

					nFrames = 0;
				}
			}
		}

	// sprzatanie 

	  Cleanup();
	  KillWindow();

	  return msg.wParam;
	}





