#pragma once
	
	#include "..//Label//Label.h"


	class Edit: public Label
	{
		friend UpperClass;


		public:

			Edit();
				private:
				void SetDefaultEditData();
			public:

			Edit(Edit const &edit);
				private:
				void CopyEditData(Edit const &edit);
			public:

			~Edit();

			Edit& operator= (Control const &ctrl);
			Edit& operator= (Label const &label);
			Edit& operator= (Edit const &edit);

	/******************************************************
			Text managment.
	******************************************************/

			void SetText (std::string const &text);

			void AddCharacter    (char const &Char);
			void RemoveCharacter ();

			private:

			bool InputTextCorrect(std::string const &text);

				inline bool FoundLetters (std::string const &text);
				inline bool FoundNumbers (std::string const &text);

			public:

			void ScrollDown ();
			void ScrollUp   ();

	/******************************************************
			Caret managment.
	******************************************************/

			void MoveCaret_Left  ();
			void MoveCaret_Right ();

			void MoveCaret_Up    ();
			void MoveCaret_Down  ();

			private:

			void CaretIndexToPosition ();

			inline bool DesiredIndexIsNotInThisRow (int index, std::string row);
			inline bool RowHasEndofLineMarker      (int index, std::string row);

			public:

	/******************************************************
			Properties managment.
	******************************************************/

			void SetCaretBlipFrequency (unsigned int const &frequency);

			void SetCaretIndex    (unsigned int const &index);
			void SetCaretPosition (POINT const &position);
				private:
				void CaretCorrection      ();
			public:

			void SetNumbersOnly  (bool const &IsNumbersOnly);
			void SetLettersOnly  (bool const &IsLettersOnly);
				private:
				void ClearFilterFlags ();
			public:

			void SetOverwrite    (bool const &IsOverwriting);
			void SetEdition      (bool const &IsEdited);


			int GetCaretBlipFrequency ();

			int   GetCaretIndex    ();
			POINT GetCaretPosition ();


			bool IsNumbersOnly  ();
			bool IsLettersOnly  ();
			bool IsOverwriting  ();
			bool IsEdited       ();

		protected:

	/******************************************************
			Standard function redefinitions.
	******************************************************/

			void Resize (POINT const &mousePosition);

			public:
			void Scale  (float const &scaleMultiplier);
			protected:

	/******************************************************
			Rendering.
	******************************************************/

			void Render     (Color const &GUIColor, bool const &TransparentGUI);
			void PrintCaret (Color const &GUIColor, bool const &TransparentGUI);

	/*****************************************************/

			void Save (std::fstream &file);
				private:
				inline void SaveEditCaret    (std::fstream &file);
				inline void SaveEditBooleans (std::fstream &file);
			public:

			void Load (std::fstream &file);
				private:
				inline void LoadEditCaret    (std::fstream &file);
				inline void LoadEditBooleans (std::fstream &file);

	/******************************************************
			Variables, flags.
	******************************************************/
	
			CHiResTimer caretBlipTimer;

			float timeElapsed;
			float caretBlipFrequency;

			unsigned int caretIndex;
			POINT caretPosition;


			bool NumbersOnly,
				 LettersOnly;

			bool Overwrite;

			bool Edition;


			bool RenderCaret;
	};