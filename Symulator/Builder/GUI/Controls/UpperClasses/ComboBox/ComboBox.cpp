	#include "GlobalHeaders.h"

	#include "ComboBox.h"


	
	ComboBox::ComboBox()
	{
		controlType = "ComboBox";
		SetDefaultComboData();

		AdjustContentSize();
		AdjustContentColor();
	}
//
		void ComboBox::SetDefaultComboData()
		{
			PrepareTextureSlots();
			PrepareContent();
			PrepareAttributes();
		}
//
		void ComboBox::PrepareTextureSlots ()
		{
			for(int i = 0; i < COMBOBOX_TEXTURE_COUNT; i++)
			{
				textures.push_back(0);
			}
		}

		void ComboBox::PrepareContent      ()
		{
			selection   = new Button;
			button      = new Button;
			elementList = new ListBox;

			selection->SetSwitchMode(true);
			button->SetSwitchMode(true);

			selection->SetFrameThickness(0);
			button->SetFrameThickness(0);
			elementList->SetFrameThickness(0);
	
			contentArray.push_back(selection);
			contentArray.push_back(button);
			contentArray.push_back(elementList);
		}

		void ComboBox::PrepareAttributes   ()
		{
			textColor = Color(0,0,0,1);
			selectionColor = Color(0, 0.2, 0.8, 1);

			SetButtonSize(35);
			SetListLength(5);
		}
//
	ComboBox::ComboBox(ComboBox const &comboBox)
	{
		UpperClass::operator=(comboBox);
		CopyComboData(comboBox);
	}

	ComboBox& ComboBox::operator= (ComboBox const &comboBox)
	{
		UpperClass::operator=(comboBox);
		CopyComboData(comboBox);
		return *this;
	}
//
		void ComboBox::CopyComboData(ComboBox const &comboBox)
		{
			CopyContent(comboBox);
			CopyAttributes(comboBox);	

			RefreshContentArrayPointers();
			UpdateContent();
		}
//
		void ComboBox::CopyContent      (ComboBox const &comboBox)
		{
			*selection   = *comboBox.selection;
			*button      = *comboBox.button;
			*elementList = *comboBox.elementList;
		}

		void ComboBox::CopyAttributes   (ComboBox const &comboBox)
		{
			currentFont       = comboBox.currentFont;

			textColor         = comboBox.textColor;
			selectionColor    = comboBox.selectionColor;

			buttonSize        = comboBox.buttonSize;
			elementListLength = comboBox.elementListLength;
		}

		void ComboBox::RefreshContentArrayPointers ()
		{
			contentArray.clear();

			contentArray.push_back(selection);
			contentArray.push_back(button);
			contentArray.push_back(elementList);
		}
//
	ComboBox::~ComboBox()
	{}
//
/******************************************************
		Element managment functions.
******************************************************/

	void ComboBox::AddElement (std::string const &element, unsigned int const &index)
	{
		elementList->AddElement(element, index);

		if( selection->GetText() == "" )
		{
			selection->SetText(element);
		}
	}
//
	void ComboBox::RemoveElement (std::string const &element) throw (std::string)
	{
		elementList->RemoveElement(element);
		CorrigateSelection();
	}

	void ComboBox::RemoveElement (unsigned int const &index)  throw (std::string)
	{
		elementList->RemoveElement(index);
		CorrigateSelection();
	}
//
	void ComboBox::AddSubset    (std::vector <std::string>  const &subset, unsigned int const &index)
	{
		elementList->AddSubset(subset, index);
	}

	void ComboBox::RemoveSubset (unsigned int const &firstIndex, unsigned int const &lastIndex)		    throw (std::string)
	{
		elementList->RemoveSubset(firstIndex, lastIndex);
		CorrigateSelection();
	}
//
	void ComboBox::RemoveAllElements ()
	{
		elementList->RemoveAllElements();
		CorrigateSelection();
	}
//
		void ComboBox::CorrigateSelection ()
		{
			if( elementList->GetElementCount() == 0 )
			{
				selection->SetText("");
			}
			else
			{
				bool SelectionHasBeenRemoved = true;

				for(int i = 0; i < elementList->GetElementCount(); i++)
				{
					if( elementList->GetElement(i) == selection->GetText() )
					{
						SelectionHasBeenRemoved = false;
						break;
					}
				}

				if( SelectionHasBeenRemoved )
				{
					SetSelection(0);
				}
			}
		}
//
//
	void		ComboBox::SetSelection (int const &index) throw (std::string)
	{
		selection->SetText( elementList->GetElement(index) );
	}

	std::string ComboBox::GetSelection ()
	{
		return selection->GetText();
	}
//
	std::string ComboBox::GetElement (int const &index)
	{
		return elementList->GetElement(index);
	}
//
	int ComboBox::GetElementCount ()
	{
		return elementList->GetElementCount();
	}
//
/******************************************************
		Basic managment functions.
******************************************************/

	void ComboBox::SetFont (Font &font)
	{
		currentFont = font;
		UpdateContent();
	}

	Font ComboBox::GetFont ()
	{
		return currentFont;
	}
//
	void ComboBox::SetTextColor (Color const &color)
	{
		textColor = color;
		UpdateContent();
	}

	Color ComboBox::GetTextColor ()
	{
		return textColor;
	}
//
	void ComboBox::SetSelectionColor (Color const &color)
	{
		selectionColor = color;
		UpdateContent();
	}

	Color ComboBox::GetSelectionColor ()
	{
		return selectionColor;
	}
//
	void ComboBox::SetListLength (unsigned int const &length)
	{
		elementListLength = length;
		UpdateContent();
		AdjustContentSize();
	}

	unsigned int ComboBox::GetListLength ()
	{
		return elementListLength;
	}
//
	void ComboBox::SetButtonSize (float const &size)
	{
		if( size == 0 )
		{
			buttonSize = 1;
		}
		else
			buttonSize = size;

		AdjustContentSize();
	}

	float ComboBox::GetButtonSize ()
	{
		return buttonSize;
	}
//
	void ComboBox::SetNavigatorSize (float const &size)
	{
		elementList->SetNavigatorSize(size);
	}

	float ComboBox::GetNavigatorSize ()
	{
		return elementList->GetNavigatorSize();;
	}
//
/******************************************************
		Texture managment functions.
******************************************************/

	void ComboBox::Button_SetPressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_BUTTON_PRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::Button_GetPressedTexture ()
	{
		return textures[COMBOBOX_BUTTON_PRESSED];
	}
//
	void ComboBox::Button_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_BUTTON_UNPRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::Button_GetUnpressedTexture ()
	{
		return textures[COMBOBOX_BUTTON_UNPRESSED];
	}
//
//
	void ComboBox::Selection_SetPressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_SELECTION_PRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::Selection_GetPressedTexture ()
	{
		return textures[COMBOBOX_SELECTION_PRESSED];
	}
//
	void ComboBox::Selection_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_SELECTION_UNPRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::Selection_GetUnpressedTexture ()
	{
		return textures[COMBOBOX_SELECTION_UNPRESSED];
	}
//
//
	void ComboBox::Element_SetTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_ELEMENT] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::Element_GetTexture ()
	{
		return textures[COMBOBOX_ELEMENT];
	}
//
	void ComboBox::Bar_SetTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_BAR] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::Bar_GetTexture ()
	{
		return textures[COMBOBOX_BAR];
	}
//
//
	void ComboBox::Slider_SetPressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_SLIDER_PRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::Slider_GetPressedTexture ()
	{
		return textures[COMBOBOX_SLIDER_PRESSED];
	}
//
	void ComboBox::Slider_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_SLIDER_UNPRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::Slider_GetUnpressedTexture ()
	{
		return textures[COMBOBOX_SLIDER_UNPRESSED];
	}
//
//
	void ComboBox::NavigatorA_SetPressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_NAVIGATOR_A_PRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::NavigatorA_GetPressedTexture ()
	{
		return textures[COMBOBOX_NAVIGATOR_A_PRESSED];
	}
//
	void ComboBox::NavigatorA_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_NAVIGATOR_A_UNPRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::NavigatorA_GetUnpressedTexture ()
	{
		return textures[COMBOBOX_NAVIGATOR_A_UNPRESSED];
	}
//
//
	void ComboBox::NavigatorB_SetPressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_NAVIGATOR_B_PRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::NavigatorB_GetPressedTexture ()
	{
		return textures[COMBOBOX_NAVIGATOR_B_PRESSED];
	}
//
	void ComboBox::NavigatorB_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[COMBOBOX_NAVIGATOR_B_UNPRESSED] = textureID;
		UpdateContent();
	}

	unsigned int ComboBox::NavigatorB_GetUnpressedTexture ()
	{
		return textures[COMBOBOX_NAVIGATOR_B_UNPRESSED];
	}
//
/******************************************************/
			
	void ComboBox::AdjustContentSize  ()
	{
		CorrigateButtonSize();
		AdjustButtonToTopRightCorner();
		AdjustSelectionToButton();

		if( button->IsNotPressed() )
		{
			HideElementList();
		}
		else
		{
			CorrigateElementList();
		}
	}
//
	void ComboBox::CorrigateButtonSize		    ()
	{
		if( buttonSize > maxControlHeight )
		{
			buttonSize = maxControlHeight;
		}

		if( buttonSize > maxControlWidth )
		{
			buttonSize = maxControlWidth;
		}
	}

	void ComboBox::AdjustButtonToTopRightCorner ()
	{
		RECT buttonBorder = controlBorder;

		buttonBorder.left   = buttonBorder.right - buttonSize;
		buttonBorder.bottom = buttonBorder.top   - buttonSize;

		button->SetBorders(buttonBorder);
	}

	void ComboBox::AdjustSelectionToButton		()
	{
		RECT selectionBorder = controlBorder;

		selectionBorder.right  = selectionBorder.right - buttonSize;
		selectionBorder.bottom = selectionBorder.top   - buttonSize;

		selection->SetBorders(selectionBorder);
	}


	void ComboBox::CorrigateElementList		    ()
	{
		RECT listBorder = elementList->GetBorders();

		listBorder.top    = controlBorder.top - buttonSize;
		listBorder.bottom = controlBorder.bottom;

		listBorder.left  = controlBorder.left;
		listBorder.right = controlBorder.right;

		elementList->SetBorders(listBorder);
	}
//
	void ComboBox::AdjustContentColor ()
	{
		selection->SetColor(controlColor);
		button->SetColor(controlColor);
		elementList->SetColor(controlColor);
	}

//
	void ComboBox::UpdateContent ()
	{
		SetMinWidth(buttonSize);
		SetMinHeight(buttonSize);

		elementList->SetSelectionColor(selectionColor);

		UpdateContentTextures();
		UpdateContentFonts();
		UpdateContentTextColor();
	}
//
	void ComboBox::UpdateContentTextures ()
	{
		UpdateButtonTextures();
		UpdateSelectionTextures();
		UpdateElementListTextures();
	}
//
		void ComboBox::UpdateButtonTextures      ()
		{
			button->SetTextures(textures);

			button->SetPressTexture( COMBOBOX_BUTTON_PRESSED+1 );
			button->SetUnpressTexture( COMBOBOX_BUTTON_UNPRESSED+1 );
			button->SetCurrentTexture( COMBOBOX_BUTTON_UNPRESSED+1 );
		}

		void ComboBox::UpdateSelectionTextures   ()
		{
			selection->SetTextures(textures);

			selection->SetPressTexture( COMBOBOX_SELECTION_PRESSED+1 );
			selection->SetUnpressTexture( COMBOBOX_SELECTION_UNPRESSED+1 );
			selection->SetCurrentTexture( COMBOBOX_SELECTION_UNPRESSED+1 );
		}

		void ComboBox::UpdateElementListTextures ()
		{
			elementList->Element_SetPressedTexture( textures[COMBOBOX_ELEMENT] );
			elementList->Element_SetUnpressedTexture( textures[COMBOBOX_ELEMENT] );

			elementList->SetBarTexture( textures[COMBOBOX_BAR] );

			elementList->Slider_SetPressedTexture( textures[COMBOBOX_SLIDER_PRESSED] );
			elementList->Slider_SetUnpressedTexture( textures[COMBOBOX_SLIDER_UNPRESSED] );

			elementList->NavigatorA_SetPressedTexture( textures[COMBOBOX_NAVIGATOR_A_PRESSED] );
			elementList->NavigatorA_SetUnpressedTexture( textures[COMBOBOX_NAVIGATOR_A_UNPRESSED] );

			elementList->NavigatorB_SetPressedTexture( textures[COMBOBOX_NAVIGATOR_B_PRESSED] );
			elementList->NavigatorB_SetUnpressedTexture( textures[COMBOBOX_NAVIGATOR_B_UNPRESSED] );
		}
//
	void ComboBox::UpdateContentFonts     ()
	{
		selection->SetFont(currentFont);
		elementList->SetFont(currentFont);
	}

	void ComboBox::UpdateContentTextColor ()
	{
		selection->SetTextColor(textColor);
		elementList->SetTextColor(textColor);
	}
//
/******************************************************
		Standard functions redefinition.
******************************************************/

	void ComboBox::MouseLeftDown       (POINT const &mousePosition)
	{
		bool buttonWasntPressedBefore = !button->IsPressed();

		UpperClass::MouseLeftDown(mousePosition);

		ManageMouseInput(buttonWasntPressedBefore);
	}

	void ComboBox::MouseMiddleDown     (POINT const &mousePosition)
	{
		bool buttonWasntPressedBefore = !button->IsPressed();

		UpperClass::MouseMiddleDown(mousePosition);

		ManageMouseInput(buttonWasntPressedBefore);

	}

	void ComboBox::MouseRightDown      (POINT const &mousePosition)
	{
		bool buttonWasntPressedBefore = !button->IsPressed();

		UpperClass::MouseRightDown(mousePosition);

		ManageMouseInput(buttonWasntPressedBefore);

	}
//
	void ComboBox::MouseLeftDblClick   (POINT const &mousePosition)
	{
		bool buttonWasntPressedBefore = !button->IsPressed();

		UpperClass::MouseLeftDblClick(mousePosition);

		ManageMouseInput(buttonWasntPressedBefore);

	}

	void ComboBox::MouseMiddleDblClick (POINT const &mousePosition)
	{
		bool buttonWasntPressedBefore = !button->IsPressed();

		UpperClass::MouseMiddleDblClick(mousePosition);

		ManageMouseInput(buttonWasntPressedBefore);

	}

	void ComboBox::MouseRightDblClick  (POINT const &mousePosition)
	{
		bool buttonWasntPressedBefore = !button->IsPressed();

		UpperClass::MouseRightDblClick(mousePosition);

		ManageMouseInput(buttonWasntPressedBefore);

	}
//
	void ComboBox::MouseDragged        (POINT const &mousePosition)
	{
		bool buttonWasntPressedBefore = !button->IsPressed();

		UpperClass::MouseDragged(mousePosition);

		ManageMouseInput(buttonWasntPressedBefore);
	}
//
	void ComboBox::FocusLost			 ()
	{
		UpperClass::FocusLost();


		if( button->IsPressed() )
		{
			button->Press();
		}

		HideElementList();
	}
//
/*****************************************************/

	void ComboBox::ManageMouseInput    (bool const &buttonWasntPressedBefore)
	{
		if( IsCurrentlyResized() )
		{
			if( button->IsPressed() )
			{
				button->Press();
			}

			HideElementList();
			return;
		}


		if( buttonWasntPressedBefore && button->IsPressed() )
		{
			ShowElementList();
			return;
		}
		else if( button->IsNotPressed() )
		{
			HideElementList();
			return;
		}


		UpdateCurrentSelection();
	}

	void ComboBox::UpdateCurrentSelection ()
	{
		std::vector <int> selectedIndex = elementList->GetSelectedIndices();
	
		if( selectedIndex.size() )
		{
			assert( selectedIndex.size() == 1 );
			assert( button->IsPressed() );


			selection->SetText( elementList->GetElement( selectedIndex[0] ) );

			elementList->UnselectElement( selectedIndex[0] );
			button->Press();
			HideElementList();
		}
	}
//
/*****************************************************/

	void ComboBox::HideElementList ()
	{
		assert( button->IsNotPressed() );

		ClipOffElementListFromControlBorder();

		elementList->SetRendering(false);
	}

	void ComboBox::ClipOffElementListFromControlBorder ()
	{
		controlBorder.bottom = controlBorder.top - buttonSize;	
	}
//
	void ComboBox::ShowElementList ()
	{
		assert( button->IsPressed() );
		
		AdjustElementListBorder( CalculateNewListLength() );
		ExtendControlBorderByElementListBorder();

		elementList->SetRendering(true);
	}
	
	float ComboBox::CalculateNewListLength ()
	{
		float newListLength;
		
		float maximumListLength = currentFont.height * elementListLength;
		float elementsLength    = currentFont.height * elementList->GetElementCount();	

		if( elementsLength < maximumListLength )
		{
			newListLength = elementsLength;
		}
		else
			newListLength = maximumListLength;


		if( newListLength < elementList->GetMinHeight() )
		{
			newListLength = elementList->GetMinHeight();
		}


		return newListLength;
	}

	void ComboBox::AdjustElementListBorder (float const &newListLength)
	{
		RECT listBorder = controlBorder;

		listBorder.top    = controlBorder.bottom;
		listBorder.bottom = listBorder.bottom - newListLength;

		elementList->SetBorders(listBorder);
	}

	void ComboBox::ExtendControlBorderByElementListBorder ()
	{
		RECT listBorder = elementList->GetBorders();

		controlBorder.bottom = listBorder.bottom;
	}
//
/*****************************************************/

	void ComboBox::Save (std::fstream &file)
	{
		UpperClass::Save(file);

		file << "\t" << "#----------------------------------------ComboBox_components----------------------------------------#" << "\n\n";

		SaveCurrentFont(file);
		SaveTextColor(file);
		SaveSelectionColor(file);
		SaveAttributes(file);
	}
//
	void ComboBox::SaveCurrentFont    (std::fstream &file)
	{
		file << "\t" << "#Current_font:" << "\t" << currentFont.fontName << "\n"

			 << "\t" << "#Height:" << "\t" << currentFont.height << "\n"
			 << "\t" << "#Width:"  << "\t" << currentFont.width  << "\n"
			 << "\t" << "#Weight:" << "\t" << currentFont.weight << "\n"

			 << "\t" << "#Italic:"    << "\t" << currentFont.Italic    << "\n"
			 << "\t" << "#Underline:" << "\t" << currentFont.Underline << "\n"
			 << "\t" << "#Strikeout:" << "\t" << currentFont.Strikeout << "\n\n";
	}

	void ComboBox::SaveTextColor      (std::fstream &file)
	{
		file << "\t" << "#Text_color:" << "\n"

			 << "\t" << "#Red:"   << "\t" << textColor.GetRed  () << "\n"
			 << "\t" << "#Green:" << "\t" << textColor.GetGreen() << "\n"
			 << "\t" << "#Blue:"  << "\t" << textColor.GetBlue () << "\n"
			 << "\t" << "#Alpha:" << "\t" << textColor.GetAlpha() << "\n\n";
	}

	void ComboBox::SaveSelectionColor (std::fstream &file)
	{
		file << "\t" << "#Selection_color:" << "\n"

			 << "\t" << "#Red:"   << "\t" << selectionColor.GetRed  () << "\n"
			 << "\t" << "#Green:" << "\t" << selectionColor.GetGreen() << "\n"
			 << "\t" << "#Blue:"  << "\t" << selectionColor.GetBlue () << "\n"
			 << "\t" << "#Alpha:" << "\t" << selectionColor.GetAlpha() << "\n\n";
	}

	void ComboBox::SaveAttributes	  (std::fstream &file)
	{
		file << "\t" << "#Button_size:"         << "\t" << buttonSize        << "\n"
			 << "\t" << "#Element_list_length:" << "\t" << elementListLength << "\n\n";
	}
//
	void ComboBox::Load (std::fstream &file)
	{
		UpperClass::Load(file);

		LoadCurrentFont(file);
		LoadTextColor(file);
		LoadSelectionColor(file);
		LoadAttributes(file);

		UpdateContent();
	}
//
	void ComboBox::LoadCurrentFont    (std::fstream &file)
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

	void ComboBox::LoadTextColor      (std::fstream &file)
	{
		std::string line;

		line = ReadNewValue(file, '#');		textColor.SetRed   ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		textColor.SetGreen ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		textColor.SetBlue  ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		textColor.SetAlpha ( atof( line.c_str() ) );
	}

	void ComboBox::LoadSelectionColor (std::fstream &file)
	{
		std::string line;

		line = ReadNewValue(file, '#');		selectionColor.SetRed   ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		selectionColor.SetGreen ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		selectionColor.SetBlue  ( atof( line.c_str() ) );
		line = ReadNewValue(file, '#');		selectionColor.SetAlpha ( atof( line.c_str() ) );
	}

	void ComboBox::LoadAttributes	  (std::fstream &file)
	{
		std::string line;

		line = ReadNewValue(file, '#');		buttonSize        = atof( line.c_str() );
		line = ReadNewValue(file, '#');		elementListLength = atoi( line.c_str() );
	}
//
//

	#undef COMBOBOX_BUTTON_PRESSED  
	#undef COMBOBOX_BUTTON_UNPRESSED

	#undef COMBOBOX_SELECTION_PRESSED 
	#undef COMBOBOX_SELECTION_UNPRESSED

	#undef COMBOBOX_ELEMENT

	#undef COMBOBOX_BAR	

	#undef COMBOBOX_SLIDER_PRESSED	
	#undef COMBOBOX_SLIDER_UNPRESSED

	#undef COMBOBOX_NAVIGATOR_A_PRESSED
	#undef COMBOBOX_NAVIGATOR_A_UNPRESSED

	#undef COMBOBOX_NAVIGATOR_B_PRESSED
	#undef COMBOBOX_NAVIGATOR_B_UNPRESSED


	#undef COMBOBOX_TEXTURE_COUNT 