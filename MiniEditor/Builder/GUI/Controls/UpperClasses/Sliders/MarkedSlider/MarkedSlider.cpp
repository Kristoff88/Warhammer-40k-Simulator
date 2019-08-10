	#include "GlobalHeaders.h"

	#include "MarkedSlider.h"





	MarkedSlider::MarkedSlider()
	{
		controlType = "MarkedSlider";
		SetDefaultMarkedSliderData();

		AdjustContentSize();
		AdjustContentColor();
	}
//
		void MarkedSlider::SetDefaultMarkedSliderData()
		{
			PrepareContent();
			PrepareAttributes();			
		}
//
		void MarkedSlider::PrepareContent    ()
		{
			Mark_A = new Label;
			Mark_B = new Label;

			Mark_A->SetFrameThickness(0);
			Mark_B->SetFrameThickness(0);
			
			Mark_A->SetMultiline(false);
			Mark_B->SetMultiline(false);

			contentArray.push_back(Mark_A);
			contentArray.push_back(Mark_B);
		}

		void MarkedSlider::PrepareAttributes ()
		{
			SetTransparency(true);
			textColor = Color(0,0,0,1);
			RenderMarks = true;
		}
//
	MarkedSlider::MarkedSlider(MarkedSlider const &slider)
	{
		Slider::operator =(slider);
		CopyMarkedSliderData(slider);
	}

	MarkedSlider& MarkedSlider::operator= (MarkedSlider const &slider)
	{
		Slider::operator =(slider);
		CopyMarkedSliderData(slider);
		return *this;
	}
//
		void MarkedSlider::CopyMarkedSliderData(MarkedSlider const &slider)
		{
			*Mark_A = *slider.Mark_A;
			*Mark_B = *slider.Mark_B;

			currentFont = slider.currentFont;
			textColor   = slider.textColor;
			RenderMarks = slider.RenderMarks;


			RefreshContentArrayPointers();
			UpdateContent();
		}

		void MarkedSlider::RefreshContentArrayPointers ()
		{
			Slider::RefreshContentArrayPointers();

			contentArray.push_back(Mark_A);
			contentArray.push_back(Mark_B);
			SetTransparency(true);
		}
//
	MarkedSlider::~MarkedSlider()
	{}
//
/******************************************************
		Basic managment functions.
******************************************************/

	void MarkedSlider::SetFont (Font const &font)
	{
		currentFont = font;
		UpdateContent();
	}

	Font MarkedSlider::GetFont ()
	{
		return currentFont;
	}
//
	void MarkedSlider::SetTextColor (Color const &color)
	{
		textColor = color;
		UpdateContent();
	}

	Color MarkedSlider::GetTextColor ()
	{
		return textColor;
	}
//
	void MarkedSlider::SetText_A (std::string const &text)
	{
		Mark_A->SetText(text);
		Mark_A->FilterOutEndOfLineMarks();

		AdjustContentSize();
	}

	std::string MarkedSlider::GetText_A ()
	{
		return Mark_A->GetText();
	}
//
	void MarkedSlider::SetText_B (std::string const &text)
	{
		Mark_B->SetText(text);
		Mark_B->FilterOutEndOfLineMarks();

		AdjustContentSize();
	}

	std::string MarkedSlider::GetText_B ()
	{
		return Mark_B->GetText();
	}
//
/*****************************************************/

	void MarkedSlider::AdjustContentSize  ()
	{
		Slider::AdjustContentSize();

		SqueezeMarkBordersToSliderRidges();

		if( RenderMarks && SliderIsHighEnoughToFitSingleRowOfText() )
		{
			AdjustLeftMark();
			AdjustRightMark();
		}
	}
//
	void MarkedSlider::SqueezeMarkBordersToSliderRidges       ()
	{
		RECT border = controlBorder;

		border.left = border.right = controlBorder.left;
		Mark_A->SetBorders(border);

		border.left = border.right = controlBorder.right;
		Mark_B->SetBorders(border);
	}

	bool MarkedSlider::SliderIsHighEnoughToFitSingleRowOfText ()
	{
		return GetHeight() >= currentFont.height;
	}

	void MarkedSlider::AdjustLeftMark  ()
	{
		RECT border = controlBorder;
		border.right =  border.left;
		Mark_A->SetBorders(border);


		std::string currentText = Mark_A->GetRow(0);
		std::string wantedText  = Mark_A->GetText();

		while( currentText != wantedText )
		{
			border.left -= currentFont.width;
			Mark_A->SetBorders(border);

			currentText = Mark_A->GetRow(0);
		}
	}

	void MarkedSlider::AdjustRightMark ()
	{
		RECT border = controlBorder;
		border.left =  border.right;
		Mark_B->SetBorders(border);


		std::string currentText = Mark_B->GetRow(0);
		std::string wantedText  = Mark_B->GetText();

		while( currentText != wantedText )
		{
			border.right += currentFont.width;
			Mark_B->SetBorders(border);

			currentText = Mark_B->GetRow(0);
		}
	}
//
	void MarkedSlider::AdjustContentColor ()
	{
		Slider::AdjustContentColor();

		if( Mark_A->GetCurrentTexture() == 0 )
		{
			Mark_A->SetColor(Color(1,1,1,0)*controlColor);
		}
		else
			Mark_A->SetColor(controlColor);

		if( Mark_B->GetCurrentTexture() == 0 )
		{
			Mark_B->SetColor(Color(1,1,1,0)*controlColor);
		}
		else
			Mark_B->SetColor(controlColor);
	}

	void MarkedSlider::UpdateContent ()
	{
		Slider::UpdateContent();

		Mark_A->SetFont(currentFont);
		Mark_B->SetFont(currentFont);

		Mark_A->SetTextColor(textColor);
		Mark_B->SetTextColor(textColor);

		SetMinHeight(currentFont.height + 2*frameThickness);
	}
//
/*****************************************************/

	void MarkedSlider::Save (std::fstream &file)
	{
		Slider::Save(file);

		file << "\t" << "#Render_marks:" << "\t" << RenderMarks << "\n\n";
	}

	void MarkedSlider::Load (std::fstream &file)
	{
		Slider::Load(file);

		std::string line;
		line = ReadNewValue(file, '#');		RenderMarks = atoi( line.c_str() );

		currentFont = Mark_A->GetCurrentFont();
		textColor = Mark_A->GetTextColor();
	}