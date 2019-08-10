	#include "GlobalHeaders.h"

	#include "Tab.h"



	TabBar::TabBar()
	{
		controlType = "TabBar";
		SetDefaultTabData();
		AdjustContentSize();
		AdjustContentColor();
	}
//
		void TabBar::SetDefaultTabData()
		{
			LeftNavigator = new Button;
			RightNavigator = new Button;

			contentArray.push_back(LeftNavigator);
			contentArray.push_back(RightNavigator);


			// blank texture slots
			for(int i = 0; i < TAB_TEXTURE_COUNT; i++)
			{
				textures.push_back(0);
			}

			SetDefaultNavigatorsData();
			SetDefaultTabsData();
		}
//
		void TabBar::SetDefaultNavigatorsData ()
		{
			LeftNavigator->SetTransparency(true);
			RightNavigator->SetTransparency(true);

			SetNavigatorSize(25);
			navigatorShift = 0;

			RenderNavigators = false;
		}

		void TabBar::SetDefaultTabsData       ()
		{
			tabsCaption_Color = Color(0,0,0,1);

			currentTabIndex = -1;
			tabsClipped = 0;
		}
//
	TabBar::TabBar(TabBar const &tab)
	{
		UpperClass::operator=(tab);
		CopyTabData(tab);
	}
//
		void TabBar::CopyTabData(TabBar const &tab)
		{
			CopyNavigatorsData(tab);
			CopyTabsData(tab);

			RefreshContentArrayPointers();
			UpdateContent();
		}
//
		void TabBar::CopyNavigatorsData (TabBar const &tab)
		{
			*LeftNavigator = *tab.LeftNavigator;
			*RightNavigator = *tab.RightNavigator;

			navigatorSize = tab.navigatorSize;
			navigatorShift = tab.navigatorShift;

			RenderNavigators = tab.RenderNavigators;
		}

		void TabBar::CopyTabsData       (TabBar const &tab)
		{
			tabsCaption_Font = tab.tabsCaption_Font;
			tabsCaption_Color = tab.tabsCaption_Color;

			RemoveAllTabs();
			for(int i = 0; i < tab.tabs.size(); i++)
			{
				AddTab(tab.tabs[i]->GetText(), i);
			}

			currentTabIndex = tab.currentTabIndex;
			tabsClipped = tab.tabsClipped;
		}
		void TabBar::RefreshContentArrayPointers ()
		{
			contentArray.clear();

			contentArray.push_back(LeftNavigator);
			contentArray.push_back(RightNavigator);

			for(int i = 0; i < tabs.size(); i++)
			{
				contentArray.push_back(tabs[i]);
			}
		}
//
	TabBar::~TabBar()
	{}
//
	TabBar& TabBar::operator= (TabBar const &tab)
	{
		UpperClass::operator=(tab);
		CopyTabData(tab);
		return *this;
	}
//
/*****************************************************/

	void TabBar::SetFont          (Font const &font)
	{
		tabsCaption_Font = font;
		UpdateContent();
	}
	Font TabBar::GetFont          ()
	{
		return tabsCaption_Font;
	}
//
	void  TabBar::SetCaptionColor (Color const &color)
	{
		tabsCaption_Color = color;
		UpdateContent();
	}
	Color TabBar::GetCaptionColor ()
	{
		return tabsCaption_Color;
	}
//
/******************************************************
		Tab managment functions.
******************************************************/

	void TabBar::AddTab    (std::string const &Caption, unsigned int const &index) throw (std::string)
	{
		if( !TabCaptionCollision(Caption) )
		{
			Button *newTab = new Button;

			PrepareNewTab(newTab, Caption);
			tabs.insert(tabs.begin()+index, newTab);

			contentArray.push_back(newTab);

			UpdateContent();
			AdjustContentSize();
			AdjustContentColor();
		}
		else
		{
			std::string error;

			error  = "There is already a tab with caption \"";
			error += Caption;
			error += "\"\n\nOperation: AddTab(...)";

			throw error;
		}
	}
//
		bool TabBar::TabCaptionCollision (std::string const &Caption)
		{
			bool CaptionCollision = false;

			for(int i = 0; i < tabs.size(); i++)
			{
				if( Caption == tabs[i]->GetText() )
				{
					CaptionCollision = true;
					break;
				}
			}

			return CaptionCollision;
		}
		void TabBar::PrepareNewTab       (Button *newTab, std::string const &Caption)
		{
			newTab->SetText(Caption);
			newTab->FilterOutEndOfLineMarks();
			newTab->SetCenterAlignment();

			newTab->SetSwitchMode(true);
		}
//
	void TabBar::RemoveTab (std::string const &Caption)
	{
		for(int i = 0; i < tabs.size(); i++)
		{
			if( tabs[i]->GetText() == Caption )
			{
				RemoveTabFromContentVector(tabs[i]);
				DeleteTabAndRemoveFromTabVector(i);
		
				AdjustContentSize();
			}
		}
	}

	void TabBar::RemoveTab (unsigned int const &index)
	{
		if( index < tabs.size() )
		{
			RemoveTabFromContentVector(tabs[index]);
			DeleteTabAndRemoveFromTabVector(index);

			AdjustContentSize();
		}
	}
//
	void TabBar::RemoveAllTabs ()
	{
		for(int i = tabs.size()-1; i >= 0; i--)
		{
			RemoveTabFromContentVector(tabs[i]);
			DeleteTabAndRemoveFromTabVector(i);
		}

		AdjustContentSize();
	}
//
		void TabBar::RemoveTabFromContentVector (Control *removedTab)
		{
			bool TabFoundInContentArray = false;

			for(int i = 0; i < contentArray.size(); i++)
			{
				if( contentArray[i] == removedTab )
				{
					contentArray.erase( contentArray.begin()+i );

					TabFoundInContentArray = true;
					break;
				}
			}

			assert( TabFoundInContentArray );
		}
		void TabBar::DeleteTabAndRemoveFromTabVector (int const &tabIndex)
		{
			delete tabs[tabIndex];
			tabs.erase( tabs.begin()+tabIndex );
		}
//
//
	void TabBar::Tab_SetPressedTexture   (unsigned int const &textureID)
	{
		textures[TAB_PRESSED] = textureID;
		UpdateContent();
	}

	void TabBar::Tab_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[TAB_UNPRESSED] = textureID;
		UpdateContent();
	}
//
	unsigned int TabBar::Tab_GetPressedTexture   ()
	{
		return textures[TAB_PRESSED];
	}

	unsigned int TabBar::Tab_GetUnpressedTexture ()
	{
		return textures[TAB_UNPRESSED];
	}
//
//
	int TabBar::GetCurrentTab ()
	{
		return currentTabIndex;
	}
	int TabBar::GetTabCount   ()
	{
		return tabs.size();
	}
//
/******************************************************
		Navigator managment functions.
******************************************************/

	void TabBar::SetNavigatorSize (int const &size)
	{
		navigatorSize = size;
		UpdateContent();
		AdjustContentSize();
	}
	int  TabBar::GetNavigatorSize ()
	{
		return navigatorSize;
	}
//
//
	void TabBar::LeftNavigator_SetPressedTexture    (unsigned int const &textureID)
	{
		textures[TAB_LEFT_NAVIGATOR_PRESSED] = textureID;
		UpdateContent();
	}

	void TabBar::LeftNavigator_SetUnpressedTexture  (unsigned int const &textureID)
	{
		textures[TAB_LEFT_NAVIGATOR_UNPRESSED] = textureID;
		UpdateContent();
	}

//
	void TabBar::RightNavigator_SetPressedTexture   (unsigned int const &textureID)
	{
		textures[TAB_RIGHT_NAVIGATOR_PRESSED] = textureID;
		UpdateContent();
	}

	void TabBar::RightNavigator_SetUnpressedTexture (unsigned int const &textureID)
	{
		textures[TAB_RIGHT_NAVIGATOR_UNPRESSED] = textureID;
		UpdateContent();
	}

//
//
	unsigned int TabBar::LeftNavigator_GetPressedTexture    ()
	{
		return textures[TAB_LEFT_NAVIGATOR_PRESSED];
	}

	unsigned int TabBar::LeftNavigator_GetUnpressedTexture  ()
	{
		return textures[TAB_LEFT_NAVIGATOR_UNPRESSED];
	}

//
	unsigned int TabBar::RightNavigator_GetPressedTexture   ()
	{
		return textures[TAB_RIGHT_NAVIGATOR_PRESSED];
	}

	unsigned int TabBar::RightNavigator_GetUnpressedTexture ()
	{
		return textures[TAB_RIGHT_NAVIGATOR_UNPRESSED];
	}

//
/*****************************************************/

	void TabBar::AdjustContentSize  ()
	{
		AdjustNavigators();
		AdjustTabs();
	}
//
		void TabBar::AdjustNavigators ()
		{
			RECT leftNavigator,
				 rightNavigator;

			leftNavigator = rightNavigator = controlBorder;

			leftNavigator.left  = controlBorder.left;
			leftNavigator.right = leftNavigator.left + navigatorSize;

			rightNavigator.right = controlBorder.right;
			rightNavigator.left  = rightNavigator.right - navigatorSize;


			LeftNavigator->SetBorders( leftNavigator );
			RightNavigator->SetBorders( rightNavigator );
		}
		void TabBar::AdjustTabs       ()
		{
			if( tabs.size() )
			{
				ResetTabsRendering();
				HideShiftedOverTabs();

				int leftoverSpace = ClipSuperflousTabs();
				
				if( leftoverSpace > 0 )
				{
					AdjustTabsToLeftoverSpace(leftoverSpace);
				}
			}
		}
//
			void TabBar::ResetTabsRendering ()
			{
				tabsClipped = 0;

				for(int i = 0; i < tabs.size(); i++)
				{
					tabs[i]->SetRendering(true);
				}
			}

//
			void TabBar::HideShiftedOverTabs ()
			{
				for(int i = 0; i < navigatorShift; i++)
				{
					tabs[i]->SetRendering(false);
					tabsClipped++;
				}
			}
//
			int  TabBar::ClipSuperflousTabs ()
			{
				int tabBarWidth = controlBorder.right-controlBorder.left - navigatorSize,
					tabsWidth = navigatorSize;

				bool ClipTab = false;

				for(int i = navigatorShift; i < tabs.size(); i++)
				{
					if( !ClipTab )
					{
						int tabWidthAfterAdjusting = AdjustTabToCaptionWidth(tabs[i], tabsWidth);

						tabsWidth += tabWidthAfterAdjusting;

						if( tabsWidth > tabBarWidth )
						{
							ClipTab = true;
							tabsWidth -= tabWidthAfterAdjusting;
						}
					}

					if( ClipTab )
					{
						tabs[i]->SetRendering(false);
						tabsClipped++;
					}
				}

				int leftoverSpace = tabBarWidth - tabsWidth;

				return leftoverSpace;
			}

			int  TabBar::AdjustTabToCaptionWidth(Button *Tab, int const &previoslyAddedTabsWidth)
			{
				RECT tabBorder;

				tabBorder.top    = controlBorder.top;
				tabBorder.bottom = controlBorder.bottom;

				tabBorder.left = tabBorder.right = controlBorder.left + previoslyAddedTabsWidth;


				int wantedWidth = Tab->GetText().size()*tabsCaption_Font.width  + 2*frameThickness;
				int tabWidth  = tabBorder.right - tabBorder.left;


				while( tabWidth < wantedWidth )
				{
					tabBorder.right += tabsCaption_Font.width;
					tabWidth = tabBorder.right - tabBorder.left;
				}

				Tab->SetBorders(tabBorder);

				return tabWidth;
			}
//
			void TabBar::AdjustTabsToLeftoverSpace (int const &leftoverSpace)
			{
				int tabWidthIncrementation = leftoverSpace/tabs.size(),
					previousTab_RightBorder = controlBorder.left + navigatorSize;


				for(int i = navigatorShift; i < tabs.size(); i++)
				{
					RECT tabBorder = tabs[i]->GetBorders();

					int tabWidth = tabBorder.right - tabBorder.left;

					tabBorder.left = previousTab_RightBorder;
					tabBorder.right = tabBorder.left + tabWidth + tabWidthIncrementation;

					tabs[i]->SetBorders(tabBorder);

					previousTab_RightBorder = tabBorder.right;
				}
			}
//
/*****************************************************/

	void TabBar::AdjustContentColor ()
	{
		AdjustNavigatorsColor();
		AdjustTabsColor();
	}
//
		void TabBar::AdjustNavigatorsColor ()
		{
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

		void TabBar::AdjustTabsColor       ()
		{
			for(int i = 0; i < tabs.size(); i++)
			{
				if( tabs[i]->GetCurrentTexture() == 0 )
				{
					tabs[i]->SetColor(Color(1,1,1,0)*controlColor);
				}
				else
					tabs[i]->SetColor(controlColor);
			}
		}
//
/*****************************************************/

	void TabBar::UpdateContent ()
	{
		UpperClass::UpdateContent();


		// Standard functions redefinitions, that also update contentArray.
		SetMinHeight(navigatorSize);
		SetMinWidth(navigatorSize*2);

		SetMaxHeight(maxControlHeight);
		SetMaxWidth(maxControlWidth);

		SetColor(controlColor);
		SetTransparency(Transparent);
		/**/

		UpdateTabs();
		UpdateNavigators();
	}
//
		void TabBar::UpdateTabs       ()
		{
			UpdateTabs_Font();
			UpdateTabs_Textures();
		}
//
		void TabBar::UpdateTabs_Font     ()
		{
			for(int i = 0; i < tabs.size(); i++)
			{
				try
				{
					tabs[i]->SetMinHeight(tabsCaption_Font.height + 2*frameThickness);

					tabs[i]->SetFont(tabsCaption_Font, GetDC(NULL));
					tabs[i]->SetTextColor(tabsCaption_Color);
				}
				catch(std::string error)
				{
					bool UpdateTabs_SetMinHeight_Error = false;
					assert( UpdateTabs_SetMinHeight_Error );
				}
			}
		}

		void TabBar::UpdateTabs_Textures ()
		{
			for(int i = 0; i < tabs.size(); i++)
			{
				tabs[i]->SetTextures(textures);

				tabs[i]->SetPressTexture(TAB_PRESSED+1);
				tabs[i]->SetUnpressTexture(TAB_UNPRESSED+1);
				tabs[i]->SetCurrentTexture(TAB_UNPRESSED+1);
			}
		}
//
		void TabBar::UpdateNavigators ()
		{
			UpdateNavigators_BorderRestrictions();
			UpdateNavigators_Textures();
		}
//
		void TabBar::UpdateNavigators_BorderRestrictions ()
		{
			try
			{
				LeftNavigator->SetMinHeight(0);
				LeftNavigator->SetMaxHeight(maxControlHeight);
				LeftNavigator->SetMinHeight(minControlHeight);
				
				LeftNavigator->SetMinWidth(0);
				LeftNavigator->SetMaxWidth(navigatorSize);
				LeftNavigator->SetMinWidth(navigatorSize);
				

				RightNavigator->SetMinHeight(0);
				RightNavigator->SetMaxHeight(maxControlHeight);
				RightNavigator->SetMinHeight(minControlHeight);

				RightNavigator->SetMinWidth(0);
				RightNavigator->SetMaxWidth(navigatorSize);
				RightNavigator->SetMinWidth(navigatorSize);
			}
			catch(std::string error)
			{
				bool UpdateNavigators_Error = false;
				assert( UpdateNavigators_Error );
			}
		}

		void TabBar::UpdateNavigators_Textures           ()
		{
			LeftNavigator->SetTextures(textures);

			LeftNavigator->SetPressTexture(TAB_LEFT_NAVIGATOR_PRESSED+1);
			LeftNavigator->SetUnpressTexture(TAB_LEFT_NAVIGATOR_UNPRESSED+1);
			LeftNavigator->SetCurrentTexture(TAB_LEFT_NAVIGATOR_UNPRESSED+1);

			RightNavigator->SetTextures(textures);

			RightNavigator->SetPressTexture(TAB_RIGHT_NAVIGATOR_PRESSED+1);
			RightNavigator->SetUnpressTexture(TAB_RIGHT_NAVIGATOR_UNPRESSED+1);
			RightNavigator->SetCurrentTexture(TAB_RIGHT_NAVIGATOR_UNPRESSED+1);
		}
//
/******************************************************
		Standard functions redefinition.
******************************************************/

	void TabBar::MouseLeftDown       (POINT const &mousePosition)
	{
		PopAllTabs();

		UpperClass::MouseLeftDown(mousePosition);

		FindNewTabIndex();
		DetectPressedNavigators();
	}

	void TabBar::MouseMiddleDown     (POINT const &mousePosition)
	{
		PopAllTabs();

		UpperClass::MouseMiddleDown(mousePosition);

		FindNewTabIndex();
		DetectPressedNavigators();
	}

	void TabBar::MouseRightDown      (POINT const &mousePosition)
	{
		PopAllTabs();

		UpperClass::MouseRightDown(mousePosition);

		FindNewTabIndex();
		DetectPressedNavigators();
	}
//
	void TabBar::MouseLeftDblClick   (POINT const &mousePosition)
	{
		PopAllTabs();

		UpperClass::MouseLeftDblClick(mousePosition);

		FindNewTabIndex();
		DetectPressedNavigators();
	}

	void TabBar::MouseMiddleDblClick (POINT const &mousePosition)
	{
		PopAllTabs();

		UpperClass::MouseMiddleDblClick(mousePosition);

		FindNewTabIndex();
		DetectPressedNavigators();
	}

	void TabBar::MouseRightDblClick  (POINT const &mousePosition)
	{
		PopAllTabs();

		UpperClass::MouseRightDblClick(mousePosition);

		FindNewTabIndex();
		DetectPressedNavigators();
	}
//
//
	void TabBar::PopAllTabs ()
	{
		for(int i = 0; i < tabs.size(); i++)
		{
			if( tabs[i]->IsPressed() )
			{
				assert( i == currentTabIndex );
				tabs[i]->Press();
			}
		}

		currentTabIndex = -1;
	}

	void TabBar::FindNewTabIndex ()
	{
		int pressedTabCounter = 0;

		for(int i = 0; i < tabs.size(); i++)
		{
			if( tabs[i]->IsPressed() )
			{
				currentTabIndex = i;
				pressedTabCounter++;
			}
		}

		assert( pressedTabCounter <= 1 );
	}
//
	void TabBar::DetectPressedNavigators ()
	{
		int navigatorsPressed = 0;

		navigatorsPressed += LeftNavigator_Pressed();
		navigatorsPressed += RightNavigator_Pressed();
		
		assert( navigatorsPressed <= 1 );

		if( navigatorsPressed )
		{
			AdjustContentSize();
		}
	}
//
		bool TabBar::LeftNavigator_Pressed  ()
		{
			if( LeftNavigator->IsPressed() && navigatorShift > 0 )
			{
				navigatorShift--;
				return true;
			}
			else
				return false;
		}

		bool TabBar::RightNavigator_Pressed ()
		{
			if( RightNavigator->IsPressed() && navigatorShift < tabsClipped )
			{
				navigatorShift++;
				return true;
			}
			else
				return false;
		}
//
/*****************************************************/

	void TabBar::Save (std::fstream &file)
	{
		std::vector <std::string> tabRegistry = CreateTabRegistryAndRemoveTabsFromContent();


		UpperClass::Save(file);

		SaveBasicData(file);
		SaveTabs(file, tabRegistry);


		// Tabs were previosly removed from content, because
		// we don't want the whole tab controls to be saved, if we need
		// only to save their captions.
		RestoreTabsToContent(tabRegistry);
	}
//
		std::vector <std::string> TabBar::CreateTabRegistryAndRemoveTabsFromContent ()
		{
			std::vector <std::string> tabRegistry;

			for(int i = 0; i < tabs.size(); i++)
			{
				tabRegistry.push_back(tabs[i]->GetText());
			}

			RemoveAllTabs();

			return tabRegistry;
		}
		void					  TabBar::RestoreTabsToContent (std::vector <std::string> const &tabRegistry)
		{
			for(int i = 0; i < tabRegistry.size(); i++)
			{
				try
				{
					AddTab(tabRegistry[i], i);
				}
				catch( std::string error )
				{
					int RestoreTabsError = 0;
					assert( RestoreTabsError );
				}
			}
		}
//
		void TabBar::SaveBasicData (std::fstream &file)
		{
			file << "\t" << "#CaptionColor:"   
				 << "\t" << tabsCaption_Color.GetRed()
				 << "\t" << tabsCaption_Color.GetGreen()
				 << "\t" << tabsCaption_Color.GetBlue()
				 << "\t" << tabsCaption_Color.GetAlpha()
				 << "\n\n";


			if( tabsCaption_Font.GetFontID() )
			{
				file << "\t" << "#CurrentFont:" << "\n" 

					 << "\t" << "#Height:"    << "\t" << tabsCaption_Font.height    << "\n"
					 << "\t" << "#Width:"     << "\t" << tabsCaption_Font.width     << "\n"
					 << "\t" << "#Weight:"    << "\t" << tabsCaption_Font.weight    << "\n"
					 << "\t" << "#Italic:"    << "\t" << tabsCaption_Font.Italic    << "\n"
					 << "\t" << "#Underline:" << "\t" << tabsCaption_Font.Underline << "\n"
					 << "\t" << "#Strikeout:" << "\t" << tabsCaption_Font.Strikeout << "\n"
					 << "\t" << "#FontName:"  << "\t" << tabsCaption_Font.fontName  << "\n\n";
			}
			else
				file << "\t" << "#CurrentFont:" << "\t" << "None" << "\n\n";
				
		}

		void TabBar::SaveTabs      (std::fstream &file, std::vector <std::string> const &tabRegistry)
		{
			file << "\t" << "#TabsClipped:"     << "\t" << tabsClipped     << "\n"
				 << "\t" << "#CurrentTabIndex:" << "\t" << currentTabIndex << "\n\n"

				 << "\t" << "#TabCount:" << "\t" << tabRegistry.size() << "\n\n";


			file << "\t" << "#TabsArray:" << "\n";

			for(int i = 0; i < tabRegistry.size(); i++)
			{
				file << "\t" << tabRegistry[i] << "\n";
			}
		}
//
	void TabBar::Load (std::fstream &file)
	{
		UpperClass::Load(file);

		LoadBasicData(file);
		LoadTabs(file);

		UpdateContent();
	}
//
		void TabBar::LoadBasicData (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		tabsCaption_Color.SetRed   ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		tabsCaption_Color.SetGreen ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		tabsCaption_Color.SetBlue  ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		tabsCaption_Color.SetAlpha ( atof(line.c_str()) );

			line = ReadNewValue(file, '#');

			if( line != "None" )
			{
				tabsCaption_Font.height = atoi( line.c_str() );

				line = ReadNewValue(file, '#');		tabsCaption_Font.width     = atoi( line.c_str() );
				line = ReadNewValue(file, '#');		tabsCaption_Font.weight    = atoi( line.c_str() );
				line = ReadNewValue(file, '#');		tabsCaption_Font.Italic    = atoi( line.c_str() );
				line = ReadNewValue(file, '#');		tabsCaption_Font.Underline = atoi( line.c_str() );
				line = ReadNewValue(file, '#');		tabsCaption_Font.Strikeout = atoi( line.c_str() );

				line = ReadNewValue(file, '#');		tabsCaption_Font.fontName = const_cast<char*>(line.c_str());
			}
		}

		void TabBar::LoadTabs      (std::fstream &file)
		{
			std::string line;
			int tabCount;

			line = ReadNewValue(file, '#');		
			tabsClipped = atoi( line.c_str() );
			assert( tabsClipped >= 0 );

			line = ReadNewValue(file, '#');		
			currentTabIndex = atoi( line.c_str() );
			assert( currentTabIndex >= -1 );

			line = ReadNewValue(file, '#');		
			tabCount = atoi( line.c_str() );
			assert( tabCount >= 0 );

			assert( currentTabIndex < tabCount && tabsClipped <= tabCount );


			for(int i = 0; i < tabCount; i++)
			{
				line = ReadNewValue(file, '#');
				AddTab(line, i);
			}
		}
//


/*****************************************************/

	#undef TAB_PRESSED		      
	#undef TAB_UNPRESSED		  

	#undef TAB_LEFT_NAVIGATOR_PRESSED   
	#undef TAB_LEFT_NAVIGATOR_UNPRESSED

	#undef TAB_RIGHT_NAVIGATOR_PRESSED   
	#undef TAB_RIGHT_NAVIGATOR_UNPRESSED 

	#undef TAB_TEXTURE_COUNT