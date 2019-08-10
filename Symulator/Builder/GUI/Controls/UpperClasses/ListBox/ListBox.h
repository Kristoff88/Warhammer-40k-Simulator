#pragma once

	#include "..//..//UpperClass//UpperClass.h"
	#include "..//Sliders//ScrollBar//ScrollBar.h"


	#define LISTBOX_ELEMENT_PRESSED        (1)
	#define LISTBOX_ELEMENT_UNPRESSED	   (2)

	#define LISTBOX_NAVIGATOR_A_PRESSED    (3)
	#define LISTBOX_NAVIGATOR_A_UNPRESSED  (4)

	#define LISTBOX_NAVIGATOR_B_PRESSED    (5)
	#define LISTBOX_NAVIGATOR_B_UNPRESSED  (6)

	#define LISTBOX_BAR				       (7)

	#define LISTBOX_SLIDER_PRESSED		   (8)
	#define LISTBOX_SLIDER_UNPRESSED       (9)
	

	#define LISTBOX_TEXTURE_COUNT          (9)


	class ListBox: public UpperClass
	{
		public:

			ListBox();
				private:
				void SetDefaultListData();

				inline void PrepareTextureSlots ();
				inline void PrepareScroll       ();
				inline void PrepareAttributes   ();
			public:

			ListBox(ListBox const &listBox);
			ListBox& operator= (ListBox const &listBox);
				private:
				void CopyListData(ListBox const &listBox);

				inline void CopyContent    (ListBox const &listBox);
				inline void CopyAttributes (ListBox const &listBox);
				inline void RefreshContentArrayPointers ();
			public:

			~ListBox();

	/******************************************************
			Element managment functions.
	******************************************************/

			void AddElement (std::string const &element, unsigned int const &index = 0);

				inline Button* CreateNewElement (std::string const &element);
				inline void    InsertNewElement (Button *newElement, unsigned int const &index);

			void RemoveElement (std::string const &element) throw (std::string);
			void RemoveElement (unsigned int const &index)	throw (std::string);

			void AddSubset    (std::vector <std::string>  const &subset, unsigned int const &index = 0);

			void RemoveSubset (unsigned int const &firstIndex, unsigned int const &lastIndex)		    throw (std::string);
				
				inline bool GivenIndicesAreCorrect      (unsigned int const &firstIndex, unsigned int const &lastIndex);
				inline void PerformSubsetRemoval        (unsigned int const &firstIndex, unsigned int const &lastIndex);
				inline void InformAboutIncorrectIndices (unsigned int const &firstIndex, unsigned int const &lastIndex) throw (std::string);


			void RemoveAllElements ();

			void RemoveElementFromContentVector (Control *removedTab);
			void DeleteElementAndRemoveFromList (int const &tabIndex);


			std::string GetElement (unsigned int const &index) throw (std::string);
			int GetElementIndex (std::string const &element)   throw (std::string);
			int GetElementCount ();

	/******************************************************
			Selection managment functions.
	******************************************************/

			void SelectElement (unsigned int const &index)      throw (std::string);
			void SelectElement (std::string const &element)     throw (std::string);

			void UnselectElement (unsigned int const &index)	throw (std::string);
			void UnselectElement (std::string const &element)   throw (std::string);

			void SelectAllElements   ();
			void UnselectAllElements ();

			std::vector <int>         GetSelectedIndices  ();
			std::vector <std::string> GetSelectedElements ();

			void RemoveSelectedElements ();

			private:
			void RefreshSelections ();
			public:

	/*****************************************************/

			void CheckGivenIndexCorrectness (unsigned int const &index) throw (std::string);

	/******************************************************
			Basic managment functions.
	******************************************************/

			void SetFont (Font &font);
			Font GetFont ();

			void SetTextColor (Color const &color);
			Color GetTextColor ();

			void SetSelectionColor (Color const &color);
			Color GetSelectionColor ();


			void SetNavigatorSize (float const &navigatorSize);
			float GetNavigatorSize ();

			void SetStep (unsigned int const &step);
			unsigned int GetStep ();


			void SetMultiSelection (bool const &multiSelection);
			bool IsMultiSelecting  ();

			void SetIndexing (bool const &indexing);
			bool IsIndexed   ();


			void SetLeftAlignment   ();
			void SetCenterAlignment ();
			void SetRightAlignment  ();

	/******************************************************
			Texture managment functions.
	******************************************************/

			void Element_SetPressedTexture   (unsigned int const &textureID);
			unsigned int Element_GetPressedTexture ();

			void Element_SetUnpressedTexture (unsigned int const &textureID);
			unsigned int Element_GetUnressedTexture ();


			void SetBarTexture (unsigned int const &textureID);
			unsigned int GetBarTexture ();


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

			ScrollBar *scroll;

			std::vector <Button*> list;
			std::vector <std::string> captionList;

			Font currentFont;
			Color textColor;
			Color selectionColor;

			std::vector <int> selectedIndices;
			int scrollDown;


			bool Indexed;
			bool MultiSelection;

	/******************************************************/
			
			void AdjustContentSize  ();

				inline void HideAllElements        ();
				inline void AdjustElementsToHeight ();
				inline void CalculateScrollDown    ();
				inline void AdjustAndShowElements  ();
				inline void AdjustElementsToScroll (float const &navigatorSize);

				inline void HideScrollBar		   ();
				inline void AdjustAndShowScrollBar ();

				int GetClippedElements ();
				

			void AdjustContentColor ();

			void UpdateContent ();

				inline void UpdateElementsAttributes ();

				inline void AdjustIndexPrefices ();
				inline void AddIndexPrefices    ();
				inline void RemoveIndexPrefices ();

				inline void UpdateElementsTextures  ();

				inline void CorrigateNavigatorSize  ();
				inline void UpdateScrollBarTextures ();

	/******************************************************
			Standard functions redefinition.
	******************************************************/

			void MouseLeftDown       (POINT const &mousePosition);
			void MouseMiddleDown     (POINT const &mousePosition);
			void MouseRightDown      (POINT const &mousePosition);

			void MouseLeftDblClick   (POINT const &mousePosition);
			void MouseMiddleDblClick (POINT const &mousePosition);
			void MouseRightDblClick  (POINT const &mousePosition);

	/*****************************************************/

			void Save (std::fstream &file);

			inline std::vector <std::string> RemoveElementsAndCopyCaptions ();
			inline void SaveElementList (std::fstream &file, std::vector <std::string> elementsCaptions);
			inline void RestoreElements (std::vector <std::string> elementsCaptions);

			inline void SaveListBoxData    (std::fstream &file);
			inline void SaveCurrentFont    (std::fstream &file);
			inline void SaveTextColor      (std::fstream &file);
			inline void SaveSelectionColor (std::fstream &file);
			inline void SaveBooleanFlags   (std::fstream &file);

			void Load (std::fstream &file);

			inline void LoadElementList    (std::fstream &file);

			inline void LoadListBoxData    (std::fstream &file);
			inline void LoadCurrentFont    (std::fstream &file);
			inline void LoadTextColor      (std::fstream &file);
			inline void LoadSelectionColor (std::fstream &file);
			inline void LoadBooleanFlags   (std::fstream &file);

	};