#pragma once



	class Color
	{
		public:

			Color  ();
			Color  (float const &Red, float const &Green, float const &Blue, float const &Alpha);
			Color  (Color const &color);
			~Color ();

	/******************************************************
			Color managment.
	******************************************************/

			void SetRed   (float const &Red);
			void SetGreen (float const &Green);
			void SetBlue  (float const &Blue);
			void SetAlpha (float const &Alpha);

			void SetColor (float const &Red, float const &Green, float const &Blue, float const &Alpha);
			void SetColor (Color const &color);
			void SetColor (DWORD const &color);

	/******************************************************
			Funkcje pobierajace kolor
	******************************************************/

			float  GetRed   ();
			float  GetGreen ();
			float  GetBlue  ();
			float  GetAlpha ();

	/******************************************************
			Funkcje zarzadzajace kolorem domyslnym
	******************************************************/

			void SetDefaultRed   (float const &Red);
			void SetDefaultGreen (float const &Green);
			void SetDefaultBlue  (float const &Blue);
			void SetDefaultAlpha (float const &Alpha);

			void SetDefaultColor (float const &Red, float const &Green, float const &Blue, float const &Alpha);
			void SetDefaultColor (Color const &color);
			void SetDefaultColor (DWORD const &color);

	/******************************************************
			Funkcje pobierajace kolor domyslny
	******************************************************/

			float GetDefaultRed   ();
			float GetDefaultGreen ();
			float GetDefaultBlue  ();
			float GetDefaultAlpha ();

			Color GetDefaultColor ();

	/******************************************************
			Dodatkowe funkcje
	******************************************************/

			bool   Equals	   (Color const &color);
			bool   NotEquals   (Color const &color);

			Color& Invert      ();
			Color& MixWith     (Color const &color);

			Color& Add         (Color const &color);
			Color& Substract   (Color const &color);

			Color& Multiply    (Color const &color);
			Color& Divide      (Color const &color);

	/******************************************************
			Pomocnicze operatory
	******************************************************/

			bool   operator== (Color const &arg);
			bool   operator!= (Color const &arg);

			Color  operator!  ();
			Color  operator&  (Color const &arg) const;

			Color  operator+  (Color const &arg) const;
			Color& operator+= (Color const &arg);

			Color  operator-  (Color const &arg) const;
			Color& operator-= (Color const &arg);

			Color  operator*  (Color const &arg) const;
			Color& operator*= (Color const &arg);

			Color  operator/  (Color const &arg) const;
			Color& operator/= (Color const &arg);

			Color& operator=  (Color const &arg);
			float  operator[] (int index)        throw( std::string );

	/******************************************************
				Konwertery
	******************************************************/

			/*operator double* ();
			operator float*  ();
			operator int*    ();*/
			

		private:

	/******************************************************
			Prosta funkcje dbajace o poprawnosc 
			wartosci kolorow
	******************************************************/

			void ValueCorrection        (float &value);

			void ColorCorrection        ();
				
				void RedCorrection          ();
				void GreenCorrection        ();
				void BlueCorrection         ();
				void AlphaCorrection        ();

			void DefaultColorCorrection ();

				void DefaultRedCorrection   ();
				void DefaultGreenCorrection ();
				void DefaultBlueCorrection  ();
				void DefaultAlphaCorrection ();


		// Zmienne okreslajace kolor

			float red, green, blue, alpha;

		// Domyslne wartosci koloru przy inicjalizacji

			static float defaultRed, defaultGreen, defaultBlue, defaultAlpha;

	};