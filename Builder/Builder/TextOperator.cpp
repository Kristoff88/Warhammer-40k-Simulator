#include "GlobalHeaders.h"

#include "TextOperator.h"


	// zmienne zewnetrzne

		extern HDC			g_hdc;				// kontekst urzadzenia potrzeby do inicjalizacji czcionki

		extern std::vector <std::string>  textOutput;	// wektor przechowujacy wiersze z danymi

		extern bool enableLighting;		// chcemy, aby tekst byl neutralny wzgledem oswietlenia

	// wczesniejsze deklaracje zewnetrznych funkcji

		void morphingText();
		void textureText();
		

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
					
	// Buildings.h
	   // in progres.. :)

	// Cubemap.h
    #define	CHANGE_CUBEMAP_COLORS	34		// zmiana kolorow cubemap
    #define	RELOAD_CUBEMAPS			35		// reload tekstury cubemap
	#define	CHANGE_CUBEMAP			36		// zmiana tekstury cubemap

    // Clouds.h
    #define	UPDATE_CLOUD			39		// aktualizacja atrybutow chmur
    #define	RELOAD_CLOUDS			40		// reload tekstury chmur
	#define	CHANGE_CLOUD			41		// zmiana tekstury chmur


		unsigned int fontID;


		void TextOperator(unsigned int Action)
		{

			// zaklada, ze jest uzywana macierz modelowania

				glPushMatrix();

				// resetuje macierz modelowania

					glLoadIdentity();

					glMatrixMode(GL_PROJECTION);

					glPushMatrix();

					// resetuje macierz projekcji

						glLoadIdentity();

					// pobranie rozmiarow viewport'a

						GLint viewport[4];
						glGetIntegerv(GL_VIEWPORT, viewport);

					// ustawienie rzutu ortograficznego

						gluOrtho2D(0, viewport[2], 0, viewport[3]);

						if( enableLighting )
							glDisable(GL_LIGHTING);
						
					// renderowanie tekstu czcionka rastrowa

						glColor4f(.15f, 1.f, .3f, 1.f);

						int height = 650;

						for(int i = 0; i < textOutput.size(); i++)
						{
							glRasterPos2f(0.7*viewport[2], height);
							PrintString(fontID, textOutput[i].c_str());
							height -= 20;
						}

						if( enableLighting )
							glEnable(GL_LIGHTING);


					glPopMatrix();

					glMatrixMode(GL_MODELVIEW);

				glPopMatrix();



				switch( Action )
				{

				// morfowanie terenu
					case MORPH:
					{
						morphingText(); // < -- funkcja z Morphing.h
					}
					break;


				// umieszczanie zrodla swiatla
					case PLACE_LIGHT:
					{
						textOutput.erase( textOutput.begin(), textOutput.end() );
						textOutput.push_back( "Action: Placing light" );
					}
					break;


				// malowanie tekstur ladu
					case PAINT_LAND:
					{
						textureText();	// < -- funkcja z Textures.h
					}
					break;


				// umieszczanie detalu
					case PLACE_DETAIL:
					{
						textOutput.erase( textOutput.begin(), textOutput.end() );
						textOutput.push_back( "Action: Placing detail" );
					}
					break;


				// plantowanie trawy pedzelkiem
					case PLANT_GRASS:
					{
						textOutput.erase( textOutput.begin(), textOutput.end() );
						textOutput.push_back( "Action: Planting grass" );
					}	
					break;

				// usuwanie trawy pedzelkiem
					case WEED_OUT:
					{
						textOutput.erase( textOutput.begin(), textOutput.end() );
						textOutput.push_back( "Action: Weeding out" );
					}	
					break;


				// umieszczanie wody
					case PLACE_LIQUID:
					{
						textOutput.erase( textOutput.begin(), textOutput.end() );
						textOutput.push_back( "Action: Placing liquid" );
					}
					break;

				
				// umieszczanie modelu na mapie
					case PLACE_MODEL:
					{
						textOutput.erase( textOutput.begin(), textOutput.end() );
						textOutput.push_back( "Action: Placing model" );
					}
					break;


					default:
					{
						textOutput.erase( textOutput.begin(), textOutput.end() );
						textOutput.push_back( "Action: None" );
					}
					break;
				}
		}


//
		bool InitTextOperator()
		{
			// utworzenie czcionki
			fontID = CreateBitmapFont("Arial", false, 20, g_hdc);

			if( fontID == 0 )
			{
				return false;
			}
				return true;
		}
		void CloseTextOperator()
		{
			ClearFont(fontID);
		}