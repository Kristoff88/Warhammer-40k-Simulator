	#include "GlobalHeaders.h"

	#include "DeploymentWindow.h"

	#include "..//HANDLER_MACROS.h"

	#include "..//..//..//BattleForceManager//BattleForceManager.h"


	#define DEPLOYMENT_WINDOW_WIDTH  (650)
	#define DEPLOYMENT_WINDOW_TOP    (650)
	#define DEPLOYMENT_WINDOW_BOTTOM (150)





	std::string ComposeMiniSelectionsEntry2 (Squad *squad)
	{
		std::string squadEntry;


		char memberCount[MAX_PATH];
		_itoa_s( squad->GetOriginalSquadSize(), memberCount, MAX_PATH, 10 );

		char pointsValue[MAX_PATH];
		_itoa_s( squad->GetPointsValue(), pointsValue, MAX_PATH, 10 );

		squadEntry  = memberCount;
		squadEntry += " x ";
		squadEntry += squad->GetMiniName();
		squadEntry += " - ";
		squadEntry += pointsValue;
		squadEntry += " points";


		return squadEntry;
	}

	std::string ComposeTotalPointsLabel2    (int const &pointsValue)
	{
		std::string totalPointsLabel;


		char pointsCountAsChar[MAX_PATH];
		_itoa_s(pointsValue, pointsCountAsChar, MAX_PATH, 10);

		totalPointsLabel  = "Total points: ";
		totalPointsLabel += pointsCountAsChar;


		return totalPointsLabel;
	}
//
	void RefreshBattleForceList ()
	{
		ComboBox *battleForceListCombo = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW_BATTLE_FORCE_LIST) );


		BattleForceManager &bfManager = BattleForceManager::Singleton();

		int battleForceCount = bfManager.GetBattleForceCount();

		for(int i = 0; i < battleForceCount; i++)
		{
			battleForceListCombo->AddElement( bfManager.GetBattleForce(i)->GetBattleForceName() );
		}

		battleForceListCombo->SetSelection(0);
	}

	void RefreshMiniSelections2 ()
	{
		ComboBox *battleForceList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW_BATTLE_FORCE_LIST) );
		ListBox  *miniSelections  = reinterpret_cast <ListBox*>  ( GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW_MINI_SELECTIONS) );
		Label    *totalPoints     = reinterpret_cast <Label*>    ( GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW_TOTAL_POINTS) );


		miniSelections->RemoveAllElements();


		std::string selectedBattleForceName = battleForceList->GetSelection();

		if( !selectedBattleForceName.empty() )
		{
			BattleForce *selectedBattleForce = BattleForceManager::Singleton().GetBattleForce(selectedBattleForceName);
			BattleForceManager::Singleton().SetCurrentBattleForce( selectedBattleForce );


			int squadCount = selectedBattleForce->GetSquadCount();

			for(int i = squadCount-1; i >= 0; i--)
			{
				miniSelections->AddElement( ComposeMiniSelectionsEntry2(selectedBattleForce->GetSquad(i)) );
			}

			miniSelections->SetTextColor( Color(0,0,0,1) );

			totalPoints->SetText( ComposeTotalPointsLabel2(selectedBattleForce->GetPointsValue()) );
			totalPoints->AdjustWidthToCaption();
		}
	}
//
//




	Handler* DeploymentWindow::CreateDeploymentWindow ()
	{
		Handler *deploymentWindow = new Handler;

		DeploymentWindow_SetBorder(deploymentWindow);
		DeploymentWindow_SetLabel(deploymentWindow);
		DeploymentWindow_SetStartButton(deploymentWindow);
		deploymentWindow->SetName(DEPLOYMENT_WINDOW);


		deploymentWindow->AddHandling( CreateSection_BattleForceList() );
		deploymentWindow->AddHandling( CreateSection_MiniSelections() );


		Control_SetDefaultSettings(deploymentWindow);
		deploymentWindow->SetReadingInput(false);
		deploymentWindow->SetRendering(false);

		ShiftSection(50, 0, deploymentWindow);

		return deploymentWindow;
	}
//
		void StartSimulation_MouseLeftDown (Control *ctrl, POINT const &mousePosition)
		{
			Button *startButton = reinterpret_cast <Button*> ( GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW_START_SIMULATION) );

			try
			{
				MessageBox(NULL, "pytanie!", "QUESTION!", MB_OK);

				int result = MessageBox(NULL, "Are you sure, that you've got all squads set?", "Confirmation...", MB_YESNO);

				std::string info = "Kod wyniku: ";
				info += NumberToString(result);

				MessageBox(NULL, info.c_str(), "Result", MB_OK);

				if( result == IDYES )
				{
					MessageBox(NULL, "1", "step", MB_OK);

					BattleForceManager::Singleton().StartSimulationPhase();

					MessageBox(NULL, "2", "step", MB_OK);

					Handler *simulationGUI = reinterpret_cast <Handler*> ( GUI::Singleton().Controls.GetControl(SIMULATION_GUI) );
			

					MessageBox(NULL, "3", "step", MB_OK);

					for(int i = 0; i < simulationGUI->GetHandlingsCount(); i++)
					{
						simulationGUI->GetHandling(i)->SetReadingInput(true);
						simulationGUI->GetHandling(i)->SetRendering(true);
					}

					MessageBox(NULL, "4", "step", MB_OK);


					GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetRendering(false);
					GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetReadingInput(false);
					startButton->Unpress();

		
					MessageBox(NULL, "5", "step", MB_OK);

					ComboBox *battleForceList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW_BATTLE_FORCE_LIST) );
					
					MessageBox(NULL, "6", "step", MB_OK);


					int lastElementIndex = battleForceList->GetElementCount()-1;

					if( lastElementIndex >= 0 )
					{
						battleForceList->SetSelection(lastElementIndex);
						RefreshMiniSelections2();
					}

					MessageBox(NULL, "7", "step", MB_OK);
				}
				else
				{
					MessageBox(NULL, "sad face", "error", MB_OK);
					startButton->Unpress();
				}
			}
			catch(std::string error)
			{
				startButton->Unpress();
				MessageBox(NULL, error.c_str(), "Start simulation - error.", MB_OK);
			}
		}
//
		void DeploymentWindow::DeploymentWindow_SetBorder      (Handler *deploymentWindow)
		{
			RECT border;

			border.left = 0;
			border.top  = DEPLOYMENT_WINDOW_TOP;

			border.right  = DEPLOYMENT_WINDOW_WIDTH;
			border.bottom = DEPLOYMENT_WINDOW_BOTTOM;

			deploymentWindow->SetBorders(border);
		}

		void DeploymentWindow::DeploymentWindow_SetLabel       (Handler *deploymentWindow)
		{
			Label *label = new Label;

			Control_SetDefaultSettings(label);

			RECT border;

			border.left = border.right = DEPLOYMENT_WINDOW_WIDTH/3;
			border.top    = DEPLOYMENT_WINDOW_TOP;
			border.bottom = DEPLOYMENT_WINDOW_TOP - 30;


			label->SetBorders(border);

			Font font;
			font.weight = FW_BOLD;
			font.Underline = true;
			label->SetFont(font);

			label->SetText("Deployment Window");
			label->AdjustWidthToCaption();

			deploymentWindow->AddHandling(label);
		}

		void DeploymentWindow::DeploymentWindow_SetStartButton (Handler *deploymentWindow)
		{
			Button *startButton = new Button;

			Control_SetDefaultSettings(startButton);
			Button_SetBorder(575, 210, startButton);

			std::vector <unsigned int> textures;
			textures.push_back( GUI::Singleton().Data.GetTexture("accept") );
			textures.push_back( GUI::Singleton().Data.GetTexture("accept_pressed") );

			startButton->SetTextures(textures);
			startButton->SetPressTexture(2);
			startButton->SetUnpressTexture(1);
			startButton->SetCurrentTexture(1);

			startButton->EventManager.Ptr_MouseLeftDown = StartSimulation_MouseLeftDown;
			startButton->SetName(DEPLOYMENT_WINDOW_START_SIMULATION);

			deploymentWindow->AddHandling(startButton);
		}
//
		Handler* DeploymentWindow::CreateSection_BattleForceList ()
		{
			Handler *battleForceList = new Handler;

			BattleForceList_SetBorder(battleForceList);
			BattleForceList_SetLabel(battleForceList);
			BattleForceList_SetComboBox(battleForceList);

			ShiftSection(50, -50, battleForceList);

			return battleForceList;
		}
//
			void BattleForceList_MouseLeftDown2 (Control *ctrl, POINT const &mousePosition)
			{
				try
				{
					RefreshMiniSelections2();
				}
				catch(std::string error)
				{
					MessageBox(NULL, error.c_str(), "Mini selections refreshing operation - error.", MB_OK);
				}
			}

//
			void DeploymentWindow::BattleForceList_SetBorder       (Handler *battleForceList)
			{
				RECT border;

				border.left = 0;
				border.right  = DEPLOYMENT_WINDOW_WIDTH;

				border.top  = DEPLOYMENT_WINDOW_TOP;
				border.bottom = DEPLOYMENT_WINDOW_TOP-100;

				battleForceList->SetBorders(border);
			}

			void DeploymentWindow::BattleForceList_SetLabel        (Handler *battleForceList)
			{
				Label *label = new Label;


				Control_SetDefaultSettings(label);
				Label_SetDefaultSettings(label);
				Label_SetBorder(0, DEPLOYMENT_WINDOW_TOP, label);

				label->SetText("Battle Forces:");
				label->AdjustWidthToCaption();


				battleForceList->AddHandling(label);
			}

			void DeploymentWindow::BattleForceList_SetComboBox     (Handler *battleForceList)
			{
				ComboBox *battleForceListCombo = new ComboBox;

				Control_SetDefaultSettings(battleForceListCombo);
				ComboBox_SetDefaultSettings(battleForceListCombo);
				ComboBox_SetBorder(0, DEPLOYMENT_WINDOW_TOP-40, battleForceListCombo);

				battleForceListCombo->EventManager.Ptr_MouseLeftDown = BattleForceList_MouseLeftDown2;
				battleForceListCombo->SetName(DEPLOYMENT_WINDOW_BATTLE_FORCE_LIST);

				battleForceList->AddHandling(battleForceListCombo);
			}
//
		Handler* DeploymentWindow::CreateSection_MiniSelections ()
		{
			Handler *miniSelections = new Handler;

			MiniSelections_SetBorder(miniSelections);
			MiniSelections_SetLabel(miniSelections);
			MiniSelections_SetListBox(miniSelections);
			MiniSelections_SetDeployButton(miniSelections);

			ShiftSection(0, -150, miniSelections);

			return miniSelections;
		}
//
			void DeploySquad_MouseLeftDown (Control *ctrl, POINT const &mousePosition)
			{
				Button  *deploySquad    = reinterpret_cast <Button*>  ( GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW_DEPLOY_SQUAD) );
				ListBox *miniSelections = reinterpret_cast <ListBox*> ( GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW_MINI_SELECTIONS) );

				try
				{
					std::vector <int> miniSelectionsIndices = miniSelections->GetSelectedIndices();
					assert( miniSelectionsIndices.size() == 0 || miniSelectionsIndices.size() == 1 );


					if( !miniSelectionsIndices.empty() )
					{
						BattleForce *currentBattleForce = BattleForceManager::Singleton().GetCurrentBattleForce();

						currentBattleForce->SetCurrentSquad( miniSelectionsIndices[0] );

						Squad *currentSelection = currentBattleForce->GetCurrentSquad();
						currentSelection->SetFirstMiniAsCurrent();

						BattleForceManager::Singleton().SetCurrentSelection(currentSelection);
						currentSelection->GetCurrentMini()->SetDrawingSelection(false);
						

						if( currentBattleForce->GetCurrentSquad()->IsDeployed() )
						{
							int result = MessageBox(NULL, "Selected squad is already deployed! Redeploy?", "Warning.", MB_YESNO);

							if( result == IDNO )
							{
								deploySquad->Unpress();
								return;
							}
							else
								currentSelection->Undeploy();
						}


						
						GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetRendering(false);
						GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetReadingInput(false);

						deploySquad->Unpress();
					}
					else
					{
						deploySquad->Unpress();
						MessageBox(NULL, "First select some squad!", "Deploy Squad operation - error.", MB_OK);
					}
				}
				catch(std::string error)
				{
					deploySquad->Unpress();
					MessageBox(NULL, error.c_str(), "Deploy Squad operation - error.", MB_OK);
				}
			}
//
			void DeploymentWindow::MiniSelections_SetBorder       (Handler *miniSelections)
			{
				RECT border;

				border.left = 0;
				border.right  = DEPLOYMENT_WINDOW_WIDTH;

				border.top  = DEPLOYMENT_WINDOW_TOP;
				border.bottom = DEPLOYMENT_WINDOW_TOP-400;

				miniSelections->SetBorders(border);
			}

			void DeploymentWindow::MiniSelections_SetLabel        (Handler *miniSelections)
			{
				Label *label = new Label;


				Control_SetDefaultSettings(label);
				Label_SetDefaultSettings(label);
				Label_SetBorder(0, DEPLOYMENT_WINDOW_TOP, label);
				label->SetName(DEPLOYMENT_WINDOW_TOTAL_POINTS);

				label->SetText("Total points: 0");
				label->AdjustWidthToCaption();
	

				miniSelections->AddHandling(label);
			}

			void DeploymentWindow::MiniSelections_SetListBox      (Handler *miniSelections)
			{
				ListBox *selectedMinis = new ListBox;

				Control_SetDefaultSettings(selectedMinis);
				ListBox_SetDefaultSettings(selectedMinis);
				selectedMinis->SetMultiSelection(false);
				ListBox_SetBorder(-20, DEPLOYMENT_WINDOW_TOP-30, selectedMinis);
				
				selectedMinis->SetName(DEPLOYMENT_WINDOW_MINI_SELECTIONS);

				miniSelections->AddHandling(selectedMinis);
			}

			void DeploymentWindow::MiniSelections_SetDeployButton (Handler *miniSelections)
			{
				Button *deploySquad = new Button;

				Control_SetDefaultSettings(deploySquad);
				Button_SetBorder(575, DEPLOYMENT_WINDOW_TOP-30, deploySquad);

				std::vector <unsigned int> textures;
				textures.push_back( GUI::Singleton().Data.GetTexture("plus") );
				textures.push_back( GUI::Singleton().Data.GetTexture("plus_pressed") );

				deploySquad->SetTextures(textures);
				deploySquad->SetPressTexture(2);
				deploySquad->SetUnpressTexture(1);
				deploySquad->SetCurrentTexture(1);


				deploySquad->EventManager.Ptr_MouseLeftDown = DeploySquad_MouseLeftDown;
				deploySquad->SetName(DEPLOYMENT_WINDOW_DEPLOY_SQUAD);

				miniSelections->AddHandling(deploySquad);
			}
//
//





	#undef DEPLOYMENT_WINDOW_WIDTH 
	#undef DEPLOYMENT_WINDOW_TOP   
	#undef DEPLOYMENT_WINDOW_BOTTOM 