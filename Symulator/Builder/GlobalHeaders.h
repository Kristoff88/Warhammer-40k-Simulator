#pragma once

#pragma warning( disable: 4018 4244 4290 4800 4996 )



/******************************** Makra ***************************/

	#define WIN64_LEAN_AND_MEAN   // "odchudza" aplikacjê Windows

	#define _WIN32_WINDOWS 0x501
	#define _WIN32_WINNT 0x0501 


	#define WND_CLASS_NAME	"Builder"

	#define PI  3.14159


	// makra z nazwami obiektow
	
    #define DETAIL_NAME         "1detailObject"
    #define MODEL_NAME			"2SimpleModel"
    #define LIGHT_BULB_NAME	    "3LightSource"
	#define LIQUID_NAME			"4liquidObject"



/********************************* Pliki nag³ówkowe *********************************/

	#include "Resource.h"


	#include <windows.h>    // standardowy plik nag³ówkowy Windows
	#include <winuser.h>    // sta³e Windows 
	#include <windowsx.h>

	#include <Commctrl.h>	// potrzebne do obslugi roznego rodzaju kontrolek WinApi

	
	#include <iosfwd>  //< --- iosfwd ponoc zmniejsza "compilation dependency"
	//#include <iostream>
	#include <fstream>
	#include <sstream>

	//#include <direct.h>	// do przegladania folderow (jak narazie nie bylo potrzebne...)

	#include <cassert>

	#include <string>

	#include <map>		// podstawowe pojemniki STL
	#include <vector>

	#include <algorithm>	// dla sort()
	
	#include <math.h>



	#include <gl/gl.h>      // standardowy plik nag³ówkowy OpenGL
	#include <gl/glu.h>     // plik nag³ówkowy biblioteki GLU
	#include <gl/glaux.h>
	#include "glext.h"		// rozszerzenia OpenGL


	#include "targetver.h"	// pierdolowate makra


	#include <cg\cg.h>									// NEW: Cg Header
	#include <cg\cggl.h>								// NEW: Cg OpenGL Specific Header

	#pragma comment( lib, "cg.lib" )					// Search For Cg.lib While Linking
	#pragma comment( lib, "cggl.lib" )	



	

	#include "Utilities//Tools//ToolsHeader.h"


	#include "GUI//Utilities//GUI_utilities.h"
	#include "GUI//GUI_Operator.h"
	GUI& gui = GUI::Singleton();

