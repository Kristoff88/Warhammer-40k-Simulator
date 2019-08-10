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

				oldPos.x = cameraPos.x,
				oldPos.y = cameraPos.y,
				oldPos.z = cameraPos.z;

			// przeksztalcenie

				float radians =  float(PI * (angleXZ-90.0f)/180.0f);

				cameraPos.x = focusPos.x + ( (float)sin(radians)*zoom ); 
				cameraPos.z = focusPos.z + ( (float)cos(radians)*zoom );

				radians =  float(PI * (pitch-90.0f)/180.0f);

				cameraPos.y = focusPos.y + (float)tan(-radians)*zoom;


			// sprawdzenie czy kamera wyszla za granice

				if( cameraPos.x < minX || cameraPos.x > maxX ||
				    cameraPos.z < minZ || cameraPos.z > maxZ )
				{
					cameraPos = oldPos;
					focusPos = oldFocus;

					angleXZ = oldAngleXZ;
					pitch = oldPitch;
				}

			// ustawienie kamery w opengl

				gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, 
						  focusPos.x, focusPos.y, focusPos.z,
						  orientation.x, orientation.y, orientation.z);
			}

	};






	void Camera::SetFocusPosition(float x, float y, float z)
	{ 
		focusPos.x = x; 
		focusPos.y = y; 
		focusPos.z = z; 
	}

	void Camera::SetOrientation(float x, float y, float z) 
	{ 
		orientation.x = x; 
		orientation.y = y; 
		orientation.z = z; 
	}



	void Camera::RescaleCamera(float scale)
	{
		zoomSpeed *= scale;
		minZoom   *= scale;
		maxZoom   *= scale;

		movementSpeed *= scale;
	}


	Vector3 Camera::GetCameraPos()
	{
		return cameraPos;
	}

	void Camera::SetZoom(float zoom) { Camera::zoom = zoom; }
	void Camera::SetZoomSpeed(float speed) { zoomSpeed = speed; }
	void Camera::SetMaxZoom(float max)   { maxZoom = max; }
	void Camera::SetMinZoom(float min)   { minZoom = min; }
	void Camera::ZoomIn()  
	{
		if( zoom >= minZoom ) 
		{
			zoom -= zoomSpeed; 
		}
	}
	void Camera::ZoomOut() 
	{ 
		if(zoom <= maxZoom) 
		{
			zoom += zoomSpeed; 
		}
	}




	void Camera::SetRotationSpeed(float speed) 
	{ 
		rotationSpeed = speed; 
	}

	void Camera::RotateLeft()
	{
		oldAngleXZ = angleXZ;

		angleXZ -= rotationSpeed;

		if(angleXZ < 0.f)
		{
		   angleXZ = 365.f;
		}
		
	}
	void Camera::RotateRight()
	{
		oldAngleXZ = angleXZ;

		angleXZ += rotationSpeed;

		if(angleXZ > 365.f)
		{
		   angleXZ = 0.f;
		}
	}

	void Camera::SetPitchSensitivity(float sens)  
	{  
		pitchSensitivity = sens; 
	}

	void Camera::SetMaxPitch(float max)           
	{  
		pitch = maxPitch = max;  
	}

	void Camera::SetMinPitch(float min)           
	{  
		minPitch = min;  
	}

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

	void Camera::SetMovementSpeed(float mov)  
	{ 
		movementSpeed = mov; 
	}

	void Camera::GoForward()
	{
		float radians =  float(PI*(angleXZ-90.0f)/180.0f);

		oldFocus.x = focusPos.x;
		oldFocus.y = focusPos.y;
		oldFocus.z = focusPos.z;

		focusPos.x -= (float)sin(radians)*movementSpeed; 
		focusPos.z -= (float)cos(radians)*movementSpeed; 
		
	}
	void Camera::GoBackward()
	{
		float radians =  float(PI*(angleXZ-90.0f)/180.0f);

		oldFocus.x = focusPos.x;
		oldFocus.y = focusPos.y;
		oldFocus.z = focusPos.z;

		focusPos.x  -= -( (float)sin(radians)*movementSpeed ); 
		focusPos.z  -= -( (float)cos(radians)*movementSpeed ); 

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

