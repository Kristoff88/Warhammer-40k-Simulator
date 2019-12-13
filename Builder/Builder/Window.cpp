	#include "GlobalHeaders.h"

	#include "Window.h"

#include <wingdi.h>


// zewnetrzne zmienne oraz funkcje

	extern HINSTANCE   g_hInstance;
	extern HWND		   g_hwnd;
	extern HDC		   g_hdc;
	extern HGLRC	   g_hrc;
	extern BOOL		   g_isFullscreen;



	extern GLvoid ResizeScene(GLsizei width, GLsizei height);
	extern BOOL InitializeScene();

	extern LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


/*****************************************************************************
	Tworzy okno, kontekst urz¹dzenia oraz kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL SetupWindow(const char *title, int width, int height, BYTE bits, bool isFullscreen)
	{
	// ustawia globalny znacznik

		g_isFullscreen = isFullscreen;

	// pobiera uchwyt instancji aplikacji

		g_hInstance = GetModuleHandle(NULL);

	// --- Rejestracja klasy glownego okna ----- //

		WNDCLASSEX  wc;    // klasa okna

		// wype³nia strukturê opisuj¹c¹ klasê okna
		wc.cbSize         = sizeof(WNDCLASSEX);
		wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc    = WndProc;
		wc.cbClsExtra     = 0; 
		wc.cbWndExtra     = 0;
		wc.hInstance      = g_hInstance;
		wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);	      // domyœlna ikona
		wc.hIconSm        = LoadIcon(NULL, IDI_WINLOGO);		      // logo windows 
		wc.hCursor        = LoadCursor(NULL, IDC_CROSS);			  // kursor
		wc.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);   // t³o w kolorze czarnym
		wc.lpszMenuName   = MAKEINTRESOURCE(IDC_BUILDER);
		wc.lpszClassName  = WND_CLASS_NAME;

	// rejestruje klasê okna

		if( !RegisterClassEx(&wc) )
		{
			MessageBox(NULL,"Nie mo¿na zarejestrowaæ klasy okna", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// konfiguruje tryb pe³noekranowy

		if( g_isFullscreen )
		{
		// wype³nia strukturê opisuj¹c¹ tryb wyœwietlania

			DEVMODE screenSettings;
			memset(&screenSettings,0,sizeof(screenSettings));

			screenSettings.dmSize       = sizeof(screenSettings);	
			screenSettings.dmPelsWidth  = width;	
			screenSettings.dmPelsHeight = height;		
			screenSettings.dmBitsPerPel = bits;	
			screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// próbuje prze³aczyæ kartê na zdefiniowany wy¿ej tryb graficzny

			if( ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL )
			{
			// jeœli prze³¹czenie siê nie powiod³o, to pozwala wybraæ tryb okienkowy
				if( MessageBox(NULL, "Prze³¹czenie do wybranego trybu graficznego nie jest mo¿liwe\n"
									 "Czy przejœæ do tryby okienkowego?",
									 "Programowanie gier w OpenGL",
									 MB_YESNO | MB_ICONEXCLAMATION) == IDYES )
				{
					g_isFullscreen = FALSE;
				}
				else
				{
					return FALSE;
				}
			}
		}

		DWORD dwExStyle;
		DWORD dwStyle;

	// konfiguruje styl okna

		if( g_isFullscreen )
		{
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;	// okno bez ramki i paska tytu³u
			//ShowCursor(FALSE);            // ukrywa kursor
		}
		else
		{
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
		}


		RECT  windowRect;
		windowRect.left = 0;
		windowRect.right = (LONG) width;
		windowRect.top = 0;
		windowRect.bottom = (LONG) height;

	// dopasowanie okna

		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	// tworzy okno

		g_hwnd = CreateWindowEx(dwExStyle,          // styl rozszerzony
		WND_CLASS_NAME,     // nazwa klasy
		title,              // nazwa aplikacji
		dwStyle |           // styl okna
		WS_CLIPCHILDREN |   // wymagane przez Opengl
		WS_CLIPSIBLINGS,    
		0, 0,               // wspó³rzêdne x,y coordinate
		windowRect.right  - windowRect.left, // szerokoœæ
		windowRect.bottom - windowRect.top, // wysokoœæ
		NULL,               // uchwyt okna nadrzêdnego 
		NULL,               // uchwyt menu
		g_hInstance,        // instancja aplikacji
		NULL);              // bez dodatkowych parametrów

	// sprawdza poprawnoœæ uchwytu okna

		if( !g_hwnd )
		{
			MessageBox(NULL, "Nie mo¿na utworzyæ okna", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// pobiera kontekst urzadzenia

		if( !(g_hdc = GetDC(g_hwnd)) )
		{
			MessageBox(NULL,"Nie mo¿na utworzyæ kontekstu urz¹dzenia", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// konfiguruje format pikseli

		PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// rozmiar struktury
		1,                              // domyœlna wersja
		PFD_DRAW_TO_WINDOW |            // okno grafiki OpenGL
		PFD_SUPPORT_OPENGL |            
		PFD_DOUBLEBUFFER,               // podwójne buforowanie
		PFD_TYPE_RGBA,                  // tryb kolorów RGBA 
		bits,                           // 32-bitowy opis kolorów
		0, 0, 0, 0, 0, 0,               // ignoruje bity koloru, bez palety
		0,                              // bez bufora alfa
		0,                              // ignoruje bit przesuniêcia
		0,                              // bez bufora akumulacji
		0, 0, 0, 0,                     // ignoruje bity akumulacji
		16,                             // 16-bitowy bufor z
		0,                              // bez bufora powielania
		0,                              // bez bufora pomocniczego
		PFD_MAIN_PLANE,                 // g³ówny plan rysowania
		0,                              // zarezerwowane
		0, 0, 0 };                      // ignoruje maski warstw

		pfd.cColorBits = 16;
		pfd.cDepthBits = 16;

		GLuint  pixelFormat;

	// wybiera najodpowiedniejszy format pikseli

		if( !(pixelFormat = ChoosePixelFormat(g_hdc, &pfd)) )
		{
			MessageBox(NULL, "Nie mo¿na dopasowaæ formatu pikseli", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// wybiera format pikseli dla kontekstu urz¹dzenia

		if( !SetPixelFormat(g_hdc, pixelFormat,&pfd) )
		{
			MessageBox(NULL, "Nie mo¿na dopasowaæ formatu pikseli", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// tworzy kontekst tworzenia grafiki OpenGL

		if( !(g_hrc = wglCreateContext(g_hdc)) )
		{
			MessageBox(NULL, "Nie mo¿na utworzyæ kontekstu grafiki OpenGL", "B³¹d",MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// aktywuje kontekst tworzenia grafiki

		if( !wglMakeCurrent(g_hdc, g_hrc) )
		{
			MessageBox(NULL,"Nie mo¿na aktywowaæ kontekstu grafiki OpenGL", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// aktywuje okno

		ShowWindow(g_hwnd, SW_SHOW);
		SetForegroundWindow(g_hwnd);
		SetFocus(g_hwnd);

	// zmienia rozmiary sceny

		ResizeScene(width, height);

	// wykonuje jednorazow¹ inicjacjê

		if( !InitializeScene() )
		{
			MessageBox(NULL, "Inicjalizacja aplikacji nie powiodla sie!", "Error", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		return TRUE;
	}


/*****************************************************************************
	Przywraca wyjœciowy tryb wyœwietlania. Usuwa okno, kontekst urz¹dzenia 
	i kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL KillWindow()
	{
	// przywraca oryginalny tryb wyœwietlania, jeœli prze³¹czy³ w tryb pe³noekranowy

		if( g_isFullscreen )
		{
			ChangeDisplaySettings(NULL, 0);
			ShowCursor(TRUE);
		}

	// zwalnia kontekst tworzenia grafiki

		if( g_hrc )
		{
		// zwalnia kontekst

			if( !wglMakeCurrent(NULL,NULL) )
			{
				MessageBox(NULL, "Nie mo¿na zwolniæ kontekstu grafiki", "B³¹d", MB_OK | MB_ICONINFORMATION);
			}

		// usuwa kontekst

			if( !wglDeleteContext(g_hrc) )
			{
				MessageBox(NULL, "Nie mo¿na usun¹æ kontekstu grafiki", "B³¹d", MB_OK | MB_ICONINFORMATION);
			}

			g_hrc = NULL;
		}

	// zwalnia kontekst urz¹dzenia

		if( g_hdc && !ReleaseDC(g_hwnd, g_hdc) )
		{
			MessageBox(NULL, "Nie mo¿na zwolniæ kontekstu urz¹dzenia", "B³¹d", MB_OK | MB_ICONINFORMATION);
			g_hdc = NULL;
		}

	// usuwa okno

		if( g_hwnd && !DestroyWindow(g_hwnd) )
		{
			MessageBox(NULL, "Nie mo¿na usun¹æ okna", "B³¹d", MB_OK | MB_ICONINFORMATION);
			g_hwnd = NULL;
		}

	// wyrejestrowuje klasê okna

		if( !UnregisterClass(WND_CLASS_NAME, g_hInstance) )
		{
			MessageBox(NULL, "Nie mo¿na wyrejestrowaæ klasy okna", "B³¹d", MB_OK | MB_ICONINFORMATION);
			g_hInstance = NULL;
		}

		return TRUE;
	}