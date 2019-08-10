#pragma once



	class Vector3
	{
		public:	

			double x, y, z;

			Vector3();
			Vector3(double x, double y, double z);

	/******************************************************
			Dodatkowe funkcje
	******************************************************/

			bool   Equals	   (Vector3 const &arg);
			bool   NotEquals   (Vector3 const &arg);

			Vector3& Add       (Vector3 const &arg);
			Vector3& Substract (Vector3 const &arg);

			Vector3& Multiply  (Vector3 const &arg);
			Vector3& Divide    (Vector3 const &arg);

	/******************************************************
			Pomocnicze operatory
	******************************************************/

			bool   operator== (Vector3 const &arg);
			bool   operator!= (Vector3 const &arg);

			Vector3  operator+  (Vector3 const &arg) const;
			Vector3& operator+= (Vector3 const &arg);

			Vector3  operator-  (Vector3 const &arg) const;
			Vector3& operator-= (Vector3 const &arg);

			Vector3  operator*  (Vector3 const &arg) const;
			Vector3& operator*= (Vector3 const &arg);

			Vector3  operator/  (Vector3 const &arg) const;
			Vector3& operator/= (Vector3 const &arg);

			Vector3& operator=  (Vector3 const &arg);
			float  operator[] (int index)        throw( std::string );

	/******************************************************
				Konwertery
	******************************************************/

			operator double* ();
			operator float*  ();
			operator int*    ();
	};