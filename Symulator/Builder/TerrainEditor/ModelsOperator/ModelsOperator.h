#pragma once


	#include "..//..//ObjectOperator//ObjectOperator.h"

	#include "Model//Model.h"



	extern  HWND g_hwnd;

	// Zmienne myszy, klawiatury i kamery

	extern  int		g_mouseX, g_mouseY,    
					oldMouseX ,
					oldMouseY,

					wheelMovement,	    
					mouseWheelSteps;


	extern bool		altPressed ,
					ctrlPressed,

					deletePressed,

					lButtonDown ,
					rButtonDown,
		
					lButtonDblClk,
					rButtonDblClk,

					mouseMoved; 


	

	extern  PFNGLGENBUFFERSARBPROC	         glGenBuffersARB;           // VBO Name Generation Procedure
	extern  PFNGLBINDBUFFERARBPROC			 glBindBufferARB;           // VBO Bind Procedure
	extern  PFNGLBUFFERDATAARBPROC			 glBufferDataARB;           // VBO Data Loading Procedure
	extern  PFNGLBUFFERSUBDATAARBPROC		 glBufferSubDataARB;        // VBO Sub Data Loading Procedure
	extern  PFNGLDELETEBUFFERSARBPROC		 glDeleteBuffersARB;        // VBO Deletion Procedure
	extern  PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB; // return various parameters of VBO
	extern  PFNGLMAPBUFFERARBPROC			 glMapBufferARB;            // map VBO procedure
	extern  PFNGLUNMAPBUFFERARBPROC			 glUnmapBufferARB;      


	extern	int MAP_SCALE;

	extern  std::vector <std::string> textOutput;

	extern  bool Saved;




	struct modelTexture_DataObject
	{
		char	        filename[MAX_PATH];
		unsigned int    textureID;
	};
//
	class model_DataObject
	{
		public:

			unsigned int vboIDv,
						 vboIDvt,
						 vboIDvn;

			std::vector <Mesh> meshVector;
			float sizeX, sizeY, sizeZ;

			char filename[MAX_PATH];


			model_DataObject(char *filename)
			{
				strcpy_s(this->filename, filename);
			}

			~model_DataObject()
			{}

		//
			void CalculateModelSize (float (*vertexArray)[3], int const &vertexCount)
			{
				assert( vertexArray != NULL && vertexCount > 0 );


				float minX, minY, minZ, 
					  maxX, maxY, maxZ,

					  x, y, z;

				minX = maxX = vertexArray[0][0];
				minY = maxY = vertexArray[1][0];
				minZ = maxZ = vertexArray[2][0];


				for(int i = 0; i < vertexCount; i++)
				{
					x = vertexArray[i][0];
					y = vertexArray[i][1];
					z = vertexArray[i][2];

					if( maxX < x )	maxX = x;
					if( minX > x )	minX = x;

					if( maxY < y )	maxY = y;
					if( minY > y )	minY = y;

					if( maxZ < z )	maxZ = z;
					if( minZ > z )	minZ = z;
				}

				sizeX = (maxX - minX) / 2.f;
				sizeY = maxY - minY;
				sizeZ = (maxZ - minZ) / 2.f;
			}

			int GetIndexCount ()
			{
				int indexCount = 0;

				for(int i = 0; i < meshVector.size(); i++)
				{
					indexCount += meshVector[i].indexCounter;
				}

				return indexCount;
			}
	};
//

	class ModelsOperator
	{
		public:

		// Pola

			HWND		 models;

			unsigned int modelBrush;				// okresla id tekstury, ktora sie naklada na model

			char		 currentModel[MAX_PATH];	// nazwa aktualnego modelu, ktory mozna klasc na mape
	
			unsigned int modelCounter;				// licznik modeli (jezeli 0, to pewne akcje sie nie wykonuje w ObjectOperator.h)

			bool		 ObstacleCheck;				// jezeli true, to zaznaczony/postawiony SimpleModel bedzie mial ustawiona flage IsObstacle na true


			std::vector <modelTexture_DataObject> modelTextureArray;
			std::vector <model_DataObject*>		  modelArray;


		// Metody

			static ModelsOperator& Singleton()
			{
				static ModelsOperator s;
				return s;
			}
		//
			static unsigned int generateModelTexture(unsigned char *bits, unsigned int &width, unsigned int &height)
			{
				glGenTextures(1, &ModelsOperator::Singleton().modelBrush);
				glBindTexture(GL_TEXTURE_2D, ModelsOperator::Singleton().modelBrush);
			
	
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);//_MIPMAP_LINEAR);

				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


				gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height,
							  GL_RGBA, GL_UNSIGNED_BYTE, bits );


				glBindTexture(GL_TEXTURE_2D, 0);


				return ModelsOperator::Singleton().modelBrush;
			}
		//
			

			//void placeModel     ();
			//void removeAllModels();


			bool loadModel          (char *filename) throw(std::string);
				private:

				inline bool ModelIsOriginal (char *filename);

				inline model_DataObject* ComposeModelDataObjectFromObjectStructure (char *filename, OBJstruct *OBJ); 

				inline bool ObjectLacksNormals (OBJstruct *OBJ);
				inline void CalculateNormals   (OBJstruct *OBJ);

				inline std::vector <Mesh> ExtractMeshData (OBJstruct *OBJ);

				inline unsigned int GenerateVertexVBO   (int const &indexCount, float (*vertexArray)[3]);
				inline unsigned int GenerateTexCoordVBO (int const &indexCount, float (*texCoordArray)[2]);
				inline unsigned int GenerateNormalVBO   (int const &indexCount, float (*normalArray)[3]);

			//	inline void AddNewModelsNameToModelComboBox (char *filename);

			public:

			int  loadModels         ();
		//	void reloadModels       ();
		//	void changeModel        ();
			void removeModelObjects ();

		
			//static void scaleModel      (float &, float &, float &, float &sizeX, float &sizeY, float &sizeZ, void *object)
			//{
			//	Model *model = reinterpret_cast <Model*> (object);

			//// przywrocenie oryginalnych rozmiarow ramki (zeby nizej potem poprawnie przeskalowac -> zeby skala sie nie "stackowala")

			//	model->sizeX /= model->scale;
			//	model->sizeY /= model->scale;
			//	model->sizeZ /= model->scale;

			//	sizeX = model->sizeX;
			//	sizeY = model->sizeY;
			//	sizeZ = model->sizeZ;


			//// pobranie modyfikatora skali modelu

			//	model->scale = (float)GetDlgItemInt(ModelsOperator::Singleton().models, IDC_MODEL_SCALE, FALSE, FALSE) / 10000.f;
			//	
			//	
			//// drobne zabezpieczenie (nigdy nie wiadomo...)

			//	if( model->scale == 0.f )
			//	{
			//		model->scale = 0.1f;
			//	}

			//// przemnozenie rozmiarow obiektu

			//	model->sizeX *= model->scale;
			//	model->sizeY *= model->scale;
			//	model->sizeZ *= model->scale;

			//	sizeX = model->sizeX;
			//	sizeY = model->sizeY;
			//	sizeZ = model->sizeZ;
			//}

			static bool preModel        ()
			{
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_CULL_FACE);
				glEnable(GL_BLEND);
				glEnable(GL_ALPHA_TEST)	;

				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);

				return true;
			}


			static void drawModel       (float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ,  void *object, bool Selected)
			{
				Model *model = reinterpret_cast <Model*> (object);


				glColor4f(model->R, model->G, model->B, model->A);

				glPushMatrix();

					glTranslatef(x, y, z);

					glScalef(model->scale, model->scale, model->scale);

					glRotatef(model->rotateX, 1.f, 0.f, 0.f);
					glRotatef(model->rotateY, 0.f, 1.f, 0.f);
					glRotatef(model->rotateZ, 0.f, 0.f, 1.f);

				// rysowanie VBO
					
					glBindBufferARB(GL_ARRAY_BUFFER_ARB, model->vboIDv);
					glVertexPointer(3, GL_FLOAT, 0, 0);

					glBindBufferARB(GL_ARRAY_BUFFER_ARB, model->vboIDvt);
					glTexCoordPointer(2, GL_FLOAT, 0, 0);

					glBindBufferARB(GL_ARRAY_BUFFER_ARB, model->vboIDvn);
					glNormalPointer(GL_FLOAT, 0, 0);


					int startingIndex = 0;

					for(int i = 0; i < model->meshVector.size(); i++)
					{
						glBindTexture(GL_TEXTURE_2D, model->meshVector[i].textureID);
						glDrawArrays(GL_TRIANGLES, startingIndex, model->meshVector[i].indexCounter);

						startingIndex += model->meshVector[i].indexCounter;
					}
				
				glPopMatrix();
			}

			static void postModel       ()
			{
				glBindTexture(GL_TEXTURE_2D, 0);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);

				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
				glEnable(GL_ALPHA_TEST)	;
			}


			//static void moveModel       (float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
			//{
			//	//SimpleModel *simpleModel = reinterpret_cast < SimpleModel* > (object);
			//}


			//static void modelKeyManager (char key, void *object, float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ)
			//{
			//	Model *model = reinterpret_cast <Model*> (object);


			//	#define	ROTATE_SPEED 5


			//	switch( toupper(key) )
			//	{
			//	// obracanie modelem

			//		case 'Q':	
			//		{
			//			model->rotateY -= ROTATE_SPEED;	

			//			if( model->rotateY > 360 || model->rotateY < -360 )
			//				model->rotateY = 0;

			//			Saved = false;
			//		}
			//		break;
			//		case 'E':	
			//		{
			//			model->rotateY += ROTATE_SPEED;	

			//			if( model->rotateY > 360 || model->rotateY < -360 )
			//				model->rotateY = 0;

			//			Saved = false;
			//		}
			//		break;


			//		case 'R':	
			//		{
			//			model->rotateX -= ROTATE_SPEED;	

			//			if( model->rotateX > 360 || model->rotateX < -360 )
			//				model->rotateX = 0;

			//			Saved = false;
			//		}
			//		break;
			//		case 'F':	
			//		{
			//			model->rotateX += ROTATE_SPEED;	

			//			if( model->rotateX > 360 || model->rotateX < -360 )
			//				model->rotateX = 0;

			//			Saved = false;
			//		}
			//		break;


			//		case 'T':	
			//		{
			//			model->rotateZ -= ROTATE_SPEED;	

			//			if( model->rotateZ > 360 || model->rotateZ < -360 )
			//				model->rotateZ = 0;

			//			Saved = false;
			//		}
			//		break;
			//		case 'G':	
			//		{
			//			model->rotateZ += ROTATE_SPEED;	

			//			if( model->rotateZ > 360 || model->rotateZ < -360 )
			//				model->rotateZ = 0;

			//			Saved = false;
			//		}
			//		break;

			//	

			//	// odswiezanie atrybutow modelu

			//		case 'L':
			//		{
			//			model->IsObstacle = ModelsOperator::Singleton().ObstacleCheck;

			//			model->R = (float)GetDlgItemInt(ModelsOperator::Singleton().models, IDC_MODEL_RED,   FALSE, FALSE) / 255.f;
			//			model->G = (float)GetDlgItemInt(ModelsOperator::Singleton().models, IDC_MODEL_GREEN, FALSE, FALSE) / 255.f;
			//			model->B = (float)GetDlgItemInt(ModelsOperator::Singleton().models, IDC_MODEL_BLUE,  FALSE, FALSE) / 255.f;
			//			model->A = (float)GetDlgItemInt(ModelsOperator::Singleton().models, IDC_MODEL_ALPHA, FALSE, FALSE) / 255.f;


			//		// zmiana skali (jezeli uzytkownik wcisnie kontrol i ruszy scrollem)

			//			if(wheelMovement < 0)
			//			{
			//				int additive = 0;
			//				for(int i = 0; i > mouseWheelSteps; i--)
			//					additive -= 100;

			//			// zabezpieczenie

			//				while( (model->scale * 10000.f) + additive < 0 )
			//				{
			//					additive++;
			//				}

			//				SetDlgItemInt(ModelsOperator::Singleton().models, IDC_MODEL_SCALE, GetDlgItemInt(ModelsOperator::Singleton().models, IDC_MODEL_SCALE, FALSE, FALSE) + additive, FALSE);

			//				wheelMovement %= WHEEL_DELTA;
			//			}
			//			else if(wheelMovement > 0)
			//			{
			//				int additive = 0;
			//				for(int i=0; i<mouseWheelSteps; i++)
			//					additive += 100;

			//				SetDlgItemInt(ModelsOperator::Singleton().models, IDC_MODEL_SCALE, GetDlgItemInt(ModelsOperator::Singleton().models, IDC_MODEL_SCALE, FALSE, FALSE) + additive, FALSE);

			//				wheelMovement %= WHEEL_DELTA;
			//			}

			//		// skalowanie modelu

			//			scaleModel(x, y, z, sizeX, sizeY, sizeZ, object);


			//			Saved = false;
			//		}


			//		default:
			//		break;
			//	}

			//	#undef ROTATE_SPEED
			//}


			static void removeModel     (void *object)
			{
				ModelsOperator::Singleton().modelCounter--;
			}
		//

			//void saveModels (char *filename);
			bool loadModels (char *filename);


			/*static void modelText(float &x, float &y, float &z, float &sizeX, float &sizeY, float &sizeZ, void *object)
			{
				Model *model = reinterpret_cast <Model*> (object);

				if( !textOutput.empty() )
				{
					textOutput.erase( textOutput.begin(), textOutput.end() );
				}

				static char CurrentAction[50];
				static char CurrentX[50];
				static char CurrentY[50];
				static char CurrentZ[50];
				static char CurrentRotateX[50];
				static char CurrentRotateY[50];
				static char CurrentRotateZ[50];
				static char CurrentScale[50];
				static char CurrentIsObstacle[50];

				sprintf_s( CurrentAction, "Action : Moving model" ); 
				textOutput.push_back( CurrentAction );


				sprintf_s( CurrentX, "X: %8.2f", x );
				textOutput.push_back( CurrentX );

				sprintf_s( CurrentY, "Y: %8.2f", y );
				textOutput.push_back( CurrentY );

				sprintf_s( CurrentZ, "Z: %8.2f", z );
				textOutput.push_back( CurrentZ );


				sprintf_s( CurrentRotateX, "Rotate X: %i", model->rotateX );
				textOutput.push_back( CurrentRotateX );

				sprintf_s( CurrentRotateY, "Rotate Y: %i", model->rotateY );
				textOutput.push_back( CurrentRotateY );

				sprintf_s( CurrentRotateZ, "Rotate Z: %i", model->rotateZ );
				textOutput.push_back( CurrentRotateZ );


				sprintf_s( CurrentScale, "Scale: %8.4f", model->scale );
				textOutput.push_back( CurrentScale );


				sprintf_s( CurrentIsObstacle, "Obstacle: %s", model->IsObstacle ? "True" : "False" );
				textOutput.push_back( CurrentIsObstacle );
			}*/
		//

			bool loadModelTexture    (char *filename);
			int  loadModelTextures   ();
			/*void reloadModelTextures ();
			void changeModelTexture  ();*/
			void removeModelTextures ();

		private:

			ModelsOperator  ();
			~ModelsOperator ();
	};
