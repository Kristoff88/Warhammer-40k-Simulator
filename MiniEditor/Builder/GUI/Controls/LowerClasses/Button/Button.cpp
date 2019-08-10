	#include "GlobalHeaders.h"

	#include "Button.h"


	#define DEFAULT_PRESSTIME (500)


	Button::Button()
	{
		controlType = "Button";
		SetDefaultButtonData();
	}
//
		void Button::SetDefaultButtonData()
		{
			ClearAlignmentFlags();
			SetCenterAlignment();

			if( !buttonTimer.Init() )
			{
				MessageBox(NULL, "HiResTimer initialization failure.", "(Button constructor)", MB_OK);
			}

			pressTime = DEFAULT_PRESSTIME;
			pressTimeLeft = 0;

			pressed_Texture = unpressed_Texture = 0;

			Pressed    = false;
			SwitchMode = false;
		}
//
	Button::Button(Button const &button)
	{
		this->Label::operator=(button);
		CopyButtonData(button);
	}
//
		void Button::CopyButtonData(Button const &button)
		{
			pressTime  = button.pressTime;
			SwitchMode = button.SwitchMode;
		}
//
	Button::~Button()
	{}
//
	Button& Button::operator= (Control const &ctrl)
	{
		Control::operator=(ctrl);
		return *this;
	}

	Button& Button::operator= (Label const &label)
	{
		Label::operator=(label);
		return *this;
	}

	Button& Button::operator= (Button const &button)
	{
		this->Label::operator=(button);
		CopyButtonData(button);
		return *this;
	}
//
/******************************************************
		Properties managment.
******************************************************/

	void Button::SetSwitchMode (bool const &IsSwitchMode)
	{
		SwitchMode = IsSwitchMode;
	}
//
	void Button::SetPressTime    (float const &time)
	{
		pressTime = time / 1000.f;
	}

//
	void Button::SetPressTexture   (int const &texture)
	{
		if( texture < 0 || texture > textures.size() )
			pressed_Texture = 0;
		else
			pressed_Texture = texture;
	}


	void Button::SetUnpressTexture (int const &texture)
	{
		if( texture < 0 || texture > textures.size() )
			unpressed_Texture = 0;
		else
			unpressed_Texture = texture;
	}
//
	void Button::Press   ()
	{
		if( SwitchMode )
		{
			Pressed = !Pressed;
		}
		else
			Pressed = true;


		if( Pressed )
		{
			currentTexture = pressed_Texture;
		}
		else if( !SwitchMode )
		{
			pressTimeLeft = pressTime;
		}
		else
			currentTexture = unpressed_Texture;
	}

	void Button::Unpress ()
	{
		if( !SwitchMode )
		{
			Pressed = false;
			pressTimeLeft = pressTime;
		}
	}
//
	bool Button::IsSwitch     ()
	{
		return SwitchMode;
	}

	bool Button::IsPressed    ()
	{
		return Pressed;
	}
	bool Button::IsNotPressed ()
	{
		return !Pressed;
	}
//
/******************************************************
		Standard functions redefinitions.
******************************************************/

	void Button::MouseLeftDown       (POINT const &mousePosition)
	{
		Press();
	}

	void Button::MouseMiddleDown     (POINT const &mousePosition)
	{
		Press();
	}

	void Button::MouseRightDown      (POINT const &mousePosition)
	{
		Press();
	}

//
	void Button::MouseLeftUp         (POINT const &mousePosition)
	{
		Unpress();
	}

	void Button::MouseMiddleUp       (POINT const &mousePosition)
	{
		Unpress();
	}

	void Button::MouseRightUp        (POINT const &mousePosition)
	{
		Unpress();
	}

//
	void Button::MouseLeftDblClick   (POINT const &mousePosition)
	{
		Press();
	}

	void Button::MouseMiddleDblClick (POINT const &mousePosition)
	{
		Press();
	}

	void Button::MouseRightDblClick  (POINT const &mousePosition)
	{
		Press();
	}

//
//
	void Button::Render              (Color const &GUIColor, bool const &TransparentGUI)
	{
		if( pressTimeLeft )
		{
			PressedCountdown();	

			if( !pressTimeLeft )
			{
				currentTexture = unpressed_Texture;
			}
		}

		Label::Render(GUIColor, TransparentGUI);
	}
//
		void Button::PressedCountdown ()
		{
			float miliSeconds = buttonTimer.GetElapsedSeconds()*1000;

			pressTimeLeft -= miliSeconds;

			if( pressTimeLeft < 0 )
			{
				pressTimeLeft = 0;
			}
		}
//
/*****************************************************/

	void Button::Save (std::fstream &file)
	{
		Label::Save(file);

		file << "\t" << "#----------------------------------------Button_components----------------------------------------#\n\n";

		file << "\t" << "#Press_time:"        << "\t" << pressTime         << "\n"
			 << "\t" << "#Pressed_texture:"   << "\t" << pressed_Texture   << "\n"
			 << "\t" << "#Unpressed_texture:" << "\t" << unpressed_Texture << "\n"
			 << "\t" << "#Switch_mode:"       << "\t" << SwitchMode        << "\n\n";
	}

	void Button::Load (std::fstream &file)
	{
		Label::Load(file);


		std::string line;

		line = ReadNewValue(file, '#');		pressTime         = atof( line.c_str() );
		line = ReadNewValue(file, '#');		pressed_Texture   = atoi( line.c_str() );
		line = ReadNewValue(file, '#');		unpressed_Texture = atoi( line.c_str() );
		line = ReadNewValue(file, '#');		SwitchMode        = atoi( line.c_str() );
	}