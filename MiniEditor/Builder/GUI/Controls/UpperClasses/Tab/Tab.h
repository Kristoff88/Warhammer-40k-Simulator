#pragma once

	#include "..//..//UpperClass//UpperClass.h"


	#define TAB_PRESSED					  (1)
	#define TAB_UNPRESSED				  (2)

	#define TAB_LEFT_NAVIGATOR_PRESSED    (3)
	#define TAB_LEFT_NAVIGATOR_UNPRESSED  (4)

	#define TAB_RIGHT_NAVIGATOR_PRESSED   (5)
	#define TAB_RIGHT_NAVIGATOR_UNPRESSED (6)

	#define TAB_TEXTURE_COUNT			  (6)



	class TabBar: public UpperClass
	{
		public:

			TabBar();
				private:
				void SetDefaultTabData();

				inline void SetDefaultNavigatorsData ();
				inline void SetDefaultTabsData       ();
			public:

			TabBar(TabBar const &tab);
				private:
				void CopyTabData(TabBar const &tab);

				inline void CopyNavigatorsData (TabBar const &tab);
				inline void CopyTabsData       (TabBar const &tab);
				inline void RefreshContentArrayPointers ();
			public:

			~TabBar();

			TabBar& operator= (TabBar const &tab);


	/*****************************************************/
		
			void SetFont          (Font const &font);
			Font GetFont          ();

			void  SetCaptionColor (Color const &color);
			Color GetCaptionColor ();

	/******************************************************
			Tab managment functions.
	******************************************************/

			void AddTab    (std::string const &Caption, unsigned int const &index) throw (std::string);
				private:
				inline bool TabCaptionCollision (std::string const &Caption);
				inline void PrepareNewTab       (Button *newTab, std::string const &Caption);
			public:

			void RemoveTab (std::string const &Caption);
			void RemoveTab (unsigned int const &index);

			void RemoveAllTabs ();
				private:
				inline void RemoveTabFromContentVector (Control *removedTab);
				inline void DeleteTabAndRemoveFromTabVector (int const &tabIndex);	  
			public:


			void Tab_SetPressedTexture   (unsigned int const &textureID);
			void Tab_SetUnpressedTexture (unsigned int const &textureID);

			unsigned int Tab_GetPressedTexture   ();
			unsigned int Tab_GetUnpressedTexture ();


			int GetCurrentTab ();
			int GetTabCount   ();

	/******************************************************
			Navigator managment functions.
	******************************************************/

			void SetNavigatorSize (int const &size);
			int  GetNavigatorSize ();


			void LeftNavigator_SetPressedTexture    (unsigned int const &textureID);
			void LeftNavigator_SetUnpressedTexture  (unsigned int const &textureID);

			void RightNavigator_SetPressedTexture   (unsigned int const &textureID);
			void RightNavigator_SetUnpressedTexture (unsigned int const &textureID);


			unsigned int LeftNavigator_GetPressedTexture    ();
			unsigned int LeftNavigator_GetUnpressedTexture  ();

			unsigned int RightNavigator_GetPressedTexture   ();
			unsigned int RightNavigator_GetUnpressedTexture ();


		private:

			Font tabsCaption_Font;
			Color tabsCaption_Color;

			std::vector <Button*> tabs;
			int currentTabIndex;
			int tabsClipped;

		
			Button *LeftNavigator, 
				   *RightNavigator;

			int navigatorSize,
				navigatorShift;

			bool RenderNavigators;


	/*****************************************************/

			void AdjustContentSize ();

				inline void AdjustNavigators ();

				inline void AdjustTabs       ();

					inline void ResetTabsRendering ();

					inline void HideShiftedOverTabs ();

					inline int  ClipSuperflousTabs ();
					inline int  AdjustTabToCaptionWidth (Button *Tab, int const &previoslyAddedTabsWidth);

					inline void AdjustTabsToLeftoverSpace (int const &leftoverSpace);

	/*****************************************************/

			void AdjustContentColor ();

				inline void AdjustNavigatorsColor ();
				inline void AdjustTabsColor       ();

	/*****************************************************/
			
			void UpdateContent ();

				inline void UpdateTabs ();
				inline void UpdateTabs_Font     ();
				inline void UpdateTabs_Textures ();

				inline void UpdateNavigators ();
				inline void UpdateNavigators_BorderRestrictions ();
				inline void UpdateNavigators_Textures           ();

	/******************************************************
			Standard functions redefinition.
	******************************************************/

			void MouseLeftDown       (POINT const &mousePosition);
			void MouseMiddleDown     (POINT const &mousePosition);
			void MouseRightDown      (POINT const &mousePosition);

			void MouseLeftDblClick   (POINT const &mousePosition);
			void MouseMiddleDblClick (POINT const &mousePosition);
			void MouseRightDblClick  (POINT const &mousePosition);

			void PopAllTabs      ();
			void FindNewTabIndex ();

			void DetectPressedNavigators ();

				inline bool LeftNavigator_Pressed ();
				inline bool RightNavigator_Pressed ();

	/*****************************************************/

			void Save (std::fstream &file);

				inline std::vector <std::string> CreateTabRegistryAndRemoveTabsFromContent ();
				inline void					     RestoreTabsToContent (std::vector <std::string> const &tabRegistry);

				inline void SaveBasicData (std::fstream &file);
				inline void SaveTabs      (std::fstream &file, std::vector <std::string> const &tabRegistry);
				
			void Load (std::fstream &file);

				inline void LoadBasicData (std::fstream &file);
				inline void LoadTabs      (std::fstream &file);
	};		  