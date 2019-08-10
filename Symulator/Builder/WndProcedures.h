#pragma once

	#define KEY_LEFT	  ('<')
	#define KEY_RIGHT	  ('>')

	#define KEY_SPACE	  ('_')
	#define KEY_BACKSPACE ('/')

	#define KEY_0 ('0')
	#define KEY_1 ('1')
	#define KEY_2 ('2')
	#define KEY_3 ('3')
	#define KEY_4 ('4')
	#define KEY_5 ('5')
	#define KEY_6 ('6')
	#define KEY_7 ('7')
	#define KEY_8 ('8')
	#define KEY_9 ('9')

	#define KEY_A ('A')
	#define KEY_B ('B')
	#define KEY_C ('C')
	#define KEY_D ('D')
	#define KEY_E ('E')
	#define KEY_F ('F')
	#define KEY_G ('G')
	#define KEY_H ('H')
	#define KEY_I ('I')
	#define KEY_J ('J')
	#define KEY_K ('K')
	#define KEY_L ('L')
	#define KEY_M ('M')
	#define KEY_N ('N')
	#define KEY_O ('O')
	#define KEY_P ('P')
	#define KEY_R ('R')
	#define KEY_S ('S')
	#define KEY_T ('T')
	#define KEY_U ('U')
	#define KEY_W ('W')
	#define KEY_X ('X')
	#define KEY_Y ('Y')
	#define KEY_Z ('Z')




/*****************************************************************************
 WndProc()

 procedura okienkowa - dla glownego okna
*****************************************************************************/

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch(message)
	{
		case WM_CREATE:
		{
			return 0;
		}
		break;

		case WM_ACTIVATE:  // okno jest zwijane do ikony b¹dŸ rozwijane
		{
			if (!HIWORD(wParam))
			{
			// okno zosta³o rozwiniête
				g_isActive = TRUE;


				DWORD resolution;

				RECT clientArea;
				GetClientRect(hwnd, &clientArea);

				int   width  = (clientArea.right  - clientArea.left),
					  height = (clientArea.bottom - clientArea.top);

				resolution = height << 16 | width;

				SendMessageA(hwnd, WM_SIZE, 0, resolution);
			}
			else
			{
			// okno zosta³o zwiniête do ikony
				g_isActive=FALSE;
			}

			return 0;
		}

		case WM_SYSCOMMAND:  // wygaszacz ekranu b¹dŸ tryb oszczêdny monitora
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:     // uruchamiany jest wygaszacz ekranu 
				case SC_MONITORPOWER:   // monitor prze³¹cza siê w tryb oszczêdny
				// zwrócenie wartoœci 0 zapobiega obu akcjom
				return 0;

				default:
				break;
			}
		} 
		break;

		case WM_COMMAND:
		{
			// obsluga menu
			switch(LOWORD(wParam))
			{
				default:
				break;
			}
		}
		break;

		case WM_CLOSE:    // okno jest zamykane
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_SIZE:
		{
			ResizeScene(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}

		case WM_KEYDOWN:
		{
			switch(toupper(wParam))
			{
				case VK_ESCAPE:
				{
					int result = MessageBox(NULL, "Restart simulator?", "Confirmation...", MB_YESNO);

					if( result == IDYES )
					{
						MiniOperator::Singleton().battleForceManager.Reset();
					}
				}
				break;

				case VK_CONTROL:
					ctrlPressed = true;
				break;


				case VK_DELETE:
					deletePressed = true;
				break;

				case VK_LEFT:
				{
					MiniOperator::Singleton().input.KeyDown(KEY_LEFT);
				}
				break;

				case VK_RIGHT:
				{
					MiniOperator::Singleton().input.KeyDown(KEY_RIGHT);
				}
				break;


				
				case VK_BACK:
				{
					MiniOperator::Singleton().input.KeyDown(KEY_BACKSPACE);
				}
				break;

				case VK_SPACE:
				{
					MiniOperator::Singleton().input.KeyDown(KEY_SPACE);
				}
				break;


				case '0':	MiniOperator::Singleton().input.KeyDown(KEY_0);		break;
				case '1':	MiniOperator::Singleton().input.KeyDown(KEY_1);		break;
				case '2':	MiniOperator::Singleton().input.KeyDown(KEY_2);		break;
				case '3':	MiniOperator::Singleton().input.KeyDown(KEY_3);		break;
				case '4':	MiniOperator::Singleton().input.KeyDown(KEY_4);		break;
				case '5':	MiniOperator::Singleton().input.KeyDown(KEY_5);		break;
				case '6':	MiniOperator::Singleton().input.KeyDown(KEY_6);		break;
				case '7':	MiniOperator::Singleton().input.KeyDown(KEY_7);		break;
				case '8':	MiniOperator::Singleton().input.KeyDown(KEY_8);		break;
				case '9':	MiniOperator::Singleton().input.KeyDown(KEY_9);		break;


				case 'B':	MiniOperator::Singleton().input.KeyDown(KEY_B);		break;
				case 'C':	MiniOperator::Singleton().input.KeyDown(KEY_C);		break;
				
				case 'E':	MiniOperator::Singleton().input.KeyDown(KEY_E);		break;
				case 'F':	MiniOperator::Singleton().input.KeyDown(KEY_F);		break;
				case 'G':	MiniOperator::Singleton().input.KeyDown(KEY_G);		break;
				case 'H':	MiniOperator::Singleton().input.KeyDown(KEY_H);		break;
				case 'I':	MiniOperator::Singleton().input.KeyDown(KEY_I);		break;
				case 'J':	MiniOperator::Singleton().input.KeyDown(KEY_J);		break;
				case 'K':	MiniOperator::Singleton().input.KeyDown(KEY_K);		break;
				case 'L':	MiniOperator::Singleton().input.KeyDown(KEY_L);		break;
				case 'M':	MiniOperator::Singleton().input.KeyDown(KEY_M);		break;
				case 'N':	MiniOperator::Singleton().input.KeyDown(KEY_N);		break;
				case 'O':	MiniOperator::Singleton().input.KeyDown(KEY_O);		break;
				case 'P':	MiniOperator::Singleton().input.KeyDown(KEY_P);		break;
				case 'R':	MiniOperator::Singleton().input.KeyDown(KEY_R);		break;
				
				case 'T':	MiniOperator::Singleton().input.KeyDown(KEY_T);		break;
				case 'U':	MiniOperator::Singleton().input.KeyDown(KEY_U);		break;

				case 'X':	MiniOperator::Singleton().input.KeyDown(KEY_X);		break;
				case 'Y':	MiniOperator::Singleton().input.KeyDown(KEY_Y);		break;
				case 'Z':	MiniOperator::Singleton().input.KeyDown(KEY_Z);		break;



				case 'W':	
				{
					if( GUI::Singleton().Controls.GetCurrentFocus() )
					{
						MiniOperator::Singleton().input.KeyDown(KEY_W);		
					}
					else
					{
						kamera.GoForward();
						cameraPosChanged = true;
					}
				}
				break;

				case 'S':	
				{
					if( GUI::Singleton().Controls.GetCurrentFocus() )
					{
						MiniOperator::Singleton().input.KeyDown(KEY_S);		
					}
					else
					{
						kamera.GoBackward();
						cameraPosChanged = true;
					}	
				}
				break;

				case 'A':	
				{
					if( GUI::Singleton().Controls.GetCurrentFocus() )
					{
						MiniOperator::Singleton().input.KeyDown(KEY_A);		
					}
					else
					{
						kamera.GoLeft();
						cameraPosChanged = true;
					}	
				}
				break;

				case 'D':	
				{
					if( GUI::Singleton().Controls.GetCurrentFocus() )
					{
						MiniOperator::Singleton().input.KeyDown(KEY_D);		
					}
					else
					{
						kamera.GoRight();
						cameraPosChanged = true;
					}
				}
				break;


				default:					  
				break;
			}

		}
		break;

		case WM_KEYUP:
		{
			switch(toupper(wParam))
			{
				case VK_CONTROL:
					ctrlPressed = false;
				break;

				case VK_DELETE:
					deletePressed = false;
				break;
			}
		}
		break;



		case WM_LBUTTONDOWN:
		{
			MiniOperator::Singleton().input.LeftMouseDown();

			lButtonDown = true;

			if( mouseMoved )
				mouseMoved = false;

			return 0; 
		}
		case WM_LBUTTONUP:
		{
			MiniOperator::Singleton().input.LeftMouseUp();

			lButtonDown = false;
			return 0;
		} 

		case WM_RBUTTONDOWN:
		{
			MiniOperator::Singleton().input.RightMouseDown();

			rButtonDown = true;

			if( mouseMoved )
				mouseMoved = false;

			return 0;
		}
		case WM_RBUTTONUP:
		{
			MiniOperator::Singleton().input.RightMouseUp();

			rButtonDown = false;
			return 0;  
		}

		case WM_SYSKEYDOWN:
		{
			altPressed = true;
			return 0;
		}
		case WM_SYSKEYUP:
		{
			altPressed = false;
			return 0;
		}


		case WM_MOUSEWHEEL:
		{
			wheelMovement = GET_WHEEL_DELTA_WPARAM(wParam);
			mouseWheelSteps = wheelMovement / WHEEL_DELTA;	// < --- dla terrain editor'a

			objectOperator.keyPressed = 'L';	// ctrl + scroll daje mozliwosc skalowania zaznaczonego modelu

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			mouseMoved = true;

		// zapamiêtuje poprzednie wspó³rzêdne myszy

			oldMouseX = g_mouseX;
			oldMouseY = g_mouseY;

		// pobiera bie¿¹ce wspó³rzêdne myszy

			g_mouseX = LOWORD(lParam);
			g_mouseY = HIWORD(lParam);

			

			POINT cursor;

			cursor.x = g_mouseX;
			cursor.y = g_mouseY;
			//cursor.y = -g_mouseY + SCREEN_HEIGHT;

			miniOperator.input.MoveCursor(cursor);


		// pierdolki kontrolne

			/*std::stringstream strum; 
			std::string       wiersz;*/

			//Vector3 lol;

			//lol = GetOGLPos(g_mouseX, g_mouseY);

			//strum << "X:_" << lol.posX << "_Y:_" << lol.posY << "_Z:_" << lol.posZ;
			//strum >> wiersz;
			//strum.clear();
			//SetWindowText(hwnd, wiersz.c_str());

			return 0;
		}

		//w momencie opuszczenia client area (minimalizacja, zmiana okna etc.)
		case WM_NCMOUSELEAVE:

			altPressed = lButtonDown = rButtonDown = false;


			RECT rcClient;
			GetClientRect(hwnd, &rcClient);
			g_mouseX = rcClient.right  /2.f;
			g_mouseY = rcClient.bottom /2.f;

		break;

		default:
		break;

	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
} // WndProc()

