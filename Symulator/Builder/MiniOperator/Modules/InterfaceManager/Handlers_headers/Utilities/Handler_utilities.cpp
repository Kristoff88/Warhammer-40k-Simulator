#pragma once

	#include "GlobalHeaders.h"

	#include "Handler_utilities.h"


	
	Font& Handler_Utilities::GenerateFont_Label ()
	{
		Font font;

		font.fontName = "Courier";

		font.height = 18;
		font.width  = 7;
		
		font.weight = FW_NORMAL;

		font.Italic    = false;
		font.Underline = false;
		font.Strikeout = false;

		return font;
	}

	Font& Handler_Utilities::GenerateFont_Edit  ()
	{
		Font font;

		font.fontName = "Courier";

		font.height = 24;
		font.width  = 12;
		
		font.weight = FW_BOLD;

		font.Italic    = false;
		font.Underline = false;
		font.Strikeout = false;

		return font;
	}
//
/*****************************************************************
	Basic functions, that help initialize controls.
*****************************************************************/

	void Handler_Utilities::Control_SetDefaultSettings  (Control *ctrl)
	{
		ctrl->SetFrameThickness(0.f);
		ctrl->SetResizability(false);
		ctrl->SetStatic(true);
	}

	void Handler_Utilities::Label_SetDefaultSettings    (Label *label)
	{
		Font font = GenerateFont_Label();
		label->SetFont(font);
	}

	void Handler_Utilities::Edit_SetDefaultSettings     (Edit *edit)
	{
		Font font = GenerateFont_Edit();
		edit->SetFont(font);
		edit->SetColor( Color(0,0.45,0.9,1) );
	}

	void Handler_Utilities::ComboBox_SetDefaultSettings (ComboBox *comboBox)
	{
		Font font = GenerateFont_Edit();
		comboBox->SetFont(font);


		GUI_DataManager &data = GUI_DataManager::Singleton();

		comboBox->Selection_SetPressedTexture( data.GetTexture("Element_Pressed") );
		comboBox->Selection_SetUnpressedTexture( data.GetTexture("Element_Unpressed") );

		comboBox->Button_SetPressedTexture( data.GetTexture("Button_Pressed") );
		comboBox->Button_SetUnpressedTexture( data.GetTexture("Button_Unpressed") );

		comboBox->Element_SetTexture( data.GetTexture("Element_Unpressed") );

		comboBox->NavigatorA_SetPressedTexture( data.GetTexture("NavigatorA_Pressed")  );
		comboBox->NavigatorA_SetUnpressedTexture( data.GetTexture("NavigatorA_Unpressed")  );

		comboBox->NavigatorB_SetPressedTexture( data.GetTexture("NavigatorB_Pressed")  );
		comboBox->NavigatorB_SetUnpressedTexture( data.GetTexture("NavigatorB_Unpressed")  );

		comboBox->Slider_SetPressedTexture( data.GetTexture("Slider_Pressed") );
		comboBox->Slider_SetUnpressedTexture( data.GetTexture("Slider_Unpressed") );

		comboBox->Bar_SetTexture( data.GetTexture("bar2") );
	}

	void Handler_Utilities::ListBox_SetDefaultSettings  (ListBox *listBox)
	{
		Font font = GenerateFont_Edit();
		listBox->SetFont(font);

		listBox->SetMultiSelection(true);


		GUI_DataManager &data = GUI_DataManager::Singleton();

		listBox->SetBackground( data.GetTexture("Element_Unpressed") );

		listBox->Element_SetPressedTexture( data.GetTexture("Element_Unpressed") );
		listBox->Element_SetUnpressedTexture( data.GetTexture("Element_Unpressed") );

		listBox->NavigatorA_SetPressedTexture( data.GetTexture("NavigatorA_Pressed")  );
		listBox->NavigatorA_SetUnpressedTexture( data.GetTexture("NavigatorA_Unpressed")  );

		listBox->NavigatorB_SetPressedTexture( data.GetTexture("NavigatorB_Pressed")  );
		listBox->NavigatorB_SetUnpressedTexture( data.GetTexture("NavigatorB_Unpressed")  );

		listBox->Slider_SetPressedTexture( data.GetTexture("Slider_Pressed") );
		listBox->Slider_SetUnpressedTexture( data.GetTexture("Slider_Unpressed") );
	}
//
/*****************************************************************
		Border setters.
*****************************************************************/

	#define LABEL_WIDTH      (150)
	#define LABEL_HEIGHT     (20)

	#define EDIT_WIDTH       (350)
	#define EDIT_HEIGHT      (30)

	#define BUTTON_WIDTH     (50)
	#define BUTTON_HEIGHT    (50)

	#define COMBO_BOX_WIDTH  (450)
	#define COMBO_BOX_HEIGHT (50)

	#define LIST_BOX_WIDTH   (550)
	#define LIST_BOX_HEIGHT  (300)


	void Handler_Utilities::Label_SetBorder    (int const &left, int const &top, Label *label)
	{
		RECT border;

		border.left = left;
		border.top = top;

		border.right = border.left + LABEL_WIDTH;
		border.bottom = border.top - LABEL_HEIGHT;

		label->SetBorders(border);
	}

	void Handler_Utilities::Edit_SetBorder     (int const &left, int const &top, Edit *edit)
	{
		RECT border;

		border.left = left;
		border.top = top;

		border.right = border.left + EDIT_WIDTH;
		border.bottom = border.top - EDIT_HEIGHT;

		edit->SetBorders(border);
	}

	void Handler_Utilities::Button_SetBorder   (int const &left, int const &top, Button *button)
	{
		RECT border;

		border.left = left;
		border.top = top;

		border.right = border.left + BUTTON_WIDTH;
		border.bottom = border.top - BUTTON_HEIGHT;

		button->SetBorders(border);
	}

	void Handler_Utilities::ComboBox_SetBorder (int const &left, int const &top, ComboBox *comboBox)
	{
		RECT border;

		border.left = left;
		border.top = top;

		border.right = border.left + COMBO_BOX_WIDTH;
		border.bottom = border.top - COMBO_BOX_HEIGHT;

		comboBox->SetBorders(border);
	}

	void Handler_Utilities::ListBox_SetBorder  (int const &left, int const &top, ListBox *listBox)
	{
		RECT border;

		border.left = left;
		border.top = top;

		border.right = border.left + LIST_BOX_WIDTH;
		border.bottom = border.top - LIST_BOX_HEIGHT;

		listBox->SetBorders(border);
	}
//
	void Handler_Utilities::ShiftSection (int const &x, int const &y, Handler *section)
	{
		POINT shift;
		
		shift.x = x;
		shift.y = y;

		section->Move(shift);
	}
//
//

	#undef LABEL_WIDTH  
	#undef LABEL_HEIGHT

	#undef EDIT_WIDTH
	#undef EDIT_HEIGHT 

	#undef BUTTON_WIDTH 
	#undef BUTTON_HEIGHT 

	#undef COMBO_BOX_WIDTH 
	#undef COMBO_BOX_HEIGHT

	#undef LIST_BOX_WIDTH  
	#undef LIST_BOX_HEIGHT


	