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
	Tworzy okno, kontekst urz�dzenia oraz kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL SetupWindow(const char *title, int width, int height, BYTE bits, bool isFullscreen)
	{
	// ustawia globalny znacznik

		g_isFullscreen = isFullscreen;

	// pobiera uchwyt instancji aplikacji

		g_hInstance = GetModuleHandle(NULL);

	// --- Rejestracja klasy glownego okna ----- //

		WNDCLASSEX  wc;    // klasa okna

		// wype�nia struktur� opisuj�c� klas� okna
		wc.cbSize         = sizeof(WNDCLASSEX);
		wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc    = WndProc;
		wc.cbClsExtra     = 0; 
		wc.cbWndExtra     = 0;
		wc.hInstance      = g_hInstance;
		wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);	      // domy�lna ikona
		wc.hIconSm        = LoadIcon(NULL, IDI_WINLOGO);		      // logo windows 
		wc.hCursor        = LoadCursor(NULL, IDC_CROSS);			  // kursor
		wc.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);   // t�o w kolorze czarnym
		wc.lpszMenuName   = MAKEINTRESOURCE(IDC_BUILDER);
		wc.lpszClassName  = WND_CLASS_NAME;

	// rejestruje klas� okna

		if( !RegisterClassEx(&wc) )
		{
			MessageBox(NULL,"Nie mo�na zarejestrowa� klasy okna", "B��d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// konfiguruje tryb pe�noekranowy

		if( g_isFullscreen )
		{
		// wype�nia struktur� opisuj�c� tryb wy�wietlania

			DEVMODE screenSettings;
			memset(&screenSettings,0,sizeof(screenSettings));

			screenSettings.dmSize       = sizeof(screenSettings);	
			screenSettings.dmPelsWidth  = width;	
			screenSettings.dmPelsHeight = height;		
			screenSettings.dmBitsPerPel = bits;	
			screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// pr�buje prze�aczy� kart� na zdefiniowany wy�ej tryb graficzny

			if( ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL )
			{
			// je�li prze��czenie si� nie powiod�o, to pozwala wybra� tryb okienkowy
				if( MessageBox(NULL, "Prze��czenie do wybranego trybu graficznego nie jest mo�liwe\n"
									 "Czy przej�� do tryby okienkowego?",
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
			dwStyle = WS_POPUP;	// okno bez ramki i paska tytu�u
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
		0, 0,               // wsp�rz�dne x,y coordinate
		windowRect.right  - windowRect.left, // szeroko��
		windowRect.bottom - windowRect.top, // wysoko��
		NULL,               // uchwyt okna nadrz�dnego 
		NULL,               // uchwyt menu
		g_hInstance,        // instancja aplikacji
		NULL);              // bez dodatkowych parametr�w

	// sprawdza poprawno�� uchwytu okna

		if( !g_hwnd )
		{
			MessageBox(NULL, "Nie mo�na utworzy� okna", "B��d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// pobiera kontekst urzadzenia

		if( !(g_hdc = GetDC(g_hwnd)) )
		{
			MessageBox(NULL,"Nie mo�na utworzy� kontekstu urz�dzenia", "B��d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// konfiguruje format pikseli

		PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// rozmiar struktury
		1,                              // domy�lna wersja
		PFD_DRAW_TO_WINDOW |            // okno grafiki OpenGL
		PFD_SUPPORT_OPENGL |            
		PFD_DOUBLEBUFFER,               // podw�jne buforowanie
		PFD_TYPE_RGBA,                  // tryb kolor�w RGBA 
		bits,                           // 32-bitowy opis kolor�w
		0, 0, 0, 0, 0, 0,               // ignoruje bity koloru, bez palety
		0,                              // bez bufora alfa
		0,                              // ignoruje bit przesuni�cia
		0,                              // bez bufora akumulacji
		0, 0, 0, 0,                     // ignoruje bity akumulacji
		16,                             // 16-bitowy bufor z
		0,                              // bez bufora powielania
		0,                              // bez bufora pomocniczego
		PFD_MAIN_PLANE,                 // g��wny plan rysowania
		0,                              // zarezerwowane
		0, 0, 0 };                      // ignoruje maski warstw

		pfd.cColorBits = 16;
		pfd.cDepthBits = 16;

		GLuint  pixelFormat;

	// wybiera najodpowiedniejszy format pikseli

		if( !(pixelFormat = ChoosePixelFormat(g_hdc, &pfd)) )
		{
			MessageBox(NULL, "Nie mo�na dopasowa� formatu pikseli", "B��d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// wybiera format pikseli dla kontekstu urz�dzenia

		if( !SetPixelFormat(g_hdc, pixelFormat,&pfd) )
		{
			MessageBox(NULL, "Nie mo�na dopasowa� formatu pikseli", "B��d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// tworzy kontekst tworzenia grafiki OpenGL

		if( !(g_hrc = wglCreateContext(g_hdc)) )
		{
			MessageBox(NULL, "Nie mo�na utworzy� kontekstu grafiki OpenGL", "B��d",MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// aktywuje kontekst tworzenia grafiki

		if( !wglMakeCurrent(g_hdc, g_hrc) )
		{
			MessageBox(NULL,"Nie mo�na aktywowa� kontekstu grafiki OpenGL", "B��d", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

	// aktywuje okno

		ShowWindow(g_hwnd, SW_SHOW);
		SetForegroundWindow(g_hwnd);
		SetFocus(g_hwnd);

	// zmienia rozmiary sceny

		ResizeScene(width, height);

	// wykonuje jednorazow� inicjacj�

		if( !InitializeScene() )
		{
			MessageBox(NULL, "Inicjalizacja aplikacji nie powiodla sie!", "Error", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		return TRUE;
	}


/*****************************************************************************
	Przywraca wyj�ciowy tryb wy�wietlania. Usuwa okno, kontekst urz�dzenia 
	i kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL KillWindow()
	{
	// przywraca oryginalny tryb wy�wietlania, je�li prze��czy� w tryb pe�noekranowy

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
				MessageBox(NULL, "Nie mo�na zwolni� kontekstu grafiki", "B��d", MB_OK | MB_ICONINFORMATION);
			}

		// usuwa kontekst

			if( !wglDeleteContext(g_hrc) )
			{
				MessageBox(NULL, "Nie mo�na usun�� kontekstu grafiki", "B��d", MB_OK | MB_ICONINFORMATION);
			}

			g_hrc = NULL;
		}

	// zwalnia kontekst urz�dzenia

		if( g_hdc && !ReleaseDC(g_hwnd, g_hdc) )
		{
			MessageBox(NULL, "Nie mo�na zwolni� kontekstu urz�dzenia", "B��d", MB_OK | MB_ICONINFORMATION);
			g_hdc = NULL;
		}

	// usuwa okno

		if( g_hwnd && !DestroyWindow(g_hwnd) )
		{
			MessageBox(NULL, "Nie mo�na usun�� okna", "B��d", MB_OK | MB_ICONINFORMATION);
			g_hwnd = NULL;
		}

	// wyrejestrowuje klas� okna

		if( !UnregisterClass(WND_CLASS_NAME, g_hInstance) )
		{
			MessageBox(NULL, "Nie mo�na wyrejestrowa� klasy okna", "B��d", MB_OK | MB_ICONINFORMATION);
			g_hInstance = NULL;
		}

		return TRUE;
	}