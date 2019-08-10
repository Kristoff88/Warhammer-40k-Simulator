#pragma once


	#include "camera.h"


	Camera kamera;



	void InitCameraOperator()
	{
		kamera.SetOrientation(0.0, 1.0, 0.0);
		kamera.SetFocusPosition(0, 1.f, 0);


		kamera.SetMaxZoom(7.5f);
		kamera.SetMinZoom(7.5f);
		kamera.SetZoom(7.5f);
		

		kamera.SetRotationSpeed(5.f);

		for(int i = 0; i < 25; i++)
			kamera.RotateRight();
		
		

		kamera.SetPitchSensitivity(1.5f);
		kamera.SetMaxPitch(90.f);
		kamera.SetMinPitch(45.f);

		kamera.SetMovementSpeed(2.5f);
		kamera.GoLeft();

		kamera.SetBorders(-250*MAP_SCALE, 250 + MAP_X*MAP_SCALE, -250 -MAP_Z*MAP_SCALE, 250*MAP_SCALE);
	}



	void cameraRun()
	{
	// w przypadku przemieszczania obiektu, scroll bedzie mial wplyw na jego wysokosc, a nie na kamere

		/*if( (!lButtonDown && !rButtonDown) && !ctrlPressed  ) 
		{*/
			if( wheelMovement )
			{
				if( wheelMovement < 0 )
				{
					for(int i = 0; i > mouseWheelSteps; i--)
						kamera.ZoomOut();
				}
				else if( wheelMovement > 0 )
				{
					for(int i = 0; i < mouseWheelSteps; i++)
						kamera.ZoomIn();
				}

				wheelMovement %= WHEEL_DELTA;
				cameraPosChanged = mouseMoved = true;
			}
		//}

	// uzytkownik obraca kamera, po wcisnieciu ALT'a

		if( altPressed )
			cameraRotate = true;

	//cameraRotate == true, jesli ALT jest wcisniety

		if( cameraRotate )
		{
			if ( (g_mouseX - oldMouseX) > 2 )       // mysz przesuniêta w prawo
			{
				kamera.GoRight();
				kamera.RotateLeft();
				kamera.GoLeft();
			}
			else if( (g_mouseX - oldMouseX) < -2 )   // mysz przesuniêta w lewo
			{
				kamera.GoRight();
				kamera.RotateRight();
				kamera.GoLeft();
			}

			if ( (g_mouseY - oldMouseY) > 2 )       // mysz przesuniêta w dol
				kamera.PitchUp();
			else if( (g_mouseY - oldMouseY) < -2 )   // mysz przesuniêta w gore
				kamera.PitchDown();
				

			cameraRotate = false;
			cameraPosChanged = mouseMoved = true;

			oldMouseX = g_mouseX;
			oldMouseY = g_mouseY;
		}
	

		kamera.run();
	}	