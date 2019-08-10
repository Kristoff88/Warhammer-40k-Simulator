#pragma once

	#include "..//..//UpperClass//UpperClass.h"

	#include "..//ListBox//ListBox.h"


	#define COMBOBOX_BUTTON_PRESSED        (1)
	#define COMBOBOX_BUTTON_UNPRESSED	   (2)

	#define COMBOBOX_SELECTION_PRESSED     (3)
	#define COMBOBOX_SELECTION_UNPRESSED   (4)
	
	#define COMBOBOX_ELEMENT			   (5)

	#define COMBOBOX_BAR				   (6)

	#define COMBOBOX_SLIDER_PRESSED		   (7)
	#define COMBOBOX_SLIDER_UNPRESSED	   (8)

	#define COMBOBOX_NAVIGATOR_A_PRESSED   (9)
	#define COMBOBOX_NAVIGATOR_A_UNPRESSED (10)

	#define COMBOBOX_NAVIGATOR_B_PRESSED   (11)
	#define COMBOBOX_NAVIGATOR_B_UNPRESSED (12)


	#define COMBOBOX_TEXTURE_COUNT         (12)



	class ComboBox: public UpperClass
	{
		public:

			ComboBox();
				private:
				void SetDefaultComboData();

				inline void PrepareTextureSlots ();
				inline void PrepareContent      ();
				inline void PrepareAttributes   ();
				
			public:

			ComboBox(ComboBox const &comboBox);
			ComboBox& operator= (ComboBox const &comboBox);
				private:
				void CopyComboData(ComboBox const &comboBox);

				inline void CopyContent      (ComboBox const &comboBox);
				inline void CopyAttributes   (ComboBox const &comboBox);
				inline void RefreshContentArrayPointers ();
			public:

			~ComboBox();

	/******************************************************
			Element managment functions.
	******************************************************/

			void AddElement (std::string const &element, unsigned int const &index = 0);

			void RemoveElement (std::string const &element) throw (std::string);
			void RemoveElement (unsigned int const &index)	throw (std::string);

			void AddSubset    (std::vector <std::string>  const &subset, unsigned int const &index = 0);
			void RemoveSubset (unsigned int const &firstIndex, unsigned int const &lastIndex)		    throw (std::string);
				
			void RemoveAllElements ();

				private:
				// Used when elements are removed.
				inline void CorrigateSelection ();
			public:

			void		SetSelection (int const &index) throw (std::string);
			std::string GetSelection ();

			std::string GetElement (int const &index);

			int GetElementCount ();

	/******************************************************
			Basic managment functions.
	******************************************************/

			void SetFont (Font &font);
			Font GetFont ();

			void SetTextColor (Color const &color);
			Color GetTextColor ();

			void SetSelectionColor (Color const &color);
			Color GetSelectionColor ();

			void SetListLength (unsigned int const &length);
			unsigned int GetListLength ();

			void SetButtonSize (float const &size);
			float GetButtonSize ();

			void SetNavigatorSize (float const &size);
			float GetNavigatorSize ();

	/******************************************************
			Texture managment functions.
	******************************************************/

			void Button_SetPressedTexture (unsigned int const &textureID);
			unsigned int Button_GetPressedTexture ();

			void Button_SetUnpressedTexture (unsigned int const &textureID);
			unsigned int Button_GetUnpressedTexture ();


			void Selection_SetPressedTexture (unsigned int const &textureID);
			unsigned int Selection_GetPressedTexture ();

			void Selection_SetUnpressedTexture (unsigned int const &textureID);
			unsigned int Selection_GetUnpressedTexture ();


			void Element_SetTexture (unsigned int const &textureID);
			unsigned int Element_GetTexture ();

			void Bar_SetTexture (unsigned int const &textureID);
			unsigned int Bar_GetTexture ();


			void Slider_SetPressedTexture (unsigned int const &textureID);
			unsigned int Slider_GetPressedTexture ();

			void Slider_SetUnpressedTexture (unsigned int const &textureID);
			unsigned int Slider_GetUnpressedTexture ();


			void NavigatorA_SetPressedTexture (unsigned int const &textureID);
			unsigned int NavigatorA_GetPressedTexture ();

			void NavigatorA_SetUnpressedTexture (unsigned int const &textureID);
			unsigned int NavigatorA_GetUnpressedTexture ();


			void NavigatorB_SetPressedTexture (unsigned int const &textureID);
			unsigned int NavigatorB_GetPressedTexture ();

			void NavigatorB_SetUnpressedTexture (unsigned int const &textureID);
			unsigned int NavigatorB_GetUnpressedTexture ();

		private:

			Button *selection, *button;
			ListBox *elementList;

			Font currentFont;

			Color textColor;
			Color selectionColor;

			float buttonSize;
			int elementListLength;

	/******************************************************/
			
			void AdjustContentSize  ();

			inline void CorrigateButtonSize		     ();
			inline void AdjustButtonToTopRightCorner ();
			inline void AdjustSelectionToButton		 ();
			inline void CorrigateElementList		 ();


			void AdjustContentColor ();


			void UpdateContent ();

			inline void UpdateContentTextures ();

				inline void UpdateButtonTextures      ();
				inline void UpdateSelectionTextures   ();
				inline void UpdateElementListTextures ();

			inline void UpdateContentFonts     ();
			inline void UpdateContentTextColor ();

	/******************************************************
			Standard functions redefinition.
	******************************************************/

			void MouseLeftDown       (POINT const &mousePosition);
			void MouseMiddleDown     (POINT const &mousePosition);
			void MouseRightDown      (POINT const &mousePosition);

			void MouseLeftDblClick   (POINT const &mousePosition);
			void MouseMiddleDblClick (POINT const &mousePosition);
			void MouseRightDblClick  (POINT const &mousePosition);

			void MouseDragged        (POINT const &mousePosition);

			void FocusLost			 ();

	/*****************************************************/

			void ManageMouseInput    (bool const &buttonWasntPressedBefore);
			inline void UpdateCurrentSelection ();

	/*****************************************************/

			void HideElementList ();
			inline void ClipOffElementListFromControlBorder ();

			void ShowElementList ();

			inline float CalculateNewListLength ();
			inline void AdjustElementListBorder (float const &newListLength);
			inline void ExtendControlBorderByElementListBorder ();


	/*****************************************************/

			void Save (std::fstream &file);

			inline void SaveCurrentFont    (std::fstream &file);
			inline void SaveTextColor      (std::fstream &file);
			inline void SaveSelectionColor (std::fstream &file);
			inline void SaveAttributes	   (std::fstream &file);

			void Load (std::fstream &file);

			inline void LoadCurrentFont    (std::fstream &file);
			inline void LoadTextColor      (std::fstream &file);
			inline void LoadSelectionColor (std::fstream &file);
			inline void LoadAttributes	   (std::fstream &file);
	};