#pragma once





	class ObjectOperator
	{

		public:

			float   heightModifier,      // sluzy do ustalania modyfikatora wysokosci, przy przemieszczaniu obiektow
					heightModifierSpeed; // ma on wplyw na modyfikator wysokosci obiektu, w trakcie zmiany wysokosci obiektow


			bool scanReady,   // okresla czy operator ma skanowac obszar (czy kliknieto w obiekt, przemieszczanie etc.)
				 
				 Stickied,    // sygnalizuje przyklejenie sie obiektu do kursora

				 Selecting;   // okresla czy operator obiektow jest w trakcie wybierania nowego obiektu
								 // (jezeli sie zaznaczylo cos, i ne puscilo lewego przycisku myszy, to ne chcemy zeby dalej sie nam obiekty zaznaczaly)

			char keyPressed;   // znak ten jest ustawiany w procedurach okienkowych.
								// sygnalizuje on potrzebe wykonania odpowiednich akcji na obiektach, ktore zinterpretuja ten znak.


		// deklaracja klasy Obiektu

			class Object
			{
				float posX, posY, posZ, 
					  sizeX, sizeY, sizeZ;


				void  *pointer;


				bool  (*preRenderer)();		// wskaznik na funkcje przygotowujaca maszyne OpenGL do rysowania jednego/wielu obiektow okreslonego typu
				void  (*renderer)(float &, float &, float &, float &, float &, float &, void *, bool);       // wskaznik na funkcje rysujaca obiekt
				void  (*postRenderer)();	// wskaznik na funkcje "sprzatajaca" ustawienia maszyny stanow OpenGL


				void  (*mover)(float &, float &, float &, float &, float &, float &, void *);	  // wskaznik na funckje przemieszczajaca obiekty (dlatego trzeba wyslac referencje
																						// nie tyle przemieszcza ona obiekt, co dokonuje specyficznych zmian wystepujacych przy przemieszczaniu
																						// okreslonych obiektow, np. dla swiatla, trzeba od nowa ustawic jego pozycje w OpenGL

				void (*remover)(void *);      // wskaznik na funkcje usuwajaca obiekt

				void (*keyManager)(char, void*, float &, float &, float &, float &, float &, float &);		// wskaznik na funkcje obslugujaca klawisze

				void (*text)(float &, float &, float &, float &, float &, float &, void *);

				void (*specific)(float &, float &, float &, float &, float &, float &, void*);		// wskaznik na funkcje specyficzna


				std::string typeName;		  // przechowuje nazwe typu obiektu (potrzebne do usuwania calej serii specyficznych obiektow)


				bool Scaled;	// okresla czy obiekt (i w tym ramka) jest skalowany wobec mapy
				bool MorphAdjustable; // okresla czy wysokosc obiektu ma byc modyfikowana, wraz ze zmianami terenu
				bool Selected;	
				bool DrawingFrame;
													

			public:

				Object( float x, float y, float z, 
						float sizX, float sizY, float sizZ, 
						void *object, 
						bool (*pre)(),
						void (*rend)(float &, float &, float &, float &, float &, float &, void *, bool), 
						void (*post)(),
						void (*mov)(float &, float &, float &, float &, float &, float &, void *),
						void (*remov)(void *),
						void (*keyMng)(char, void*, float &, float &, float &, float &, float &, float &),
						void (*txt)(float &, float &, float &, float &, float &, float &, void *),
						void (*spec)(float &, float &, float &, float &, float &, float &, void*),
						std::string typName,
						bool Scal,
						bool morphAdj,
						bool drawFrame);
				~Object();


				void Select     ();
				void Unselect   ();
				bool IsSelected ();
				
				bool CoordinatesAreInsideObject (float x, float y, float z);

			
				void*       GetObject   ();
				std::string GetType     ();
				Vector3     GetPosition ();
				Vector3     GetSize     ();


				bool InitDrawing  ();
				void DrawFrame    ();
				void Draw         ();
				void CloseDrawing ();



				void Move(float heightModifier);

				void Remove();



				void AdjustHeight(float modifier);

				void Rescale(float newScale, float oldScale);

				void ManageKey(char key);

				
				void MorphAdjust();
				bool GetMorphAdjustable();

				void SpecificFunction();


				void Text();
			};

		
		// glowne wektory, na ktorych ObjectOperator bedzie pracowac

			std::vector <Object> objectArray; // wektor z wszelkiego rodzaju obiektami na mapie (jedynie trawa nie jest obslugiwana przez ObjectOperator'a)

			struct range
			{
				unsigned int firstIndex, lastIndex;
			};
			std::vector <range> typeIndices; // wektor przechowujacy indeksy obiektow danych typow (w objectArray)

			std::vector <int> movingID;		 // wektor z indeksami aktualnie zaznaczonych / przemieszczanych obiektow


	// **** metody ****


		// dostep do singleton'a

			static ObjectOperator& Singleton()
			{
				static ObjectOperator s;
				return s;
			}
		//

		// funktor sluzacy do sortowania wektora obiektow

			static bool Functor(Object a, Object b)
			{
				return ( a.GetType() < b.GetType() );
			}
		//


		// deklaracje funkcji

			//void objectsText();	// ustawia tekst 2D, ktory ma byc wypluty  

			void Sort();	// sortuje wektor obiektow

			void Add(Object& newObject);

			//void dropObjects();

			//void objectScan();

			//void unselectObjects();

			//void moveObjects();

			void drawObjects();

			void removeObjects();


		//	void adjustObjectsHeight(float modifier);

			//void rescaleObjects(float newScale, float oldScale);

		//	bool removeSpecificObjects(std::string typeName);

			//void manageKeys();

			//void morphAdjustment();

			void executeSpecificFunctions(std::string typeName);


			void Clear(std::string name = "All");



		// * --- glowna funkcja --- * //

			void ObjectOperatorRun();


		private:

			ObjectOperator();
			~ObjectOperator();

	};
