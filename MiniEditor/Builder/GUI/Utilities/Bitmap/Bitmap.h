#pragma once



#include <FreeImage.h>

#define BITMAP_ID   0x4D42    // identyfikator pliku BMP




// WriteBitmapFile()
// opis: zapisuje obraz o wymiarach width X height
//       w 24-bitowym pliku RGB w formacie .BMP 
int WriteBitmapFile(char *filename, int width, int height, unsigned char *imageData)
{
	FILE			 *filePtr;			// wskaŸnik pliku
	BITMAPFILEHEADER bitmapFileHeader;	// nag³ówek pliku
	BITMAPINFOHEADER bitmapInfoHeader;	// nag³ówek obrazu
	int				 imageIdx;			// indeks obrazu
	unsigned char	 tempRGB;			// zmienna zamiany sk³adowych

	// otwiera plik do zapisu w trybie "writing binary"
	filePtr = fopen(filename, "wb");
	if (!filePtr)
		return 0;

	// definiuje nag³ówek pliku
	bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
	bitmapFileHeader.bfType = 0x4D42;
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	
	// definiuje nag³ówek mapy bitowej
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;						// 24-bitowy
	bitmapInfoHeader.biCompression = BI_RGB;				// bez kompresji
	bitmapInfoHeader.biSizeImage = width * abs(height) * 3;	// width * height * (bajty RGB)
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
	bitmapInfoHeader.biWidth = width;						// szerokoœæ mapy
	bitmapInfoHeader.biHeight = height;						// wysokoœæ mapy

	// zamienia format danych z RGB na BGR
	for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx+=3)
	{
		tempRGB = imageData[imageIdx];
		imageData[imageIdx] = imageData[imageIdx + 2];
		imageData[imageIdx + 2] = tempRGB;
	}

	// zapisuje nag³ówek pliku
	fwrite(&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), filePtr);

	// zapisuje nag³ówek mapy bitowej
	fwrite(&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), filePtr);

	// zapisuje dane mapy bitowej
	fwrite(imageData, 1, bitmapInfoHeader.biSizeImage, filePtr);

	// zamyka plik
	fclose(filePtr);

	return 1;
}


/*****************************************************************************
 LoadBitmapFile()

 ³aduje mapê bitow¹ z pliku i zwraca jej adres.
 Wype³nia tak¿e strukturê nag³ówka. Nie obs³uguje map 8-bitowych.
*****************************************************************************/
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;								// wskaŸnik pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag³ówek pliku
	unsigned char		*bitmapImage;			// bufor obrazu
	int					imageIdx = 0;			// licznik bajtów obrazu
	unsigned char		tempRGB;				// zmienna zamiany sk³adowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// wczytuje nag³ówek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// sprawdza, czy rzeczywiœcie jest to plik BMP
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// wczytuje nag³ówek obrazu zapisanego w pliku
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ustawia wskaŸnik pliku na pocz¹tku danych opisuj¹cych obraz
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// przydziela pamiêæ na bufor obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// sprawdza, czy pamiêæ zosta³a przydzielona
	if (!bitmapImage)
	{
		delete [] bitmapImage;
		fclose(filePtr);
		return NULL;
	}

	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// sprawdza, czy operacja powiod³a siê
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// zamienia sk³adowe R i B values, aby uzyskaæ w³aœciwy format RGB w OpenGL
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// zamyka plik i zwraca wskaŸnik bufora zawieraj¹cego obraz
	fclose(filePtr);
	return bitmapImage;
}




/*****************************************************************************
 LoadBitmapFileWithAlpha

 Loads a bitmap file normally, and then adds an alpha component to use for
 blending
*****************************************************************************/
unsigned char *LoadBitmapFileWithAlpha(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
  unsigned char *bitmapImage = LoadBitmapFile(filename, bitmapInfoHeader);
  unsigned char *bitmapWithAlpha = (unsigned char *)malloc(bitmapInfoHeader->biSizeImage * 4 / 3);

  if (bitmapImage == NULL || bitmapWithAlpha == NULL)
    return NULL;

  // loop through the bitmap data
  for (unsigned int src = 0, dst = 0; src < bitmapInfoHeader->biSizeImage; src +=3, dst +=4)
  {
    // if the pixel is black, set the alpha to 0. Otherwise, set it to 255.
    if (bitmapImage[src] == 255 && bitmapImage[src+1] == 255 && bitmapImage[src+2] == 255)
      bitmapWithAlpha[dst+3] = 0;
    else
      bitmapWithAlpha[dst+3] = 0xFF;

    // copy pixel data over
    bitmapWithAlpha[dst] = bitmapImage[src];
    bitmapWithAlpha[dst+1] = bitmapImage[src+1];
    bitmapWithAlpha[dst+2] = bitmapImage[src+2];
  }

  delete [] bitmapImage;

  return bitmapWithAlpha;
} // end LoadBitmapFileWithAlpha()




//

// #include "Color//Color.h"
//
unsigned int LoadTexture (const char *filename, bool WithAlpha, unsigned int (*GenerateTexture)(unsigned char *bits, unsigned int &width, unsigned int &height), Color keyColor = Color(1,1,1,1)) throw( std::string );



// przykladowa funkcja generujaca teksture w OpenGL

	//unsigned int generateTerrainTexture(unsigned char *bits, unsigned int &width, unsigned int &height)
	//{
		//	unsigned int textureID;

		////wygeneruj nowy obiekt tekstury oraz zbinduj go

		//	glGenTextures(1, &textureID);
		//	glBindTexture(GL_TEXTURE_2D, textureID);


		//// okreslenie parametrow tekstury

		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		//	/*gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, textureInfo.biWidth, textureInfo.biHeight,
		//				  GL_RGB, GL_UNSIGNED_BYTE, texture );*/

		//// wygenerowanie mipmapy

		//	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, width, height,
		//				  GL_RGB, GL_UNSIGNED_BYTE, bits );

		//	return textureID;

	//}
