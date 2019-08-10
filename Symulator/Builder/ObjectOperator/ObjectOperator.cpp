	#include "GlobalHeaders.h"

	#include "ObjectOperator.h"



	// do strzelania message box'ami
	extern  HWND      g_hwnd;	

	 // ObjectOperator potrzebuje instancji aplikacji, do wygenerowania ProgressDialog'u)
	extern  HINSTANCE g_hInstance; 

	// wektor sluzacy do wypisywania tekstu 2D na scenie
	extern std::vector <std::string> textOutput;


// Zmienne myszy, klawiatury i kamery

	extern  int		g_mouseX, g_mouseY,    
					oldMouseX ,
					oldMouseY,

					wheelMovement,	    
					mouseWheelSteps;


	extern bool		altPressed ,
					ctrlPressed,

					deletePressed,

					lButtonDown ,
					rButtonDown,
		
					lButtonDblClk,
					rButtonDblClk,

					mouseMoved; 

// uniwersalne zmienne

	extern   int	MAP_X,      
					MAP_Z,        
					MAP_SCALE;     


	extern  float	(*g_terrain)[3];

		
// Zmienne detali

	extern  bool	showDetails;

// Liquids

	extern  bool	showLiquids,
					enableLighting;

// Zmienne swiatla

	extern  bool	showLights;

// Zmienne menu operatora

//	extern bool		Saved;
//
//
//// handler toolbox'a (samotny siedzi w rogu :( )
//
//	extern  HWND	dialog;

	

	

/*****************************************************************
	Konstruktor, destruktor Object
*****************************************************************/

	ObjectOperator::Object::Object( float x, float y, float z, 
									float sizX, float sizY, float sizZ, 
									void *object, 
									bool (*pre)(),
									void (*rend)(float &, float &, float &, float &, float &, float &, void *, bool), 
									void (*post)(),
									void (*mov)(float &, float &, float &, float &, float &, float &, void *),
									void (*remov)(void *),
									void (*keyMng)(char, void*, float &, float &, float &, float &, float &, float &),
									void (*txt)(float &, float &, float &, float &, float &, float &, void *),
									void (*spec)(float &, float &, float &, float &, float &, float &, void*),
									std::string typName,
									bool Scal,
									bool morphAdj,
									bool drawFrame)
								   :	  
								   posX(x), posY(y), posZ(z), 
								   sizeX(sizX), sizeY(sizY), sizeZ(sizZ),
								   pointer(object),
								   preRenderer(pre),
								   renderer(rend),
								   postRenderer(post),
								   mover(mov),
								   remover(remov),
								   keyManager(keyMng),
								   text(txt),
								   specific(spec),
								   typeName(typName),
								   Scaled(Scal),
								   MorphAdjustable(morphAdj),
								   DrawingFrame(drawFrame),
								   Selected(false)
	{}
	ObjectOperator::Object::~Object()
	{}
//
/*****************************************************************
	Metody Object
*****************************************************************/

	void ObjectOperator::Object::Select     ()     
	{  
		Selected = true;  
	}

	void ObjectOperator::Object::Unselect   ()   
	{  
		Selected = false; 
	}

	bool ObjectOperator::Object::IsSelected () 
	{  
		return Selected;  
	}
//
//
	bool ObjectOperator::Object::CoordinatesAreInsideObject (float x, float y, float z)
	{
		float resultX, resultY, resultZ;

		resultX = posX - x;
		resultY = posY - y;
		resultZ = posZ - z;

		if(resultX < 0)
			resultX *= -1;

		if(resultY < 0)
			resultY *= -1;

		if(resultZ < 0)
			resultZ *= -1;


		if( resultX <= sizeX &&
			resultY <= sizeY &&
			resultZ <= sizeZ )
		{
			return true;
		}
		else
			return false;
	}
//
//
	void*       ObjectOperator::Object::GetObject   ()
	{  
		return pointer;  
	}

	std::string ObjectOperator::Object::GetType     ()
	{  
		return typeName;  
	}


	Vector3     ObjectOperator::Object::GetPosition ()
	{
		Vector3 position(posX, posY, posZ);
		return position;
	}

	Vector3     ObjectOperator::Object::GetSize     ()
	{
		Vector3 size(sizeX, sizeY, sizeZ);
		return size;
	}
//
//

	bool ObjectOperator::Object::InitDrawing()
	{
		if( preRenderer != NULL )
		{
			return preRenderer();
		}
		else
			return true;
	}

	void ObjectOperator::Object::DrawFrame()
	{
		glLineWidth(2.5f);

		glDisable(GL_TEXTURE_2D);

		if( enableLighting )
			glDisable(GL_LIGHTING);

		if ( !Scaled )
		{
			glPushMatrix();
				
				glColor3f(0.f, 0.35f, 0.65f);

				glTranslatef(posX, posY, posZ);

				glBegin(GL_LINE_LOOP);

					// œciana dolna
					glVertex3f(sizeX, 0, sizeZ);
					glVertex3f(sizeX, 0, -sizeZ);
					glVertex3f(-sizeX, 0, -sizeZ);
					glVertex3f(-sizeX, 0, sizeZ);
					glVertex3f(sizeX, 0, sizeZ);

					// œciana przednia
					glVertex3f(sizeX,  sizeY, sizeZ);
					glVertex3f(sizeX,  sizeY, -sizeZ);
					glVertex3f(sizeX,  0, -sizeZ);
					
					// œciana prawa
					glVertex3f(sizeX, sizeY,   -sizeZ);
					glVertex3f(-sizeX, sizeY,  -sizeZ);
					glVertex3f(-sizeX, 0, -sizeZ);
					
					// œciana tylna
					glVertex3f(-sizeX, sizeY, -sizeZ);
					glVertex3f(-sizeX, sizeY, sizeZ);
					glVertex3f(-sizeX, 0, sizeZ);

					// œciana lewa
					glVertex3f(-sizeX, sizeY, sizeZ);
					glVertex3f(sizeX, sizeY, sizeZ);

				glEnd();

			glPopMatrix();
		}
		else
		{
			// jezeli pozycja obiektu jest skalowana wzgledem mapy, to wypada zeby jego ramka takze byla skalowana
			// makra sa uzywane w celu unikniecia rzeczywistej zmiany pozycji (co by doprowadzilo do bezsensownego odejmowania pozycji)
				
			#define posX ( (int)posX - ((int)posX % MAP_SCALE) )
			#define posZ ( (int)posZ - ((int)posZ % MAP_SCALE) )


			glPushMatrix();
				
				glColor3f(0.f, 0.35f, 0.65f);

				glTranslatef(posX, posY, posZ);

				glBegin(GL_LINE_LOOP);

					// œciana dolna
					glVertex3f(sizeX, 0, sizeZ);
					glVertex3f(sizeX, 0, -sizeZ);
					glVertex3f(-sizeX, 0, -sizeZ);
					glVertex3f(-sizeX, 0, sizeZ);
					glVertex3f(sizeX, 0, sizeZ);

					// œciana przednia
					glVertex3f(sizeX,  sizeY, sizeZ);
					glVertex3f(sizeX,  sizeY, -sizeZ);
					glVertex3f(sizeX,  0, -sizeZ);
					
					// œciana prawa
					glVertex3f(sizeX, sizeY,   -sizeZ);
					glVertex3f(-sizeX, sizeY,  -sizeZ);
					glVertex3f(-sizeX, 0, -sizeZ);
					
					// œciana tylna
					glVertex3f(-sizeX, sizeY, -sizeZ);
					glVertex3f(-sizeX, sizeY, sizeZ);
					glVertex3f(-sizeX, 0, sizeZ);

					// œciana lewa
					glVertex3f(-sizeX, sizeY, sizeZ);
					glVertex3f(sizeX, sizeY, sizeZ);

				glEnd();

			glPopMatrix();


			#undef posX
			#undef posZ
			
		}


		if( enableLighting )
			glEnable(GL_LIGHTING);


		glEnable(GL_TEXTURE_2D);

		glLineWidth(1.f);
	}

	void ObjectOperator::Object::Draw()
	{  
		if( Selected && DrawingFrame )
		{
			DrawFrame();
		}

		if( renderer != NULL )
		{
			renderer(posX, posY, posZ, sizeX, sizeY, sizeZ, pointer, Selected);  
		}
	}

	void ObjectOperator::Object::CloseDrawing()
	{
		if( postRenderer != NULL )
		{
			postRenderer();
		}
	}
//
//
	void ObjectOperator::Object::Move(float heightModifier)
	{
		Vector3 wektor    =  GetOGLPos(g_mouseX, g_mouseY),
				oldWektor =  GetOGLPos(oldMouseX, oldMouseY);


		if( wektor.x > oldWektor.x )
		{
			posX += wektor.x - oldWektor.x;
		}
		else if ( wektor.x < oldWektor.x )
		{
			posX -= oldWektor.x - wektor.x;
		}

		if( wektor.z > oldWektor.z )
		{
			posZ += wektor.z - oldWektor.z;
		}
		else if( wektor.z < oldWektor.z )
		{
			posZ -= oldWektor.z - wektor.z;
		}

		// heightModifier ustawiany jest w funckji moveObjects() w ObjectOperator.h
		// (przy ruszaniu kolkiem myszy WHEEL_DATA rosnie/maleje w procedurze okienkowej WndProc())

			posY += heightModifier;


	// korygacja polozenia obiektu (jezeli wystaje poza mape)

		if( posX + sizeX > MAP_X*MAP_SCALE )
		{
			posX = (MAP_X*MAP_SCALE) - sizeX;
		}
		else if( posX - sizeX < 0 )
		{
			posX = sizeX;
		}

	// Z jest ujemne <.<

		if( posZ - sizeZ < -MAP_Z*MAP_SCALE )
		{
			posZ = -MAP_Z*MAP_SCALE + sizeZ;
		}
		else if( posZ + sizeZ > 0 )
		{
			posZ = -sizeZ;
		}

		if( posY < 0 )
		{
			posY = 0;
		}

		if( mover != NULL)
		{
			mover(posX, posY, posZ, sizeX, sizeY, sizeZ, pointer);
		}
	}



	void ObjectOperator::Object::Remove()
	{  
		if( remover != NULL )
			remover(pointer);  
	}



	void ObjectOperator::Object::AdjustHeight(float modifier)
	{
		posY += modifier;
	}

	void ObjectOperator::Object::Rescale(float newScale, float oldScale)
	{
		float scale = newScale / oldScale;

	// przeskalowanie pozycji

		if( Scaled )
		{
			posX /= oldScale;
			posY /= oldScale;
			posZ /= oldScale;

			posX *= newScale;
			posY *= newScale;
			posZ *= newScale;
		}
		else
		{
			posX *= scale;
			posY *= scale;
			posZ *= scale;
		}

	
	// przeskalowanie rozmiaru

		sizeX /= oldScale;
		sizeY /= oldScale;
		sizeZ /= oldScale;

		sizeX *= newScale;
		sizeY *= newScale;
		sizeZ *= newScale;
	}

	void ObjectOperator::Object::ManageKey(char key)
	{
		if( keyManager != NULL )
		{
			keyManager(key, pointer, posX, posY, posZ, sizeX, sizeY, sizeZ);
		}
	}



	
	void ObjectOperator::Object::MorphAdjust()
	{
		int index, X, Z;

		X =  posX / MAP_SCALE;
		Z = -posZ / MAP_SCALE;

		index = X + MAP_X*Z;

	// obiekt nie moze znajdowac sie pod terenem...

		if( posY <= g_terrain[index][1] )
			posY = g_terrain[index][1]+0.1f;
	}

	bool ObjectOperator::Object::GetMorphAdjustable()
	{
		return MorphAdjustable;
	}



	void ObjectOperator::Object::SpecificFunction()
	{
		if( specific != NULL )
		{
			specific(posX, posY, posZ, sizeX, sizeY, sizeZ, pointer);
		}
	}


	void ObjectOperator::Object::Text()
	{
		if( text != NULL )
		{
			text(posX, posY, posZ, sizeX, sizeY, sizeZ, pointer);
		}
	}

/*****************************************************************

	Metody ObjectOperator

*****************************************************************/

	/*void ObjectOperator::objectsText()
	{
		if( movingID.size() == 1 )
		{
			objectArray[ movingID[0] ].Text();
		}
	}*/


	void ObjectOperator::Sort()
	{
		// posortowanie wektora obiektow, z uzyciem funktora bioracego pod uwage nazwe typu (string)

			std::sort( objectArray.begin(), objectArray.end(), ObjectOperator::Singleton().Functor );

		// wyczyszczenie wektora zawierajacego pierwsze indeksu listy obiektow danego typu w wektorze objectArray

			typeIndices.erase( typeIndices.begin(), typeIndices.end() );

		// zmienna uzywana do porownaniua nazw typow

			std::string previousType = "";


			if( !objectArray.empty() )
			{
				for(int i = 0; i < objectArray.size(); i++)
				{
					if( strcmp( previousType.c_str(), objectArray[i].GetType().c_str() ) != 0 )
					{
						if( !typeIndices.empty() )
						{
							typeIndices.back().lastIndex = i-1;
						}
						
						range newRange;
						newRange.firstIndex = i;

						typeIndices.push_back(newRange);
					}

					previousType = objectArray[i].GetType();
				}

				typeIndices.back().lastIndex = objectArray.size() - 1;
			}

	}
	
	void ObjectOperator::Add(Object& newObject)
	{
		//unselectObjects();
		objectArray.push_back(newObject);
		Sort();
	}

	//void ObjectOperator::dropObjects()
	//{
	//	if( !objectArray.empty() )
	//	{
	//		std::vector <Object> objectArrayCopy;	// do niego beda kopiowane te pola obiekty, ktore nie wystaja poza granice terenu

	//		Vector3 position;	// do niego beda ladowane pozycje obiektow


	//		unsigned int xBorder = MAP_X*MAP_SCALE,
	//					 zBorder = MAP_Z*MAP_SCALE;


	//		unsigned int workTime = objectArray.size();

	//		ProgressDialog progressDlg(g_hInstance, g_hwnd, "Deleting objects...",
	//								   0, workTime, 1);


	//	// jezeli sa jakies obiekty zaznaczone -> odznacz je

	//		if( !movingID.empty() )
	//		{
	//			for(int i = 0; i < movingID.size(); i++)
	//			{
	//				objectArray[movingID[i]].Unselect();
	//			}

	//			movingID.erase(movingID.begin(), movingID.end());
	//		}


	//		for(int object = 0; object < objectArray.size(); object++)
	//		{
	//			position = objectArray[object].GetPosition();

	//			if( position.posX > xBorder || -position.posZ > zBorder )
	//			   ;
	//			else
	//				objectArrayCopy.push_back( objectArray[object] );

	//			progressDlg.Increment();
	//		}


	//		objectArray.erase(objectArray.begin(), objectArray.end());
	//		objectArray = objectArrayCopy;
	//	}
	//}



	//void ObjectOperator::objectScan()
	//{
	//	Vector3 wektor = GetOGLPos(g_mouseX, g_mouseY);

	//// pojedyncze zaznaczanie

	//	if ( ctrlPressed == false )
	//	{

	//		for(int i = 0; i < objectArray.size(); i++)
	//		{
	//					
	//			if( objectArray[i].CoordinatesAreInsideObject(wektor.posX, wektor.posY, wektor.posZ) )
	//			{
	//				if( !objectArray[i].IsSelected() )  // jesli obiekt nie jest juz zaznaczony
	//				{

	//				// zabezpieczenie przed zaznaczaniem ukrytych obiektow

	//					if( objectArray[i].GetType()      == LIGHT_BULB_NAME && showLights != true )
	//						;
	//					else if( objectArray[i].GetType() == DETAIL_NAME && showDetails != true )
	//						;
	//					else if( objectArray[i].GetType() == LIQUID_NAME && showLiquids != true )
	//						;
	//					else
	//					{
	//						objectArray[i].Select();

	//						if( !movingID.empty() )
	//						{
	//							objectArray[movingID.front()].Unselect();
	//							movingID.erase(movingID.begin());
	//						}

	//						movingID.push_back(i);

	//						Stickied = true;
	//					}
	//				}
	//			}
	//			else if ( !objectArray[i].CoordinatesAreInsideObject(wektor.posX, wektor.posY, wektor.posZ) )
	//			{

	//				if( !movingID.empty() )
	//				{
	//					for(int j = 0; j < movingID.size(); j++)
	//					{
	//						if( i == movingID[j] )
	//						{
	//							movingID.erase(movingID.begin()+j);
	//							objectArray[i].Unselect();
	//						}
	//					}
	//				}

	//				if( movingID.empty() )
	//					Stickied = false;

	//			}

	//		} // for...

	//	} // ctrlPressed == false

	//	// zaznaczanie wielu obiektow
	//	else if ( ctrlPressed )
	//	{

	//		for(int i = 0; i < objectArray.size(); i++)
	//		{
	//					
	//			if( objectArray[i].CoordinatesAreInsideObject(wektor.posX, wektor.posY, wektor.posZ) )
	//			{

	//				if( !objectArray[i].IsSelected() )  // jesli obiekt nie jest juz zaznaczony
	//				{

	//						if( objectArray[i].GetType() == LIGHT_BULB_NAME && showLights != true )
	//							;
	//						else if( objectArray[i].GetType() == DETAIL_NAME && showDetails != true )
	//							;
	//						else if( objectArray[i].GetType() == LIQUID_NAME && showLiquids != true )
	//							;
	//						else
	//						{
	//							objectArray[i].Select();
	//							movingID.push_back(i);

	//							Stickied = true;
	//						}
	//					
	//				}
	//				
	//				// jesli jest juz zaznaczony
	//				else
	//				{
	//					if( !movingID.empty() )
	//					{
	//						objectArray[i].Unselect();

	//						for(int j = 0; j < movingID.size(); j++)
	//						{
	//							if( i == movingID[j] )
	//							{
	//								movingID.erase(movingID.begin()+j);
	//								break;
	//							}
	//						}
	//					}
	//				}

	//			}// if( Check(...) )

	//		} // for...

	//	} // ctrlPressed == true


	//	Selecting = false;
	//	
	//}




	//void ObjectOperator::unselectObjects()
	//{
	//	for(int i = 0; i < objectArray.size(); i++)
	//	{
	//		objectArray[i].Unselect();
	//	}

	//	movingID.erase( movingID.begin(), movingID.end() );

	//	Stickied = false;
	//}
	//void ObjectOperator::moveObjects()
	//{
	//	if( Stickied && ( ( lButtonDown && mouseMoved ) ||  ( lButtonDown && wheelMovement ) ) ) 
	//	{
	//		// obsluga modulacji wysokosci obiektu

	//			if( wheelMovement < 0 )
	//			{
	//				for(int i = 0; i > mouseWheelSteps; i--)
	//					heightModifier -= heightModifierSpeed * MAP_SCALE;
	//			}
	//			else if(wheelMovement > 0)
	//			{
	//				for(int i = 0; i < mouseWheelSteps; i++)
	//					heightModifier += heightModifierSpeed * MAP_SCALE;
	//			}

	//			wheelMovement %= WHEEL_DELTA;

	//		// przemieszcza wszystkimi obiektami

	//			for(int i = 0; i < movingID.size(); i++)
	//				objectArray[movingID[i]].Move(heightModifier);

	//		// reset wyznacznika modyfikacji wysokosci
	//			    
	//			heightModifier = 0.f;
	//		
	//		// reset flagi poruszania myszka

	//			mouseMoved = false;
	//	}

	//	
	//}



	void ObjectOperator::drawObjects()
	{

			for(int typeIdx = 0; typeIdx < typeIndices.size(); typeIdx++)
			{
				// inicjalizacja rysowania
				if( objectArray[ typeIndices[typeIdx].firstIndex ].InitDrawing() )
				{

					// rysowanie wszystkich obiektow danego typu
					for(int objectIdx = typeIndices[typeIdx].firstIndex; 
						objectIdx <= typeIndices[typeIdx].lastIndex; 
						objectIdx++)
					{
						objectArray[objectIdx].Draw();
					}

					// "sprzatanie" po rysowaniu obiektow
					objectArray[ typeIndices[typeIdx].lastIndex ].CloseDrawing();

				}
			}

	}



	//void ObjectOperator::removeObjects()
	//{
	//	if( !movingID.empty() )
	//	{

	//		for(int i = 0; i < movingID.size(); i++)
	//		{
	//			// odpalenie funkcji "sprzatajacej po obiekcie

	//				objectArray[movingID[i]].Remove(); 

	//			// teraz mozna obiekt wywalic z tablicy

	//				objectArray.erase(objectArray.begin() + movingID[i]);

	//			// obsuniecie ID'kow wyzszych od obecnie wywalonego

	//				for(int j = 0; j < movingID.size(); j++)
	//				{
	//					if(movingID[j] > movingID[i])
	//						movingID[j]--;
	//				}

	//		}

	//		movingID.erase(movingID.begin(), movingID.end());

	//		Stickied = false;

	//		deletePressed = false;

	//	// odswiezenie tablicy indeksow

	//		Sort();

	//	}  // !movingID.empty()
	//	else
	//	{
	//		MessageBox(g_hwnd, "Probowano uruchomic funkcje removeObjects(), pomimo faktu, ze tablica movingID jest pusta!", "Error!", MB_OK);
	//		deletePressed = false;
	//	}
	//}




	//void ObjectOperator::adjustObjectsHeight(float modifier)
	//{
	//	for(int i = 0; i < objectArray.size(); i++)
	//		objectArray[i].AdjustHeight(modifier);
	//}

	//void ObjectOperator::rescaleObjects(float newScale, float oldScale)
	//{

	//	for(int i = 0; i < objectArray.size(); i++)
	//		objectArray[i].Rescale(newScale, oldScale);

	//}
	//bool ObjectOperator::removeSpecificObjects(std::string typeName)
	//{

	//	if( !objectArray.empty() )
	//	{
	//		bool Found = false; // okresla czy znaleziono obiekt danego typu, w wektorze obiektow
	//		std::vector <int> indexArray;

	//		
	//		// usuniecie specyficznych obiektow z aktualnie poruszanych obiektow
	//		for(int i = 0; i < objectArray.size(); i++)
	//		{
	//			if( objectArray[i].GetType() == typeName )
	//			{
	//				// zapamietanie indeksu na przyszla (wlasciwa) petle usuwajaca
	//				indexArray.push_back(i);

	//				
	//				// Sprawdzenie, czy ktores z obiektow obecnie usuwanego typu, bylo aktualnie zaznaczone.
	//				// Jesli tak, to trzeba usunac ID obiektu z tablicy movingID oraz zrobic obsuniecie wyzszych ID.

	//				for(int j = 0; j < movingID.size(); j++)
	//				{
	//					if( movingID[j] == i )
	//						movingID.erase(movingID.begin() + j--);
	//					
	//				}

	//			} // if ( objectArray[i].GetType() == typeName )
	//		}


	//		if( !indexArray.empty() )
	//		{
	//			// wlasciwe usuwanie obiektow z wektora obiektow
	//			for(int i = 0; i < indexArray.size(); i++)
	//			{
	//				// sprzatanie po obiekcie
	//				objectArray[indexArray[i]].Remove();


	//				// obsuniecie ID w 4
	//				for(int j = 0; j < movingID.size(); j++)
	//				{
	//					if( movingID[j] > indexArray[i] )
	//						movingID[j]--;
	//				}


	//				// wlasciwe usuniecie obiektu
	//				objectArray.erase(objectArray.begin() + indexArray[i]);

	//				for(int j = i+1; j < indexArray.size(); j++)
	//					indexArray[j]--;

	//			}

	//			// jezeli, w efekcie usuwania okreslonych obiektow, movingID zostalo wyczyszczone, 
	//			// nalezy ustawic flage Stickied na FALSE
	//			if( movingID.empty() )
	//				Stickied = false;

	//			Found = true;

	//		// odswiezenie tablicy indeksow

	//			Sort();
	//		}

	//		Saved = false;

	//		return Found;
	//	}
	//	else
	//		return false;
	//}





	//void ObjectOperator::manageKeys()
	//{
	//	if( keyPressed != NULL )
	//	{
	//		for(int i = 0; i < movingID.size(); i++)
	//		{
	//			objectArray[movingID[i]].ManageKey(keyPressed);
	//		}

	//		keyPressed = NULL;
	//	}
	//}





	//void ObjectOperator::morphAdjustment()
	//{
	//	for(int i = 0; i < objectArray.size(); i++)
	//	{
	//		if(objectArray[i].GetMorphAdjustable())
	//			objectArray[i].MorphAdjust();
	//	}
	//}





	void ObjectOperator::executeSpecificFunctions(std::string typeName)
	{
		for(int i = 0; i < objectArray.size(); i++)
		{
			if( typeName == objectArray[i].GetType() )
			{
				objectArray[i].SpecificFunction();
			}
		}
	}



	void ObjectOperator::Clear(std::string name)
	{
	// wyczyszczenie wektora aktualnie zaznaczonych/przemieszczanych obiektow

		if( movingID.size() )
		{
			movingID.erase( movingID.begin(), movingID.end() );
		}

	// wyczyszczenie wektora obiektow

		if( objectArray.size() )
		{
			for(int i = 0; i < objectArray.size(); i++)
			{
				if( name == "All" || name == objectArray[i].GetType() )
				{
					objectArray[i].Remove();
				}
			}

			objectArray.erase( objectArray.begin(), objectArray.end() );
		}

	// odswiezenie wektora indeksow

		Sort();
	}
//
/*****************************************************************
	Glowna funkcja
*****************************************************************/

	void ObjectOperator::ObjectOperatorRun()
	{
	//	moveObjects();
		drawObjects(); 
	

	// jezeli uzytkownik puscil klawisz myszy, mozna ponownie oczekiwac na wybieranie nowego obiektu

		/*if( lButtonDown == false && scanReady )
		{
			Selecting = true;
		}*/

	// jezeli jest odpowiedni tryb pracy edytora ( scanReady ustawiane w editRun() ),
	// mozna skanowac obszar, czy zaznaczono obiekt etc.

		/*if( scanReady && lButtonDown && Selecting )
		{
			objectScan();
		}*/

		/*if( scanReady && Stickied && deletePressed )
		{
			removeObjects();
		}


		manageKeys();*/
	}
//
/*****************************************************************
	Konstruktor, destruktor ObjectOperator
*****************************************************************/

	ObjectOperator::ObjectOperator(): heightModifier(0.f), heightModifierSpeed(.5f),
									  scanReady(true), Stickied(false), Selecting(false),
									  keyPressed(NULL)
	{}

	ObjectOperator::~ObjectOperator()
	{}