#include "GlobalHeaders.h"

#include "crossProduct.h"



	void crossProduct(float point1[3], float point2[3], float point3[3], float normal[3])
	{
		float vector1[3], vector2[3];

		
		vector1[0] = point1[0] - point2[0];
		vector1[1] = point1[1] - point2[1];
		vector1[2] = point1[2] - point2[2];


		vector2[0] = point2[0] - point3[0];
		vector2[1] = point2[1] - point3[1];
		vector2[2] = point2[2] - point3[2];


		normal[0] = vector1[1]*vector2[2] - vector1[2]*vector2[2];
		normal[1] = vector1[2]*vector2[0] - vector1[0]*vector2[2];
		normal[2] = vector1[0]*vector2[1] - vector1[1]*vector2[0];
	}