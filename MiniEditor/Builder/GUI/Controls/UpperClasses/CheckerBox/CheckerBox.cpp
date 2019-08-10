	#include "GlobalHeaders.h"

	#include "CheckerBox.h"



	CheckerBox::CheckerBox()
	{	
		controlType = "CheckerBox";
		SetDefaultCheckerData();
		AdjustContentSize();
		AdjustContentColor();
	}
//
		void CheckerBox::SetDefaultCheckerData()
		{
			PrepareTextureSlots();
			PrepareContent();
			PrepareAttributes();
		}
//
		void CheckerBox::PrepareTextureSlots ()
		{
			// pushing empty texture slots
			for(int i = 0; i < CHECKER_TEXTURE_COUNT; i++)
			{
				textures.push_back(0);
			}
		}

		void CheckerBox::PrepareContent		 ()
		{
			checker = new Button;
			caption = new Label;


			checker->SetSwitchMode(true);

			caption->SetCenterAlignment();
			caption->SetColor( Color(0,0,0,1) );
			caption->SetFrameThickness(0);


			contentArray.push_back(checker);
			contentArray.push_back(caption);
		}

		void CheckerBox::PrepareAttributes   ()
		{
			SetColor(Color(1,1,1,1));
			Inverted = false;
		}
//
	CheckerBox::CheckerBox(CheckerBox const &checkerBox)
	{
		UpperClass::operator=(checkerBox);
		CopyCheckerData(checkerBox);	
	}
	CheckerBox& CheckerBox::operator= (CheckerBox const &checkerBox)
	{
		UpperClass::operator=(checkerBox);
		CopyCheckerData(checkerBox);	
		return *this;
	}
//
		void CheckerBox::CopyCheckerData(CheckerBox const &checkerBox)
		{
			*checker = *checkerBox.checker;
			*caption = *checkerBox.caption;

			Inverted = checkerBox.Inverted;

			RefreshContentArrayPointers();
			UpdateContent();
		}
		void CheckerBox::RefreshContentArrayPointers ()
		{
			contentArray.clear();

			contentArray.push_back(checker);
			contentArray.push_back(caption);
		}
//
	CheckerBox::~CheckerBox()
	{}
//
/******************************************************
		Basic managment functions.
******************************************************/

	void CheckerBox::Invert ()
	{
		Inverted = !Inverted;
		AdjustContentSize();
	}
//
//
	void CheckerBox::Checker_SetPressedTexture   (unsigned int const &textureID)
	{
		textures[CHECKER_PRESSED] = textureID;
		UpdateContent();
	}

	void CheckerBox::Checker_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[CHECKER_UNPRESSED] = textureID;
		UpdateContent();
	}
//
	unsigned int CheckerBox::Checker_GetPressedTexture   ()
	{
		return textures[CHECKER_PRESSED];
		UpdateContent();
	}

	unsigned int CheckerBox::Checker_GetUnpressedTexture ()
	{
		return textures[CHECKER_UNPRESSED];
		UpdateContent();
	}
//
	void CheckerBox::SetFont (Font &font)
	{
		caption->SetFont(font);
	}

	Font CheckerBox::GetFont ()
	{
		return caption->GetCurrentFont();
	}
//
	void CheckerBox::SetText (std::string const &text)
	{
		caption->SetText(text);
	}
	std::string CheckerBox::GetText ()
	{
		return caption->GetText();
	}
//
	void CheckerBox::SetTextColor (Color const &color)
	{
		caption->SetTextColor(color);
	}

	Color CheckerBox::GetTextColor ()
	{
		return caption->GetTextColor();
	}
//
//
	void CheckerBox::Press        ()
	{
		checker->Press();
	}
	bool CheckerBox::IsPressed    ()
	{
		return checker->IsPressed();
	}

	bool CheckerBox::IsNotPressed ()
	{
		return checker->IsNotPressed();
	}
//
/*****************************************************/

	void CheckerBox::AdjustContentSize ()
	{
		if( !Inverted )
		{
			CheckerToLeft();
		}
		else
		{
			CheckerToRight();
		}
	}
//
		void CheckerBox::CheckerToLeft  ()
		{
			float captionSize = 0.75f;
			RECT leftControl, rightControl;


			leftControl = controlBorder;
			leftControl.right = leftControl.right - (leftControl.right-leftControl.left)*captionSize;

			rightControl = AdjustLabelToTextHeight();
			rightControl.left = leftControl.right;
			rightControl.right = controlBorder.right;

			
			checker->SetBorders(leftControl);
			caption->SetBorders(rightControl);
		}

		void CheckerBox::CheckerToRight ()
		{
			float captionSize = 0.75f;
			RECT leftControl, rightControl;


			rightControl = controlBorder;
			rightControl.left = rightControl.left + (rightControl.right-rightControl.left)*captionSize;

			leftControl = AdjustLabelToTextHeight();
			leftControl.right = rightControl.left;
			leftControl.left = controlBorder.left;


			checker->SetBorders(rightControl);
			caption->SetBorders(leftControl);
		}
//
		RECT CheckerBox::AdjustLabelToTextHeight ()
		{
			RECT captionBorder;

			captionBorder.left = captionBorder.right = 
			captionBorder.top = captionBorder.bottom = 0;


			int textHeight = caption->GetRowCount() * caption->GetCurrentFont().height;
			float leftoverSpace = GetHeight() - textHeight;


			if( leftoverSpace >= 0 )
			{
				captionBorder.top    = controlBorder.top    - (leftoverSpace/2.f);
				captionBorder.bottom = controlBorder.bottom + (leftoverSpace/2.f);
			}

			return captionBorder;
		}
//
	void CheckerBox::AdjustContentColor ()
	{
		if( checker->GetCurrentTexture() == 0 )
		{
			checker->SetColor(Color(1,1,1,0)*controlColor);
		}
		else
			checker->SetColor(controlColor);

		assert( caption->GetCurrentTexture() == 0 );
		caption->SetColor(Color(1,1,1,0)*controlColor);
	}
	void CheckerBox::UpdateContent ()
	{
		UpperClass::UpdateContent();


		assert( textures.size() == CHECKER_TEXTURE_COUNT+1 );

		checker->SetTextures(textures);

		checker->SetPressTexture(CHECKER_PRESSED+1);
		checker->SetUnpressTexture(CHECKER_UNPRESSED+1);
		checker->SetCurrentTexture(CHECKER_UNPRESSED+1);
	}
//
/*****************************************************/

	void CheckerBox::Save (std::fstream &file)
	{
		UpperClass::Save(file);

		file << "\t" << "#----------------------------------------CheckerBox_components----------------------------------------#\n\n"
			 << "\t" << "#Inverted:" << "\t" << Inverted << "\n\n";
	}

	void CheckerBox::Load (std::fstream &file)
	{
		UpperClass::Load(file);

		std::string line = ReadNewValue(file, '#');
		Inverted = atoi( line.c_str() );
	}
//
//


	#undef CHECKER_PRESSED      
	#undef CHECKER_UNPRESSED         

	#undef CHECKER_TEXTURE_COUNT 