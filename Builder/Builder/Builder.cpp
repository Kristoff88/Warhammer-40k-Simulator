
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

const int   SCREEN_WIDTH = 900;
const int	  SCREEN_HEIGHT = 700;
const int   SCREEN_BPP = 32;
const bool  USE_FULLSCREEN = false;
const char* APP_TITLE = "Builder";

const float  toolsModifier = 0.7f;   //odpowiada za proporcjonalna szerokosc paska narzedzi


/********************************* Zmienne globalne **********************************/

HDC       g_hdc;                   // kontekst urz¹dzenia
HGLRC     g_hrc;                   // kontekst tworzenia grafiki
BOOL      g_isFullscreen = FALSE;  // prze³¹cza tryb pe³noekranowy i okinekowy
BOOL      g_isActive = TRUE;   // false, gdy okno jest zwiniête
HWND      g_hwnd = NULL;   // uchwyt g³ównego okna aplikacji
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

wheelMovement = 0,	    //okresla, jak bardzo uzytkownik kreci scrollem
mouseWheelSteps = 0;


bool	altPressed = false,
ctrlPressed = false,

deletePressed = false,

cameraRotate = false,   //true w przypadku wcisniecia klawisza ALT (WM_SYSKEYDOWN)

lButtonDown = false,
rButtonDown = false,

lButtonDblClk,
rButtonDblClk,

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

	// uniwersalne zmienne

extern const int	MAX_HEIGHT = 50,	// wysokosc poziomu terenu (brzegow)
MIN_HEIGHT = 1,

MAX_MAP_SCALE = 20,	// skala
MIN_MAP_SCALE = 1,

MAX_MAP_X = 1024,
MIN_MAP_X = 4,

MAX_MAP_Z = 1024,
MIN_MAP_Z = 4,

OVERALL_MAX_HEIGHT = 300,
OVERALL_MIN_HEIGHT = 1;

int	MAP_X = 128,        // rozmiar mapy terenu wzd³u¿ osi X
MAP_Z = 128,        // rozmiar mapy terenu wzd³u¿ osi Z
MAP_SCALE = 1,      // skala mapy

MAX_TERRAIN_HEIGHT = 50,  // wysokosc terenu (poszczegolnych wierzcholkow)
MIN_TERRAIN_HEIGHT = 1,

terrainHeight = 1;

float(*g_terrain)[3] = NULL;

bool	droppingObjects = false;  // okresla, czy w trakcie zmian rozmiarow terenu, nalezy usuwac obiekty wystajace poza teren


CGparameter terrainLightPosition[8],
terrainEyePosition,
terrainKa,
terrainKd[8],
terrainLightCount,
terrainEnableLighting;


// Zmienne Morphingu


// Zmienne tekstur


// Zmienne detali

bool showDetails = true;

int detailCounter = 0;  // jesli 0, zaoszczedza wywolywania funkcji korygujacej detale przy morphingu (executeSpeficicFunctions("detailObject"));

// Liquids

bool showLiquids = true;

int liquidCounter = 0;  // jesli 0, zaoszczedza wywolywania funkcji korygujacej detale przy morphingu (executeSpeficicFunctions("liquidObject"));


CGparameter liquidLightPosition[8],
liquidEyePosition,
liquidKa,
liquidKd[8],
liquidLightCount,
liquidEnableLighting;

// Trawa


// Zmienne swiatla

bool showLights = true,
enableLighting = false;	// wlacza/wylacza oswietlenie

// Cubemap

bool showCubemap = true;

// Clouds

bool showCloud = true;


// Zmienne kursora

int  	cursorX, cursorZ; // pozycja, wokol ktorej nalezy rysowac kursor, morphowac teren, etc.

bool roundCursor = false;

// Zmienne TextOperator'a

std::vector <std::string> textOutput;	// ta zmienna bedzie modyfikowana w ActionOperator() (TerrainEditor.h)

bool	render2Dtext = false;

// Menu Operator

#define UNNAMED          "Unnamed"
char lastSave[MAX_PATH] = UNNAMED;	    // sciezka do pliku ostatnio zapisywanego

bool Saved = true;				// indykuje aktualny stan zapisu

// zmienna okreslajaca zakonczenie aplikacji

BOOL  isDone;

// Slownik Akcji (uzywaja go ActionOperator(), procedury okienkowe, Cursor.h itd.)

#define NONE					0		// brak akcji

// Morphing.h
#define MORPH					1		// morfowanie terenu

// Lighting.h
#define PLACE_LIGHT				2		// ustawianie swiatla
#define	REMOVE_LIGHTS			3		// usuwanie wszystkich swiatel 

// Textures.h
#define	PAINT_LAND				4		// malowanie terenu
#define	SET_LAYER				5	    // ustawianie warstwy tekstury
#define REMOVE_LAYER			6		// usuwanie warstwy tekstury
#define	LAYER_UP				7	    // popchniecie warstwy tekstury wyzej
#define LAYER_DOWN			    8		// popchniecie warstwy tekstury nizej
#define	CLEAR_LAND				9		// czyszczenie terenu z warstw tekstur
#define	RELOAD_TEXTURES			10		// reload tekstur terenu
#define	CHANGE_TEXTURE			11		// zmiana tekstury terenu

// Details.h
#define	PLACE_DETAIL			12		// umieszczanie detalu
#define	REMOVE_DETAILS			13		// usuwanie wszystkich detali
#define	RELOAD_DETAILS			14		// reload tekstur detali
#define	CHANGE_DETAIL			15		// zmiana tekstury detali

// Grass.h
#define	PLANT_GRASS				16		// plantowanie trawy
#define	WEED_OUT				17		// usuwanie trawy (chwastow!!)
#define	FILL_GRASS				18		// wypelnienie terenu sciolka
#define	REMOVE_GRASS			19		// calkowite usuniecie sciolki
#define	RELOAD_GRASS			20		// reload tekstur trawy
#define	CHANGE_GRASS			21		// zmiana tekstury trawy

// Liquids.h
#define	PLACE_LIQUID			22		// umieszczanie wody
#define	REMOVE_LIQUIDS			23		// usuwanie wszystkich wod
#define	RELOAD_LIQUIDS			24		// reload tekstur wody
#define	CHANGE_LIQUID			25		// zmiana tekstury wody

// Terrain.h
#define	EQUILIZE_TERRAIN		26		// wyrownywanie terenu wg. terrainHeight
#define	ADJUST_TERRAIN			27		// dostosowanie terenu do nowych max/min_terrain_height

// Models.h									
#define	PLACE_MODEL				28		// umieszczanie modelu
#define	REMOVE_MODELS			29		// usuwanie wszystkich modeli
#define	RELOAD_MODEL_TEXTURES	30		// reload tekstur modelu			
#define	CHANGE_MODEL_TEXTURE	31		// zmiana tekstury modelu
#define	RELOAD_MODELS			32		// reload model object'ow
#define	CHANGE_MODEL			33		// zmiana model object'a

// Cubemap.h
#define	CHANGE_CUBEMAP_COLORS	34		// zmiana kolorow cubemap
#define	RELOAD_CUBEMAPS			35		// reload tekstury cubemap
#define	CHANGE_CUBEMAP			36		// zmiana tekstury cubemap

// Clouds.h
#define	UPDATE_CLOUD			39		// aktualizacja atrybutow chmur
#define	RELOAD_CLOUDS			40		// reload tekstury chmur
#define	CHANGE_CLOUD			41		// zmiana tekstury chmur


unsigned int  Action = NONE;  // definiuje typ akcji, aktualnie podejmowanej przez editRun()


// handler toolbox'a (samotny siedzi w rogu :( )

#define TAB_COUNT 10  // ilosc zakladek w tool box'ie

HWND	dialog = NULL;


// syf do VBO
PFNGLGENBUFFERSARBPROC	         glGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC			 glBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC			 glBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC		 glBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC		 glDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC			 glMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC			 glUnmapBufferARB = 0;                   // unmap VBO procedure

#include "Resource.h"
#include "Scene.h"
#include "TerrainEditor.h"
#include "WndProcedures.h"
#include "Window.h"


/*****************************************************************************
 WinMain()

 Tutaj rozpoczyna siê wykonywanie aplikacji Windows.
*****************************************************************************/
int WINAPI WinMain(HINSTANCE g_hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG   msg;       // komunikat

// tworzy okno 
	if (SetupWindow(APP_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, USE_FULLSCREEN))
	{
		isDone = FALSE;
	}

	// jeœli utworzenie okna nie powiod³o siê, ustawia znacznik zakoñczenia aplikacji

	else
	{
		isDone = TRUE;
		return 0;
	}


	CHiResTimer timer;
	timer.Init();

	int nFrames = 0;

	// pêtla przetwarzania komunikatów
	while (!isDone)
	{
		// pompa komunikatow
		if (PeekMessage(&msg, g_hwnd, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)   // czy odebrano komunikat WM_QUIT ?
			{
				isDone = TRUE;              // jeœli tak, to ustawia znacznik zakoñczenia
			}
			else if (!IsDialogMessage(dialog, &msg))
			{
				TranslateMessage(&msg);     // tlumaczy komunikat i przekazuje do kolejki
				DispatchMessage(&msg);
			}
		}

		// nie aktualizuje sceny, jeœli okno jest zwiniete
		if (g_isActive)
		{
			// rysuje scenê w ka¿dym przebiegu pêtli

			DisplayScene();

			nFrames++;
			if (nFrames > 50)
			{
				// wyœwietla prêdkoœæ tworzenia klatek

				static char Caption[MAX_PATH];

				sprintf_s(Caption, "TerrainBuilder (FPS: %i) OpenGLver: %s Map: %s", (int)timer.GetFPS(50), OpenGLversion, lastSave);

				SetWindowText(g_hwnd, Caption);

				nFrames = 0;
			}
		}
	}


	// sprzatanie 
	CloseEditor();

	Cleanup();
	KillWindow();


	return msg.wParam;

}