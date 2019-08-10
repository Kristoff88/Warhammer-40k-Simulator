#pragma once

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
	#define KEY_W ('W')
	#define KEY_X ('X')
	#define KEY_Y ('Y')
	#define KEY_Z ('Z')


	class Handler_Utilities
	{
		public:

			Font& GenerateFont_Label ();
			Font& GenerateFont_Edit  ();
		
	/*****************************************************************
		Basic functions, that help initialize controls.
	*****************************************************************/

			static void Edit_KeyManager (Control *ctrl, int const &key)
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
					case KEY_W:	edit->AddCharacter(KEY_W);		break;
					case KEY_X:	edit->AddCharacter(KEY_X);		break;
					case KEY_Y:	edit->AddCharacter(KEY_Y);		break;
					case KEY_Z:	edit->AddCharacter(KEY_Z);		break;
				}
			}
		//

			void Control_SetDefaultSettings  (Control *ctrl);
			void Label_SetDefaultSettings    (Label *label);
			void Edit_SetDefaultSettings     (Edit *edit);
			void ComboBox_SetDefaultSettings (ComboBox *comboBox);
			void ListBox_SetDefaultSettings  (ListBox *listBox);
		
	/*****************************************************************
			Border setters.
	*****************************************************************/

			void Label_SetBorder    (int const &left, int const &top, Label *label);
			void Edit_SetBorder     (int const &left, int const &top, Edit *edit);
			void Button_SetBorder   (int const &left, int const &top, Button *button);
			void ComboBox_SetBorder (int const &left, int const &top, ComboBox *comboBox);
			void ListBox_SetBorder  (int const &left, int const &top, ListBox *listBox);


			void ShiftSection (int const &x, int const &y, Handler *section);
	};
	