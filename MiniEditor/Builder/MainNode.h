#pragma once



// wstepne deklaracje niektorych funkcji, aby kompilator nie plakal

	BOOL SetupWindow(const char *title, int width, int height, int bits, bool isFullscreen);
	BOOL KillWindow();

	GLvoid ResizeScene(GLsizei width, GLsizei height);
	BOOL InitializeScene();
	BOOL DisplayScene();
	BOOL Cleanup();


	#include "CameraOperator.h"

	#include "MiniOperator//MiniOperator.h"
	MiniOperator &miniOperator = MiniOperator::Singleton();


	#include "Scene.h"
	#include "WndProcedures.h"
	#include "Window.h"



	