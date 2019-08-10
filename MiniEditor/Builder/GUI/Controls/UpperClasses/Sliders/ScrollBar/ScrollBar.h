#pragma once


	#include "..//Slider//Slider.h"


	#define SLIDER_TEXTURE_COUNT			    (3)

	#define SCROLLBAR_LEFT_NAVIGATOR_PRESSED    (1+SLIDER_TEXTURE_COUNT)
	#define SCROLLBAR_LEFT_NAVIGATOR_UNPRESSED  (2+SLIDER_TEXTURE_COUNT)

	#define SCROLLBAR_RIGHT_NAVIGATOR_PRESSED   (3+SLIDER_TEXTURE_COUNT)
	#define SCROLLBAR_RIGHT_NAVIGATOR_UNPRESSED (4+SLIDER_TEXTURE_COUNT)

	#define SCROLLBAR_TEXTURE_COUNT			    (4)



	class ScrollBar: public Slider
	{
		public:

			ScrollBar();
				private:
				void SetDefaultScrollBarData();
				inline void PrepareTextureSlots ();
				inline void PrepareContent	    ();
				inline void PrepareAttributes   ();
			public:

			ScrollBar(ScrollBar const &scrollBar);
			ScrollBar& operator= (ScrollBar const &scrollBar);
				private:
				void CopyScrollBarData(ScrollBar const &scrollBar);
				inline void RefreshContentArrayPointers();
			public:

			~ScrollBar();

	/******************************************************
			Navigator managment functions.
	******************************************************/

			void SetNavigatorSize (int const &size);
			int  GetNavigatorSize ();

			void SetStep (unsigned int const &step);


			void LeftNavigator_SetPressedTexture    (unsigned int const &textureID);
			void LeftNavigator_SetUnpressedTexture  (unsigned int const &textureID);

			void RightNavigator_SetPressedTexture   (unsigned int const &textureID);
			void RightNavigator_SetUnpressedTexture (unsigned int const &textureID);


			unsigned int LeftNavigator_GetPressedTexture    ();
			unsigned int LeftNavigator_GetUnpressedTexture  ();

			unsigned int RightNavigator_GetPressedTexture   ();
			unsigned int RightNavigator_GetUnpressedTexture ();

			

		private:

			Button *LeftNavigator, 
				   *RightNavigator;

			float navigatorSize;

	/*****************************************************/

			void AdjustContentSize  ();

			inline void AdjustNavigators ();
			inline void AdjustNavigatorsVertically   ();
			inline void AdjustNavigatorsHorizontally ();

			inline void SliceOffNavigatorsFromControlBorder ();
			inline void GlueNavigatorsOntoControlBorder     ();

			void AdjustContentColor ();
			void UpdateContent ();

	/******************************************************
			Standard functions redefinition.
	******************************************************/

			void MouseLeftDown (POINT const &mousePosition);

			inline bool DetectPressedNavigators ();

				inline bool LeftNavigator_Pressed  ();
				inline bool RightNavigator_Pressed ();

	/*****************************************************/

			void Save (std::fstream &file);
			void Load (std::fstream &file);
	};