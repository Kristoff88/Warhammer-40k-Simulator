#pragma once

	#include "camera.h"
	Camera kamera;


	void InitCameraOperator()
	{
		kamera.SetOrientation(0.0, 1.0, 0.0);
		kamera.SetFocusPosition((MAP_X*MAP_SCALE)/2.0,  terrainHeight + 10.f, -(MAP_Z*MAP_SCALE)/2.0);


		kamera.SetZoom(100.f);
		kamera.SetMaxZoom((OVERALL_MAX_HEIGHT/2)*MAP_SCALE);
		kamera.SetMinZoom(5);
		kamera.SetZoomSpeed(4.f);

		kamera.SetRotationSpeed(5.f);

		kamera.SetPitchSensitivity(1.5f);
		kamera.SetMaxPitch(90.f);
		kamera.SetMinPitch(45.f);

		kamera.SetMovementSpeed(3.f);

		kamera.SetBorders(-250*MAP_SCALE, 250 + MAP_X*MAP_SCALE, -250 -MAP_Z*MAP_SCALE, 250*MAP_SCALE);
	}



	void cameraRun()
	{
	// w przypadku przemieszczania obiektu, scroll bedzie mial wplyw na jego wysokosc, a nie na kamere

		if( (!lButtonDown && !rButtonDown) && !ctrlPressed  ) 
		{
			if( wheelMovement )
			{
				if( wheelMovement < 0 )
					for(int i = 0; i > mouseWheelSteps; i--)
						kamera.ZoomOut();

				if( wheelMovement > 0 )
					for(int i = 0; i < mouseWheelSteps; i++)
						kamera.ZoomIn();

				wheelMovement %= WHEEL_DELTA;

				cameraPosChanged = mouseMoved = true;
			}
		}

	// uzytkownik obraca kamera, po wcisnieciu ALT'a

		if( altPressed )
			cameraRotate = true;

	//cameraRotate == true, jesli ALT jest wcisniety

		if( cameraRotate )
		{
			if ( (g_mouseX - oldMouseX) > 2 )       // mysz przesuniêta w prawo
				kamera.RotateLeft();
			else if( (g_mouseX - oldMouseX) < -2 )   // mysz przesuniêta w lewo
				kamera.RotateRight();

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