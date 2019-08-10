#include "GlobalHeaders.h"

#include "Color.h"

// poczatkowy domyslny kolor = bialy, nieprzezroczysty

	float Color::defaultRed   = 1.f;
	float Color::defaultGreen = 1.f;
	float Color::defaultBlue  = 1.f;
	float Color::defaultAlpha = 1.f;



	Color::Color  (): red (defaultRed),  green(defaultGreen), 
		              blue(defaultBlue), alpha(defaultAlpha)
	{}

	Color::Color  (float const &Red, float const &Green, float const &Blue, float const &Alpha)
				
				: red (Red),  green(Green), 
		          blue(Blue), alpha(Alpha)
	{
		ValueCorrection(red);
		ValueCorrection(green);
		ValueCorrection(blue);
		ValueCorrection(alpha);
	}

	Color::Color  (Color const &color)
	{
		red   = color.red;
		green = color.green;
		blue  = color.blue;
		alpha = color.alpha;
	}
	Color::~Color ()
	{}
//
/******************************************************
		Funkcje zarzadzajace kolorem
******************************************************/

	void Color::SetRed   (float const &Red)
	{
		red = Red;
		RedCorrection();
	}

	void Color::SetGreen (float const &Green)
	{
		green = Green;
		GreenCorrection();
	}

	void Color::SetBlue  (float const &Blue)
	{
		blue = Blue;
		BlueCorrection();
	}

	void Color::SetAlpha (float const &Alpha)
	{
		alpha = Alpha;
		AlphaCorrection();
	}
//
	void Color::SetColor (float const &Red, float const &Green, float const &Blue, float const &Alpha)
	{
		red   = Red;
		green = Green;
		blue  = Blue;
		alpha = Alpha;

		ColorCorrection();
	}

	void Color::SetColor (Color const &color)
	{
		red   = color.red;
		green = color.green;
		blue  = color.blue;
		alpha = color.alpha;

		ColorCorrection();
	}

	void Color::SetColor (DWORD const &color)
	{
		red   = (color & 0xFF000000) >> 24;
		green = (color & 0x00FF0000) >> 16;
		blue  = (color & 0x0000FF00) >> 8;
		alpha = (color & 0x000000FF);

		ColorCorrection();
	}
//
/******************************************************
		Funkcje pobierajace kolor
******************************************************/

	float  Color::GetRed            ()
	{
		return red;
	}

	float  Color::GetGreen          ()
	{
		return green;
	}

	float  Color::GetBlue           ()
	{
		return blue;
	}

	float  Color::GetAlpha          ()
	{
		return alpha;
	}
//
/******************************************************
		Funkcje zarzadzajace kolorem domyslnym
******************************************************/

	void Color::SetDefaultRed   (float const &Red)
	{
		defaultRed = Red;
		DefaultRedCorrection();
	}

	void Color::SetDefaultGreen (float const &Green)
	{
		defaultGreen = Green;
		DefaultGreenCorrection();
	}

	void Color::SetDefaultBlue  (float const &Blue)
	{
		defaultBlue = Blue;
		DefaultBlueCorrection();
	}

	void Color::SetDefaultAlpha (float const &Alpha)
	{
		defaultAlpha = Alpha;
		DefaultAlphaCorrection();
	}
//
	void Color::SetDefaultColor (float const &Red, float const &Green, float const &Blue, float const &Alpha)
	{
		defaultRed   = Red;
		defaultGreen = Green;
		defaultBlue  = Blue;
		defaultAlpha = Alpha;

		DefaultColorCorrection();
	}

	void Color::SetDefaultColor (Color const &color)
	{
		defaultRed   = color.red;
		defaultGreen = color.green;
		defaultBlue  = color.blue;
		defaultAlpha = color.alpha;

		DefaultColorCorrection();
	}

	void Color::SetDefaultColor (DWORD const &color)
	{
		defaultRed   = (color & 0xFF000000) >> 24;
		defaultGreen = (color & 0x00FF0000) >> 16;
		defaultBlue  = (color & 0x0000FF00) >> 8;
		defaultAlpha = (color & 0x000000FF);

		DefaultColorCorrection();
	}
//
/******************************************************
		Funkcje pobierajace kolor domyslny
******************************************************/

	float Color::GetDefaultRed   ()
	{
		return defaultRed;
	}

	float Color::GetDefaultGreen ()
	{
		return defaultGreen;
	}

	float Color::GetDefaultBlue  ()
	{
		return defaultBlue;
	}

	float Color::GetDefaultAlpha ()
	{
		return defaultAlpha;
	}
//
	Color Color::GetDefaultColor ()
	{
		return Color(defaultRed, defaultGreen, defaultBlue, defaultAlpha);
	}
//
/******************************************************
		Dodatkowe funkcje
******************************************************/

	bool   Color::Equals	  (Color const &color)
	{
		if( red   == color.red   &&
			green == color.green &&
			blue  == color.blue  &&
			alpha == color.alpha )
		{
			return true;
		}
		else
			return false;
	}

	bool   Color::NotEquals	  (Color const &color)
	{
		return !Equals(color); 
	}
//
	Color& Color::Invert      ()
	{
		red   = 1 - red;
		green = 1 - green;
		blue  = 1 - blue;
		alpha = 1 - alpha;

		return *this;
	}


	Color& Color::MixWith	  (Color const &color)
	{
		red   += (color.red   - red)   / 2.f;
		green += (color.green - green) / 2.f;
		blue  += (color.blue  - blue)  / 2.f;
		alpha += (color.alpha - alpha) / 2.f;

		ColorCorrection();

		return *this;
	}
//
	Color& Color::Add         (Color const &color)
	{
		red   += color.red;
		green += color.green;
		blue  += color.blue;
		alpha += color.alpha;

		ColorCorrection();

		return *this;
	}

	Color& Color::Substract   (Color const &color)
	{
		red   -= color.red;
		green -= color.green;
		blue  -= color.blue;
		alpha -= color.alpha;

		ColorCorrection();

		return *this;
	}
//
	Color& Color::Multiply    (Color const &color)
	{
		red   *= color.red;
		green *= color.green;
		blue  *= color.blue;
		alpha *= color.alpha;

		ColorCorrection();

		return *this;
	}

	Color& Color::Divide      (Color const &color)
	{
		red   /= color.red;
		green /= color.green;
		blue  /= color.blue;
		alpha /= color.alpha;

		ColorCorrection();

		return *this;
	}
//
/******************************************************
		Pomocnicze operatory
******************************************************/

	bool   Color::operator== (Color const &arg)
	{
		return Equals(arg);
	}

	bool   Color::operator!= (Color const &arg)
	{
		return NotEquals(arg);
	}
//
	Color  Color::operator!  ()
	{
		Color result = *this;

		result.Invert();

		return result;
	}

	Color  Color::operator&  (Color const &arg) const
	{
		Color result = *this;

		return result.MixWith(arg);
	}
//
	Color  Color::operator+  (Color const &arg) const
	{
		Color result = *this;

		return result.Add(arg);
	}

	Color& Color::operator+= (Color const &arg)
	{
		Add(arg);
		return *this;
	}
//
	Color  Color::operator-  (Color const &arg) const
	{
		Color result = *this;

		result.Substract(arg);

		return result;
	}

	Color& Color::operator-= (Color const &arg)
	{
		Substract(arg);
		return *this;
	}
//
	Color  Color::operator*  (const Color  &arg) const
	{
		Color result = *this;

		result.Multiply(arg);

		return result;
	}

	Color& Color::operator*= (Color const &arg)
	{
		Multiply(arg);
		return *this;
	}
//
	Color  Color::operator/  (Color const &arg) const
	{
		Color result = *this;

		result.Divide(arg);

		return result;
	}

	Color& Color::operator/= (Color const &arg)
	{
		Divide(arg);
		return *this;
	}
//
	Color& Color::operator=  (const Color &arg)
	{
		SetColor(arg);
		return *this;
	}

	float  Color::operator[] (int index)        throw( std::string )
	{
		switch( index )
		{
			case 0:		return red;
			case 1:		return green;
			case 2:		return blue;
			case 3:		return alpha;

			default:
			{
				std::string error;

				char wrongIndex[MAX_PATH];
				_itoa_s(index, wrongIndex, MAX_PATH, 10);
				
				error += "Color::operator[]. Incorrect index: ";
				error += wrongIndex;

				throw error;
			}
			break;
		}
	}	
//
/******************************************************
			Konwertery
******************************************************/

	/*Color::operator double* ()
	{
		double conversion[4] = {red, green, blue, alpha};
		return conversion;
	}

	Color::operator float*  ()
	{
		float conversion[4] = {red, green, blue, alpha};
		return conversion;
	}

	Color::operator int*    ()
	{
		int conversion[4] = {red, green, blue, alpha};
		return conversion;
	}*/
//
/******************************************************
		Proste funkcje sprawdzajace
		poprawnosc wartosci kolorow
******************************************************/

	void Color::ValueCorrection        (float &value)
	{
		if( value > 1 )
		{
			value = 1;
		}
		else if( value < 0 )
		{
			value = 0;
		}
	}
//
	void Color::ColorCorrection        ()
	{
		RedCorrection();
		GreenCorrection();
		BlueCorrection();
		AlphaCorrection();
	}
//
	void Color::RedCorrection          ()
	{
		ValueCorrection(red);
	}

	void Color::GreenCorrection        ()
	{
		ValueCorrection(green);
	}

	void Color::BlueCorrection         ()
	{
		ValueCorrection(blue);
	}

	void Color::AlphaCorrection        ()
	{
		ValueCorrection(alpha);
	}


//
	void Color::DefaultColorCorrection ()
	{
		DefaultRedCorrection();
		DefaultGreenCorrection();
		DefaultBlueCorrection();
		DefaultAlphaCorrection();
	}

//
	void Color::DefaultRedCorrection   ()
	{
		ValueCorrection(defaultRed);
	}

	void Color::DefaultGreenCorrection ()
	{
		ValueCorrection(defaultGreen);
	}

	void Color::DefaultBlueCorrection  ()
	{
		ValueCorrection(defaultBlue);
	}

	void Color::DefaultAlphaCorrection ()
	{
		ValueCorrection(defaultAlpha);
	}

