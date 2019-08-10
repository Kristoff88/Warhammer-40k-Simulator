#pragma once



// Zmienne, struktury

	
	HWND	textures = NULL;

	int		textureRangeX = 1,	   
			textureRangeZ = 1;


	// do zapisu / odczytu poszczegolnych tekstur
		struct textureObj
		{
			char	        filename[MAX_PATH];
			unsigned int   	textureID;
		};

	std::vector <textureObj> textureArray;


		unsigned int landBrush = 0;       // terenowy pedzelek! :)


	// warstwy tekstur

		std::string TEXTURE_NOT_SET("Texture not set");

	class	TextureLayer
	{
		public:

			float		   (*colorArray)[4];
			unsigned int	VBOvc,
							textureID;

			std::string     name;

			TextureLayer(): colorArray(NULL), VBOvc(0), textureID(0), name(TEXTURE_NOT_SET)
			{}

			~TextureLayer()
			{
				if( colorArray != NULL )
					delete [] colorArray;

				if( VBOvc != 0 )
					glDeleteBuffersARB(1, &VBOvc);
			}
	};
	
	TextureLayer *layers = NULL;

	int			layerCount   = 0,
				currentLayer = -1;
	
	#define		MAX_LAYERS	  5

	bool		OnlyTexture    = false,	// decyduje o tym czy przy przenoszeniu warstwy, jest przenoszone ID tekstury i tablica kolorow czy tylko same ID tekstury
				SolidBrush     = false,	// decyduje o sposobie malowania tekstur
				RenderTextures = true;

	float       textureA;				// decyduje o sposobie modyfikowania alphy warstwy tekstury

	char		textureKey = NULL;		// ustawiany w WndProc()


// Funkcje

	void resizeColorArrays(int oldX, int oldZ)	// tylko jeden z tych argumentow moze byc niezerowy
	{						// zero = bez zmian

	// utworzenie kopii tablic kolorow

		TextureLayer *copy = new TextureLayer[MAX_LAYERS];

		for(int i = 0; i < MAX_LAYERS; i++)
		{
									  // jezeli ktores z nich == 0, automatycznie wyklucza to jeden z czlonow indeksowania
			copy[i].colorArray = new float [oldX*MAP_Z + MAP_X*oldZ][4];

			int currentVertex;

		// kopiuje sie tylko druga teksture (nie ma sensu kopiowac bazowej tekstury, zawsze bedzie taka sama)

			if( oldX )
			{
				for(int z = 0; z < MAP_Z; z++)
				for(int x = 0; x < oldX; x++)
				{
					currentVertex = x + oldX*z;

					copy[i].colorArray[currentVertex][0] = layers[i].colorArray[currentVertex][0];
					copy[i].colorArray[currentVertex][1] = layers[i].colorArray[currentVertex][1];
					copy[i].colorArray[currentVertex][2] = layers[i].colorArray[currentVertex][2];
					copy[i].colorArray[currentVertex][3] = layers[i].colorArray[currentVertex][3];
				}
			}
			else if( oldZ )
			{
				for(int z = 0; z < oldZ; z++)
				for(int x = 0; x < MAP_X; x++)
				{
					currentVertex = x + MAP_X*z;

					copy[i].colorArray[currentVertex][0] = layers[i].colorArray[currentVertex][0];
					copy[i].colorArray[currentVertex][1] = layers[i].colorArray[currentVertex][1];
					copy[i].colorArray[currentVertex][2] = layers[i].colorArray[currentVertex][2];
					copy[i].colorArray[currentVertex][3] = layers[i].colorArray[currentVertex][3];
				}
			}

		// usuniecie poprzednich tablic kolorow oraz ich VBO

			assert( layers[i].colorArray != NULL );
			delete [] layers[i].colorArray;

			assert( layers[i].VBOvc != 0 );
			glDeleteBuffersARB(1, &layers[i].VBOvc);

		// utworzenie nowej tablicy kolorow

			layers[i].colorArray      = new float [MAP_X*MAP_Z][4];

		// ustawienie domyslnych wartosci dla nowych tablic

			for(int z = 0; z < MAP_Z; z++)
			for(int x = 0; x < MAP_X; x++)
			{
				currentVertex = x + MAP_X*z;

				layers[i].colorArray[currentVertex][0] = 
				layers[i].colorArray[currentVertex][1] =
				layers[i].colorArray[currentVertex][2] = .6f;

				if( i == 0 )
				{
					layers[i].colorArray[currentVertex][3] = 1.f;
				}
				else
				{
					layers[i].colorArray[currentVertex][3] = 0.f;
				}
			}


		// kopiowanie tablicy kolorow drugorzednej tekstury

			if( oldX )
			{
				if( oldX <= MAP_X )
				{
					for(int z = 0; z < MAP_Z; z++)
					for(int x = 0; x < oldX; x++)
					{
						currentVertex = x + MAP_X*z;

						int helperIndex = x + oldX*z;
			
						layers[i].colorArray[currentVertex][0] = copy[i].colorArray[helperIndex][0];
						layers[i].colorArray[currentVertex][1] = copy[i].colorArray[helperIndex][1];
						layers[i].colorArray[currentVertex][2] = copy[i].colorArray[helperIndex][2];
						layers[i].colorArray[currentVertex][3] = copy[i].colorArray[helperIndex][3];
					}
				}
				else
				{
					for(int z = 0; z < MAP_Z; z++)
					for(int x = 0; x < MAP_X; x++)
					{
						currentVertex = x + MAP_X*z;

						int helperIndex = x + oldX*z;

						layers[i].colorArray[currentVertex][0] = copy[i].colorArray[helperIndex][0];
						layers[i].colorArray[currentVertex][1] = copy[i].colorArray[helperIndex][1];
						layers[i].colorArray[currentVertex][2] = copy[i].colorArray[helperIndex][2];
						layers[i].colorArray[currentVertex][3] = copy[i].colorArray[helperIndex][3];
					}
				}
			}
			if( oldZ )
			{
				if( oldZ <= MAP_Z )
				{
					for(int z = 0; z < oldZ; z++)
					for(int x = 0; x < MAP_X; x++)
					{
						currentVertex = x + MAP_X*z;

						layers[i].colorArray[currentVertex][0] = copy[i].colorArray[currentVertex][0];
						layers[i].colorArray[currentVertex][1] = copy[i].colorArray[currentVertex][1];
						layers[i].colorArray[currentVertex][2] = copy[i].colorArray[currentVertex][2];
						layers[i].colorArray[currentVertex][3] = copy[i].colorArray[currentVertex][3];
					}
				}
				else
				{
					for(int z = 0; z < MAP_Z; z++)
					for(int x = 0; x < MAP_X; x++)
					{
						currentVertex = x + MAP_X*z;

						layers[i].colorArray[currentVertex][0] = copy[i].colorArray[currentVertex][0];
						layers[i].colorArray[currentVertex][1] = copy[i].colorArray[currentVertex][1];
						layers[i].colorArray[currentVertex][2] = copy[i].colorArray[currentVertex][2];
						layers[i].colorArray[currentVertex][3] = copy[i].colorArray[currentVertex][3];
					}
				}
			}

		// utworzenie nowych VBO

			glGenBuffersARB(1, &layers[i].VBOvc);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[i].VBOvc);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[i].colorArray, GL_DYNAMIC_DRAW_ARB);

		}

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	// zwolnienie tablicy pomocniczej

		delete [] copy;
	}


	void updateTextures()
	{
		// pobranie rozmiarow pedzla

			textureRangeX = GetDlgItemInt(textures, IDC_TEXT_X_EDIT, FALSE, FALSE);
			textureRangeZ = GetDlgItemInt(textures, IDC_TEXT_Z_EDIT, FALSE, FALSE);
		
		// pobranie wartosci modyfikatorow kolorow warstw

			textureA  = (float)GetDlgItemInt(textures, IDC_TERRAIN_ALPHA, FALSE, FALSE) / 2550.f;

		// pobranie aktualnie modyfikowanej warstwy

			HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

		// jezeli combobox jest pusty...

			if( ComboBox_GetCount(comboBox) == 0 )
			{
				currentLayer = -1;
				return;
			}

		// jezeli uzytkownik nie wybral warstwy...

			char buffer[MAX_PATH];

			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			if( strcmp( buffer, "" ) == 0 )
			{
				currentLayer = -2;
				return;
			}

		// pobranie aktualnego indeksu...

			currentLayer = ComboBox_GetCurSel(comboBox);

	}

	void paintLand()
	{

		// obsluga modulacji modyfikatora alfy

			if(wheelMovement < 0)
			{
				int additive = 0;
				for(int i = 0; i > mouseWheelSteps; i--)
					additive -= 5;

			// zabezpieczenie

				while( (textureA * 255.f) + additive < 0 )
				{
					additive++;
				}

				SetDlgItemInt(textures, IDC_TERRAIN_ALPHA, GetDlgItemInt(textures, IDC_TERRAIN_ALPHA, FALSE, FALSE) + additive, FALSE);

				wheelMovement %= WHEEL_DELTA;
			}
			else if(wheelMovement > 0)
			{
				int additive = 0;
				for(int i=0; i<mouseWheelSteps; i++)
					additive += 5;

				SetDlgItemInt(textures, IDC_TERRAIN_ALPHA, GetDlgItemInt(textures, IDC_TERRAIN_ALPHA, FALSE, FALSE) + additive, FALSE);

				wheelMovement %= WHEEL_DELTA;
			}

			updateTextures();

		// kliknieto przycisk myszki oraz wybrano terenowy pedzelek :)

			if( lButtonDown || rButtonDown )
			{

			// drobne zabezpieczenie

				if( currentLayer == -1 )
				{
					MessageBox(g_hwnd, "There haven't been any texture layers added yet!", "Error!", MB_OK);
					lButtonDown = rButtonDown = false;
					return;
				}

				if( currentLayer == -2 )
				{
					MessageBox(g_hwnd, "Choose a layer, before attempting to paint.", "Error!", MB_OK);
					lButtonDown = rButtonDown = false;
					return;
				}

			// przerobienie koordynatow myszki na pozycje w przestrzenii opengl

				int X =  cursorX / MAP_SCALE,
					Z = -cursorZ / MAP_SCALE;


				float sign = 1;

				if( rButtonDown )
					sign *= -1;

				int currentVertex;


			// zmienna decydujaca o poziomie modyfikacji alphy

				float modifier  = 0.f;	


				for (int z = Z - textureRangeZ + 1; z < Z + textureRangeZ; z++)
				for (int x = X - textureRangeX + 1; x < X + textureRangeX; x++)
				{
				// solidny, lity pedzelek

					if( SolidBrush )
					{
						modifier = textureA;
					}
					else
					{
					// obliczenie promienia jako przeciwprostokatnej w trojkacie prostakatnym wg. twierdzenia pitagorasa

						float a = x - X,
							  b = z - Z;

						if( a < 0 )
							a *= -1;

						if( b < 0 )
							b *= -1;

					// promien aktualnego punktu

						float pointRange = sqrtf( pow(a, 2) + pow(b, 2) );

						modifier = textureA / pointRange;
					}

				// pedzelek kragly

					if( roundCursor )
					{
					// obliczenie czy punkt lezy na elipsie (na podstawie rownania elipsy)

						float pX = (x - X),
							  pZ = (z - Z),

							  elipseEq = pow(pX, 2.f)/pow((float)textureRangeX, 2.f)
														+
										 pow(pZ, 2.f)/pow((float)textureRangeZ, 2.f);

					// jezeli punkt nie lezy na elipsie

						if( elipseEq > 1 )
						{
							modifier = 0;
						}
					}


				// nie ma sensu modyfikowac kolorow poza terenem (moze, ze chce sie access violation):)

					if( x >= 0 && x < MAP_X && z >= 0 && z < MAP_Z )
					{
					// wierzcho³ki uporz¹dkowane s¹ od lewej do prawej, z góry w dó³

						currentVertex = z * MAP_X + x;

					// umieszcza wartoœci w tablicy kolorów

						layers[currentLayer].colorArray[currentVertex][3] += modifier * sign;

					// korygacja A... 

						if( layers[currentLayer].colorArray[currentVertex][3] > 1.f )
						{
							layers[currentLayer].colorArray[currentVertex][3] = 1.f;
						}
						else if( layers[currentLayer].colorArray[currentVertex][3] < 0.f )
						{
							layers[currentLayer].colorArray[currentVertex][3] = 0.f;
						}
					}
				}

			// odswiezenie VBO kolorow

				if( layers[currentLayer].VBOvc != 0 )
					glDeleteBuffersARB(1, &layers[currentLayer].VBOvc);

				glGenBuffersARB( 1, &layers[currentLayer].VBOvc );

				glBindBufferARB( GL_ARRAY_BUFFER_ARB, layers[currentLayer].VBOvc );
				glBufferDataARB( GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[currentLayer].colorArray,	GL_DYNAMIC_DRAW_ARB );

				
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


				Saved = false;
			}
			else if( lButtonDown )
			{
				MessageBox(g_hwnd, "Terrain texture hasn't been chosen.", "Error!", MB_OK);
				lButtonDown = false;
			}
	}



	void setLayer()
	{
			if( layerCount < MAX_LAYERS )
			{
			// pobranie handlerow do obu combo box'ow

				HWND comboBox  = GetDlgItem(textures, IDC_TERRAIN_COMBO),
					 comboBox2 = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

			// sprawdzenie czy sa w ogole jakies item'y w combobox'ie z teksturami

				if( !ComboBox_GetCount(comboBox) )
				{
					MessageBox(g_hwnd, "There are no terrain textures loaded!", "Error.", MB_OK);
					return;
				}

			// tablica znakow, ktora bedzie umieszczona w combo box'ie z warstwami tekstur

				char buffer[MAX_PATH], 
					
			// nazwa tekstury, ktora bedzie pobrana z IDC_TERRAIN_COMBO

					textureName[MAX_PATH];

			// dodanie indeksu warstwy do bufora z nazwa warstwy

				_itoa_s(layerCount+1, buffer, 10);
				strcat_s(buffer, " - ");

			// pobranie nazwy tekstury

				ComboBox_GetText(comboBox, textureName, MAX_PATH);

				if( strcmp(textureName, "") == 0 )
				{
					MessageBox(g_hwnd, "Choose a texture, before trying to set a texture layer!", "Error.", MB_OK);
					return;
				}

			// dodanie jej do bufora nazwy

				strcat_s(buffer, textureName);

			// odnalezienie nazwy tekstury w wektorze tekstur

				std::string itemName;
				bool trigger;
				std::string::size_type result;

				bool itemFound = false;
				int  index;

				for(int i = 0; i < textureArray.size(); i++)
				{
					itemName.clear();
					itemName.insert(0, textureArray[i].filename, 0, MAX_PATH);

					trigger = false;
					
				// "obranie" sciezki do pliku z tekstura, wylacznie do samej nazwy (bez rozszerzenia)

					while( !trigger )
					{
						result = itemName.find('\\');

						if(result != std::string::npos)
						{
							itemName.erase(0, result+1);
						}
						else
						{
							result = itemName.rfind('.');
							itemName.erase(result, std::string::npos);

							trigger = true;
						}
					}

				// jezeli znaleziono, nie ma sensu dalej prowadzic petli
				// (niemozliwe aby byly 2 tekstury o identycznej nazwie.
				// loader tekstur "krzyczy" kiedy jest kolizja w nazwach.)

					if( strcmp( itemName.c_str(), textureName ) == 0 )
					{
						itemFound = true;
						index = i;
						break;
					}
				}

				if( itemFound )
				{
				// zabezpieczenie przed dwoma identycznymi teksturami

					for(int i = 0; i < layerCount; i++)
					{
						if( layers[i].textureID == textureArray[index].textureID )
						{
							MessageBox(g_hwnd, "There can't be two texture layers with the same texture ID!", "Error!", MB_OK);
							return;
						}
					}

				// ustawienie nazwy warstwy w combo box'ie z warstwami

					ComboBox_AddString(comboBox2, buffer);

				// ustawienie nazwy warstwy

					layers[layerCount].name = buffer;

				// ustawienie id tekstury w wektorze warstw tekstur

					layers[layerCount].textureID = textureArray[index].textureID;
				
				// inkremetnacja licznika warstw
			
					layerCount++;
				}
				else
				{
					MessageBox(g_hwnd, "Texture name from combo box haven't been found in texureArray! (serious bug)", "Error!", MB_OK);
				}


				Saved = false;

			}// if( layerCount < MAX_LAYERS )
			else
			{
				MessageBox(g_hwnd, "Layer count has exceeded the maximum number of layers!", "Error!", MB_OK);
			}
	}


	void removeLayer()
	{
		if( layerCount )
		{

			HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

		// zabezpieczenie...

			char buffer[MAX_PATH];

			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			if( strcmp( buffer, "" ) == 0 )
			{
				MessageBox(g_hwnd, "Choose a texture layer, before attempting to remove it!", "Error!", MB_OK);
				return;
			}

		// pobranie aktualnego indeksu...

			currentLayer = ComboBox_GetCurSel(comboBox);


		// "obsuniecie" sie wyzszych warstw

			for(int i = currentLayer; i < layerCount-1; i++)
			{
			// nie ma sensu kopiowac kolorow do warstwy bazowej

				if( i != 0 )
				{
					assert( layers[i].colorArray != NULL );

					int currentVertex;

				// skopiowanie kolorow

					for(int z = 0; z < MAP_Z; z++)
					for(int x = 0; x < MAP_X; x++)
					{
						currentVertex = x + MAP_X*z;

						layers[i].colorArray[currentVertex][0] = layers[i+1].colorArray[currentVertex][0];
						layers[i].colorArray[currentVertex][1] = layers[i+1].colorArray[currentVertex][1];
						layers[i].colorArray[currentVertex][2] = layers[i+1].colorArray[currentVertex][2];
						layers[i].colorArray[currentVertex][3] = layers[i+1].colorArray[currentVertex][3];
					}

				// utworzenie nowego VBO kolorow

					assert( layers[i].VBOvc != 0 );
					glDeleteBuffersARB(1, &layers[i].VBOvc);

					glGenBuffersARB(1, &layers[i].VBOvc);

					glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[i].VBOvc);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[i].colorArray, GL_DYNAMIC_DRAW_ARB);
				}

			// skopiowanie ID tekstury

				layers[i].textureID = layers[i+1].textureID;

			// skopiowanie nazwy wyzszej warstwy

				layers[i].name = layers[i+1].name;

			// dekrementacja indeksu w nazwie tekstury

				layers[i].name[0] -= 1;
			}


		// ustawienie domyslnych wartosci kolorow dla ostatniej warstwy

			int currentVertex;

			for(int z = 0; z < MAP_Z; z++)
			for(int x = 0; x < MAP_X; x++)
			{
				currentVertex = x + MAP_X*z;

				layers[layerCount-1].colorArray[currentVertex][0] = 
				layers[layerCount-1].colorArray[currentVertex][1] = 
				layers[layerCount-1].colorArray[currentVertex][2] = 0.6f;

				if( layerCount-1 == 0 )
				{
					layers[layerCount-1].colorArray[currentVertex][3] = 1.f;
				}
				else
				{
					layers[layerCount-1].colorArray[currentVertex][3] = 0.f;
				}
			}

		// wygenerowanie nowego VBO ostatniej warstwy

			assert( layers[layerCount-1].VBOvc != 0 );
			glDeleteBuffersARB(1, &layers[layerCount-1].VBOvc);

			glGenBuffersARB(1, &layers[layerCount-1].VBOvc);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[layerCount-1].VBOvc);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[layerCount-1].colorArray, GL_DYNAMIC_DRAW_ARB);


			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		// wyzerowanie ID tekstury ostatniej warstwy

			layers[layerCount-1].textureID = 0;

		// ustawienie pustej nazwy

			layers[layerCount-1].name = TEXTURE_NOT_SET;

		// dekrementacja licznika warstw

			layerCount--;

		// wyczyszczenie combo box'a z warstwami

			int size = ComboBox_GetCount(comboBox);

			for(int i = 0; i < size; i++)
			{
				ComboBox_DeleteString(comboBox, 0);
			}

		// ustawienie nowych nazw

			for(int i = 0; i < layerCount; i++)
			{
				ComboBox_AddString(comboBox, layers[i].name.c_str());
			}

			if( currentLayer )
				ComboBox_SetCurSel(comboBox, currentLayer-1);
			else
				ComboBox_SetCurSel(comboBox, 0);


			Saved = false;
		}
		else
		{
			MessageBox(g_hwnd, "There is no texture layer to remove!", "Error!", MB_OK);
		}
	}


	void layerUp()
	{
		if( layerCount )
		{

			HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

		// zabezpieczenie...

			char buffer[MAX_PATH];

			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			if( strcmp( buffer, "" ) == 0 )
			{
				MessageBox(g_hwnd, "Choose a texture layer, before attempting to push it up!", "Error!", MB_OK);
				return;
			}

		// pobranie aktualnego indeksu...

			currentLayer = ComboBox_GetCurSel(comboBox);

		// kolejne zabezpieczenie...

			if( currentLayer+1 == layerCount )
			{
				MessageBox(g_hwnd, "It's not possible to push up this texture layer any further...", "Error!", MB_OK);
				return;
			}


		// zamiana wartosci warstw

			unsigned int	helper;
			std::string		helper2;

			helper  = layers[currentLayer].textureID;
			helper2 = layers[currentLayer].name;

			layers[currentLayer].textureID = layers[currentLayer+1].textureID;
			layers[currentLayer].name      = layers[currentLayer+1].name;

			layers[currentLayer+1].textureID = helper;
			layers[currentLayer+1].name      = helper2;

		// korekcja indeksow w nazwach warstw

			layers[currentLayer].name[0]   -= 1;
			layers[currentLayer+1].name[0] += 1;

		// zamiana tablic kolorow

			if( !OnlyTexture )
			{
				float (*helper3)[4] = new float [MAP_X*MAP_Z][4];

				int currentVertex;

				for(int z = 0; z < MAP_Z; z++)
				for(int x = 0; x < MAP_X; x++)
				{
					currentVertex = x + MAP_X*z;

					helper3[currentVertex][0] = layers[currentLayer].colorArray[currentVertex][0];
					helper3[currentVertex][1] = layers[currentLayer].colorArray[currentVertex][1];
					helper3[currentVertex][2] = layers[currentLayer].colorArray[currentVertex][2];
					helper3[currentVertex][3] = layers[currentLayer].colorArray[currentVertex][3];

					layers[currentLayer].colorArray[currentVertex][0] = layers[currentLayer+1].colorArray[currentVertex][0];
					layers[currentLayer].colorArray[currentVertex][1] = layers[currentLayer+1].colorArray[currentVertex][1];
					layers[currentLayer].colorArray[currentVertex][2] = layers[currentLayer+1].colorArray[currentVertex][2];
					layers[currentLayer].colorArray[currentVertex][3] = layers[currentLayer+1].colorArray[currentVertex][3];

					layers[currentLayer+1].colorArray[currentVertex][0] = helper3[currentVertex][0];
					layers[currentLayer+1].colorArray[currentVertex][1] = helper3[currentVertex][1];
					layers[currentLayer+1].colorArray[currentVertex][2] = helper3[currentVertex][2];
					layers[currentLayer+1].colorArray[currentVertex][3] = helper3[currentVertex][3];
				}

			// wygenerowanie nowych VBO

				assert( layers[currentLayer].VBOvc != 0 );
				glDeleteBuffersARB(1, &layers[currentLayer].VBOvc);

				glGenBuffersARB(1, &layers[currentLayer].VBOvc);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[currentLayer].VBOvc);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[currentLayer].colorArray, GL_DYNAMIC_DRAW_ARB);


				assert( layers[currentLayer+1].VBOvc != 0 );
				glDeleteBuffersARB(1, &layers[currentLayer+1].VBOvc);

				glGenBuffersARB(1, &layers[currentLayer+1].VBOvc);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[currentLayer+1].VBOvc);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[currentLayer+1].colorArray, GL_DYNAMIC_DRAW_ARB);


				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			// zwolnienie tablicy pomocniczej

				free(helper3);

			}


		// wyczyszczenie combo box'a z warstwami

			int size = ComboBox_GetCount(comboBox);

			for(int i = 0; i < size; i++)
			{
				ComboBox_DeleteString(comboBox, 0);
			}

		// ustawienie nowych nazw

			for(int i = 0; i < layerCount; i++)
			{
				ComboBox_AddString(comboBox, layers[i].name.c_str());
			}

			ComboBox_SetCurSel(comboBox, currentLayer+1);


			Saved = false;
		}
		else
		{
			MessageBox(g_hwnd, "There is no texture layer to push up!", "Error!", MB_OK);
		}
	}



	void layerDown()
	{
		if( layerCount )
		{

			HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

		// zabezpieczenie...

			char buffer[MAX_PATH];

			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			if( strcmp( buffer, "" ) == 0 )
			{
				MessageBox(g_hwnd, "Choose a texture layer, before attempting to push it up!", "Error!", MB_OK);
				return;
			}

		// pobranie aktualnego indeksu...

			currentLayer = ComboBox_GetCurSel(comboBox);

		// kolejne zabezpieczenie...

			if( currentLayer == 0 )
			{
				MessageBox(g_hwnd, "It's not possible to push down this texture layer any further...", "Error!", MB_OK);
				return;
			}


		// zamiana wartosci warstw

			unsigned int	helper;
			std::string		helper2;

			helper  = layers[currentLayer].textureID;
			helper2 = layers[currentLayer].name;

			layers[currentLayer].textureID = layers[currentLayer-1].textureID;
			layers[currentLayer].name      = layers[currentLayer-1].name;

			layers[currentLayer-1].textureID = helper;
			layers[currentLayer-1].name      = helper2;

		// korekcja indeksow w nazwach warstw

			layers[currentLayer].name[0]   += 1;
			layers[currentLayer-1].name[0] -= 1;

		// zamiana tablic kolorow

			if( !OnlyTexture )
			{
				float (*helper3)[4] = new float [MAP_X*MAP_Z][4];

				int currentVertex;

				for(int z = 0; z < MAP_Z; z++)
				for(int x = 0; x < MAP_X; x++)
				{
					currentVertex = x + MAP_X*z;

					helper3[currentVertex][0] = layers[currentLayer].colorArray[currentVertex][0];
					helper3[currentVertex][1] = layers[currentLayer].colorArray[currentVertex][1];
					helper3[currentVertex][2] = layers[currentLayer].colorArray[currentVertex][2];
					helper3[currentVertex][3] = layers[currentLayer].colorArray[currentVertex][3];

					layers[currentLayer].colorArray[currentVertex][0] = layers[currentLayer-1].colorArray[currentVertex][0];
					layers[currentLayer].colorArray[currentVertex][1] = layers[currentLayer-1].colorArray[currentVertex][1];
					layers[currentLayer].colorArray[currentVertex][2] = layers[currentLayer-1].colorArray[currentVertex][2];
					layers[currentLayer].colorArray[currentVertex][3] = layers[currentLayer-1].colorArray[currentVertex][3];

					layers[currentLayer-1].colorArray[currentVertex][0] = helper3[currentVertex][0];
					layers[currentLayer-1].colorArray[currentVertex][1] = helper3[currentVertex][1];
					layers[currentLayer-1].colorArray[currentVertex][2] = helper3[currentVertex][2];
					layers[currentLayer-1].colorArray[currentVertex][3] = helper3[currentVertex][3];
				}

			// wygenerowanie nowych VBO

				assert( layers[currentLayer].VBOvc != 0 );
				glDeleteBuffersARB(1, &layers[currentLayer].VBOvc);

				glGenBuffersARB(1, &layers[currentLayer].VBOvc);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[currentLayer].VBOvc);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[currentLayer].colorArray, GL_DYNAMIC_DRAW_ARB);


				assert( layers[currentLayer-1].VBOvc != 0 );
				glDeleteBuffersARB(1, &layers[currentLayer-1].VBOvc);

				glGenBuffersARB(1, &layers[currentLayer-1].VBOvc);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[currentLayer-1].VBOvc);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[currentLayer-1].colorArray, GL_DYNAMIC_DRAW_ARB);


				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			// zwolnienie tablicy pomocniczej

				free(helper3);
			}


		// wyczyszczenie combo box'a z warstwami

			int size = ComboBox_GetCount(comboBox);

			for(int i = 0; i < size; i++)
			{
				ComboBox_DeleteString(comboBox, 0);
			}

		// ustawienie nowych nazw

			for(int i = 0; i < layerCount; i++)
			{
				ComboBox_AddString(comboBox, layers[i].name.c_str());
			}

			ComboBox_SetCurSel(comboBox, currentLayer-1);


			Saved = false;
		}
		else
		{
			MessageBox(g_hwnd, "There is no texture layer to push down!", "Error!", MB_OK);
		}
	}



	void clearLand()
	{

		if( layerCount )
		{
			int result = MessageBox(textures, "Are you sure, that you want to erase all the progress?", "Warning.", MB_YESNO);

			if( result == IDYES )
			{

				for(int i = 0; i < layerCount; i++)
				{
					assert(layers[i].colorArray != NULL);

					for(int z = 0; z < MAP_Z; z++)
					for(int x = 0; x < MAP_X; x++)
					{
						layers[i].colorArray[x + MAP_X*z][0] =
						layers[i].colorArray[x + MAP_X*z][1] =
						layers[i].colorArray[x + MAP_X*z][2] = .6f;

						if( i == 0 )
							layers[i].colorArray[x + MAP_X*z][3] = 1.f;
						else
							layers[i].colorArray[x + MAP_X*z][3] = 0.f;
					}

					assert(layers[i].VBOvc != 0);
				
				// usuniecie poprzedniego VBO kolorow

					glDeleteBuffersARB(1, &layers[i].VBOvc);

				// utworzenie nowego ID kolorow

					glGenBuffersARB(1, &layers[i].VBOvc);

				// zbindowanie i przypisanie danych

					glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[i].VBOvc);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[i].colorArray, GL_DYNAMIC_DRAW_ARB);
				}

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


				Saved = false;
		
			}// if( result == IDYES )
		}
		else
		{
			MessageBox(g_hwnd, "There is nothing to clear!", "Error!", MB_OK);
		}
	}






	void textureKeyManager(char key)
	{
		switch( textureKey )
		{

			case '1':
			{
				if( layerCount )
				{
					currentLayer = 0;

					HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

					ComboBox_SetCurSel(comboBox, 0);
				}
			}
			break;


			case '2':
			{
				if( layerCount >= 2 )
				{
					currentLayer = 1;

					HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

					ComboBox_SetCurSel(comboBox, 1);
				}
			}
			break;


			case '3':
			{
				if( layerCount >= 3 )
				{
					currentLayer = 2;

					HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

					ComboBox_SetCurSel(comboBox, 2);
				}
			}
			break;


			case '4':
			{
				if( layerCount >= 4 )
				{
					currentLayer = 3;

					HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

					ComboBox_SetCurSel(comboBox, 3);
				}
			}
			break;


			case '5':
			{
				if( layerCount == 5 )
				{
					currentLayer = 4;

					HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

					ComboBox_SetCurSel(comboBox, 4);
				}
			}
			break;


			case '-':
			{
				layerDown();
			}
			break;


			case '+':
			{
				layerUp();
			}
			break;
		}
	}

	void textureText()
	{
		// wyczyszczenie wektora zawierajacego tekst

			textOutput.erase( textOutput.begin(), textOutput.end() );

		// tablice, ktore beda zawierac informacje

			static char CurrentAction[50];
			static char CurrentLayerCount[50];
			static char CurrentLayer[50];
			static char CurrentAlphaModifier[50];

			sprintf_s( CurrentAction, "Action : Painting land" ); 
			textOutput.push_back( CurrentAction );

			sprintf_s( CurrentLayerCount, "Layer count: %i", layerCount );
			textOutput.push_back( CurrentLayerCount );

			if( layerCount )
			{
				sprintf_s( CurrentLayer, "Layer: %s", layers[currentLayer].name.c_str() );
				textOutput.push_back( CurrentLayer );

				sprintf_s( CurrentAlphaModifier, "Alpha modifier: %3.2f", textureA * 255 );
				textOutput.push_back( CurrentAlphaModifier );
			}

	}
// --------------------------------------------

	void saveTextures(char *filename)
	{
			std::fstream file;

		// otworzenie pliku w trybie "nadpisz" + binarny

			file.open( filename, std::ios::out | std::ios::app | std::ios::binary );

		// ustawienie markera poczatkowego

			file << "\nTextures\n";

		// zapis licznika tekstur

			file << "\t" << layerCount << "\n";

			for(int i = 0; i < MAX_LAYERS; i++)
			{
			// zapis nazwy tekstury

				file << "\t" << layers[i].name << "\n";

			// zapis kolorow warstwy

				int currentVertex;

				for(int z = 0; z < MAP_Z; z++)
				{
					for(int x = 0; x < MAP_X; x++)
					{
						currentVertex = x + MAP_X*z;

						file //<< "\t" << layers[i].colorArray[currentVertex][0]
							 //<< "\t" << layers[i].colorArray[currentVertex][1]
							// << "\t" << layers[i].colorArray[currentVertex][2]
							 << "\t" << layers[i].colorArray[currentVertex][3] << " ";
					}

					file << "\n";
				}
			}

		// ustawienie markera koncowego

			file << "End\n\n";

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();
	}

	bool loadTextures(char *filename)
	{
			std::fstream file;

		// otworzenie pliku w trybie binarnym

			file.open( filename, std::ios::in | std::ios::binary );

			if( !file )
			{
				return false;
			}

		// przyszykowanie glownych zmiennych

			int oldLayerCount = layerCount;

			TextureLayer *copy = new TextureLayer[MAX_LAYERS];

			char buffer[MAX_PATH];

			strcpy_s( buffer, "Save file \"" );
			strcat_s( buffer, filename );
			strcat_s( buffer, "\" is corrupted!\n\n" );

			std::string errorInfo = buffer;

			bool markerFound      = false,
				 loadingCommenced = false,
				 loadingCorrect   = true;

			std::string line;

		// czytanie pliku

			while( !file.eof() )
			{
			// pobranie wiersza ze strumienia

				file >> line;

			// marker check

				if( line == "Textures" )
				{
					if( !markerFound )
					{
						markerFound = true;
					}
					else
					{
						loadingCorrect = false;
						errorInfo += "There are two or more texture info markers (should be one).";
						break;
					}
				}

			// jezeli znaleziono marker, mozna odczytywac dane

				if( markerFound  && !loadingCommenced )
				{
				// ustawienie falgi

					loadingCommenced = true;

				// licznik wyrazow

					int wordCounter = 0;

				// wczytanie liczby warstw
	
					file >> line;

					if( line == "End"  )
					{
						loadingCorrect = false;

						char buffer[MAX_PATH], buffer2[MAX_PATH];

						strcpy_s( buffer, "\"End\" marker found too early, while loading texture layers.\nWords expected: " );
						_itoa_s( 1+(3+(MAP_X*MAP_Z))*MAX_LAYERS, buffer2, 10 );
						strcat_s( buffer2, "\n" );
						strcat_s( buffer, buffer2 );
						strcat_s( buffer, "Words registered: " );
						_itoa_s( wordCounter, buffer2, 10 );
						strcat_s( buffer, buffer2 );

						errorInfo +=  buffer;
											
						break;
					}
					else if( !IsNumber(line, true) )
					{
						loadingCorrect = false;
						errorInfo += "Word where layerCount value should be, isn't a number!\n\n";
						errorInfo += "\"";
						errorInfo += line;
						errorInfo += "\"";
						break;
					}
					else
					{
						layerCount = atoi(line.c_str());
						wordCounter++;
					}

				// wczytanie danych warstw tekstur

					int currentVertex;

					for(int i = 0; i < MAX_LAYERS; i++)
					{
					// zabezpieczenie

						if( !loadingCorrect )
							break;

					// zarezerwowanie pamieci dla tablicy kolorow

						assert( copy[i].colorArray == NULL );
						copy[i].colorArray = new float [MAP_X*MAP_Z][4];

					// pobranie nazwy warstwy (sa w niej spacje + zawsze powinna byc 3 czlonowa)

						char buffer[MAX_PATH], buffer2[MAX_PATH], buffer3[MAX_PATH];

						file >> buffer;
						file >> buffer2;
						file >> buffer3;

						strcat_s( buffer, " " );
						strcat_s( buffer, buffer2 );
						strcat_s( buffer, " " );
						strcat_s( buffer, buffer3 );

					// sprawdzenie poprawnosci skladni pisowni nazwy tekstury

						if( ( strcmp( &buffer[0], "0" ) < 0 )  || 
							( strcmp( &buffer[0], "9" ) > 0 )  ||  // pierwszy znak musi byc liczba

							buffer[1] != ' ' || 
							buffer[2] != '-' ||
							buffer[3] != ' ' )
						{
							if( strcmp( buffer, TEXTURE_NOT_SET.c_str() ) )
							{
								loadingCorrect = false;

								errorInfo += "Texture layer name is not correct: ";
								errorInfo += "\"";
								errorInfo += buffer;
								errorInfo += "\"";

								break;
							}
						}
						else if( strcmp( buffer, "End" ) == 0 )
						{
							loadingCorrect = false;

							char buffer[MAX_PATH], buffer2[MAX_PATH];

							strcpy_s( buffer, "\"End\" marker found too early, while loading texture layers.\nWords expected: " );
							_itoa_s( 1+(3+(MAP_X*MAP_Z))*MAX_LAYERS, buffer2, 10 );
							strcat_s( buffer2, "\n" );
							strcat_s( buffer, buffer2 );
							strcat_s( buffer, "Words registered: " );
							_itoa_s( wordCounter, buffer2, 10 );
							strcat_s( buffer, buffer2 );

							errorInfo += buffer;
						}
						else
						{
							copy[i].name = buffer;
							wordCounter += 3;
						}

						for(int z = 0; z < MAP_Z; z++)
						for(int x = 0; x < MAP_X; x++)
						{
							if( !loadingCorrect )
								break;

							currentVertex = x + MAP_X*z;

						// pobranie wartosci alfa

							file >> line;

							if( line == "End"  )
							{
								loadingCorrect = false;

								char buffer[MAX_PATH], buffer2[MAX_PATH];

								strcpy_s( buffer, "\"End\" marker found too early, while loading texture layers.\nWords expected: " );
								_itoa_s( 1+(3+(MAP_X*MAP_Z))*MAX_LAYERS, buffer2, 10 );
								strcat_s( buffer2, "\n" );
								strcat_s( buffer, buffer2 );
								strcat_s( buffer, "Words registered: " );
								_itoa_s( wordCounter, buffer2, 10 );
								strcat_s( buffer, buffer2 );

								errorInfo += buffer;
													
								break;
							}
							else if( !IsNumber(line, false) )
							{
								loadingCorrect = false;
								errorInfo += "Word where alpha value should be, isn't a number: ";
								errorInfo += "\"";
								errorInfo += line;
								errorInfo += "\"";
								break;
							}
							else
							{
								copy[i].colorArray[currentVertex][0] =
								copy[i].colorArray[currentVertex][1] =
								copy[i].colorArray[currentVertex][2] = 0.6f;

								copy[i].colorArray[currentVertex][3] = atof(line.c_str());
								wordCounter++;
							}
		
						}// podwojny for X, Z

					}// for idacy przez warstwy

				// sprawdzenie czy nastepny wiersz to "End"

					if( loadingCorrect )
					{
						file >> line;

						if( line != "End" )
						{
							loadingCorrect = false;
							errorInfo +=  "Didn't found \"End\" marker after loading texture layers (no marker or superflous characters).";			
							break;
						}
				}

				}// if( markerFound  && !loadingCorrect  )

			}// while( !file.eof() )

		// zamkniecie i wyczyszczenie strumienia

			file.close();
			file.clear();


			if( !markerFound )
			{
				loadingCorrect = false;
				errorInfo += "\"Textures\" marker hasn't been found!";
			}

		// jezeli ladowanie odbylo sie pomyslnie

			if( loadingCorrect )
			{
				assert( copy != NULL );

			// wyczyszczenie combo box'a z warstwami

				HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

				int size = ComboBox_GetCount(comboBox);

				for(int i = 0; i < size; i++)
				{
					ComboBox_DeleteString(comboBox, 0);
				}

			// ustawienie nowych nazw

				for(int i = 0; i < layerCount; i++)
				{
					ComboBox_AddString(comboBox, copy[i].name.c_str());
				}

				ComboBox_SetCurSel(comboBox, 0);

			// skopiowanie tablicy kolorow z kopii pomocniczej

				int currentVertex;

				for(int i = 0; i < MAX_LAYERS; i++)
				{
				// skopiowanie nazwy warstwy

					layers[i].name = copy[i].name;

				// odnalezienie nazwy tekstury w wektorze tekstur, aby skopiowac ID tekstury

					if( i < layerCount )
					{
					// skopiowanie nazwy warstwy (bez indeksu etc.)

						char textureName[MAX_PATH];

						for(int j = 4; j < layers[i].name.size(); j++)
						{
							textureName[j-4] = layers[i].name[j];

						// przesuwanie null'a

							textureName[j-3] = 0;
						}

					// odnalezienie nazwy tekstury w wektorze tekstur

						std::string itemName;
						bool trigger;
						std::string::size_type result;

						bool itemFound = false;
						int  index;

						for(int j = 0; j < textureArray.size(); j++)
						{
							itemName.insert(0, textureArray[j].filename, 0, MAX_PATH);

							trigger = false;
							
						// "obranie" sciezki do pliku z tekstura, wylacznie do samej nazwy (bez rozszerzenia)

							while( !trigger )
							{
								result = itemName.find('\\');

								if(result != std::string::npos)
								{
									itemName.erase(0, result+1);
								}
								else
								{
									result = itemName.rfind('.');
									itemName.erase(result, std::string::npos);

									trigger = true;
								}
							}

						// jezeli znaleziono, nie ma sensu dalej prowadzic petli
						// (niemozliwe aby byly 2 tekstury o identycznej nazwie.
						// loader tekstur "krzyczy" kiedy jest kolizja w nazwach.)

							if( strcmp( itemName.c_str(), textureName ) == 0 )
							{
								itemFound = true;
								index = j;
								break;
							}
						}

					// ustawienie ID tekstury

						if( itemFound )
						{
							layers[i].textureID = textureArray[index].textureID;
						}
						else
						{
							char buffer[MAX_PATH];

							strcpy_s( buffer, "Texture name " );
							strcat_s( buffer, itemName.c_str() );
							strcat_s( buffer, " hasn't been found in textureArray register.\n\n Texture ID for this layer has been set to 0 (untextured)." );

							MessageBox(g_hwnd, buffer, "Error!", MB_OK);

							layers[i].textureID = 0;
						}

					}// if( i < layerCount )

				// skopiowanie wartosci kolorow

					for(int z = 0; z < MAP_Z;      z++)
					for(int x = 0; x < MAP_X;      x++)
					{
						currentVertex = x + MAP_X*z;
						
						for(int p = 0; p < 4; p++)
						{
							layers[i].colorArray[currentVertex][p] = copy[i].colorArray[currentVertex][p];
						}
					}

				}// for(int i = 0; i < MAX_LAYERS; i++)

			// wyczyszczenie i utworzenie nowych VBO kolorow

				for(int i = 0; i < MAX_LAYERS; i++)
				{
					if( layers[i].VBOvc != 0 )
						glDeleteBuffersARB(1, &layers[i].VBOvc);

					glGenBuffersARB(1, &layers[i].VBOvc);

					glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[i].VBOvc);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[i].colorArray, GL_DYNAMIC_DRAW_ARB);
				}

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
			}
			else
			{
			// przywrocenie dawnego licznika warstw

				layerCount = oldLayerCount;

			// poinformowanie user'a o naturze bledu

				MessageBox(g_hwnd, errorInfo.c_str(), "Error!", MB_OK);
			}

		// wyczyszczenie pomocniczego wskaznika z tablica warstw

			if( copy != NULL )
				delete [] copy;


			return loadingCorrect;
	}

// --------------------------------------------

	unsigned int generateTerrainTexture(unsigned char *bits, unsigned int &width, unsigned int &height)
	{
	//wygeneruj nowy obiekt tekstury oraz zbinduj go

		glGenTextures(1, &landBrush);
		glBindTexture(GL_TEXTURE_2D, landBrush);


	// okreslenie parametrow tekstury

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


	// wygenerowanie mipmapy

		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height,
					  GL_RGBA, GL_UNSIGNED_BYTE, bits );


		glBindTexture(GL_TEXTURE_2D, 0);


		return landBrush;
	}




	bool loadTerrainTexture(char *filename)
	{

			bool originalTexture;   //nie ma sensu generowac "nowej" tekstury

			// sprawdzenie "oryginalnosci" tekstury
			if( textureArray.empty() )
				originalTexture = true;
			else
			{
				originalTexture = true;

				for(int i = 0; i < textureArray.size(); i++)
				{
					if( strcmp(filename, textureArray[i].filename) == 0 )
					{
						originalTexture = false;
						break;
					}
				}
			}
			
		// nowa tekstura? no to dodajemy...

			if( originalTexture )
			{
			// ladowanie tekstury		
				
				try
				{
					LoadTexture(filename, TRUE, generateTerrainTexture);
				}
				catch(std::string error)
				{
					MessageBox(g_hwnd, error.c_str(), "Error!", MB_OK);
					return false;
				} 

			// dodaj nowy obiekt tekstury + sciezke do wektora tekstur

				textureObj newTexture;

				strcpy_s(newTexture.filename, filename);
				newTexture.textureID = landBrush;


				textureArray.push_back(newTexture);

			// dodaj nowa pozycje do IDC_TERRAIN_COMBO

				std::string newItemName;
				newItemName.insert(0, filename, 0, MAX_PATH);

				bool trigger = false;
				std::string::size_type result;


				while( !trigger )
				{
					result = newItemName.find('\\');

					if(result != std::string::npos)
					{
						newItemName.erase(0, result+1);
					}
					else
					{
						result = newItemName.rfind('.');
						newItemName.erase(result, std::string::npos);

						trigger = true;
					}
				}

			// dodanie do combobox'a

				HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_COMBO);

				ComboBox_AddString(comboBox, newItemName.c_str());

			// zerowanie brusha

				landBrush = 0;
			}

			return originalTexture;
	}




	int  loadTerrainTextures()
	{
		// lista nazw plikow do zaladowania
			std::vector < std::string > files;

		// uchwyt do folderu

			HANDLE directory;

		// struktura z danymi pliku/folderu

			WIN32_FIND_DATAA data;

		// bool okreslajacy czy znaleziono plik

			bool FoundFile;

		// Ladowanie BMP

			// otworzenie folderu i pobranie pierwszego pliku

				directory = FindFirstFileA("TerrainData\\Textures\\Terrain\\*.bmp", &data);

			// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

				if( directory != INVALID_HANDLE_VALUE )
				{
					files.push_back(data.cFileName);

					FoundFile = true;

				// petla pobierajaca i zapisujaca nazwy plikow w folderze
					while( FoundFile )
					{
						FoundFile = FindNextFileA(directory, &data);

						if( FoundFile )
							files.push_back(data.cFileName);
					}
				}

		// Ladowanie JPG

			// otworzenie folderu i pobranie pierwszego pliku

				directory = FindFirstFileA("TerrainData\\Textures\\Terrain\\*.jpg", &data);

			// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

				if( directory != INVALID_HANDLE_VALUE )
				{
					files.push_back(data.cFileName);

					FoundFile = true;

				// petla pobierajaca i zapisujaca nazwy plikow w folderze
					while( FoundFile )
					{
						FoundFile = FindNextFileA(directory, &data);

						if( FoundFile )
							files.push_back(data.cFileName);
					}
				}

		// Ladowanie TGA

			// otworzenie folderu i pobranie pierwszego pliku

				directory = FindFirstFileA("TerrainData\\Textures\\Terrain\\*.TGA", &data);

			// zebezpieczenie, jezeli nie znaleziono jakiegokolwiek pliku o tym rozszerzeniu

				if( directory != INVALID_HANDLE_VALUE )
				{
					files.push_back(data.cFileName);

					FoundFile = true;

				// petla pobierajaca i zapisujaca nazwy plikow w folderze
					while( FoundFile )
					{
						FoundFile = FindNextFileA(directory, &data);

						if( FoundFile )
							files.push_back(data.cFileName);
					}
				}
				

		// przygotowanie stringa ze sciezka do aplikacji

			int argCount;
		    LPWSTR *szArgList = CommandLineToArgvW((LPCWSTR)GetCommandLineA(), &argCount);

			std::string programFolder;
			programFolder.insert(0, (const char*)szArgList[0], 0, MAX_PATH);

			programFolder.erase( programFolder.rfind('\\')+1, std::string::npos );  // usuniecie nazwy aplikacji
			programFolder.erase( programFolder.begin() );							// usuniecie pierwszego znaku (cudzyslow)

		// licznik zaladowanych tekstur

			int loadedTextureCounter = 0;

		// ladowanie tekstur na podstawie wektora nazw

			for(int i = 0; i < files.size(); i++)
			{

			// zmienna ktora bedzie przechowywac sciezke do pliku z tekstura

				char szFileName[MAX_PATH] = "";

			// przygotowanie nazwy pliku z tekstura

				strcpy_s( szFileName, programFolder.c_str() );
				strcat_s( szFileName, "TerrainData\\Textures\\Terrain\\" );
				strcat_s( szFileName, files[i].c_str() );


			// Ladowanie tekstury o nazwie szFileName

				if( loadTerrainTexture(szFileName) )
					loadedTextureCounter++;

			}

			return loadedTextureCounter;
	}


	void reloadTerrainTextures()
	{
		int newTextures = loadTerrainTextures();

		char buffer[MAX_PATH] = "";

		_itoa_s( newTextures, buffer, 10 );

		if( newTextures == 1 )
			strcat_s( buffer, " new texture loaded!" );
		else
			strcat_s( buffer, " new textures loaded!" );

		MessageBox(g_hwnd, buffer, "Reloading finished.", MB_OK);
	}


	void changeTerrainTexture()
	{

			std::string cbIndexText;
			char buffer[MAX_PATH] = "";

			HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_COMBO);
			ComboBox_GetText(comboBox, buffer, MAX_PATH);

			cbIndexText.insert(0, buffer);

			if( !cbIndexText.empty() )
			{

				for(int i = 0; i < textureArray.size(); i++)
				{
					std::string helper;
					helper.insert(0, textureArray[i].filename);

					bool trigger = false;
					std::string::size_type result;

					while(!trigger)
					{
						result = helper.find('\\');

						if(result != std::string::npos)
						{
							helper.erase(0, result+1);
						}
						else
						{
							result = helper.rfind('.');
							helper.erase(result, std::string::npos);

							trigger = true;
						}
					}


					if(cbIndexText == helper)
					{
						//landBrush = textureArray[i].textureID;

						// ustaw obrazek ...

							HBITMAP hBitmap;
							hBitmap = (HBITMAP)LoadImage(NULL, textureArray[i].filename, IMAGE_BITMAP, 128, 128,
							LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

							HWND hPicture = GetDlgItem(textures, IDC_TERRAIN);
							SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); 

						break;
					}
				}

			}// !cbIndexText.empty()
			else
			{
				HWND hPicture = GetDlgItem(textures, IDC_TERRAIN);
				SendMessage(hPicture, STM_SETIMAGE, IMAGE_BITMAP, NULL); 
			}

	}



	void removeTerrainTextures()
	{
		for(int i = 0; i < textureArray.size(); i++)
		{
			glDeleteTextures(1, &textureArray[i].textureID);
		}
	}



// --------------------------------------------

	void ResetTerrainTextures(int oldMAP_X, int oldMAP_Z)
	{
		// wyczyszczenie combo box'a z warstwami tekstur

			HWND comboBox = GetDlgItem(textures, IDC_TERRAIN_LAYER_COMBO);

			int size = ComboBox_GetCount(comboBox);

			for(int i = 0; i < size; i++)
			{
				ComboBox_DeleteString(comboBox, 0);
			}

		// odswiezenie rozmiarow tablic kolorow warstw tekstur

			float helper = MAP_Z;

			MAP_Z = oldMAP_Z;
			resizeColorArrays(oldMAP_X, 0);

			MAP_Z = helper;
			resizeColorArrays(0, oldMAP_Z);

		// ustawienie domyslnych wartosci w warstwach tekstur

			for(int i = 0; i < MAX_LAYERS; i++)
			{
			// zerowanie ID tekstury

				layers[i].textureID = 0;

			// reset glownych zmiennych informujacych o stanie warstw

				layerCount   = 0;
				currentLayer = 1;

			// ustawienie domyslnych wartosci kolorow

				int currentVertex;

				for(int z = 0; z < MAP_Z; z++)
				for(int x = 0; x < MAP_X; x++)
				{
					currentVertex = x + MAP_X*z;

					layers[i].colorArray[currentVertex][0] =
					layers[i].colorArray[currentVertex][1] =
					layers[i].colorArray[currentVertex][2] = .6f;

					if( i == 0 )
						layers[i].colorArray[currentVertex][3] = 1.f;
					else
						layers[i].colorArray[currentVertex][3] = 0.f;
				}
			}

	}


	bool InitTerrainTextures()
	{

		// utworzenie tablicy warstw

			layers = new TextureLayer[MAX_LAYERS];
		
		// utworzenie tablic kolorow oraz VBO

			for(int i = 0; i < MAX_LAYERS; i++)
			{
			// rezerwacja pamieci dla nowej tablicy kolorow

				assert( layers[i].colorArray == NULL );
				layers[i].colorArray = new float [MAP_X*MAP_Z][4];

			// wypelnianie tablicy kolorow

				int currentVertex;

				for(int z = 0; z < MAP_Z; z++)
				for(int x = 0; x < MAP_X; x++)
				{
					currentVertex = x  + MAP_X*z;

					layers[i].colorArray[currentVertex][0] = 
					layers[i].colorArray[currentVertex][1] = 
					layers[i].colorArray[currentVertex][2] = .6f;

				// jezeli jest to tablica kolorow warstwy bazowej ustaw alphe 1 (bazowa tekstura zawsze ma pelna wiodcznosc)

					if( i == 0 )
					{
						layers[i].colorArray[currentVertex][3] = 1.f;
					}
					else
					{
						layers[i].colorArray[currentVertex][3] = 0.f;
					}
				}

			// utworzenie VBO

				assert( layers[i].VBOvc == 0 );

				glGenBuffersARB(1, &layers[i].VBOvc);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, layers[i].VBOvc);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAP_X*MAP_Z*4*sizeof(float), layers[i].colorArray, GL_DYNAMIC_DRAW_ARB);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
			}


			return true;
	}

	void CloseTerrainTextures()
	{
		removeTerrainTextures();

		delete [] layers;
	}