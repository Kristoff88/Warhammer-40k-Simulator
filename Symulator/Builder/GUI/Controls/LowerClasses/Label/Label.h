#pragma once

	#include "..//..//BaseControl//Control.h"
	
	#include "Font_struct.h"


	class Label: public Control
	{
		friend UpperClass;


		public:

			Label();
				private:
				void SetDefaultLabelData();
			public:

			Label(Label const &label);
				private:
				void CopyLabelData(Label const &label);

				inline void CopyLabelTextData(Label const &label);
				inline void CopyLabelBooleanFlags(Label const &label);
			public:

			~Label();

			Label& operator= (Control const &ctrl);
			Label& operator= (Label const &label);

	/******************************************************
			Properties managment.
	******************************************************/

			void SetText		 (std::string const &text);
				protected:

				std::vector <std::string> ProcessText (std::string const &text);

				std::vector <std::string> ProcessTextToLine (std::string const &text);

				std::vector <std::string> ProcessTextToRows (std::string const &text);
					
					inline std::vector <std::string> ProcessTextToWords      (std::string &text);
					std::vector <std::string>        PushBackSingleCharacter (std::vector <std::string> & wordList, char const &character);

					inline std::vector <std::string> ProcessWordsToRows   (std::vector <std::string> &wordList);
					inline std::vector <std::string> SplitWordBetweenRows (std::string const &word, float const &charWidth, float const &controlWidth);

					inline void	JustifyRow		  (std::string &row);
					inline bool	IsWhiteCharacter  (char const &Char);	 
					inline bool	IsEndOfLineMarker (char const &Char);	

			public:

			void AdjustWidthToCaption ();

			std::string FilterOutEndOfLineMarks ();

			void SetTextColor   (Color const &color);

			void SetMaxCharacterCount (unsigned int const &charCount);
				protected:
				inline void SliceOffSuperflousCharacters ();
			public:

			void SetFont (Font &font, HDC const &hdc = GetDC(NULL)) throw( std::string );   
				private:
				inline void CheckFontCorrectnes      (HFONT const &fontHandler, Font const &font) throw( std::string );
				inline void InformAboutIncorrectFont (Font const &font)                           throw( std::string );
				inline void GenerateNewFont (HDC const &hdc, HFONT const &fontHandler, Font &font);
			public:

			void SetLeftAlignment   ();
			void SetCenterAlignment ();
			void SetRightAlignment  ();
				protected:
				bool CheckAlignmentFlagsCorrectness ();
				void ClearAlignmentFlags            ();
			public:

			void SetMultiline (bool const &isMultiline);
			bool IsMultiline ();

			std::string  GetText		      ();
			std::string  GetRow               (unsigned int const &index);
			int          GetRowCount          ();
			Color		 GetTextColor         ();
			unsigned int GetMaxCharacterCount ();
			Font         GetCurrentFont       ();

	/******************************************************
			Standard function redefinitions.
	******************************************************/

			void SetBorders (RECT const &Border);

			protected:
			void Resize (POINT const &mousePosition);
			public:

			void Scale  (float const &scaleMultiplier);

		protected:

			std::string controlText;
			Color textColor;
			unsigned int maxCharacterCount;

			Font currentFont;

			std::vector <std::string> rowList;
			unsigned int scrollDown;

			bool LeftAlignment, CenterAlignment, RightAlignment;

			bool Multiline;


	/******************************************************
			Rendering.
	******************************************************/

			void Render    (Color const &GUIColor, bool const &TransparentGUI);
			void PrintText (Color const &GUIColor, bool const &TransparentGUI);

	/*****************************************************/

			void Save (std::fstream &file);
			
				inline void SaveLabelText          (std::fstream &file);
				inline void SaveLabelFont          (std::fstream &file);
				inline void SaveLabelBooleans      (std::fstream &file);	

			void Load (std::fstream &file);

				inline void LoadLabelText          (std::fstream &file);
				inline void LoadLabelFont          (std::fstream &file);
				inline void LoadLabelBooleans      (std::fstream &file);

	};