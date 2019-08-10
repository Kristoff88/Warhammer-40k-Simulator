	#include "GlobalHeaders.h"

	#include "ProgressBar.h"



	ProgressBar::ProgressBar()
	{
		controlType = "ProgressBar";
		SetDefaultProgressBarData();
		AdjustContentSize();
		AdjustContentColor();
	}
//
		void ProgressBar::SetDefaultProgressBarData()
		{
			PrepareTextureSlots();
			PrepareContent();
			PrepareAttributes();
		}
//
		void ProgressBar::PrepareTextureSlots ()
		{
			// Blank texture slots.
			for(int i = 0; i < BAR_TEXTURE_COUNT; i++)
			{
				textures.push_back(0);
			}
		}

		void ProgressBar::PrepareContent		 ()
		{
			Bar = new Control;
			PercentageText = new Label;


			PercentageText->SetText("0%");
			PercentageText->SetTextColor(Color(0,0,0,1));
			PercentageText->SetColor(Color(1,1,1,0));
			PercentageText->SetCenterAlignment();
			PercentageText->SetFrameThickness(0);
			PercentageText->SetTransparency(true);


			contentArray.push_back(Bar);
			contentArray.push_back(PercentageText);
		}

		void ProgressBar::PrepareAttributes   ()
		{
			SetColor(Color(1,1,1,1));

			range = 100;
			step  = 0;

			percentage = 0;
			RenderPercentage = true;
		}
//
	ProgressBar::ProgressBar(ProgressBar const &progressBar)
	{
		UpperClass::operator =(progressBar);
		CopyProgressBarData(progressBar);
	}
	ProgressBar& ProgressBar::operator= (ProgressBar const &progressBar)
	{
		UpperClass::operator =(progressBar);
		CopyProgressBarData(progressBar);
		return *this;
	}
//
		void ProgressBar::CopyProgressBarData(ProgressBar const &progressBar)
		{
			CopyContent(progressBar);
			CopyAttributes(progressBar);

			RefreshContentArrayPointers();
			UpdateContent();
		}

//
		void ProgressBar::CopyContent		         (ProgressBar const &progressBar)
		{
			*Bar = *progressBar.Bar;
			*PercentageText = *progressBar.PercentageText;
		}

		void ProgressBar::CopyAttributes             (ProgressBar const &progressBar)
		{
			range = progressBar.range;
			step  = progressBar.step;

			percentage = progressBar.percentage;
			RenderPercentage = progressBar.RenderPercentage;
		}

		void ProgressBar::RefreshContentArrayPointers()
		{
			contentArray.clear();

			contentArray.push_back(Bar);
			contentArray.push_back(PercentageText);
		}
//
	ProgressBar::~ProgressBar()
	{}
//
/******************************************************
		Basic managment functions.
******************************************************/

	void ProgressBar::SetBarTexture (unsigned int const &textureID)
	{
		textures[BAR_TEXTURE] = textureID;
		UpdateContent();
	}
	unsigned int ProgressBar::GetBarTexture ()
	{
		return textures[BAR_TEXTURE];
	}
//
	void ProgressBar::Increment (int const &step)
	{
 		this->step += step;

		if( this->step > range )
		{
			this->step = range;
		}

		ProcessPercentage();
		AdjustContentSize();
	}

	void ProgressBar::Decrement (int const &step)
	{
		this->step -= step;

		if( this->step < 0 )
		{
			this->step = 0;
		}

		ProcessPercentage();
		AdjustContentSize();
	}
//
	void ProgressBar::SetRange (unsigned int const &range)
	{
		this->range = range;

		ProcessPercentage();
		AdjustContentSize();
	}

	void ProgressBar::SetStep  (unsigned int const &step)
	{
		this->step = step;

		if( this->step > range )
		{
			this->step = range;
		}

		ProcessPercentage();
		AdjustContentSize();
	}
//
	int ProgressBar::GetRange ()
	{
		return range;
	}

	int ProgressBar::GetStep  ()
	{
		return step;
	}
//
	void ProgressBar::SetFont  (Font &font)
	{
		PercentageText->SetFont(font, GetDC(NULL));
	}

	void ProgressBar::SetTextColor (Color const &color)
	{
		PercentageText->SetTextColor(color);
	}
//
	void ProgressBar::ShowPercentage (bool const &show)
	{
		RenderPercentage = show;
		ProcessPercentage();
	}
//
	void ProgressBar::SetTransparency (bool const &IsTransparent)
	{
		Control::SetTransparency(IsTransparent);
		Bar->SetTransparency(IsTransparent);
	}
//
/*****************************************************/

	void ProgressBar::ProcessPercentage ()
	{
		CalculatePercentage();
		SetPercentageText();
	}
//
		void ProgressBar::CalculatePercentage ()
		{
			percentage = (float)step/(float)range;
		}

		void ProgressBar::SetPercentageText   ()
		{
			if( RenderPercentage )
			{
				char percentageAsText[MAX_PATH];

				_itoa_s(percentage*100, percentageAsText, 10);
				strcat(percentageAsText, "%");

				PercentageText->SetText(percentageAsText);
			}
			else
				PercentageText->SetText("");
		}
//
	void ProgressBar::AdjustContentColor ()
	{
		if( Bar->GetCurrentTexture() == 0 )
		{
			Bar->SetColor(Color(1,1,1,0)*controlColor);
		}
		else
			Bar->SetColor(controlColor);
	}
	void ProgressBar::AdjustContentSize  ()
    {
		RECT barBorder = controlBorder,
			 percentageBorder = controlBorder;


		barBorder.right = barBorder.left + (barBorder.right-barBorder.left)*percentage;
		percentageBorder.top = percentageBorder.top - (percentageBorder.top-percentageBorder.bottom)/4.f;


		Bar->SetBorders(barBorder);
		PercentageText->SetBorders(percentageBorder);
    }
//
	void ProgressBar::UpdateContent ()
	{
		UpperClass::UpdateContent();

		Bar->SetTextures(textures);
		Bar->SetCurrentTexture(BAR_TEXTURE+1);
	}
//
/*****************************************************/

	void ProgressBar::Save (std::fstream &file)
	{
		UpperClass::Save(file);

		file << "\t" << "#----------------------------------------ProgressBar_components----------------------------------------#\n\n"

   			 << "\t" << "#Range:"      << "\t" << range      << "\n"
			 << "\t" << "#Step:"       << "\t" << step       << "\n\n"

			 << "\t" << "#Percentage:"       << "\t" << percentage       << "\n"
			 << "\t" << "#RenderPercentage:" << "\t" << RenderPercentage << "\n\n";
	}

	void ProgressBar::Load (std::fstream &file)
	{
		std::string line;

		UpperClass::Load(file);

		line = ReadNewValue(file, '#');		range = atoi( line.c_str() );
		line = ReadNewValue(file, '#');		step  = atoi( line.c_str() );

		line = ReadNewValue(file, '#');		percentage       = atof( line.c_str() );
		line = ReadNewValue(file, '#');		RenderPercentage = atoi( line.c_str() );
	}

//
//

	#undef BAR_TEXTURE		        
	#undef BAR_TEXTURE_COUNT		