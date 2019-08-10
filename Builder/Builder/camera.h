#pragma once

	//#include "Vector3.h"

	
	#ifndef PI
	#define PI          3.14159
	#endif


//wykorzystuje OpenGL (przy rotacjach etc.)

	class Camera
	{
		//domyslny konstruktor Vector3 ustawia x,y,z na '0'

			Vector3 cameraPos, oldPos, 
					focusPos, oldFocus,   // < -- na podstawie tego wektora bedzie dokonywana translacja
					orientation;
			
			float   minX, maxX,  // ograniczniki pozycji kamery
					minZ, maxZ,
				
					angleXZ,  //katy, wokol ktorych bedzie sie obracac kamera
					oldAngleXZ,

					zoom,     //atrybuty zooma
					zoomSpeed,
					maxZoom,
					minZoom,
					
					rotationSpeed,
					
					pitch,
					oldPitch,

					minPitch,
					maxPitch,
					pitchSensitivity,
					
					movementSpeed;
				
		public:
		
			Camera(): angleXZ(0.f), oldAngleXZ(0.f), zoom(0.f), pitch(0.f), oldPitch(0.f)//, angleY(0.0f)
			{}


			void SetFocusPosition(float x, float y, float z);
			void SetOrientation(float x, float y, float z);

			void RescaleCamera(float scale);

			Vector3 GetCameraPos();

			void SetZoom(float zoom);
			void SetZoomSpeed(float speed);
			void SetMaxZoom(float max);
			void SetMinZoom(float min);
			void ZoomIn();
			void ZoomOut();

			
			void SetRotationSpeed(float speed);
			void RotateLeft();
			void RotateRight();


			void SetPitchSensitivity(float sens);
			void SetMaxPitch(float max);
			void SetMinPitch(float min);
			void PitchUp();
			void PitchDown();


			void SetMovementSpeed(float mov);
			void GoForward();
			void GoBackward();
			void GoLeft();
			void GoRight();


			void SetBorders(float minx, float maxx, float minz, float maxz)
			{ minX = minx; maxX = maxx; minZ = minz;  maxZ = maxz; }


			void run()
			{

			// zebezpieczenie

				oldPos.posX = cameraPos.posX,
				oldPos.posY = cameraPos.posY,
				oldPos.posZ = cameraPos.posZ;

			// przeksztalcenie

				float radians =  float(PI * (angleXZ-90.0f)/180.0f);

				cameraPos.posX = focusPos.posX + ( (float)sin(radians)*zoom ); 
				cameraPos.posZ = focusPos.posZ + ( (float)cos(radians)*zoom );

				radians =  float(PI * (pitch-90.0f)/180.0f);

				cameraPos.posY = focusPos.posY + (float)tan(-radians)*zoom;


			// sprawdzenie czy kamera wyszla za granice

				if( cameraPos.posX < minX || cameraPos.posX > maxX ||
				    cameraPos.posZ < minZ || cameraPos.posZ > maxZ )
				{
					cameraPos.posX = oldPos.posX;
					cameraPos.posY = oldPos.posY;
					cameraPos.posZ = oldPos.posZ;

					focusPos.posX = oldFocus.posX;
					focusPos.posY = oldFocus.posY;
					focusPos.posZ = oldFocus.posZ;

					angleXZ = oldAngleXZ;

					pitch = oldPitch;
				}

			// ustawienie kamery w opengl

				gluLookAt(cameraPos.posX, cameraPos.posY, cameraPos.posZ, 
						  focusPos.posX, focusPos.posY, focusPos.posZ,
						  orientation.posX, orientation.posY, orientation.posZ);
			}

	};






	void Camera::SetFocusPosition(float x, float y, float z)
	{ focusPos.posX = x; focusPos.posY = y; focusPos.posZ = z; }

	void Camera::SetOrientation(float x, float y, float z) 
	{ orientation.posX = x; orientation.posY = y; orientation.posZ = z; }



	void Camera::RescaleCamera(float scale)
	{
			zoomSpeed *= scale;
			minZoom   *= scale;
			maxZoom   *= scale;

			movementSpeed *= scale;
	}



	Vector3 Camera::GetCameraPos()
	{
		Vector3 wektor;

		wektor.posX = cameraPos.posX;
		wektor.posY = cameraPos.posY;
		wektor.posZ = cameraPos.posZ;

		return wektor;
	}

	void Camera::SetZoom(float zoom) { Camera::zoom = zoom; }
	void Camera::SetZoomSpeed(float speed) { zoomSpeed = speed; }
	void Camera::SetMaxZoom(float max)   { maxZoom = max; }
	void Camera::SetMinZoom(float min)   { minZoom = min; }
	void Camera::ZoomIn()  
	{
		if( zoom >= minZoom ) 
			zoom -= zoomSpeed; 
	}
	void Camera::ZoomOut() 
	{ 
		if(zoom <= maxZoom) 
			zoom += zoomSpeed; 
	}




	void Camera::SetRotationSpeed(float speed) { rotationSpeed = speed; }
	void Camera::RotateLeft()
	{
		oldAngleXZ = angleXZ;

		angleXZ -= rotationSpeed;
		if(angleXZ < 0.f)
		   angleXZ = 365.f;
		
	}
	void Camera::RotateRight()
	{
		oldAngleXZ = angleXZ;

		angleXZ += rotationSpeed;
		if(angleXZ > 365.f)
		   angleXZ = 0.f;
	}



	void Camera::SetPitchSensitivity(float sens)  {  pitchSensitivity = sens; }
	void Camera::SetMaxPitch(float max)           {  pitch = maxPitch = max;  }
	void Camera::SetMinPitch(float min)           {  minPitch = min;  }
	void Camera::PitchUp()						  
	{  
		oldPitch = pitch;

		if( pitch >= minPitch ) 
			pitch -= pitchSensitivity;  
	}
	void Camera::PitchDown()					  
	{  
		oldPitch = pitch;

		if( pitch <= maxPitch ) 
			pitch += pitchSensitivity;  
	}




	void Camera::SetMovementSpeed(float mov)  { movementSpeed = mov; }
	void Camera::GoForward()
	{
		float radians =  float(PI*(angleXZ-90.0f)/180.0f);

		oldFocus.posX = focusPos.posX;
		oldFocus.posY = focusPos.posY;
		oldFocus.posZ = focusPos.posZ;

		focusPos.posX -= (float)sin(radians)*movementSpeed; 
		focusPos.posZ -= (float)cos(radians)*movementSpeed; 
		
	}
	void Camera::GoBackward()
	{
		float radians =  float(PI*(angleXZ-90.0f)/180.0f);

		oldFocus.posX = focusPos.posX;
		oldFocus.posY = focusPos.posY;
		oldFocus.posZ = focusPos.posZ;

		focusPos.posX  -= -( (float)sin(radians)*movementSpeed ); 
		focusPos.posZ  -= -( (float)cos(radians)*movementSpeed ); 

	}
	void Camera::GoLeft()
	{
	   angleXZ += 90.f;

	   GoForward();

	   angleXZ -= 90.f;
	}
	void Camera::GoRight()
	{
	   angleXZ -= 90.f;

	   GoForward();

	   angleXZ += 90.f;
	}

