#pragma once

	#include "..//Slider//Slider.h"




	class MarkedSlider: public Slider
	{
		public:

			MarkedSlider();
				private:
				void SetDefaultMarkedSliderData();
				inline void PrepareContent    ();
				inline void PrepareAttributes ();
			public:

			MarkedSlider(MarkedSlider const &slider);
			MarkedSlider& operator= (MarkedSlider const &slider);
				private:
				void CopyMarkedSliderData(MarkedSlider const &slider);
				inline void RefreshContentArrayPointers ();
			public:

			~MarkedSlider();

	/******************************************************
			Basic managment functions.
	******************************************************/

			void SetFont (Font const &font);
			Font GetFont ();

			void SetTextColor (Color const &color);
			Color GetTextColor ();

			void SetText_A (std::string const &text);
			std::string GetText_A ();

			void SetText_B (std::string const &text);
			std::string GetText_B ();

		private:

			Label *Mark_A, *Mark_B;
			Font currentFont;
			Color textColor;

			bool RenderMarks;


	/*****************************************************/

			void AdjustContentSize  ();
			
			inline void SqueezeMarkBordersToSliderRidges       ();
			inline bool SliderIsHighEnoughToFitSingleRowOfText ();
			inline void AdjustLeftMark  ();
			inline void AdjustRightMark ();
			
			void AdjustContentColor ();
			void UpdateContent ();

	/*****************************************************/

			void Save (std::fstream &file);
			void Load (std::fstream &file);

	};