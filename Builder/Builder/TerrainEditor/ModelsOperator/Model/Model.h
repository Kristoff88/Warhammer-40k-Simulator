#pragma once



	struct Mesh
	{
		unsigned int indexCounter,
			         textureID;
	};


	class Model
	{
		public:

			unsigned int vboIDv,
						 vboIDvt,
						 vboIDvn;
						  
			std::vector <Mesh> meshVector;

			float sizeX, sizeY, sizeZ;

			float R, G, B, A,
				  scale;

			int	rotateX, rotateY, rotateZ;

			bool IsObstacle;

	/****************************************************************/

			Model();
			~Model();

			int GetIndexCount ();
	};