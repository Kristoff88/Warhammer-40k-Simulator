#pragma once

	#include "..//..//..//UpperClass//UpperClass.h"


	#define SLIDER_BAR				(1)

	#define SLIDER_THUMB_PRESSED	(2)
	#define SLIDER_THUMB_UNPRESSED	(3)

	#define SLIDER_TEXTURE_COUNT	(3)



	class Slider: public UpperClass
	{

		public:

			Slider();
				private:
				void SetDefaultSliderData();
			public:

			Slider(Slider const &slider);
				protected:
				void CopySliderData(Slider const &slider);
				inline void RefreshContentArrayPointers();
			public:

			~Slider();

			Slider& operator= (Slider const &slider);

	/******************************************************
			Basic managment functions.
	******************************************************/

			void SetStep (unsigned int const &step);
			int GetStep ();

			void SetRange (unsigned int const &range);
			int GetRange ();

			void SetVerticalAlignment   ();
			void SetHorizontalAlignment ();

			bool IsVerticallyAligned   ();
			bool IsHorizontallyAligned ();


			void SetBarTexture (unsigned int const &textureID);
			unsigned int GetBarTexture ();

			void Slider_SetPressedTexture (unsigned int const &textureID);
			unsigned int Slider_GetPressedTexture ();

			void Slider_SetUnpressedTexture (unsigned int const &textureID);
			unsigned int Slider_GetUnpressedTexture ();


	/*****************************************************/

			void AdjustContentSize  ();

			void CalculateStep ();
			inline void CalculateStepHorizontally ();
			inline void CalculateStepVertically   ();

			void AdjustThumb ();
			inline void AdjustThumbHorizontally ();
			inline void AdjustThumbVertically   ();

			void AdjustContentColor ();
			void UpdateContent ();


		protected:

			Control *Bar;
			Button *Thumb;

			int range;
			int step;
			POINT thumbPosition;

			bool Vertical;

	/*****************************************************/

			void MouseLeftDown (POINT const &mousePosition);
			inline bool CursorHitSliderBar (POINT const &mousePosition);
			inline bool CursorHitVerticalSliderBar   (POINT const &mousePosition);
			inline bool CursorHitHorizontalSliderBar (POINT const &mousePosition);

			void MouseDragged  (POINT const &mousePosition);

	/*****************************************************/

			void Save (std::fstream &file);
			void Load (std::fstream &file);

	};