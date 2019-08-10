	#include "GlobalHeaders.h"

	#include "BattleCreator.h"

	#include "..//HANDLER_MACROS.h"

	#include "..//..//..//MiniDataManager//MiniDataManager.h"
	#include "..//..//..//BattleForceManager//BattleForceManager.h"


	#define BATTLE_CREATOR_WIDTH  (900)
	#define BATTLE_CREATOR_HEIGHT (700)

	

	/* External variables and functions from TerrainEditor.h */

	extern int MAP_X, MAP_Z;

	extern std::vector <std::string> GetMapList ();
	extern float* GetHeightMap ();
	extern void LoadMap (char *filename);





	std::string ComposeSquadName           (std::string const &miniName)
	{
		std::string squadName;

		squadName  = miniName;
		squadName += " squad";

		return squadName;
	}

	std::string ComposeMiniSelectionsEntry (Squad *squad)
	{
		std::string squadEntry;


		char memberCount[MAX_PATH];
		_itoa_s( squad->GetOriginalSquadSize(), memberCount, MAX_PATH, 10 );

		char pointsValue[MAX_PATH];
		_itoa_s( squad->GetPointsValue(), pointsValue, MAX_PATH, 10 );

		squadEntry  = memberCount;
		squadEntry += " x ";
		squadEntry += squad->GetCurrentMini()->GetMiniName();
		squadEntry += " - ";
		squadEntry += pointsValue;
		squadEntry += " points";


		return squadEntry;
	}

	std::string ComposeTotalPointsLabel    (int const &pointsValue)
	{
		std::string totalPointsLabel;


		char pointsCountAsChar[MAX_PATH];
		_itoa_s(pointsValue, pointsCountAsChar, MAX_PATH, 10);

		totalPointsLabel  = "Total points: ";
		totalPointsLabel += pointsCountAsChar;


		return totalPointsLabel;
	}
//
	void RefreshMiniSelections ()
	{
		ComboBox *battleForceList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_BATTLE_FORCE_LIST) );
		ListBox  *miniSelections  = reinterpret_cast <ListBox*>  ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_MINI_SELECTIONS) );
		Label    *totalPoints     = reinterpret_cast <Label*>    ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_TOTAL_POINTS) );


		miniSelections->RemoveAllElements();


		std::string selectedBattleForceName = battleForceList->GetSelection();

		if( !selectedBattleForceName.empty() )
		{
			BattleForce *selectedBattleForce = BattleForceManager::Singleton().GetBattleForce(selectedBattleForceName);
			BattleForceManager::Singleton().SetCurrentBattleForce( selectedBattleForce );				

			int battleForceCount = selectedBattleForce->GetSquadCount();

			for(int i = battleForceCount-1; i >= 0; i--)
			{
				miniSelections->AddElement( ComposeMiniSelectionsEntry(selectedBattleForce->GetSquad(i)) );
			}

			totalPoints->SetText( ComposeTotalPointsLabel(selectedBattleForce->GetPointsValue()) );
			totalPoints->AdjustWidthToCaption();
		}
	}
//
//




	Handler* BattleCreator::CreateBattleCreator ()
	{
		Handler *battleCreator = new Handler;

		BattleCreator_SetBorder(battleCreator);
		BattleCreator_SetLabel(battleCreator);
		BattleCreator_SetStartButton(battleCreator);
		battleCreator->SetName(BATTLE_CREATOR);


		battleCreator->AddHandling( CreateSection_MapList() );
		battleCreator->AddHandling( CreateSection_BattleForceCreator() );
		battleCreator->AddHandling( CreateSection_BattleForceList() );
		battleCreator->AddHandling( CreateSection_MiniAdder() );
		battleCreator->AddHandling( CreateSection_MiniSelections() );


		Control_SetDefaultSettings(battleCreator);

		return battleCreator;
	}
//
		inline std::string GetSelectedMapName ()
		{
			ComboBox *mapList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_MAP_LIST) );
			return mapList->GetSelection();
		}
//
		inline void HideBattleCreatorWindow ()
		{
			Handler *battleCreator = reinterpret_cast <Handler*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR) );
			
			battleCreator->SetRendering(false);
			battleCreator->SetReadingInput(false);
		}

		inline void PrepareDeploymentWindow   ()
		{
			GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetReadingInput(true);
			GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetRendering(true);


			ComboBox *deploymentBattleForceList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW_BATTLE_FORCE_LIST) );
			BattleForceManager &bfManager = BattleForceManager::Singleton();


			int battleForceCount = bfManager.GetBattleForceCount();

			for(int i = 0; i < battleForceCount; i++)
			{
				deploymentBattleForceList->AddElement( bfManager.GetBattleForce(i)->GetBattleForceName() );
			}
		}
//
		void StartDeployment_MouseLeftDown (Control *ctrl, POINT const &mousePosition)
		{
			Button *startDeployment = reinterpret_cast <Button*> (ctrl);
			
			try
			{
				int result = MessageBox(NULL, "Are you ready to begin deployment phase?", "Confirmation...", MB_YESNO);

				if( result == IDYES )
				{
					BattleForceManager::Singleton().StartDeploymentPhase();

					LoadMap( const_cast <char*> ( GetSelectedMapName().c_str() ) );
					BattleForceManager::Singleton().SetHeightMap(GetHeightMap(), MAP_X, MAP_Z);
					BattleForceManager::Singleton().StartDeploymentPhase();
					
					HideBattleCreatorWindow();
					PrepareDeploymentWindow();
				}
				else
					startDeployment->Unpress();
			}
			catch(std::string error)
			{
				startDeployment->Unpress();
				MessageBox(NULL, error.c_str(), "Start deployment - error.", MB_OK);
			}
		}
//
		void BattleCreator::BattleCreator_SetBorder      (Handler *battleCreator)
		{
			RECT border;

			border.left = 0;
			border.top  = BATTLE_CREATOR_HEIGHT*2;

			border.right  = BATTLE_CREATOR_WIDTH*2;
			border.bottom = 0;

			battleCreator->SetBorders(border);
		}

		void BattleCreator::BattleCreator_SetLabel       (Handler *battleCreator)
		{
			Label *label = new Label;

			Control_SetDefaultSettings(label);

			RECT border;

			border.left = border.right = BATTLE_CREATOR_WIDTH/3;
			border.top    = BATTLE_CREATOR_HEIGHT;
			border.bottom = BATTLE_CREATOR_HEIGHT - 30;


			label->SetBorders(border);

			Font font;
			font.weight = FW_BOLD;
			font.Underline = true;
			label->SetFont(font);

			label->SetText("Battle Creator");
			label->AdjustWidthToCaption();

			battleCreator->AddHandling(label);
		}

		void BattleCreator::BattleCreator_SetStartButton (Handler *battleCreator)
		{
			Button *startButton = new Button;

			Control_SetDefaultSettings(startButton);
			Button_SetBorder(625, 75, startButton);

			std::vector <unsigned int> textures;
			textures.push_back( GUI::Singleton().Data.GetTexture("accept") );
			textures.push_back( GUI::Singleton().Data.GetTexture("accept_pressed") );

			startButton->SetTextures(textures);
			startButton->SetPressTexture(2);
			startButton->SetUnpressTexture(1);
			startButton->SetCurrentTexture(1);

			startButton->EventManager.Ptr_MouseLeftDown = StartDeployment_MouseLeftDown;
			startButton->SetName(BATTLE_CREATOR_START_DEPLOYMENT);

			battleCreator->AddHandling(startButton);
		}
//
		Handler* BattleCreator::CreateSection_MapList ()
		{
			Handler *mapSection = new Handler;

			MapList_SetBorder(mapSection);
			MapList_SetLabel(mapSection);
			MapList_SetMapList(mapSection);

			ShiftSection(50, -50, mapSection);

			return mapSection;
		}
//
			void BattleCreator::MapList_SetBorder  (Handler *mapSection)
			{
				RECT border;

				border.left = 0;
				border.right  = 900;

				border.top  = 700;
				border.bottom = 600;

				mapSection->SetBorders(border);
			}

			void BattleCreator::MapList_SetLabel   (Handler *mapSection)
			{
				Label *label = new Label;


				Control_SetDefaultSettings(label);
				Label_SetDefaultSettings(label);
				Label_SetBorder(0, BATTLE_CREATOR_HEIGHT, label);

				label->SetText("Map list:");
				label->AdjustWidthToCaption();


				mapSection->AddHandling(label);
			}

			void BattleCreator::MapList_SetMapList (Handler *mapSection)
			{
				ComboBox *mapList = new ComboBox;

				Control_SetDefaultSettings(mapList);
				ComboBox_SetDefaultSettings(mapList);
				ComboBox_SetBorder(120, BATTLE_CREATOR_HEIGHT, mapList);

				mapList->SetName(BATTLE_CREATOR_MAP_LIST);


				std::vector <std::string> mapVector = GetMapList();

				if( mapVector.empty() )
				{
					throw std::string( "There are no maps in map folder!" );
				}

				for(int i = mapVector.size()-1; i >= 0 ; i--)
				{
					mapList->AddElement( mapVector[i] );
				}

				mapList->SetSelection(0);


				mapSection->AddHandling(mapList);
			}
//
		Handler* BattleCreator::CreateSection_BattleForceCreator ()
		{
			Handler *battleForceCreator = new Handler;

			BattleForceCreator_SetBorder(battleForceCreator);
			BattleForceCreator_SetLabel(battleForceCreator);
			BattleForceCreator_SetEdit(battleForceCreator);
			BattleForceCreator_SetAddButton(battleForceCreator);

			ShiftSection(50, -125, battleForceCreator);

			return battleForceCreator;
		}
//
			void AddBattleForce_MouseLeftDown (Control *ctrl, POINT const &mousePosition)
			{
				Button *addBattleForce   = reinterpret_cast <Button*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_ADD_BATTLE_FORCE) );
				Edit *newBattleForceEdit = reinterpret_cast <Edit*>   ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_NEW_BATTLE_FORCE) );

				try
				{
					ComboBox *battleForceList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_BATTLE_FORCE_LIST) );

					if( battleForceList->GetElementCount() < 2 )
					{
						std::string newBattleForce = newBattleForceEdit->GetText();

						if( newBattleForce.empty() )
						{
							addBattleForce->Unpress();
							MessageBox(NULL, "Before adding new BattleForce, write it's name maby?", "Add BattleForce operation - error.", MB_OK);
							return;
						}

						BattleForceManager::Singleton().AddBattleForce(newBattleForce);

						
						battleForceList->AddElement(newBattleForce);

						RefreshMiniSelections();
					}
					else
						MessageBox(NULL, "There can be max two opposing sides!", "Error.", MB_OK);
				}
				catch(std::string error)
				{
					addBattleForce->Unpress();
					MessageBox(NULL, error.c_str(), "Add BattleForce operation - error.", MB_OK);
				}
			}
//
			void BattleCreator::BattleForceCreator_SetBorder    (Handler *battleForceCreator)
			{
				RECT border;

				border.left = 0;
				border.right  = 900;

				border.top  = 700;
				border.bottom = 600;

				battleForceCreator->SetBorders(border);
			}

			void BattleCreator::BattleForceCreator_SetLabel     (Handler *battleForceCreator)
			{
				Label *label = new Label;


				Control_SetDefaultSettings(label);
				Label_SetDefaultSettings(label);
				Label_SetBorder(0, BATTLE_CREATOR_HEIGHT, label);

				label->SetText("New Battle Force:");
				label->AdjustWidthToCaption();


				battleForceCreator->AddHandling(label);
			}

			void BattleCreator::BattleForceCreator_SetEdit      (Handler *battleForceCreator)
			{
				Edit *newBattleForce = new Edit;

				Control_SetDefaultSettings(newBattleForce);
				Edit_SetDefaultSettings(newBattleForce);
				Edit_SetBorder(145, BATTLE_CREATOR_HEIGHT, newBattleForce);

				newBattleForce->EventManager.Ptr_KeyDown = Edit_KeyManager;
				newBattleForce->SetName(BATTLE_CREATOR_NEW_BATTLE_FORCE);

				battleForceCreator->AddHandling(newBattleForce);
			}

			void BattleCreator::BattleForceCreator_SetAddButton (Handler *battleForceCreator)
			{
				Button *addBattleForce = new Button;

				Control_SetDefaultSettings(addBattleForce);
				Button_SetBorder(525, BATTLE_CREATOR_HEIGHT, addBattleForce);

				std::vector <unsigned int> textures;
				textures.push_back( GUI::Singleton().Data.GetTexture("plus") );
				textures.push_back( GUI::Singleton().Data.GetTexture("plus_pressed") );

				addBattleForce->SetTextures(textures);
				addBattleForce->SetPressTexture(2);
				addBattleForce->SetUnpressTexture(1);
				addBattleForce->SetCurrentTexture(1);

				addBattleForce->EventManager.Ptr_MouseLeftDown = AddBattleForce_MouseLeftDown;
				addBattleForce->SetName(BATTLE_CREATOR_ADD_BATTLE_FORCE);


				battleForceCreator->AddHandling(addBattleForce);
			}
//
		Handler* BattleCreator::CreateSection_BattleForceList ()
		{
			Handler *battleForceList = new Handler;

			BattleForceList_SetBorder(battleForceList);
			BattleForceList_SetLabel(battleForceList);
			BattleForceList_SetComboBox(battleForceList);
			BattleForceList_SetRemoveButton(battleForceList);

			ShiftSection(50, -225, battleForceList);

			return battleForceList;
		}
//
			void BattleForceList_MouseLeftDown   (Control *ctrl, POINT const &mousePosition)
			{
				try
				{
					RefreshMiniSelections();
				}
				catch(std::string error)
				{
					MessageBox(NULL, error.c_str(), "Mini selections refreshing operation - error.", MB_OK);
				}
			}

			void RemoveBattleForce_MouseLeftDown (Control *ctrl, POINT const &mousePosition)
			{
				Button *removeBattleForce   = reinterpret_cast <Button*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_REMOVE_BATTLE_FORCE) );

				try
				{
					ComboBox *battleForceList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_BATTLE_FORCE_LIST) );
					
					if( battleForceList->GetElementCount() )
					{
						BattleForceManager::Singleton().RemoveBattleForce( battleForceList->GetSelection() );
						battleForceList->RemoveElement( battleForceList->GetSelection() );

						RefreshMiniSelections();
					}
					else
					{
						removeBattleForce->Unpress();
						MessageBox(NULL, "There is nothing to remove!", "Remove BattleForce operation - error.", MB_OK);
					}
				}
				catch(std::string error)
				{
					removeBattleForce->Unpress();
					MessageBox(NULL, error.c_str(), "Remove BattleForce operation - error.", MB_OK);
				}
			}
//
			void BattleCreator::BattleForceList_SetBorder       (Handler *battleForceList)
			{
				RECT border;

				border.left = 0;
				border.right  = 900;

				border.top  = 700;
				border.bottom = 600;

				battleForceList->SetBorders(border);
			}

			void BattleCreator::BattleForceList_SetLabel        (Handler *battleForceList)
			{
				Label *label = new Label;


				Control_SetDefaultSettings(label);
				Label_SetDefaultSettings(label);
				Label_SetBorder(0, BATTLE_CREATOR_HEIGHT, label);

				label->SetText("Battle Forces:");
				label->AdjustWidthToCaption();


				battleForceList->AddHandling(label);
			}

			void BattleCreator::BattleForceList_SetComboBox     (Handler *battleForceList)
			{
				ComboBox *battleForceListCombo = new ComboBox;

				Control_SetDefaultSettings(battleForceListCombo);
				ComboBox_SetDefaultSettings(battleForceListCombo);
				ComboBox_SetBorder(120, BATTLE_CREATOR_HEIGHT, battleForceListCombo);

				battleForceListCombo->EventManager.Ptr_MouseLeftDown = BattleForceList_MouseLeftDown;
				battleForceListCombo->SetName(BATTLE_CREATOR_BATTLE_FORCE_LIST);

				battleForceList->AddHandling(battleForceListCombo);
			}

			void BattleCreator::BattleForceList_SetRemoveButton (Handler *battleForceList)
			{
				Button *removeBattleForce = new Button;

				Control_SetDefaultSettings(removeBattleForce);
				Button_SetBorder(600, BATTLE_CREATOR_HEIGHT, removeBattleForce);

				std::vector <unsigned int> textures;
				textures.push_back( GUI::Singleton().Data.GetTexture("bin") );
				textures.push_back( GUI::Singleton().Data.GetTexture("bin_pressed") );

				removeBattleForce->SetTextures(textures);
				removeBattleForce->SetPressTexture(2);
				removeBattleForce->SetUnpressTexture(1);
				removeBattleForce->SetCurrentTexture(1);

				removeBattleForce->EventManager.Ptr_MouseLeftDown = RemoveBattleForce_MouseLeftDown;
				removeBattleForce->SetName(BATTLE_CREATOR_REMOVE_BATTLE_FORCE);


				battleForceList->AddHandling(removeBattleForce);
			}

//
		Handler* BattleCreator::CreateSection_MiniAdder ()
		{
			Handler *miniAdder = new Handler;
	
			MiniAdder_SetBorder(miniAdder);
			MiniAdder_SetLabel(miniAdder);
			MiniAdder_SetComboBox(miniAdder);
			MiniAdder_SetComboBox2(miniAdder);
			MiniAdder_SetAddButton(miniAdder);

			ShiftSection(50, -325, miniAdder);

			return miniAdder;
		}
//
			void AddSquad_MouseLeftDown  (Control *ctrl, POINT const &mousePosition)
			{
				Button   *addSquad        = reinterpret_cast <Button*>   ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_ADD_SQUAD) );
				ComboBox *battleForceList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_BATTLE_FORCE_LIST) );

				ComboBox *miniList  = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_MINI_LIST) );
				ComboBox *miniCount = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_MINI_COUNT) );

				ListBox *miniSelections = reinterpret_cast <ListBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_MINI_SELECTIONS) );
				Label  *totalPoints     = reinterpret_cast <Label*>   ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_TOTAL_POINTS) );

				try
				{
					BattleForce *currentBattleForce = BattleForceManager::Singleton().GetCurrentBattleForce();

					if( currentBattleForce )
					{
						std::string miniName    = miniList->GetSelection();
						int         memberCount = atoi( miniCount->GetSelection().c_str() );
						Mini       *mini        = MiniDataManager::Singleton().GetMini( miniName );

						Squad *newSquad = new Squad(ComposeSquadName(miniName), mini, memberCount);

						currentBattleForce->InsertNewSquad(newSquad);
						miniSelections->AddElement( ComposeMiniSelectionsEntry(newSquad) );
						totalPoints->SetText( ComposeTotalPointsLabel(currentBattleForce->GetPointsValue()) );
						totalPoints->AdjustWidthToCaption();
					}
					else
					{
						addSquad->Unpress();
						MessageBox(NULL, "First create a Battle Force!", "Add Squad operation - error.", MB_OK);
					}
				}
				catch(std::string error)
				{
					addSquad->Unpress();
					MessageBox(NULL, error.c_str(), "Add Squad operation - error.", MB_OK);
				}
			}
//
			void BattleCreator::MiniAdder_SetBorder    (Handler *miniAdder)
			{
				RECT border;

				border.left = 0;
				border.right  = 900;

				border.top  = 700;
				border.bottom = 600;

				miniAdder->SetBorders(border);
			}

			void BattleCreator::MiniAdder_SetLabel     (Handler *miniAdder)
			{
				Label *label = new Label;


				Control_SetDefaultSettings(label);
				Label_SetDefaultSettings(label);
				Label_SetBorder(0, BATTLE_CREATOR_HEIGHT, label);

				label->SetText("Mini list:");
				label->AdjustWidthToCaption();


				miniAdder->AddHandling(label);
			}

			void BattleCreator::MiniAdder_SetComboBox  (Handler *miniAdder)
			{
				ComboBox *miniList = new ComboBox;

				Control_SetDefaultSettings(miniList);
				ComboBox_SetDefaultSettings(miniList);
				ComboBox_SetBorder(120, BATTLE_CREATOR_HEIGHT, miniList);

				RECT border = miniList->GetBorders();
				border.left = 120;
				border.right = 440;
				miniList->SetBorders(border);


				MiniDataManager &miniData = MiniDataManager::Singleton();
				int miniCount = miniData.GetMiniCount();

				for(int i = 0; i < miniCount; i++)
				{
					miniList->AddElement( miniData.GetMini(i)->GetMiniName() );
				}


				miniList->SetName(BATTLE_CREATOR_MINI_LIST);

				miniAdder->AddHandling(miniList);
			}

			void BattleCreator::MiniAdder_SetComboBox2 (Handler *miniAdder)
			{
				ComboBox *miniCount = new ComboBox;

				Control_SetDefaultSettings(miniCount);
				ComboBox_SetDefaultSettings(miniCount);
				ComboBox_SetBorder(120, BATTLE_CREATOR_HEIGHT, miniCount);
				
				RECT border = miniCount->GetBorders();
				border.left = 460;
				border.right = 580;
				miniCount->SetBorders(border);


				for(int i = 30; i >= 1; i--)
				{
					char numberAsChar[MAX_PATH];
					_itoa_s(i, numberAsChar, MAX_PATH, 10);

					miniCount->AddElement(numberAsChar);
				}

				miniCount->SetSelection(0);


				miniCount->SetName(BATTLE_CREATOR_MINI_COUNT);

				miniAdder->AddHandling(miniCount);
			}

			void BattleCreator::MiniAdder_SetAddButton (Handler *miniAdder)
			{
				Button *addSquad = new Button;

				Control_SetDefaultSettings(addSquad);
				Button_SetBorder(600, BATTLE_CREATOR_HEIGHT, addSquad);

				std::vector <unsigned int> textures;
				textures.push_back( GUI::Singleton().Data.GetTexture("plus") );
				textures.push_back( GUI::Singleton().Data.GetTexture("plus_pressed") );

				addSquad->SetTextures(textures);
				addSquad->SetPressTexture(2);
				addSquad->SetUnpressTexture(1);
				addSquad->SetCurrentTexture(1);

				addSquad->EventManager.Ptr_MouseLeftDown = AddSquad_MouseLeftDown;
				addSquad->SetName(BATTLE_CREATOR_ADD_SQUAD);


				miniAdder->AddHandling(addSquad);
			}
//
		Handler* BattleCreator::CreateSection_MiniSelections ()
		{
			Handler *miniSelections = new Handler;

			MiniSelections_SetBorder(miniSelections);
			MiniSelections_SetLabel(miniSelections);
			MiniSelections_SetListBox(miniSelections);
			MiniSelections_SetRemoveButton(miniSelections);

			ShiftSection(50, -375, miniSelections);

			return miniSelections;
		}
//
			void RemoveSquad_MouseLeftDown (Control *ctrl, POINT const &mousePosition)
			{
				Button   *removeSquad     = reinterpret_cast <Button*>   ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_REMOVE_SQUAD) );
				ComboBox *battleForceList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_BATTLE_FORCE_LIST) );

				ListBox *miniSelections = reinterpret_cast <ListBox*> ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_MINI_SELECTIONS) );
				Label   *totalPoints    = reinterpret_cast <Label*>   ( GUI::Singleton().Controls.GetControl(BATTLE_CREATOR_TOTAL_POINTS) );

				try
				{
					BattleForce *currentBattleForce = BattleForceManager::Singleton().GetCurrentBattleForce();

					if( currentBattleForce )
					{
						std::vector <int> indices = miniSelections->GetSelectedIndices();

						if( indices.size() )
						{
							for(int i = indices.size()-1; i >= 0; i--)
							{
								BattleForceManager::Singleton().GetCurrentBattleForce()->RemoveSquad( indices[i] );
								miniSelections->RemoveElement(indices[i]);
							}

							RefreshMiniSelections();
						}
						else
						{
							removeSquad->Unpress();
							MessageBox(NULL, "There is no squad to remove!", "Remove Squad operation - error.", MB_OK);
						}
					}
					else
					{
						removeSquad->Unpress();
						MessageBox(NULL, "First create a Battle Force!", "Remove Squad operation - error.", MB_OK);
					}
				}
				catch(std::string error)
				{
					removeSquad->Unpress();
					MessageBox(NULL, error.c_str(), "Remove Squad operation - error.", MB_OK);
				}
			}
//
			void BattleCreator::MiniSelections_SetBorder       (Handler *miniSelections)
			{
				RECT border;

				border.left = 0;
				border.right  = BATTLE_CREATOR_WIDTH;

				border.top  = BATTLE_CREATOR_HEIGHT;
				border.bottom = 300;

				miniSelections->SetBorders(border);
			}

			void BattleCreator::MiniSelections_SetLabel        (Handler *miniSelections)
			{
				Label *label = new Label;


				Control_SetDefaultSettings(label);
				Label_SetDefaultSettings(label);
				Label_SetBorder(0, BATTLE_CREATOR_HEIGHT, label);
				label->SetName(BATTLE_CREATOR_TOTAL_POINTS);

				label->SetText("Total points: 0");
				label->AdjustWidthToCaption();

				
				miniSelections->AddHandling(label);
			}

			void BattleCreator::MiniSelections_SetListBox      (Handler *miniSelections)
			{
				ListBox *selectedMinis = new ListBox;

				Control_SetDefaultSettings(selectedMinis);
				ListBox_SetDefaultSettings(selectedMinis);
				ListBox_SetBorder(-20, BATTLE_CREATOR_HEIGHT-30, selectedMinis);

				selectedMinis->SetName(BATTLE_CREATOR_MINI_SELECTIONS);

				miniSelections->AddHandling(selectedMinis);
			}

			void BattleCreator::MiniSelections_SetRemoveButton (Handler *miniSelections)
			{
				Button *removeSquad = new Button;

				Control_SetDefaultSettings(removeSquad);
				Button_SetBorder(600, BATTLE_CREATOR_HEIGHT-30, removeSquad);

				std::vector <unsigned int> textures;
				textures.push_back( GUI::Singleton().Data.GetTexture("bin") );
				textures.push_back( GUI::Singleton().Data.GetTexture("bin_pressed") );

				removeSquad->SetTextures(textures);
				removeSquad->SetPressTexture(2);
				removeSquad->SetUnpressTexture(1);
				removeSquad->SetCurrentTexture(1);


				removeSquad->EventManager.Ptr_MouseLeftDown = RemoveSquad_MouseLeftDown;
				removeSquad->SetName(BATTLE_CREATOR_REMOVE_SQUAD);

				miniSelections->AddHandling(removeSquad);
			}
//
//



	#undef BATTLE_CREATOR_WIDTH
	#undef BATTLE_CREATOR_HEIGHT