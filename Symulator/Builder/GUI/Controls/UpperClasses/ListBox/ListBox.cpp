	#include "GlobalHeaders.h"

	#include "ListBox.h"




	ListBox::ListBox()
	{
		controlType = "ListBox";
		SetDefaultListData();

		AdjustContentSize();
		AdjustContentColor();
	}
//
		void ListBox::SetDefaultListData()
		{
			PrepareTextureSlots();
			PrepareScroll();
			PrepareAttributes();
		}
//
		void ListBox::PrepareTextureSlots ()
		{
			// Inserting texture slots.
			for(int i = 0; i < LISTBOX_TEXTURE_COUNT; i++)
			{
				textures.push_back(0);
			}
		}

		void ListBox::PrepareScroll     ()
		{
			scroll = new ScrollBar();

			scroll->SetVerticalAlignment();
			scroll->SetFrameThickness(0);
			scroll->SetStep(0);
			scroll->SetRendering(false);

			contentArray.push_back(scroll);
		}

		void ListBox::PrepareAttributes ()
		{
			scrollDown = 0;
			textColor = Color(0, 0, 0, 1);
			selectionColor = Color(0, 0.2, 0.8, 1);

			Indexed = false;
			MultiSelection = false;
		}
//
	ListBox::ListBox(ListBox const &listBox)
	{
		UpperClass::operator =(listBox);
		CopyListData(listBox);
	}

	ListBox& ListBox::operator= (ListBox const &listBox)
	{
		UpperClass::operator =(listBox);
		CopyListData(listBox);
		return *this;
	}
//
		void ListBox::CopyListData(ListBox const &listBox)
		{
			RemoveAllElements();

			CopyContent(listBox);
			CopyAttributes(listBox);

			RefreshContentArrayPointers();
			UpdateContent();
		}
//
		void ListBox::CopyContent    (ListBox const &listBox)
		{
			*scroll = *listBox.scroll;
			
			for(int i = 0; i < listBox.list.size(); i++)
			{
				AddElement( listBox.list[i]->GetText() );
			}
		}

		void ListBox::CopyAttributes (ListBox const &listBox)
		{
			captionList    = listBox.captionList;
			selectedIndices = listBox.selectedIndices;

			currentFont    = listBox.currentFont;
			textColor      = listBox.textColor;
			selectionColor = listBox.selectionColor;

			scrollDown = listBox.scrollDown;

			Indexed         = listBox.Indexed;
			MultiSelection  = listBox.MultiSelection;
		}

		void ListBox::RefreshContentArrayPointers ()
		{
			contentArray.clear();

			contentArray.push_back(scroll);

			for(int i = 0; i < list.size(); i++)
			{
				contentArray.push_back(list[i]);
			}
		}
//
	ListBox::~ListBox()
	{}
//
/******************************************************
		Element managment functions.
******************************************************/

	void ListBox::AddElement (std::string const &element, unsigned int const &index)
	{
		Button *newElement = CreateNewElement(element);
		InsertNewElement(newElement, index);

		RefreshSelections();

		UpdateContent();
		AdjustContentSize();
	}
//
		Button* ListBox::CreateNewElement (std::string const &element)
		{
			Button *newElement = new Button;
		
			newElement->SetFrameThickness(0);
			newElement->SetSwitchMode(true);
			newElement->SetMultiline(false);
			newElement->SetCenterAlignment();
			newElement->SetText(element);

			return newElement;
		}

		void    ListBox::InsertNewElement (Button *newElement, unsigned int const &index)
		{
			if( index > list.size() )
			{
				captionList.insert( captionList.begin()+list.size(), newElement->GetText() );
				list.insert( list.begin()+list.size(), newElement );
			}
			else
			{
				captionList.insert( captionList.begin()+index, newElement->GetText() );
				list.insert( list.begin()+index, newElement );
			}

			// TODO: Usunac, jezeli dwie powyzsze linijki beda dzialac poprawnie.
			//captionList.push_back(element);
			//list.push_back(newElement);

			contentArray.push_back(newElement);
		}
//
	void ListBox::RemoveElement (std::string const &element) throw (std::string)
	{
		for(int i = 0; i < list.size(); i++)
		{
			if( list[i]->GetText() == element )
			{
				RemoveElementFromContentVector(list[i]);
				DeleteElementAndRemoveFromList(i);

				RefreshSelections();

				AdjustContentSize();
			}
		}
	}

	void ListBox::RemoveElement (unsigned int const &index)	 throw (std::string)
	{
		if( index < list.size() )
		{
			RemoveElementFromContentVector(list[index]);
			DeleteElementAndRemoveFromList(index);

			RefreshSelections();

			AdjustContentSize();
		}
	}
//
	void ListBox::AddSubset    (std::vector <std::string> const &subset, unsigned int const &index)
	{
		for(int i = 0; i < subset.size(); i++)
		{
			AddElement(subset[i], index+i);
		}
	}

//
	void ListBox::RemoveSubset (unsigned int const &firstIndex, unsigned int const &lastIndex) throw (std::string)
	{
		if( GivenIndicesAreCorrect(firstIndex, lastIndex) )
		{
			PerformSubsetRemoval(firstIndex, lastIndex);
		}
		else
			InformAboutIncorrectIndices(firstIndex, lastIndex);
	}
//
		bool ListBox::GivenIndicesAreCorrect      (unsigned int const &firstIndex, unsigned int const &lastIndex)
		{
			if( firstIndex >  lastIndex		|| 
				firstIndex >= list.size()	||
				lastIndex  >= list.size() )
			{
				return false;
			}
			else
				return true;
		}

		void ListBox::PerformSubsetRemoval        (unsigned int const &firstIndex, unsigned int const &lastIndex)
		{
			int steps = (lastIndex-firstIndex) + 1;

			for(int i = 0; i < steps; i++)
			{
				RemoveElement(firstIndex);
			}
		}

		void ListBox::InformAboutIncorrectIndices (unsigned int const &firstIndex, unsigned int const &lastIndex) throw (std::string)
		{
			std::string error;
			char firstIndexAsChar[MAX_PATH], 
				 lastIndexAsChar[MAX_PATH],
				 listSizeAsChar[MAX_PATH];

			_itoa_s(firstIndex,  firstIndexAsChar, MAX_PATH, 10);
			_itoa_s(lastIndex,   lastIndexAsChar,  MAX_PATH, 10);
			_itoa_s(list.size(), listSizeAsChar,   MAX_PATH, 10);


			error  = "Indices, given to a ListBox operation RemoveSubset(...), are incorrect:\n\n";

			error += "First index:\t";
			error += firstIndexAsChar;
			error += "\n";

			error += "Last index: \t";
			error += lastIndexAsChar;
			error += "\n\n";

			error += "Control name:\t";
			error += controlName;
			error += "\n";

			error += "Element count:\t";
			error += listSizeAsChar;
			error += "\n";

			throw error;
		}
//
	void ListBox::RemoveAllElements ()
	{
		for(int i = list.size()-1; i >= 0; i--)
		{
			RemoveElementFromContentVector(list[i]);
			DeleteElementAndRemoveFromList(i);
		}

		RefreshSelections();

		AdjustContentSize();
	}
//
	void ListBox::RemoveElementFromContentVector (Control *removedElement)
	{
		bool ElementFoundInContentArray = false;

		for(int i = 0; i < contentArray.size(); i++)
		{
			if( contentArray[i] == removedElement )
			{
				contentArray.erase( contentArray.begin()+i );

				ElementFoundInContentArray = true;
				break;
			}
		}

		assert( ElementFoundInContentArray );
	}

	void ListBox::DeleteElementAndRemoveFromList (int const &elementIndex)
	{
		delete list[elementIndex];
		list.erase( list.begin()+elementIndex );

		captionList.erase( captionList.begin()+elementIndex );
	}
//
	std::string ListBox::GetElement (unsigned int const &index) throw (std::string)
	{
		try
		{
			CheckGivenIndexCorrectness(index);
		}
		catch(std::string error)
		{
			error += "\n\nOperation: GetElement(unsigned int const &index)";
			throw error;
		}

		return list[index]->GetText();
	}

	int ListBox::GetElementIndex (std::string const &element)   throw (std::string)
	{
		for(int i = 0; i < list.size(); i++)
		{
			if( element == list[i]->GetText() )
			{
				return i;
			}
		}


		std::string error;

		error  = "There is no element \"";
		error += element;
		error += "\" in ListBox \"";
		error += controlName;
		error += "\"";

		error += "\n\nOperation: GetIndex(std::string element)";

		throw error;
	}
	int ListBox::GetElementCount ()
	{
		return list.size();
	}
//
/******************************************************
		Selection managment functions.
******************************************************/

	void ListBox::SelectElement (unsigned int const &index)      throw (std::string)
	{
		try
		{
			CheckGivenIndexCorrectness(index);
		}
		catch(std::string error)
		{
			error += "\n\nOperation: SelectElement(unsigned int const &index)";
			throw error;
		}


		if( list[index]->IsNotPressed() )
		{
			list[index]->Press();
			RefreshSelections();
		}
	}

	void ListBox::SelectElement (std::string const &element)     throw (std::string)
	{
		for(int i = 0; i < list.size(); i++)
		{
			if( element == list[i]->GetText() )
			{
				if( list[i]->IsNotPressed() )
				{
					list[i]->Press();
					RefreshSelections();
				}

				return;
			}
		}


		std::string error;

		error  = "There is no element \"";
		error += element;
		error += "\" in ListBox \"";
		error += controlName;
		error += "\"";

		error += "\n\nOperation: SelectElement(std::string element)";

		throw error;
	}
//
	void ListBox::UnselectElement (unsigned int const &index)	throw (std::string)
	{
		try
		{
			CheckGivenIndexCorrectness(index);
		}
		catch(std::string error)
		{
			error += "\n\nOperation: SelectElement(unsigned int const &index)";
			throw error;
		}


		if( list[index]->IsPressed() )
		{
			list[index]->Press();
			RefreshSelections();
		}
	}

	void ListBox::UnselectElement (std::string const &element)   throw (std::string)
	{
		for(int i = 0; i < list.size(); i++)
		{
			if( element == list[i]->GetText() )
			{
				if( list[i]->IsPressed() )
				{
					list[i]->Press();
					RefreshSelections();
				}

				return;
			}
		}


		std::string error;

		error  = "There is no element \"";
		error += element;
		error += "\" in ListBox \"";
		error += controlName;
		error += "\"";

		error += "\n\nOperation: UnselectElement(std::string element)";

		throw error;
	}
//
	void ListBox::SelectAllElements   ()
	{
		for(int i = 0; i < list.size(); i++)
		{
			if( list[i]->IsNotPressed() )
			{
				list[i]->Press();
			}
		}

		RefreshSelections();
	}

	void ListBox::UnselectAllElements ()
	{
		for(int i = 0; i < list.size(); i++)
		{
			if( list[i]->IsPressed() )
			{
				list[i]->Press();
			}
		}

		RefreshSelections();
	}

//
	std::vector <int>         ListBox::GetSelectedIndices  ()
	{
		return selectedIndices;
	}

	std::vector <std::string> ListBox::GetSelectedElements ()
	{
		std::vector <std::string> selectedElements;

		for(int i = 0; i < selectedIndices.size(); i++)
		{
			selectedElements.push_back( list[selectedIndices[i]]->GetText() );
		}

		return selectedElements;
	}
//
	void ListBox::RemoveSelectedElements ()
	{
		for(int i = selectedIndices.size()-1; i >= 0; i--)
		{
			RemoveElement(selectedIndices[i]);
		}

		selectedIndices.clear();
	}
//
	void ListBox::RefreshSelections ()
	{
		selectedIndices.clear();

		for(int i = 0; i < list.size(); i++)
		{
			if( list[i]->IsPressed() )
			{
				list[i]->SetColor(selectionColor);
				selectedIndices.push_back(i);
			}
			else
				list[i]->SetColor(controlColor);
		}	
	}
//
/*****************************************************/

	void ListBox::CheckGivenIndexCorrectness (unsigned int const &index) throw (std::string)
	{
		if( index >= list.size() || index < 0 )
		{
			std::string error;

			char indexAsChar[MAX_PATH];
			_itoa_s(index, indexAsChar, MAX_PATH, 10);

			error  = "Given index is too big: ";
			error += indexAsChar;

			error += "\n\nListBox: ";
			error += controlName;

			throw error;
		}
	}
//
/******************************************************
		Basic managment functions.
******************************************************/

	void ListBox::SetFont (Font &font)
	{
		currentFont = font;
		UpdateContent();
	}

	Font ListBox::GetFont ()
	{
		return currentFont;
	}
//
	void ListBox::SetTextColor (Color const &color)
	{
		textColor = color;
		UpdateContent();
	}

	Color ListBox::GetTextColor ()
	{
		return textColor;
	}
//
	void ListBox::SetSelectionColor (Color const &color)
	{
		selectionColor = color;
		UpdateContent();
	}

	Color ListBox::GetSelectionColor ()
	{
		return selectionColor;
	}
//
//
	void ListBox::SetNavigatorSize (float const &navigatorSize)
	{
		scroll->SetNavigatorSize(navigatorSize);

		UpdateContent();
		AdjustContentSize();
	}

	float ListBox::GetNavigatorSize ()
	{
		return scroll->GetNavigatorSize();
	}
//
	void ListBox::SetStep (unsigned int const &step)
	{
		scroll->SetStep(step);
		AdjustContentSize();
	}

	unsigned int ListBox::GetStep ()
	{
		return scroll->GetStep();
	}
//
//
	void ListBox::SetMultiSelection (bool const &multiSelection)
	{
		MultiSelection = multiSelection;
	}

	bool ListBox::IsMultiSelecting  ()
	{
		return MultiSelection;
	}
//
	void ListBox::SetIndexing (bool const &indexing)
	{
		Indexed = indexing;
		UpdateContent();
	}

	bool ListBox::IsIndexed   ()
	{
		return Indexed;
	}
//
//
	void ListBox::SetLeftAlignment   ()
	{
		for(int i = 0; i < list.size(); i++)
		{
			list[i]->SetLeftAlignment();
		}
	}

	void ListBox::SetCenterAlignment ()
	{
		for(int i = 0; i < list.size(); i++)
		{
			list[i]->SetCenterAlignment();
		}
	}

	void ListBox::SetRightAlignment  ()
	{
		for(int i = 0; i < list.size(); i++)
		{
			list[i]->SetRightAlignment();
		}
	}
//
/******************************************************
		Texture managment functions.
******************************************************/

	void ListBox::Element_SetPressedTexture   (unsigned int const &textureID)
	{
		textures[LISTBOX_ELEMENT_PRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ListBox::Element_GetPressedTexture ()
	{
		return textures[LISTBOX_ELEMENT_PRESSED];
	}
//
	void ListBox::Element_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[LISTBOX_ELEMENT_UNPRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ListBox::Element_GetUnressedTexture ()
	{
		return textures[LISTBOX_ELEMENT_UNPRESSED];
	}
//
//
	void ListBox::SetBarTexture (unsigned int const &textureID)
	{
		textures[LISTBOX_BAR] = textureID;
	}

	unsigned int ListBox::GetBarTexture ()
	{
		return textures[LISTBOX_BAR];
	}
//
//
	void ListBox::Slider_SetPressedTexture (unsigned int const &textureID)
	{
		textures[LISTBOX_SLIDER_PRESSED] = textureID;
	}

	unsigned int ListBox::Slider_GetPressedTexture ()
	{
		return textures[LISTBOX_SLIDER_PRESSED];
	}
//
	void ListBox::Slider_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[LISTBOX_SLIDER_UNPRESSED] = textureID;
	}

	unsigned int ListBox::Slider_GetUnpressedTexture ()
	{
		return textures[LISTBOX_SLIDER_UNPRESSED];
	}
//
//
	void ListBox::NavigatorA_SetPressedTexture (unsigned int const &textureID)
	{
		textures[LISTBOX_NAVIGATOR_A_PRESSED] = textureID;
	}

	unsigned int ListBox::NavigatorA_GetPressedTexture ()
	{
		return textures[LISTBOX_NAVIGATOR_A_PRESSED];
	}
//
	void ListBox::NavigatorA_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[LISTBOX_NAVIGATOR_A_UNPRESSED] = textureID;
	}

	unsigned int ListBox::NavigatorA_GetUnpressedTexture ()
	{
		return textures[LISTBOX_NAVIGATOR_A_UNPRESSED];
	}
//
	void ListBox::NavigatorB_SetPressedTexture (unsigned int const &textureID)
	{
		textures[LISTBOX_NAVIGATOR_B_PRESSED] = textureID;
	}

	unsigned int ListBox::NavigatorB_GetPressedTexture ()
	{
		return textures[LISTBOX_NAVIGATOR_B_PRESSED];
	}
//
	void ListBox::NavigatorB_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[LISTBOX_NAVIGATOR_B_UNPRESSED] = textureID;
	}

	unsigned int ListBox::NavigatorB_GetUnpressedTexture ()
	{
		return textures[LISTBOX_NAVIGATOR_B_UNPRESSED];
	}
//
//
/******************************************************/
			
	void ListBox::AdjustContentSize  ()
	{
		controlBorder;
		if( list.size() )
		{
			minControlHeight;
			controlBorder;
			HideAllElements();
			controlBorder;
			AdjustElementsToHeight();
			controlBorder;

			int lol = 1;

			if( GetClippedElements() )
			{
				controlBorder;
				AdjustAndShowScrollBar();
				controlBorder;
				AdjustElementsToScroll( scroll->GetNavigatorSize() );
				controlBorder;

				lol = 1;
			}
			else
				HideScrollBar();
		}
		else
			HideScrollBar();
	}
//
		void ListBox::HideAllElements        ()
		{
			for(int i = 0; i < list.size(); i++)
			{
				list[i]->SetRendering(false);
			}
		}

		void ListBox::AdjustElementsToHeight ()
		{
			CalculateScrollDown();
			AdjustAndShowElements();
		}

		void ListBox::CalculateScrollDown    ()
		{
			if( GetClippedElements() )
			{
				scrollDown = scroll->GetStep();
				assert( scrollDown < list.size() );
			}
			else
				scrollDown = 0;
		}

		void ListBox::AdjustAndShowElements  ()
		{
			float controlHeight = GetHeight();
			float fontHeight = currentFont.height;

			// Single row won't fit!
			if( fontHeight > controlHeight )
			{
				return;
			}
			

			float previousElementEnding = controlBorder.top;
			RECT elementBorder = controlBorder;

			for(int i = scrollDown; i < list.size(); i++)
			{
				elementBorder.top = previousElementEnding;
				elementBorder.bottom = elementBorder.top - fontHeight;

				previousElementEnding -= fontHeight;

				if( previousElementEnding >= controlBorder.bottom )
				{
					list[i]->SetBorders(elementBorder);
					list[i]->SetRendering(true);
				}
				else
					break;
			}
		}

		void ListBox::AdjustElementsToScroll (float const &navigatorSize)
		{
			for(int i = 0; i < list.size(); i++)
			{
				RECT elementBorder = list[i]->GetBorders();

				elementBorder.right -= navigatorSize*2;

				list[i]->SetBorders(elementBorder);
			}
		}
//
		void ListBox::HideScrollBar          ()
		{
			scroll->SetStep(0);
			scrollDown = 0;

			scroll->SetRendering(false);
		}

		void ListBox::AdjustAndShowScrollBar ()
		{
			float navigatorSize = scroll->GetNavigatorSize();

			RECT  scrollBorder  = controlBorder;
			scrollBorder.left = scrollBorder.right - navigatorSize*2;


			int elementsClipped = GetClippedElements();		
			scroll->SetRange(elementsClipped+1);

			if( !scroll->IsRendering() )
			{
				scroll->SetStep(0);
			}

			scroll->SetBorders(scrollBorder);
			scroll->SetRendering(true);
		}
//
		int ListBox::GetClippedElements ()
		{
			int elementsClipped = 0;
			float previousElementEnding = controlBorder.top;


			for(int i = 0; i < list.size(); i++)
			{
				previousElementEnding -= currentFont.height;

				if( previousElementEnding < controlBorder.bottom )
				{
					elementsClipped++;
				}
			}

			assert( elementsClipped <= list.size() );
			return elementsClipped;
		}
//
	void ListBox::AdjustContentColor ()
	{
		for(int i = 0; i < list.size(); i++)
		{
			if( list[i]->GetCurrentTexture() == 0 )
			{
				list[i]->SetColor(Color(1,1,1,0)*controlColor);
			}
			else
				list[i]->SetColor(controlColor);
		}

		scroll->SetColor(controlColor);
	}

//
	void ListBox::UpdateContent ()
	{
		UpdateElementsAttributes();
		AdjustIndexPrefices();	
		UpdateElementsTextures();


		CorrigateNavigatorSize();

		SetMinWidth( scroll->GetNavigatorSize() );
		SetMinHeight( scroll->GetNavigatorSize()*2 );

		UpdateScrollBarTextures();
	}
//
		void ListBox::UpdateElementsAttributes ()
		{
			for(int i = 0; i < list.size(); i++)
			{
				list[i]->SetMaxHeight(maxControlHeight);
				list[i]->SetMaxWidth(maxControlWidth);

				list[i]->SetFont(currentFont);
				list[i]->SetTextColor(textColor);

				if( list[i]->IsPressed() )
				{
					list[i]->SetColor(selectionColor);
				}

				list[i]->SetTransparency( IsTransparent() );
			}
		}
//
		void ListBox::AdjustIndexPrefices ()
		{
			if( Indexed )
			{
				AddIndexPrefices();
			}
			else
				RemoveIndexPrefices();
		}

		void ListBox::AddIndexPrefices    ()
		{
			assert( list.size() == captionList.size() );

			char indexAsChar[MAX_PATH];
			std::string newCaption;

			for(int i = 0; i < list.size(); i++)
			{
				newCaption.clear();

				_itoa_s(i, indexAsChar, MAX_PATH, 10);

				newCaption  = indexAsChar;
				newCaption += " - ";
				newCaption += captionList[i];

				list[i]->SetText(newCaption);
			}
		}

		void ListBox::RemoveIndexPrefices ()
		{
			assert( list.size() == captionList.size() );

			for(int i = 0; i < list.size(); i++)
			{
				list[i]->SetText(captionList[i]);
			}
		}
//
		void ListBox::UpdateElementsTextures  ()
		{
			for(int i = 0; i < list.size(); i++)
			{
				list[i]->SetTextures(textures);

				list[i]->SetPressTexture(LISTBOX_ELEMENT_PRESSED+1);
				list[i]->SetUnpressTexture(LISTBOX_ELEMENT_UNPRESSED+1);
				list[i]->SetCurrentTexture(LISTBOX_ELEMENT_UNPRESSED+1);
			}
		}

//
		void ListBox::CorrigateNavigatorSize  ()
		{
			if( scroll->GetNavigatorSize() < currentFont.height )
			{
				scroll->SetNavigatorSize( currentFont.height );
			}
		}
		void ListBox::UpdateScrollBarTextures ()
		{
			scroll->SetBarTexture( textures[LISTBOX_BAR] );

			scroll->Slider_SetPressedTexture( textures[LISTBOX_SLIDER_PRESSED] );
			scroll->Slider_SetUnpressedTexture( textures[LISTBOX_SLIDER_UNPRESSED] );

			scroll->LeftNavigator_SetPressedTexture( textures[LISTBOX_NAVIGATOR_A_PRESSED] );
			scroll->LeftNavigator_SetUnpressedTexture( textures[LISTBOX_NAVIGATOR_A_PRESSED] );

			scroll->RightNavigator_SetPressedTexture( textures[LISTBOX_NAVIGATOR_B_PRESSED] );
			scroll->RightNavigator_SetUnpressedTexture( textures[LISTBOX_NAVIGATOR_B_PRESSED] );
		}
//
/******************************************************
		Standard functions redefinition.
******************************************************/

	void ListBox::MouseLeftDown       (POINT const &mousePosition)
	{
		if( !MultiSelection )
		{
			UnselectAllElements();
		}

		UpperClass::MouseLeftDown(mousePosition);

		RefreshSelections();
		AdjustContentSize();
	}

	void ListBox::MouseMiddleDown     (POINT const &mousePosition)
	{
		if( !MultiSelection )
		{
			UnselectAllElements();
		}

		UpperClass::MouseMiddleDown(mousePosition);

		RefreshSelections();
		AdjustContentSize();
	}

	void ListBox::MouseRightDown      (POINT const &mousePosition)
	{
		if( !MultiSelection )
		{
			UnselectAllElements();
		}

		UpperClass::MouseRightDown(mousePosition);

		RefreshSelections();
		AdjustContentSize();
	}
//
	void ListBox::MouseLeftDblClick   (POINT const &mousePosition)
	{
		if( !MultiSelection )
		{
			UnselectAllElements();
		}

		UpperClass::MouseLeftDblClick(mousePosition);

		RefreshSelections();
		AdjustContentSize();
	}

	void ListBox::MouseMiddleDblClick (POINT const &mousePosition)
	{
		if( !MultiSelection )
		{
			UnselectAllElements();
		}

		UpperClass::MouseMiddleDblClick(mousePosition);

		RefreshSelections();
		AdjustContentSize();
	}

	void ListBox::MouseRightDblClick  (POINT const &mousePosition)
	{
		if( !MultiSelection )
		{
			UnselectAllElements();
		}

		UpperClass::MouseRightDblClick(mousePosition);

		RefreshSelections();
		AdjustContentSize();
	}
//
/*****************************************************/

	void ListBox::Save (std::fstream &file)
	{
		// No point in saving all the buttons, that represent list elements.
		std::vector <std::string> elementsCaptions = RemoveElementsAndCopyCaptions();
		UpperClass::Save(file);


		file << "\t" << "#----------------------------------------ListBox_components----------------------------------------#" << "\n\n";

		// Instead, it saves captions only.
		SaveElementList(file, elementsCaptions);
		RestoreElements(elementsCaptions);

		SaveListBoxData(file);
	}
//
	std::vector <std::string> ListBox::RemoveElementsAndCopyCaptions ()
	{
		std::vector <std::string> elementsCaptions = captionList;

		RemoveAllElements();

		return elementsCaptions;
	}

	void ListBox::SaveElementList (std::fstream &file, std::vector <std::string> elementsCaptions)
	{
		file << "\t" << "#Element_count:" << "\t" << elementsCaptions.size() << "\n\n";


		char indexAsChar[MAX_PATH];

		for(int i = 0; i < elementsCaptions.size(); i++)
		{
			_itoa_s(i, indexAsChar, MAX_PATH, 10);

			file << "\t" << "#" << indexAsChar << "\t" << elementsCaptions[i] << "\n";
		}

		file << "\n";
	}

	void ListBox::RestoreElements (std::vector <std::string> elementsCaptions)
	{
		for(int i = elementsCaptions.size()-1; i >= 0; i--)
		{
			AddElement(elementsCaptions[i]);
		}
	}
//
	void ListBox::SaveListBoxData    (std::fstream &file)
	{
		SaveCurrentFont(file);
		SaveTextColor(file);
		SaveSelectionColor(file);
		SaveBooleanFlags(file);
	}
//
	void ListBox::SaveCurrentFont    (std::fstream &file)
	{
		file << "\t" << "#Current_font:" << "\t" << currentFont.fontName << "\n"

			 << "\t" << "#Height:"       << "\t" << currentFont.height   << "\n"
			 << "\t" << "#Width:"        << "\t" << currentFont.width    << "\n"
			 << "\t" << "#Weight:"       << "\t" << currentFont.weight   << "\n"

			 << "\t" << "#Italic:"       << "\t" << currentFont.Italic    << "\n"
			 << "\t" << "#Underline:"    << "\t" << currentFont.Underline << "\n"
			 << "\t" << "#Strikeout:"    << "\t" << currentFont.Strikeout << "\n\n";
	}

	void ListBox::SaveTextColor      (std::fstream &file)
	{
		file << "\t" << "#Text_color:" << "\n"
			 
			 << "\t" << "#Red:"   << "\t" << textColor.GetRed()   << "\n"
			 << "\t" << "#Green:" << "\t" <<  textColor.GetGreen() << "\n"
			 << "\t" << "#Blue:"  << "\t" <<  textColor.GetBlue()  << "\n"
			 << "\t" << "#Alpha:" << "\t" <<  textColor.GetAlpha() << "\n\n";
	}

	void ListBox::SaveSelectionColor (std::fstream &file)
	{
		file << "\t" << "#Selection_color:" << "\n"
			 
			 << "\t" << "#Red:"   << "\t" <<  selectionColor.GetRed()   << "\n"
			 << "\t" << "#Green:" << "\t" <<  selectionColor.GetGreen() << "\n"
			 << "\t" << "#Blue:"  << "\t" <<  selectionColor.GetBlue()  << "\n"
			 << "\t" << "#Alpha:" << "\t" <<  selectionColor.GetAlpha() << "\n\n";
	}

	void ListBox::SaveBooleanFlags   (std::fstream &file)
	{
		file << "\t" << "#Indexed:"        << "\t" << Indexed        << "\n"
			 << "\t" << "#MultiSelection:" << "\t" << MultiSelection << "\n\n";
	}
//
//
	void ListBox::Load (std::fstream &file)
	{
		UpperClass::Load(file);

		LoadElementList(file);
		LoadListBoxData(file);
		UpdateContent();
	}
//
	void ListBox::LoadElementList    (std::fstream &file)
	{
		std::string line;

		int elementCount;

		line = ReadNewValue(file, '#');		
		elementCount = atoi( line.c_str() );

		assert( elementCount >= 0 );


		for(int i = 0; i < elementCount; i++)
		{
			line = ReadNewValue(file ,'#');
			AddElement(line, i);
		}
	}
//
	void ListBox::LoadListBoxData    (std::fstream &file)
	{
		LoadCurrentFont(file);
		LoadTextColor(file);
		LoadSelectionColor(file);
		LoadBooleanFlags(file);
	}

	void ListBox::LoadCurrentFont    (std::fstream &file)
	{
		std::string line;

		line = ReadNewValue(file, '#');		currentFont.fontName = const_cast<char*>(line.c_str());

		line = ReadNewValue(file, '#');		currentFont.height = atoi( line.c_str() );
		line = ReadNewValue(file, '#');		currentFont.width  = atoi( line.c_str() );
		line = ReadNewValue(file, '#');		currentFont.weight = atoi( line.c_str() );

		line = ReadNewValue(file, '#');		currentFont.Italic    = atoi( line.c_str() );
		line = ReadNewValue(file, '#');		currentFont.Underline = atoi( line.c_str() );
		line = ReadNewValue(file, '#');		currentFont.Strikeout = atoi( line.c_str() );

	}

	void ListBox::LoadTextColor      (std::fstream &file)
	{
		std::string line;

		line = ReadNewValue(file, '#');		textColor.SetRed   ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		textColor.SetGreen ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		textColor.SetBlue  ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		textColor.SetAlpha ( atof( line.c_str() ) );
	}

	void ListBox::LoadSelectionColor (std::fstream &file)
	{
		std::string line;

		line = ReadNewValue(file, '#');		selectionColor.SetRed   ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		selectionColor.SetGreen ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		selectionColor.SetBlue  ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		selectionColor.SetAlpha ( atof( line.c_str() ) );
	}

	void ListBox::LoadBooleanFlags   (std::fstream &file)
	{
		std::string line;

		line = ReadNewValue(file, '#');		Indexed        = atoi( line.c_str() );
		line = ReadNewValue(file, '#');		MultiSelection = atoi( line.c_str() );
	}
//
//

	#undef LISTBOX_ELEMENT_PRESSED      
	#undef LISTBOX_ELEMENT_UNPRESSED     

	#undef LISTBOX_NAVIGATOR_A_PRESSED   
	#undef LISTBOX_NAVIGATOR_A_UNPRESSED  

	#undef LISTBOX_NAVIGATOR_B_PRESSED    
	#undef LISTBOX_NAVIGATOR_B_UNPRESSED  

	#undef LISTBOX_BAR	

	#undef LISTBOX_SLIDER_PRESSED
	#undef LISTBOX_SLIDER_UNPRESSED


	#undef LISTBOX_TEXTURE_COUNT 