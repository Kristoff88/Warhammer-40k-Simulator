#pragma once

	#include "..//..//UpperClass//UpperClass.h"

	#define CHECKER_PRESSED       (1)
	#define CHECKER_UNPRESSED     (2)

	#define CHECKER_TEXTURE_COUNT (2)


	class CheckerBox: public UpperClass
	{
		public:

			CheckerBox();
				private:
				void SetDefaultCheckerData();

				inline void PrepareTextureSlots ();
				inline void PrepareContent		();
				inline void PrepareAttributes   ();
			public:

			CheckerBox(CheckerBox const &checkerBox);
			CheckerBox& operator= (CheckerBox const &checkerBox);
				private:
				void CopyCheckerData(CheckerBox const &checkerBox);
				inline void RefreshContentArrayPointers ();
			public:

			~CheckerBox();

	/******************************************************
			Basic managment functions.
	******************************************************/

			void Invert ();


			void Checker_SetPressedTexture   (unsigned int const &textureID);
			void Checker_SetUnpressedTexture (unsigned int const &textureID);

			unsigned int Checker_GetPressedTexture   ();
			unsigned int Checker_GetUnpressedTexture ();

			void SetFont (Font &font);
			Font GetFont ();

			void SetText (std::string const &text);
			std::string GetText ();

			void SetTextColor (Color const &color);
			Color GetTextColor ();


			void Press        ();
			bool IsPressed    ();
			bool IsNotPressed ();

		private:

	/*****************************************************/

			Button *checker;
			Label  *caption;


	/******************************************************/
			
			void AdjustContentSize ();

				inline void CheckerToLeft  ();
				inline void CheckerToRight ();
				inline RECT AdjustLabelToTextHeight ();

			void AdjustContentColor ();
			void UpdateContent ();

	/*****************************************************/

			void Save (std::fstream &file);
			void Load (std::fstream &file);

	/******************************************************
			Variables, flags.
	******************************************************/

			bool Inverted;
	};