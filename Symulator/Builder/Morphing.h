//#pragma once
//
//
//
//// Zmienne
//
//
//	HWND	morphing = NULL;
//
//
//	float   morphingSpeed = 10.0f,
//			morphRandGradient = 5.f;
//
//	int		morphRangeX = 1,
//			morphRangeZ = 1;
//			
//	bool	morphRandom = false;  // < -- okresla czy morphowanie terenu jest rowne czy sa randomowe odchyly
//
//
//	std::string morphType;		// do niego wczytywany jest typ obecnego typu morphowania
//
//	bool    lMorph = false,  // modyfikowanie terenu poprzez nacisniecie lewego przycisku myszy
//			rMorph = false;  // modyfikowanie terenu poprzez nacisniecie prawego przycisku myszy
//
//
//// Funkcje
//
//
//
//	void updateMorphing()
//	{
//		morphingSpeed =     ( GetDlgItemInt(morphing, IDC_SPEED_EDIT, FALSE, FALSE) / 100.f ) * (float) MAP_SCALE;
//		morphRangeX =         GetDlgItemInt(morphing, IDC_X_EDIT, FALSE, FALSE);
//		morphRangeZ =         GetDlgItemInt(morphing, IDC_Z_EDIT, FALSE, FALSE);
//		morphRandGradient = ( GetDlgItemInt(morphing, IDC_RAND_EDIT,  FALSE, FALSE) / 100.f ) * (float) MAP_SCALE;
//
//		MAX_TERRAIN_HEIGHT = GetDlgItemInt(terrain, IDC_TERRAIN_MAX_HEIGHT, FALSE, FALSE);
//		MIN_TERRAIN_HEIGHT = GetDlgItemInt(terrain, IDC_TERRAIN_MIN_HEIGHT, FALSE, FALSE); 
//
//	// pobranie typu morphingu z combobox'a
//
//		char buffer[MAX_PATH];
//
//		ComboBox_GetText( GetDlgItem(morphing, IDC_TYPE_COMBO), buffer, MAX_PATH );
//
//		morphType = buffer;
//	}
//
//
//	void morphingText()
//	{
//		// wyczyszczenie wektora zawierajacego tekst
//
//			textOutput.erase( textOutput.begin(), textOutput.end() );
//
//		// tablice, ktore beda zawierac informacje
//
//			static char CurrentAction[50];
//			static char CurrentMorphType[50];
//			static char CurrentMorphX[50];
//			static char CurrentMorphZ[50];
//			static char CurrentMorphModifier[50];
//
//			sprintf_s( CurrentAction, "Action : Morphing land" ); 
//			textOutput.push_back( CurrentAction );
//
//			sprintf_s( CurrentMorphType, "Morph type: %s", morphType.c_str() );
//			textOutput.push_back( CurrentMorphType );
//
//			sprintf_s( CurrentMorphX, "Size X: %i", morphRangeX );
//			textOutput.push_back( CurrentMorphX );
//
//			sprintf_s( CurrentMorphZ, "Size Z: %i", morphRangeZ );
//			textOutput.push_back( CurrentMorphZ );
//
//			sprintf_s( CurrentMorphModifier, "Morph speed: %8.2f", morphingSpeed );
//			textOutput.push_back( CurrentMorphModifier );
//		
//	}
//	void landMorph()
//	{
//
//			updateMorphing();
//
//
//		// obsluga modulacji szybkosci morphingu
//
//			if(wheelMovement < 0)
//			{
//				int additive = 0;
//				for(int i = 0; i > mouseWheelSteps; i--)
//					additive -= MAP_SCALE;
//
//			// zabezpieczenie
//
//				while( (morphingSpeed/MAP_SCALE)*100.f + additive < 0 )
//				{
//					additive++;
//				}
//
//				SetDlgItemInt(morphing, IDC_SPEED_EDIT, GetDlgItemInt(morphing, IDC_SPEED_EDIT, FALSE, FALSE) + additive, FALSE);
//
//				wheelMovement %= WHEEL_DELTA;
//			}
//			else if(wheelMovement > 0)
//			{
//				int additive = 0;
//				for(int i=0; i<mouseWheelSteps; i++)
//					additive += MAP_SCALE;
//
//				SetDlgItemInt(morphing, IDC_SPEED_EDIT, GetDlgItemInt(morphing, IDC_SPEED_EDIT, FALSE, FALSE) + additive, FALSE);
//
//				wheelMovement %= WHEEL_DELTA;
//			}
//
//
//			if( lButtonDown )
//			{
//				lMorph = true;
//			}
//			else if( rButtonDown )
//			{
//				rMorph =  true;
//			}
//			else
//			{
//				lMorph = rMorph = false; 
//				return;
//			}
//
//		// przerobienie koordynatow myszki na pozycje w przestrzenii opengl
//
//			int	X =  cursorX / MAP_SCALE,
//				Z = -cursorZ / MAP_SCALE;
//			
//		// ustawienie modyfikatora terenu w zaleznosci od przycisku myszy (rosnie/maleje)
//
//			float modifier;
//
//			float sign = 1;
//
//			if( rMorph )
//				sign  *= -1;
//
//		// type:
//		// 1 - Convex (wypukle - lewy klawisz)
//		// 1 - Concave (wklesle - prawy klawisz)
//		// 2 - Flat
//		// 3 - Simple
//
//		// przerobienie stringa na umownego inta (switch plumka sie przy lancuchach znakow)
//
//			unsigned int type;
//
//			if( morphType == "Convex" )
//			{
//				type = 1;
//			}
//			else if( morphType == "Concave" )
//			{
//				type = 1;
//				sign *= -1;
//			}
//			else if( morphType == "Flat" )
//			{
//				type = 2;
//			}
//			else if( morphType == "Simple" )
//			{
//				type = 3;
//			}
//			else
//			{
//				MessageBox(g_hwnd, "Niewlasciwa wartosc ComboBox'a IDC_TYPE_COMBO (typ morphowania).", "Error!", MB_OK);
//			}
//
//
//								  // + 1 zapobiega wyslaniu morphModifier'a o wartosci zerowej,
//			float	morphModifier = morphRandGradient + 1;   // jako prawego operanda dla operatora przyjmujacego funkcje rand()
//
//
//			switch( type )
//			{
//
//				// Convex / Concave
//				case 1:
//				{
//
//					float modifierX = 0,
//						  modifierZ = 0,
//
//						  incrementX = (PI/2.f)/morphRangeX,
//						  incrementZ = (PI/2.f)/morphRangeZ;
//
//				// morphing leci od 1 do MAP_X/MAP_Z - 1 zeby ne bylo widac dziur pod terenem na brzegach
//
//					int currentVertex;
//
//					for (int z = Z - morphRangeZ; z < Z + morphRangeZ; z++)
//					{
//
//						modifierX = 0;
//
//						for (int x = X - morphRangeX; x < X + morphRangeX; x++)
//						{
//							if( x >= 1 && x < MAP_X-1 && z >= 1 && z < MAP_Z-1 )
//							{
//								if( roundCursor )
//								{
//									float pX = (x - X),
//										  pZ = (z - Z),
//
//										  elipseEq = pow(pX, 2.f)/pow((float)(morphRangeX), 2.f)
//																	+
//													 pow(pZ, 2.f)/pow((float)(morphRangeZ), 2.f);
//
//									if( elipseEq <= 1 )
//									{
//										if( !morphRandom )
//										{
//											modifier = morphingSpeed * sign;
//
//											modifier *= sinf(modifierX) * sinf(modifierZ);
//										}
//										else
//										{
//											if(lMorph)
//												modifier = ( morphingSpeed + ( ( rand()%(int)morphModifier ) / (100.f*MAP_SCALE) ) ) * sign;
//											else if(rMorph)
//												modifier = ( morphingSpeed - ( ( rand()%(int)morphModifier ) / (100.f*MAP_SCALE) ) ) * sign;
//
//											modifier *= sinf(modifierX) * sinf(modifierZ);
//										}
//									}
//									else
//									{
//										modifier = 0;
//									}
//								}
//								else
//								{
//									if( !morphRandom )
//									{
//										modifier = morphingSpeed * sign;
//
//									//// obliczenie promienia jako przeciwprostokatnej w trojkacie prostakatnym wg. twierdzenia pitagorasa
//
//									//	float a = x - X,		// < -- fajny efekt kropelki :D
//									//		  b = z - Z;
//
//									//	if( a < 0 )
//									//		a *= -1;
//
//									//	if( b < 0 )
//									//		b *= -1;
//
//									//// promien aktualnego punktu
//
//									//	float pointRange = sqrtf( pow(a, 2) + pow(b, 2) );
//
//									//	modifier *= sinf(pointRange * (PI/2.f));
//
//										modifier *= sinf(modifierX) * sinf(modifierZ);
//									}
//									else
//									{
//										if(lMorph)
//											modifier = ( morphingSpeed + ( rand()%(int)(morphModifier*100) ) / 100.f / (100.f*MAP_SCALE) ) * sign;
//										else if(rMorph)
//											modifier = ( morphingSpeed - ( rand()%(int)(morphModifier*100) ) / 100.f / (100.f*MAP_SCALE) ) * sign;
//
//										modifier *= sinf(modifierX) * sinf(modifierZ);
//									}
//								}
//
//							// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³
//
//								currentVertex = x + MAP_X*z;
//
//							// umieszcza wartoœci w tablicy kolorów
//
//								g_terrain[currentVertex][1] += modifier;
//
//							// korygacja... 
//
//								if( g_terrain[currentVertex][1] > MAX_TERRAIN_HEIGHT )
//									g_terrain[currentVertex][1] = MAX_TERRAIN_HEIGHT;
//								else if( g_terrain[currentVertex][1] < MIN_TERRAIN_HEIGHT )
//									g_terrain[currentVertex][1] = MIN_TERRAIN_HEIGHT;
//							}
//
//							modifierX += incrementX;
//						}
//
//						modifierZ += incrementZ;
//					}
//
//				} // Convex / Concave
//				break;
//
//
//
//				// Flat
//				case 2:
//				{
//
//				// odnalezienie sredniej wysokosci na morphowanym terenie
//
//					float avaregeHeight = 0,
//						  divider = 0;
//
//					int currentVertex;
//
//					for (int z = Z - morphRangeZ; z < Z + morphRangeZ + 1; z++)
//					for (int x = X - morphRangeX; x < X + morphRangeX + 1; x++)
//					{
//						if( x >= 1 && x < MAP_X-1 && z >= 1 && z < MAP_Z-1 )
//						{
//							currentVertex = x + MAP_X*z;
//
//							if( roundCursor )
//							{
//								float pX = (x - X),
//									  pZ = (z - Z),
//
//									  elipseEq = pow(pX, 2.f)/pow((float)(morphRangeX), 2.f)
//																+
//												 pow(pZ, 2.f)/pow((float)(morphRangeZ), 2.f);
//
//								if( elipseEq <= 1 )
//								{
//									avaregeHeight += g_terrain[currentVertex][1];
//									divider += 1.f;
//								}
//							}
//							else
//							{
//								avaregeHeight += g_terrain[currentVertex][1];
//								divider += 1.f;
//							}
//						}
//					}
//
//					avaregeHeight /= divider;
//
//				// wyrownanie terenu
//
//					for (int z = Z - morphRangeZ + 1; z < Z + morphRangeZ; z++)
//					for (int x = X - morphRangeX + 1; x < X + morphRangeX; x++)
//					{
//						if( x >= 1 && x < MAP_X-1 && z >= 1 && z < MAP_Z-1 )
//						{
//							currentVertex = x + MAP_X*z;
//
//							if( roundCursor )
//							{
//								float pX = (x - X),
//									  pZ = (z - Z),
//
//									  elipseEq = pow(pX, 2.f)/pow((float)(morphRangeX), 2.f)
//																+
//												 pow(pZ, 2.f)/pow((float)(morphRangeZ), 2.f);
//
//								if( elipseEq <= 1 )
//								{
//									g_terrain[currentVertex][1] = avaregeHeight;
//								}
//							}
//							else
//							{
//								g_terrain[currentVertex][1] = avaregeHeight;
//							}
//						}
//					}
//
//				} // Flat
//				break;
//
//
//				// Simple
//				case 3:
//				{
//				// morphing leci od 1 do MAP_X/MAP_Z - 1 zeby ne bylo widac dziur pod terenem na brzegach
//
//					int currentVertex;
//
//					for (int z = Z - morphRangeZ; z < Z + morphRangeZ + 1; z++)
//					for (int x = X - morphRangeX; x < X + morphRangeX + 1; x++)
//					{
//						if( x >= 1 && x < MAP_X-1 && z >= 1 && z < MAP_Z-1 )
//						{
//							if( roundCursor )
//							{
//								float pX = (x - X),
//									  pZ = (z - Z),
//
//									  elipseEq = pow(pX, 2.f)/pow((float)(morphRangeX), 2.f)
//																+
//												 pow(pZ, 2.f)/pow((float)(morphRangeZ), 2.f);
//
//								if( elipseEq <= 1 )
//								{
//									if( !morphRandom )
//									{
//										modifier = morphingSpeed * sign;
//									}
//									else
//									{
//										if(lMorph)
//											modifier = ( morphingSpeed + ( ( rand()%(int)morphModifier ) / (100.f*MAP_SCALE) ) ) * sign;
//										else if(rMorph)
//											modifier = ( morphingSpeed - ( ( rand()%(int)morphModifier ) / (100.f*MAP_SCALE) ) ) * sign;
//									}
//								}
//								else
//								{
//									modifier = 0;
//								}
//							}
//							else
//							{
//								if( !morphRandom )
//								{
//									modifier = morphingSpeed * sign;
//								}
//								else
//								{
//									if(lMorph)
//										modifier = ( morphingSpeed + ( rand()%(int)(morphModifier*100) ) / 100.f / (100.f*MAP_SCALE) ) * sign;
//									else if(rMorph)
//										modifier = ( morphingSpeed - ( rand()%(int)(morphModifier*100) ) / 100.f / (100.f*MAP_SCALE) ) * sign;
//								}
//							}
//
//						// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³
//
//							currentVertex = x + MAP_X*z;
//
//						// umieszcza wartoœci w tablicy kolorów
//
//							g_terrain[currentVertex][1] += modifier;
//
//						// korygacja... 
//
//							if( g_terrain[currentVertex][1] > MAX_TERRAIN_HEIGHT )
//								g_terrain[currentVertex][1] = MAX_TERRAIN_HEIGHT;
//							else if( g_terrain[currentVertex][1] < MIN_TERRAIN_HEIGHT )
//								g_terrain[currentVertex][1] = MIN_TERRAIN_HEIGHT;
//						}
//					}
//
//				} // Simple
//				break;
//
//
//				default:
//				break;
//
//			} // switch( morphType )
//
//
//		// masa korygacji
//
//			// korygacja sciolki na terenie
//
//				adjustGrassHeight();
//
//			// korygacja detali
//
//				if( detailCounter )
//					objectOperator.executeSpecificFunctions(DETAIL_NAME);
//
//
//			// korygacja siatki
//
//				if( drawingNet )
//					computeNet();
//
//			// korygacja normali
//
//				computeNormals();
//
//			// korygacja pozycji kursora
//
//				computeCursor();
//
//
//		// korygacja VBO terenu
//
//			// czyszczenie VBO
//
//				glDeleteBuffersARB(1, &terrainVBOv);
//				glDeleteBuffersARB(1, &terrainVBOvt);
//				glDeleteBuffersARB(1, &terrainVBOvn);
//
//
//			// utworzenie nowych VBO
//
//				glGenBuffersARB( 1, &terrainVBOv );		
//
//				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOv );			
//				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_terrain,		GL_DYNAMIC_DRAW_ARB );
//
//				
//				glGenBuffersARB( 1, &terrainVBOvt );
//
//				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvt );
//				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*2*sizeof(float), g_texcoordArray, GL_DYNAMIC_DRAW_ARB );
//
//
//				glGenBuffersARB( 1, &terrainVBOvn );
//
//				glBindBufferARB( GL_ARRAY_BUFFER_ARB, terrainVBOvn );
//				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*3*sizeof(float), g_normals,		GL_DYNAMIC_DRAW_ARB );
//				
//
//				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
//
//
//
//				Saved = false;
//	}