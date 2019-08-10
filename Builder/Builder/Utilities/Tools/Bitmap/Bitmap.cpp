#include "GlobalHeaders.h"

#include "Bitmap.h"



	unsigned int LoadTexture(const char *filename, bool WithAlpha, unsigned int (*GenerateTexture)(unsigned char *bits, unsigned int &width, unsigned int &height), Color keyColor) throw( std::string )
	{
	// zmienna okreslajaca mozliwy error, ktory moze byc wyrzucony

		std::string error;

	// zmienna, ktora bedzie okreslac format pliku

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// wskaznik na zaladowany obraz

		FIBITMAP *dib(0);

	// wskaznik na dane obrazu

		unsigned char *bits(0);

	// rozmiary obrazu

		unsigned int width(0), height(0);


	// pobranie typu tekstury

		fif = FreeImage_GetFileType(filename, 0);

	// jezeli nie zostal rozpoznany, pobierz format z rozszerzenia pliku

		if( fif == FIF_UNKNOWN ) 
		{
			fif = FreeImage_GetFIFFromFilename(filename);
		}

	// jezeli dalej nieznany, zasygnalizuj to

		if( fif == FIF_UNKNOWN )
		{
			error = "Format tekstury, ktora uzytkownik proboje zaladowac, jest nieznany loaderowi tekstur.";
			throw error;
		}


	// jezeli freeimage jest w stanie zaladowac plik, zaladuj
		
		if( FreeImage_FIFSupportsReading(fif) )
		{
			dib = FreeImage_Load(fif, filename, BMP_DEFAULT);
		}

	// przy nieudanej probie zaladowania, zasygnalizuj to...

		if( !dib )
		{
			error = "Nieudana proba zaladowania tekstury: ";
			error += filename;
			throw error;
		}


	// pobierz dane obrazu

		bits = FreeImage_GetBits(dib);

	// pobierz rozmiary obrazu

		width  = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);


	// jezeli ktores z wyzej ladowanych danych sa niepoprawne, zasygnalizuj to

		if( bits == 0 )
		{
			error = "Zaladowana ilosc bitow kolorow wynosi zero!\n";
			error += filename;
			throw error;
		}
		if( width == 0 )
		{
			error = "Pobrana szerokosc tekstury wynosi zero!\n";
			error += filename;
			throw error;
		}
		if( height == 0 )
		{
			error = "Pobrana wysokosc tekstury wynosi zero!\n";
			error += filename;
			throw error;
		}


	// wskaznik na tablice bitow (jezeli bedzie potrzeba przerobienia bitow etc.)

		unsigned char *newBits(0);


	// sprawdz format koloru

		FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(dib);

	// w zaleznosci od formatu, dokonaj odpowiednich przerobek

		switch( colorType )
		{

			case FIC_RGB:
			{
			// bedzie potrzebny do zonglerki z bitami
				unsigned char helper; 

			// tekstury sa zapisano jako GBR, a wiec trza poprzestawiac troche bity...

				for(int bit = 0; bit < width * height * 3; bit += 3)
				{
					helper = bits[bit];

					bits[ bit ]   = bits[ bit+2 ];
					bits[ bit+2 ] = helper;
				}
			}
			break;
			
			case FIC_RGBALPHA:
			{
			// jezeli obiekt ma byc pokryty tekstura bez alphy, 
			// trza wyciac zbedne bity okreslajace alphe

				if( !WithAlpha )
				{
					newBits = new unsigned char [width*height*3];

					for(int src = 0, dst = 0; src < width * height * 4; src += 4, dst += 3)
					{
						newBits[ dst ]   = bits[ src ];
						newBits[ dst+1 ] = bits[ src+1 ];
						newBits[ dst+2 ] = bits[ src+2 ];
					}

					bits = newBits;
				}
			}
			break;

		// nie obslugiwane formaty

			case FIC_MINISBLACK:
			case FIC_MINISWHITE:
			{
				error = "Aplikacja nie wspiera bitmap monochromatycznych.\n";
				error += filename;
				throw error;
			}
			break;

			case FIC_PALETTE:
			{
				error = "Aplikacja nie wspiera bitmap spaletyzowanych.\n";
				error += filename;
				throw error;
			}
			break;

			case FIC_CMYK:
			{
				error = "Aplikacja nie wspiera formatu CMYK.\n";
				error += filename;
				throw error;
			}
			break;


			default:
			break;

		}


	// kluczowanie kolorow, jezeli uzytkownik chce miec teksture z alfa

		if( WithAlpha )
		{

			if( colorType == FIC_RGB )
			{

				newBits = new unsigned char[width*height*4];

				for(int src = 0, dst = 0; src < width * height * 3; src += 3, dst += 4)
				{

					newBits[ dst ]   = bits[ src ];
					newBits[ dst+1 ] = bits[ src+1 ];
					newBits[ dst+2 ] = bits[ src+2 ];


					if( bits[ src ]   == keyColor.GetRed()*255.f
							  && 
						bits[ src+1 ] == keyColor.GetGreen()*255.f
							  && 
						bits[ src+2 ] == keyColor.GetBlue()*255.f )

						newBits[ dst+3 ] = 0;
					else
						newBits[ dst+3 ] = 255;
						
				}

				bits = newBits;

			}

		}// if( WithAlpha )


	// generowanie tekstury za pomoca podanego przez uzytkownika wskaznika

		unsigned int newTextureID = GenerateTexture(bits, width, height);


	// usuniecie pomocniczego wskaznika

		if( newBits != NULL )
			free(newBits);


	// zwolnienie danych pliku z tekstura

		FreeImage_Unload(dib);


		return newTextureID;
	}