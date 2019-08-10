#pragma once



/*****************************************************************************
	Wywo�ywana podczas startu aplikacji i za ka�dym razem, gdy u�ytkownik
	zmienia rozmiary okna.
*****************************************************************************/

	GLvoid ResizeScene(GLsizei width, GLsizei height)
	{
	// zapobiega dzieleniu przez 0

		if( height == 0 )  
		{
			height = 1;
		}

	// aktualizuje rozmiary okna

		glViewport(0, 0.f, width, height);

	// wybiera macierz rzutowania i �aduje do niej macierz jednostkow�

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

	// konfiguruje perspektyw�

		gluPerspective(54.0f, (GLfloat)width/(GLfloat)height, 1.0f, 50000.0f);

	// wybiera macierz modelowania

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		GUI::Singleton().Data.AdjustGUIToClientArea(g_hwnd);
	}
//
/*****************************************************************************
	Jednorazowa inicjacja parametr�w sceny.
*****************************************************************************/

	BOOL InitializeScene()
	{
	// inicjalizacja OpenGL

		glClearColor(1.f, 1.f, 1.f, 1.f); 

	 
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


		glShadeModel(GL_SMOOTH);


		glClearDepth(1.0f);			  // Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);	  // Enables Depth Testing
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);	


		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);


		glEnable(GL_TEXTURE_2D);



		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glBlendFunc(GL_DST_ALPHA, GL_ONE);



		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);


		// taka pierdola... moze kiedys dodam wiecej tego typu (jak narazie ne chce mi sie pieprzyc ze swiatlem)
		glLightModelf(GL_AMBIENT, 1.0f);
		glLightModelf(GL_DIFFUSE, 1.0f);


		glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);


		// sprawdza dost�pno�� blokowania tablic wierzcho�k�w

		char *extList = (char *) glGetString(GL_EXTENSIONS);


		// pobranie funkcji glDrawRangeElements()
		glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) wglGetProcAddress("glDrawRangeElements");

		if( glDrawRangeElements == 0 )
		{
			MessageBox(g_hwnd, "Nie udalo sie pobrac wskaznika do funkcji 'glDrawRangeElements(...)'.", "Error!", MB_OK);
		}

		// pobranie funkcji do VBO
		if( extList && strstr(extList, "GL_ARB_vertex_buffer_object") )
		{
		// get pointers to GL functions
			glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
			glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
			glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
			glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
			glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
			glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
			glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
			glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
		}
		else
			return false;

		// pobranie wersji OpenGL
		strcpy_s( OpenGLversion, (const char*)glGetString( GL_VERSION ) );



		if( !OBJ_Loader::Singleton().Init() )
		{
			MessageBox(g_hwnd, "Nieudana inicjalizacja loader'a plikow OBJ.", "Error", MB_OK);
			return false;
		}

		InitCameraOperator();



		SetWindowText(g_hwnd, "Initializing terrain...");
		if( !InitEditor() )
		{
			return false;
		}

		SetWindowText(g_hwnd, "Initializing MiniOperator...");
		if( !miniOperator.Init(g_hdc, g_hwnd) )
		{
			miniOperator.battleForceManager.SetHeightMap( GetHeightMap(), MAP_X, MAP_Z );
			return false;
		}
		
		return true;
	}
//
/*****************************************************************************
	Rysuje scen�. Funkcja ta wywo�ywana jest dla kazdej klatki animacji.
*****************************************************************************/

	BOOL DisplayScene()
	{
	// opr�nia bufory ekranu i g�ebi

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();


		cameraRun();
		editRun();

		if( !miniOperator.Run() )
		{
			return false;
		}


	// podwojne buforowanie

		glFlush();
		SwapBuffers(g_hdc);	

		return true;

	} // DisplayScene()
//
/*****************************************************************************
	Called at the end of successful program execution.
*****************************************************************************/

	BOOL Cleanup()
	{
		return TRUE;
	}


