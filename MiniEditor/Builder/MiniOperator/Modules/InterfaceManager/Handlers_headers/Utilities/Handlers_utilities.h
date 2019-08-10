#pragma once



	Font& GenerateFont_Label ()
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

	Font& GenerateFont_Edit  ()
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

	#define KEY_LEFT	  ('<')
	#define KEY_RIGHT	  ('>')

	#define KEY_SPACE	  ('_')
	#define KEY_BACKSPACE ('/')

	#define KEY_0 ('0')
	#define KEY_1 ('1')
	#define KEY_2 ('2')
	#define KEY_3 ('3')
	#define KEY_4 ('4')
	#define KEY_5 ('5')
	#define KEY_6 ('6')
	#define KEY_7 ('7')
	#define KEY_8 ('8')
	#define KEY_9 ('9')

	#define KEY_A ('A')
	#define KEY_B ('B')
	#define KEY_C ('C')
	#define KEY_D ('D')
	#define KEY_E ('E')
	#define KEY_F ('F')
	#define KEY_G ('G')
	#define KEY_H ('H')
	#define KEY_I ('I')
	#define KEY_J ('J')
	#define KEY_K ('K')
	#define KEY_L ('L')
	#define KEY_M ('M')
	#define KEY_N ('N')
	#define KEY_O ('O')
	#define KEY_P ('P')
	#define KEY_R ('R')
	#define KEY_S ('S')
	#define KEY_T ('T')
	#define KEY_U ('U')
	#define KEY_V ('V')
	#define KEY_W ('W')
	#define KEY_X ('X')
	#define KEY_Y ('Y')
	#define KEY_Z ('Z')


	void Edit_KeyManager          (Control *ctrl, int const &key)
	{
		Edit *edit = reinterpret_cast <Edit*> (ctrl);
		
		switch(key)
		{
			case KEY_LEFT:  	edit->MoveCaret_Left();		break;
			case KEY_RIGHT:		edit->MoveCaret_Right();	break;


			case KEY_SPACE:  	edit->AddCharacter('_');	break;
			case KEY_BACKSPACE:	edit->RemoveCharacter();	break;


			case KEY_0:	edit->AddCharacter(KEY_0);		break;
			case KEY_1:	edit->AddCharacter(KEY_1);		break;
			case KEY_2:	edit->AddCharacter(KEY_2);		break;
			case KEY_3:	edit->AddCharacter(KEY_3);		break;
			case KEY_4:	edit->AddCharacter(KEY_4);		break;
			case KEY_5:	edit->AddCharacter(KEY_5);		break;
			case KEY_6:	edit->AddCharacter(KEY_6);		break;
			case KEY_7:	edit->AddCharacter(KEY_7);		break;
			case KEY_8:	edit->AddCharacter(KEY_8);		break;
			case KEY_9:	edit->AddCharacter(KEY_9);		break;


			case KEY_A:	edit->AddCharacter(KEY_A);		break;
			case KEY_B:	edit->AddCharacter(KEY_B);		break;
			case KEY_C:	edit->AddCharacter(KEY_C);		break;
			case KEY_D:	edit->AddCharacter(KEY_D);		break;
			case KEY_E:	edit->AddCharacter(KEY_E);		break;
			case KEY_F:	edit->AddCharacter(KEY_F);		break;
			case KEY_G:	edit->AddCharacter(KEY_G);		break;
			case KEY_H:	edit->AddCharacter(KEY_H);		break;
			case KEY_I:	edit->AddCharacter(KEY_I);		break;
			case KEY_J:	edit->AddCharacter(KEY_J);		break;
			case KEY_K:	edit->AddCharacter(KEY_K);		break;
			case KEY_L:	edit->AddCharacter(KEY_L);		break;
			case KEY_M:	edit->AddCharacter(KEY_M);		break;
			case KEY_N:	edit->AddCharacter(KEY_N);		break;
			case KEY_O:	edit->AddCharacter(KEY_O);		break;
			case KEY_P:	edit->AddCharacter(KEY_P);		break;
			case KEY_R:	edit->AddCharacter(KEY_R);		break;
			case KEY_S:	edit->AddCharacter(KEY_S);		break;
			case KEY_T:	edit->AddCharacter(KEY_T);		break;
			case KEY_U:	edit->AddCharacter(KEY_U);		break;
			case KEY_V:	edit->AddCharacter(KEY_V);		break;
			case KEY_W:	edit->AddCharacter(KEY_W);		break;
			case KEY_X:	edit->AddCharacter(KEY_X);		break;
			case KEY_Y:	edit->AddCharacter(KEY_Y);		break;
			case KEY_Z:	edit->AddCharacter(KEY_Z);		break;
		}
	}

//
	inline void Control_SetDefaultSettings  (Control *ctrl)
	{
		ctrl->SetFrameThickness(0.f);
		ctrl->SetResizability(false);
		ctrl->SetStatic(true);
	}
	inline void Label_SetDefaultSettings    (Label *label)
	{
		Font font = GenerateFont_Label();
		label->SetFont(font);
	}
	inline void Edit_SetDefaultSettings     (Edit *edit)
	{
		Font font = GenerateFont_Edit();
		edit->SetFont(font);

		edit->SetColor( Color(0,0.45,0.9,1) );

		edit->SetNumbersOnly(true);
		edit->AddCharacter('1');
		edit->SetMaxCharacterCount(2);
	}
	inline void ComboBox_SetDefaultSettings (ComboBox *comboBox)
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
//
/*****************************************************************
		Border setters.
*****************************************************************/

	#define LABEL_WIDTH      (150)
	#define LABEL_HEIGHT     (20)

	#define EDIT_WIDTH       (50)
	#define EDIT_HEIGHT      (30)

	#define BUTTON_WIDTH     (50)
	#define BUTTON_HEIGHT    (50)

	#define COMBO_BOX_WIDTH  (300)
	#define COMBO_BOX_HEIGHT (50)


	inline void Label_SetBorder    (int const &left, int const &top, Label *label)
	{
		RECT border;

		border.left = left;
		border.top = top;

		border.right = border.left + LABEL_WIDTH;
		border.bottom = border.top - LABEL_HEIGHT;

		label->SetBorders(border);
	}
	inline void Edit_SetBorder     (int const &left, int const &top, Edit *edit)
	{
		RECT border;

		border.left = left;
		border.top = top;

		border.right = border.left + EDIT_WIDTH;
		border.bottom = border.top - EDIT_HEIGHT;

		edit->SetBorders(border);
	}
	inline void Button_SetBorder   (int const &left, int const &top, Button *button)
	{
		RECT border;

		border.left = left;
		border.top = top;

		border.right = border.left + BUTTON_WIDTH;
		border.bottom = border.top - BUTTON_HEIGHT;

		button->SetBorders(border);
	}
	inline void ComboBox_SetBorder (int const &left, int const &top, ComboBox *comboBox)
	{
		RECT border;

		border.left = left;
		border.top = top;

		border.right = border.left + COMBO_BOX_WIDTH;
		border.bottom = border.top - COMBO_BOX_HEIGHT;

		comboBox->SetBorders(border);
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