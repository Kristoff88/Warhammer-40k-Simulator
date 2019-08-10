#include "GlobalHeaders.h"

#include "Rotate.h"



	void RotatePoint(
					 float xPos, float yPos, float zPos,  // punkt obrotu
					 float &x, float &y, float &z,		  // punkt obracany
					 float xAngle, float yAngle, float zAngle  // katy obrotu
					)
	{

			float X, Y, Z, radians;


			#ifndef PI
			#define PI 3.14159
			#endif
				  



			if( xAngle )
			{
				// przed przystapieniem do obliczen, nalezy przerobic kat na radiany

					radians = (float)(PI*(xAngle-90.f)/180.f);;

				// i rotowanie...

					Y = y,
					Z = z;

					y = yPos + (Y - yPos) * cosf(radians) + (Z - zPos) * sinf(radians);
					z = zPos + (Z - zPos) * cosf(radians) - (Y - yPos) * sinf(radians);

			}




			if( yAngle )
			{

					radians = (float)(PI*(yAngle-90.f)/180.f);;

					X = x,
					Z = z;

					x = xPos + (X - xPos) * cosf(radians) + (Z - zPos) * sinf(radians);
					z = zPos + (Z - zPos) * cosf(radians) - (X - xPos) * sinf(radians);

			}



			if( zAngle )
			{

					radians = (float)(PI*(zAngle-90.f)/180.f);;

					X = x,
					Y = y;

					x = xPos + (X - xPos) * cosf(radians) + (Y - yPos) * sinf(radians);
					y = yPos + (Y - yPos) * cosf(radians) - (X - xPos) * sinf(radians);

			}

	}