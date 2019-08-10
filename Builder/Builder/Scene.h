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

		glViewport(0, 0.f, width * toolsModifier, height);

	// wybiera macierz rzutowania i �aduje do niej macierz jednostkow�

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

	// konfiguruje perspektyw�

		gluPerspective(54.0f, (GLfloat)width/(GLfloat)height, 1.0f, 50000.0f);

	// wybiera macierz modelowania

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

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
		glEnable(GL_DEPTH_TEST);		  // Enables Depth Testing
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);	


		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);


		glEnable(GL_TEXTURE_2D);



		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glBlendFunc(GL_DST_ALPHA, GL_ONE);



		glDisable(GL_LIGHTING);  // poczatkowo swiatlo jest wylaczone
		glEnable(GL_NORMALIZE);	// zezwol normalizacje


		// taka pierdola... moze kiedys dodam wiecej tego typu (jak narazie ne chce mi sie pieprzyc ze swiatlem)
		glLightModelf(GL_AMBIENT, 1.0f);
		glLightModelf(GL_DIFFUSE, 1.0f);


		glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);


		// sprawdza dost�pno�� blokowania tablic wierzcho�k�w

		char *extList = (char *) glGetString(GL_EXTENSIONS);

		if( extList && strstr(extList, "GL_EXT_compiled_vertex_array") )
		{
			// pobiera wska�niki funkcji rozszerze�
				glLockArraysEXT =     (PFNGLLOCKARRAYSEXTPROC) wglGetProcAddress("glLockArraysEXT");
				glUnlockArraysEXT = (PFNGLUNLOCKARRAYSEXTPROC) wglGetProcAddress("glUnlockArraysEXT");
		}

		// blokuje tablice, je�li dostepne jest rozszerzenie kompilacji tablic

			if( glLockArraysEXT )
				glLockArraysEXT(0, MAP_X * MAP_Z);




		// pobranie funkcji glDrawRangeElements()
			glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) wglGetProcAddress("glDrawRangeElements");

			if( glDrawRangeElements == 0 )
				MessageBox(g_hwnd, "Nie udalo sie pobrac wskaznika do funkcji 'glDrawRangeElements(...)'.", "Error!", MB_OK);


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


		// TerrainEditor.h
			
		if( !InitEditor() )
			return FALSE;

		return TRUE;
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

	// napierdalaj edytorze terenu!

		editRun();

	// podwojne buforowanie

		glFlush();
		SwapBuffers(g_hdc);	

		return TRUE;

	} // DisplayScene()
//
/*****************************************************************************
	Called at the end of successful program execution.
*****************************************************************************/

	BOOL Cleanup()
	{
		// je�li dost�pne jest rozszerzenie tablic kompilowanych, odblokowuje tablice
		if( glUnlockArraysEXT )
		   glUnlockArraysEXT();

		return TRUE;
	}


