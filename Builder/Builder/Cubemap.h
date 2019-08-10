#pragma once




// ------------------------------

	void generateCubeMap();
	void changeCubeMapColors();
	void drawCubeMap();
	void cleanCubeMap();
// ------------------------------

	void generateCubeMapTexture(unsigned char *bits, unsigned int &width, unsigned int &height, FIBITMAP *dib);
	bool loadCubeMapTexture(char *filename);
	int  loadCubeMapTextures();
	void reloadCubeMapTextures();
	void changeCubeMapTexture();
	void removeCubeMapTextures();

// ------------------------------

	void saveCubeMap(char *filename);
	bool loadCubeMap(char *filename);

// ------------------------------
	void ResetCubeMap();
	void CloseCubeMap();