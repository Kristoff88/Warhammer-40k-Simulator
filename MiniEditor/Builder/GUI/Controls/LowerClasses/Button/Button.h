#pragma once

	#include "..//Label//Label.h"


	class Button: public Label
	{
		friend UpperClass;


		public:

			Button();
				private:
				void SetDefaultButtonData();
			public:

			Button(Button const &button);
				private:
				void CopyButtonData(Button const &button);
			public:

			~Button();

			Button& Button::operator= (Control const &ctrl);
			Button& Button::operator= (Label const &label);
			Button& operator= (Button const &button);


	/******************************************************
			Properties managment.
	******************************************************/

			void SetSwitchMode (bool const &IsSwitchMode);

			void SetPressTime    (float const &time);

			void SetPressTexture   (int const &texture);
			void SetUnpressTexture (int const &texture);

			void Press   ();
			void Unpress ();

			void ManagePress   ();
			void ManageUnpress ();

			bool IsSwitch     ();
			bool IsPressed    ();
			bool IsNotPressed ();

		private:

	/******************************************************
			Standard functions redefinitions.
	******************************************************/

			void MouseLeftDown       (POINT const &mousePosition);
			void MouseMiddleDown     (POINT const &mousePosition);
			void MouseRightDown      (POINT const &mousePosition);

			void MouseLeftUp         (POINT const &mousePosition);
			void MouseMiddleUp       (POINT const &mousePosition);
			void MouseRightUp        (POINT const &mousePosition);

			void MouseLeftDblClick   (POINT const &mousePosition);
			void MouseMiddleDblClick (POINT const &mousePosition);
			void MouseRightDblClick  (POINT const &mousePosition);


			void Render              (Color const &GUIColor, bool const &TransparentGUI);
				void PressedCountdown ();

	/*****************************************************/

			void Save (std::fstream &file);
			void Load (std::fstream &file);

	/******************************************************
			Variables, flags.
	******************************************************/

			CHiResTimer buttonTimer;

			float pressTime;
			float pressTimeLeft;


			int pressed_Texture, 
				unpressed_Texture;


			bool Pressed;

			bool SwitchMode;
	};