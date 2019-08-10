#pragma once


	struct liquidObject
	{

			unsigned int  textureID;

			float		  alpha,

						// poziom wody
						  minLevel, maxLevel, 

					   // predkosc opadania/wznoszenia sie wody
						  speed,
						  
						// amplituda fal
						  waveLevel, 
						  
					    // czestotliwosc przesuwania sie fal
						  waveFrequency, 
						  
					    // rozciagniecie fal
						  waveStretch,

					   // poruszanie sie fal wzdlu X i Z

						  movementX,	// okreslaja stopien przemieszczenia (ktory sie resetuje co jakis czas)
						  movementZ,

						  directionX,	// okreslaja kierunki
						  directionZ;

			GLuint		  *indexArray;
			unsigned int  indexCounter,  // licznik indeksow powyzszych tablic

						  VBOv,
						  VBOvt,
						  VBOvn,
						  VBOvc;

			bool		  Rising;   	// poziom wody rosnie/spada (true/false)
						  


		// konstruktor, destruktor

			liquidObject(unsigned int ID, float alph, float min, float max, float waveLVL, float waveFREQ, float stretch, int dirX, int dirZ, float sped);
			~liquidObject();

	};