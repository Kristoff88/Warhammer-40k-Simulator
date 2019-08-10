	#include "GlobalHeaders.h"

	#include "ScrollBar.h"



	ScrollBar::ScrollBar()
	{
		controlType = "ScrollBar";
		SetDefaultScrollBarData();

		AdjustContentColor();
		AdjustContentSize();
		UpdateContent();
	}
//
		void ScrollBar::SetDefaultScrollBarData()
		{
			PrepareTextureSlots();
			PrepareContent();
			PrepareAttributes();			
		}
//
			void ScrollBar::PrepareTextureSlots ()
			{
				// Inserting empty texture slots.
			for(int i = 0; i < SCROLLBAR_TEXTURE_COUNT; i++)
			{
				textures.push_back(0);
			}
			}

			void ScrollBar::PrepareContent	    ()
			{
				LeftNavigator = new Button;
				RightNavigator = new Button;

				contentArray.push_back(LeftNavigator);
				contentArray.push_back(RightNavigator);
			}

			void ScrollBar::PrepareAttributes   ()
			{
				SetNavigatorSize(25);
			}
//
	ScrollBar::ScrollBar(ScrollBar const &scrollBar)
	{
		Slider::operator =(scrollBar);
		CopyScrollBarData(scrollBar);
	}
		
	ScrollBar& ScrollBar::operator= (ScrollBar const &scrollBar)
	{
		Slider::operator =(scrollBar);
		CopyScrollBarData(scrollBar);
		return *this;
	}
//
		void ScrollBar::CopyScrollBarData(ScrollBar const &scrollBar)
		{
			*LeftNavigator = *scrollBar.LeftNavigator;
			*RightNavigator = *scrollBar.RightNavigator;

			navigatorSize = scrollBar.navigatorSize;

			RefreshContentArrayPointers();
			UpdateContent();
		}

		void ScrollBar::RefreshContentArrayPointers()
		{
			Slider::RefreshContentArrayPointers();

			contentArray.push_back(LeftNavigator);
			contentArray.push_back(RightNavigator);
		}
//
	ScrollBar::~ScrollBar()
	{}
//
/******************************************************
		Navigator managment functions.
******************************************************/

	void ScrollBar::SetNavigatorSize (int const &size)
	{
		navigatorSize = size;
		UpdateContent();
		AdjustContentSize();
	}
	int  ScrollBar::GetNavigatorSize ()
	{
		return navigatorSize;
	}
//
	void ScrollBar::SetStep (unsigned int const &step)
	{
		SliceOffNavigatorsFromControlBorder();
		Slider::SetStep(step);
		GlueNavigatorsOntoControlBorder();
	}
//
//
	void ScrollBar::LeftNavigator_SetPressedTexture    (unsigned int const &textureID)
	{
		textures[SCROLLBAR_LEFT_NAVIGATOR_PRESSED] = textureID;
		UpdateContent();
	}

	void ScrollBar::LeftNavigator_SetUnpressedTexture  (unsigned int const &textureID)
	{
		textures[SCROLLBAR_LEFT_NAVIGATOR_UNPRESSED] = textureID;
		UpdateContent();
	}

//
	void ScrollBar::RightNavigator_SetPressedTexture   (unsigned int const &textureID)
	{
		textures[SCROLLBAR_RIGHT_NAVIGATOR_PRESSED] = textureID;
		UpdateContent();
	}

	void ScrollBar::RightNavigator_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[SCROLLBAR_RIGHT_NAVIGATOR_UNPRESSED] = textureID;
		UpdateContent();
	}

//
//
	unsigned int ScrollBar::LeftNavigator_GetPressedTexture    ()
	{
		return textures[SCROLLBAR_LEFT_NAVIGATOR_PRESSED];
	}

	unsigned int ScrollBar::LeftNavigator_GetUnpressedTexture  ()
	{
		return textures[SCROLLBAR_LEFT_NAVIGATOR_UNPRESSED];
	}

//
	unsigned int ScrollBar::RightNavigator_GetPressedTexture   ()
	{
		return textures[SCROLLBAR_RIGHT_NAVIGATOR_PRESSED];
	}

	unsigned int ScrollBar::RightNavigator_GetUnpressedTexture ()
	{
		return textures[SCROLLBAR_RIGHT_NAVIGATOR_UNPRESSED];
	}
//
/*****************************************************/

	void ScrollBar::AdjustContentSize  ()
	{
		AdjustNavigators();

		SliceOffNavigatorsFromControlBorder();
		Slider::AdjustContentSize();
		GlueNavigatorsOntoControlBorder();
	}
//
	void ScrollBar::AdjustNavigators ()
	{
		if( IsVerticallyAligned() )
		{
			AdjustNavigatorsVertically();
		}
		else
		{
			AdjustNavigatorsHorizontally();
		}
	}
//
	void ScrollBar::AdjustNavigatorsVertically   ()
	{
		RECT A, B;

		A = B = controlBorder;

		/*B.bottom = B.top    - navigatorSize;
		A.top    = A.bottom + navigatorSize;*/

		//Left = Up; Right = Down;
		A.bottom = A.top    - navigatorSize;
		B.top    = B.bottom + navigatorSize;

		LeftNavigator->SetBorders(A);
		RightNavigator->SetBorders(B);
	}
	void ScrollBar::AdjustNavigatorsHorizontally ()
	{
		RECT A, B;

		A = B = controlBorder;

		A.right = A.left  + navigatorSize;
		B.left  = B.right - navigatorSize;

		LeftNavigator->SetBorders(A);
		RightNavigator->SetBorders(B);
	}
//
	void ScrollBar::SliceOffNavigatorsFromControlBorder ()
	{
		if( IsVerticallyAligned() )
		{
			controlBorder.bottom  += navigatorSize;
			controlBorder.top     -= navigatorSize;
		}
		else
		{
			controlBorder.left  += navigatorSize;
			controlBorder.right -= navigatorSize;
		}
	}

	void ScrollBar::GlueNavigatorsOntoControlBorder     ()
	{
		if( IsVerticallyAligned() )
		{
			controlBorder.bottom  -= navigatorSize;
			controlBorder.top     += navigatorSize;
		}
		else
		{
			controlBorder.left  -= navigatorSize;
			controlBorder.right += navigatorSize;
		}
	}
//
	void ScrollBar::AdjustContentColor ()
	{
		Slider::AdjustContentColor();


		if( LeftNavigator->GetCurrentTexture() == 0 )
		{
			LeftNavigator->SetColor(Color(1,1,1,0)*controlColor);
		}
		else
			LeftNavigator->SetColor(controlColor);

		if( RightNavigator->GetCurrentTexture() == 0 )
		{
			RightNavigator->SetColor(Color(1,1,1,0)*controlColor);
		}
		else
			RightNavigator->SetColor(controlColor);
	}
//
	void ScrollBar::UpdateContent ()
	{
		Slider::UpdateContent();

		SetMinWidth(navigatorSize*2);


		LeftNavigator->SetTextures(textures);

		LeftNavigator->SetPressTexture( SCROLLBAR_LEFT_NAVIGATOR_PRESSED + 1 );
		LeftNavigator->SetUnpressTexture( SCROLLBAR_LEFT_NAVIGATOR_UNPRESSED + 1 );
		LeftNavigator->SetCurrentTexture( SCROLLBAR_LEFT_NAVIGATOR_UNPRESSED + 1 );


		RightNavigator->SetTextures(textures);

		RightNavigator->SetPressTexture( SCROLLBAR_RIGHT_NAVIGATOR_PRESSED + 1 );
		RightNavigator->SetUnpressTexture( SCROLLBAR_RIGHT_NAVIGATOR_UNPRESSED + 1 );
		RightNavigator->SetCurrentTexture( SCROLLBAR_RIGHT_NAVIGATOR_UNPRESSED + 1 );
	}
//
/******************************************************
		Standard functions redefinition.
******************************************************/

	void ScrollBar::MouseLeftDown (POINT const &mousePosition)
	{
		UpperClass::MouseLeftDown(mousePosition);

		SliceOffNavigatorsFromControlBorder();


		if( DetectPressedNavigators() )
		{
			if( IsVerticallyAligned() )
			{
				AdjustThumbVertically();
			}
			else
				AdjustThumbHorizontally();
		}
		else
			Slider::MouseLeftDown(mousePosition);


		GlueNavigatorsOntoControlBorder();
		AdjustContentSize();
	}
//
	bool ScrollBar::DetectPressedNavigators ()
	{
		int navigatorsPressed = 0;

		navigatorsPressed += LeftNavigator_Pressed();
		navigatorsPressed += RightNavigator_Pressed();
		
		assert( navigatorsPressed <= 1 );

		if( navigatorsPressed )
		{
			return true;
		}
		else
			return false;
	}
//
		bool ScrollBar::LeftNavigator_Pressed  ()
		{
			if( LeftNavigator->IsPressed() && step > 0 )
			{
				step--;
				return true;
			}
			else
				return false;
		}

		bool ScrollBar::RightNavigator_Pressed ()
		{
			if( RightNavigator->IsPressed() && step < range-1 )
			{
				step++;
				return true;
			}
			else
				return false;
		}
//
/*****************************************************/

	void ScrollBar::Save (std::fstream &file)
	{
		Slider::Save(file);

		file << "\t" << "#----------------------------------------ScrollBar_components----------------------------------------#" << "\n\n";

		file << "\t" << "#Navigator_size:" << "\t" << navigatorSize << "\n\n";
	}

	void ScrollBar::Load (std::fstream &file)
	{
		Slider::Load(file);

		std::string line;

		line = ReadNewValue(file, '#');
		navigatorSize = atoi( line.c_str() );
	}
//
//


	#undef SLIDER_TEXTURE_COUNT

	#undef SCROLLBAR_LEFT_NAVIGATOR_PRESSED    
	#undef SCROLLBAR_LEFT_NAVIGATOR_UNPRESSED  

	#undef SCROLLBAR_RIGHT_NAVIGATOR_PRESSED   
	#undef SCROLLBAR_RIGHT_NAVIGATOR_UNPRESSED 

	#undef SCROLLBAR_TEXTURE_COUNT  