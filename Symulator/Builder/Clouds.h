#pragma once


// ------------------------------

	void generateCloud();
	void changeCloudTexCoords();
	//void changeTextureSpread();
	void changeCloudColors();
	//void changeCloudHeight();
	//void changeCloudSpeed();
	void changeCloudDirection();
	void updateCloud();
	void drawCloud();
	void cleanCloud();
// ------------------------------

	void generateCloudTexture(unsigned char *bits, unsigned int &width, unsigned int &height, FIBITMAP *dib);
	bool loadCloudTexture(char *filename);
	int  loadCloudTextures();
//	void reloadCloudTextures();
	//void changeCloudTexture();
	void removeCloudTextures();

// ------------------------------

	//void saveCloud(char *filename);
	bool loadCloud(char *filename);

// ------------------------------

	void ResetCloud();
	void CloseCloud();