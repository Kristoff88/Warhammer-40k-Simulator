	#include "GlobalHeaders.h"

	#include "Slider.h"



	Slider::Slider()
	{
		controlType = "Slider";
		SetDefaultSliderData();
		AdjustContentSize();
		AdjustContentColor();
	}
//
		void Slider::SetDefaultSliderData()
		{
			Bar = new Control;
			Thumb = new Button;

			contentArray.push_back(Bar);
			contentArray.push_back(Thumb);
			

			// Inserting empty texture slots.
			for(int i = 0; i < SLIDER_TEXTURE_COUNT; i++)
			{
				textures.push_back(0);
			}

			range = 10;
			step = 0;

			Vertical = false;
		}
//
	Slider::Slider(Slider const &slider)
	{
		UpperClass::operator =(slider);
		CopySliderData(slider);
	}
//
		void Slider::CopySliderData(Slider const &slider)
		{
			*Bar = *slider.Bar;
			*Thumb = *slider.Thumb;
			SetTransparency(true);


			range = slider.range;
			step = slider.step;

			Vertical = slider.Vertical;


			RefreshContentArrayPointers();
			UpdateContent();
		}
		void Slider::RefreshContentArrayPointers()
		{
			contentArray.clear();

			contentArray.push_back(Bar);
			contentArray.push_back(Thumb);
		}
//
	Slider::~Slider()
	{}
//
	Slider& Slider::operator= (Slider const &slider)
	{
		UpperClass::operator =(slider);
		CopySliderData(slider);
		return *this;
	}
//
/******************************************************
		Basic managment functions.
******************************************************/

	void Slider::SetStep (unsigned int const &step)
	{
		this->step = step;

		if( this->step >= range )
		{
			this->step = range-1;
		}
		else if( this->step < 0 )
		{
			this->step = 0;
		}

		AdjustThumb();
	}

	int Slider::GetStep ()
	{
		return step;
	}
//
	void Slider::SetRange (unsigned int const &range)
	{
		this->range = range;

		//For autocorrection.
		SetStep(step); 

		AdjustThumb();
	}

	int Slider::GetRange ()
	{
		return range;
	}
//
	void Slider::SetVerticalAlignment   ()
	{
		Vertical = true;
		AdjustContentSize();
	}

	void Slider::SetHorizontalAlignment ()
	{
		Vertical = false;
		AdjustContentSize();
	}
//
	bool Slider::IsVerticallyAligned   ()
	{
		return Vertical;
	}

	bool Slider::IsHorizontallyAligned ()
	{
		return !Vertical;
	}
//
//
	void Slider::SetBarTexture (unsigned int const &textureID)
	{
		textures[SLIDER_BAR] = textureID;
		UpdateContent();
	}

	unsigned int Slider::GetBarTexture ()
	{
		return textures[SLIDER_BAR];
	}
//
	void Slider::Slider_SetPressedTexture (unsigned int const &textureID)
	{
		textures[SLIDER_THUMB_PRESSED] = textureID;
		UpdateContent();
	}

	unsigned int Slider::Slider_GetPressedTexture ()
	{
		return textures[SLIDER_THUMB_PRESSED];
	}
//
	void Slider::Slider_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[SLIDER_THUMB_UNPRESSED] = textureID;
		UpdateContent();
	}

	unsigned int Slider::Slider_GetUnpressedTexture ()
	{
		return textures[SLIDER_THUMB_UNPRESSED];
	}
//
/*****************************************************/

	void Slider::AdjustContentSize  ()
	{
		Bar->SetBorders(controlBorder);
		AdjustThumb();
	}
//
	void Slider::CalculateStep ()
	{
		if( IsVerticallyAligned() )
		{
			CalculateStepVertically();
		}
		else
		{
			CalculateStepHorizontally();
		}
	}

	void Slider::CalculateStepHorizontally   ()
	{
		float sliderWidth 
			= controlBorder.right - controlBorder.left;

		float stepWidth 
			= sliderWidth / (float)range;


		if( thumbPosition.x <= controlBorder.left )
		{
			step = 0;
		}
		else if( thumbPosition.x >= controlBorder.right )
		{
			step = range-1;
		}
		else
		{
			float positionOnBar = thumbPosition.x - controlBorder.left;
			step = positionOnBar / stepWidth;
		}
	}

	void Slider::CalculateStepVertically ()
	{
		float sliderHeight 
			= controlBorder.top - controlBorder.bottom;

		float stepHeight 
			= sliderHeight / (float)range;

		if( thumbPosition.y <= controlBorder.bottom )
		{
			step = range-1;
		}
		else if( thumbPosition.y >= controlBorder.top )
		{
			step = 0;
		}
		else
		{
			float positionOnBar = controlBorder.top - thumbPosition.y;
			step = positionOnBar / stepHeight;
		}
	}
//
	void Slider::AdjustThumb ()
	{
		if( IsVerticallyAligned() )
		{
			AdjustThumbVertically();
		}
		else
		{
			AdjustThumbHorizontally();
		}
	}

	void Slider::AdjustThumbHorizontally   ()
	{
		RECT thumbBorder = Thumb->GetBorders();


		float sliderWidth 
			= controlBorder.right - controlBorder.left;

		float stepWidth 
			= sliderWidth / (float)range;

		thumbBorder.top = controlBorder.top;
		thumbBorder.bottom = controlBorder.bottom;

		thumbBorder.left = controlBorder.left + (step*stepWidth);
		thumbBorder.right = thumbBorder.left + stepWidth;


		Thumb->SetBorders(thumbBorder);
	}

	void Slider::AdjustThumbVertically ()
	{
		RECT thumbBorder = Thumb->GetBorders();


		float sliderHeight = GetHeight();
		float stepHeight   = sliderHeight / (float)range;

		thumbBorder.left = controlBorder.left;
		thumbBorder.right = controlBorder.right;

		thumbBorder.top = controlBorder.top - (step*stepHeight);
		thumbBorder.bottom = thumbBorder.top - stepHeight;


		Thumb->SetBorders(thumbBorder);
	}
//
	void Slider::AdjustContentColor ()
	{
		if( Bar->GetCurrentTexture() == 0 )
		{
			Bar->SetColor(Color(1,1,1,0)*controlColor);
		}
		else
			Bar->SetColor(controlColor);

		if( Thumb->GetCurrentTexture() == 0 )
		{
			Thumb->SetColor(Color(1,1,1,0)*controlColor);
		}
		else
			Thumb->SetColor(controlColor);
	}

	void Slider::UpdateContent ()
	{
		UpperClass::UpdateContent();


		Bar->SetTextures(textures);
		Bar->SetCurrentTexture(SLIDER_BAR+1);

		Thumb->SetTextures(textures);
		Thumb->SetPressTexture(SLIDER_THUMB_PRESSED+1);
		Thumb->SetUnpressTexture(SLIDER_THUMB_UNPRESSED+1);
		Thumb->SetCurrentTexture(SLIDER_THUMB_UNPRESSED+1);
	}
//
/*****************************************************/

	void Slider::MouseLeftDown (POINT const &mousePosition)
	{
		Control::MouseLeftDown(mousePosition);

		if( !IsCurrentlyResized() && CursorHitSliderBar(mousePosition) )
		{
			thumbPosition = mousePosition;
			focusedContent = Thumb;

			CalculateStep();
			AdjustContentSize();
		}
		else
			focusedContent = NULL;
	}

	bool Slider::CursorHitSliderBar (POINT const &mousePosition)
	{
		if( IsVerticallyAligned() )
		{
			return CursorHitVerticalSliderBar(mousePosition);
		}
		else
			return CursorHitHorizontalSliderBar(mousePosition);
	}

	bool Slider::CursorHitVerticalSliderBar   (POINT const &mousePosition)
	{
		bool BarHasBeenHit;

		float controlWidth = controlBorder.right - controlBorder.left;

		RECT barBorder = controlBorder;

		barBorder.left = barBorder.left + controlWidth*0.25f;
		barBorder.right = barBorder.right - controlWidth*0.25f;

		if( mousePosition.x < barBorder.right && mousePosition.x > barBorder.left )
		{
			BarHasBeenHit = true;
		}
		else
			BarHasBeenHit = false;

		return BarHasBeenHit;
	}

	bool Slider::CursorHitHorizontalSliderBar (POINT const &mousePosition)
	{
		bool BarHasBeenHit;

		float controlHeight = controlBorder.top - controlBorder.bottom;

		RECT barBorder = controlBorder;

		barBorder.top = barBorder.top - controlHeight*0.25f;
		barBorder.bottom = barBorder.bottom + controlHeight*0.25f;


		if( mousePosition.y < barBorder.top && mousePosition.y > barBorder.bottom )
		{
			BarHasBeenHit = true;
		}
		else
			BarHasBeenHit = false;

		return BarHasBeenHit;
	}
//
	void Slider::MouseDragged (POINT const &mousePosition)
	{
		UpperClass::MouseDragged(mousePosition);
		
		thumbPosition = mousePosition;
		
		CalculateStep();
		AdjustContentSize();
	}
//
/*****************************************************/

	void Slider::Save (std::fstream &file)
	{
		UpperClass::Save(file);

		file << "\t" << "#----------------------------------------Slider_components----------------------------------------#\n\n";

		file << "\t" << "#Range:" << "\t" << range << "\n"
			 << "\t" << "#Step:"  << "\t" << step  << "\n"

			 << "\t" << "#Vertical:"  << "\t" << Vertical  << "\n\n";
	}

	void Slider::Load (std::fstream &file)
	{
		UpperClass::Load(file);

		std::string line;

		line = ReadNewValue(file, '#');		range = atoi( line.c_str() );
		line = ReadNewValue(file, '#');		step  = atoi( line.c_str() );

		line = ReadNewValue(file, '#');		Vertical  = atoi( line.c_str() );
	}
//
//


	#undef SLIDER_BAR

	#undef SLIDER_THUMB_PRESSED	
	#undef SLIDER_THUMB_UNPRESSED		

	#undef SLIDER_TEXTURE_COUNT	