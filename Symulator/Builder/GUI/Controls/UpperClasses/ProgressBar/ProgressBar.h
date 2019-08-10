#pragma once

	#include "..//..//UpperClass//UpperClass.h"


	#define BAR_TEXTURE		          (1)
	#define BAR_TEXTURE_COUNT		  (1)



	class ProgressBar: public UpperClass
	{
		public:

			ProgressBar();
				private:
				void SetDefaultProgressBarData();

				inline void PrepareTextureSlots ();
				inline void PrepareContent		();
				inline void PrepareAttributes   ();
			public:

			ProgressBar(ProgressBar const &progressBar);
			ProgressBar& operator= (ProgressBar const &progressBar);
				private:
				void CopyProgressBarData(ProgressBar const &progressBar);

				inline void CopyContent		            (ProgressBar const &progressBar);
				inline void CopyAttributes              (ProgressBar const &progressBar);
				inline void RefreshContentArrayPointers ();
			public:

			~ProgressBar();

	/******************************************************
			Basic managment functions.
	******************************************************/

			void SetBarTexture (unsigned int const &textureID);
			unsigned int GetBarTexture ();
			
			void Increment (int const &step = 1);
			void Decrement (int const &step = 1);
			
			void SetRange (unsigned int const &range);
			void SetStep  (unsigned int const &step);

			int GetRange ();
			int GetStep  ();

			void SetFont  (Font &font);
			void SetTextColor (Color const &color);

			void ShowPercentage (bool const &show);

			void SetTransparency (bool const &IsTransparent);

		private:

		/*****************************************************/

			Control *Bar;
			Label   *PercentageText;

			int range;
			int step;

			float percentage;

			bool RenderPercentage;

		/*****************************************************/

			void ProcessPercentage ();

				inline void CalculatePercentage ();
				inline void SetPercentageText   ();


			void AdjustContentColor ();
			void AdjustContentSize  ();

			void UpdateContent ();

		/*****************************************************/

			void Save (std::fstream &file);
			void Load (std::fstream &file);

	};