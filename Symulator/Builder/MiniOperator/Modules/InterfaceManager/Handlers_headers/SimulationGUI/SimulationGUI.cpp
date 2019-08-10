	#include "GlobalHeaders.h"

	#include "SimulationGUI.h"

	#include "..//HANDLER_MACROS.h"
	#include "..//..//..//BattleForceManager//BattleForceManager.h"





	#define INFO_BOX_WIDTH  (200)
	#define INFO_BOX_HEIGHT (350)

	#define BATTLE_LOG_WIDTH  (900)
	#define BATTLE_LOG_HEIGHT (175)




	Handler* SimulationGUI::CreateSimulationGUI ()
	{
		Handler *simulationGUI = new Handler;

		RECT border;
		border.left = border.bottom = 0;
		border.right = border.top = 2000;

		Control_SetDefaultSettings(simulationGUI);

		simulationGUI->SetMinWidth(0);
		simulationGUI->SetMaxWidth(2000);
		simulationGUI->SetMinHeight(0);
		simulationGUI->SetMaxHeight(2000);
		simulationGUI->SetBorders(border);
		simulationGUI->SetColor( Color(1,1,1,0) );

		simulationGUI->SetName(SIMULATION_GUI);


		simulationGUI->AddHandling( CreateInfoBox() );
		simulationGUI->AddHandling( CreateTargetInfoBox() );
		simulationGUI->AddHandling( CreateBattleLog() );
		//simulationGUI->AddHandling( CreateOptionBox() );


		simulationGUI->SetTransparency(true);
		simulationGUI->SetObstacle(true);
		simulationGUI->SetReadingInput(false);
		simulationGUI->SetRendering(false);
		

		return simulationGUI;
	}
//
		inline std::string ComposeBattleForceInfo (BattleForce *currentBattleForce)
		{
			std::string battleForceInfo("");

			battleForceInfo  = "  ";
			battleForceInfo += "BattleForce: ";
			battleForceInfo += currentBattleForce->GetBattleForceName();
			battleForceInfo += "\n\n";

			Squad *currentSquad = currentBattleForce->GetCurrentSquad();

			if( currentSquad )
			{
				battleForceInfo += currentSquad->ComposeSquadInfo();
			}

			return battleForceInfo;
		}
//
		Label* SimulationGUI::CreateInfoBox   ()
		{
			Label *infoBox = new Label;

			RECT guiBorder = GUI::Singleton().Data.GetBorders();

			RECT border;
			border.left   = guiBorder.right - INFO_BOX_WIDTH;
			border.right  = guiBorder.right;
			border.bottom = guiBorder.top - INFO_BOX_HEIGHT;
			border.top    = guiBorder.top;

			infoBox->SetBorders(border);
			infoBox->SetMinWidth(100);
			infoBox->SetMaxWidth(2000);
			infoBox->SetMinHeight(100);
			infoBox->SetMaxHeight(2000);

			infoBox->SetTextColor( Color(0,1,0,0.75) );

			std::vector <unsigned int> textures;
			textures.push_back( GUI::Singleton().Data.GetTexture("InfoBox.bmp") );

			infoBox->SetTextures(textures);
			infoBox->SetCurrentTexture(1);

			Label_SetDefaultSettings(infoBox);
			infoBox->SetMaxCharacterCount(9999999);

			infoBox->SetName(SIMULATION_GUI_INFO_BOX);

			return infoBox;
		}

		Label* SimulationGUI::CreateTargetInfoBox ()
		{
			Label *targetInfoBox = new Label;


			RECT guiBorder = GUI::Singleton().Data.GetBorders();

			RECT border;
			border.left   = 0;
			border.right  = INFO_BOX_WIDTH;
			border.bottom = guiBorder.top - INFO_BOX_HEIGHT;
			border.top    = guiBorder.top;

			targetInfoBox->SetBorders(border);
			targetInfoBox->SetMinWidth(100);
			targetInfoBox->SetMaxWidth(2000);
			targetInfoBox->SetMinHeight(100);
			targetInfoBox->SetMaxHeight(2000);

			targetInfoBox->SetTextColor( Color(0,1,0,0.75) );

			std::vector <unsigned int> textures;
			textures.push_back( GUI::Singleton().Data.GetTexture("TargetInfoBox.bmp") );

			targetInfoBox->SetTextures(textures);
			targetInfoBox->SetCurrentTexture(1);

			Label_SetDefaultSettings(targetInfoBox);
			targetInfoBox->SetMaxCharacterCount(9999999);

			targetInfoBox->SetName(SIMULATION_GUI_TARGET_INFO_BOX);


			return targetInfoBox;
		}
//
//
		void BattleLog_KeyDown (Control *ctrl, int const &keyCode)
		{
			Edit *battleLog = reinterpret_cast <Edit*> (ctrl);
			

			switch( keyCode )
			{
				case '<':
				{
					BattleForceManager::Singleton().SetPreviousLog();

					battleLog->SetText( BattleForceManager::Singleton().GetCurrentLog() );
				}
				break;

				case '>':
				{
					BattleForceManager::Singleton().SetNextLog();

					battleLog->SetText( BattleForceManager::Singleton().GetCurrentLog() );
				}
				break;
			}
		}
//
		Edit*  SimulationGUI::CreateBattleLog ()
		{
			Edit *battleLog = new Edit;


			Font font;

			font.height = 20;
			font.width  = 10;
			
			font.weight = FW_BOLD;

			font.Underline = false;
			font.Italic    = false;
			font.Strikeout = false;

			font.fontName = "Courier";

			battleLog->SetFont(font);


			RECT border;

			border.left = 0;
			border.right = BATTLE_LOG_WIDTH;
			border.bottom = 0;
			border.top = BATTLE_LOG_HEIGHT;

			battleLog->SetBorders(border);
			battleLog->SetMinWidth(100);
			battleLog->SetMinHeight(50);

			battleLog->SetFrameThickness(10);

			std::vector <unsigned int> textures;
			textures.push_back( GUI::Singleton().Data.GetTexture("BattleLog.bmp") );

			battleLog->SetTextures(textures);
			battleLog->SetCurrentTexture(1);

			battleLog->SetTextColor( Color(0,1,0,1) );
			battleLog->SetMultiline(true);
			battleLog->SetEdition(false);
			battleLog->SetMaxCharacterCount(99999999999999999);

			battleLog->EventManager.Ptr_KeyDown = BattleLog_KeyDown;

			battleLog->SetName(SIMULATION_GUI_BATTLE_LOG);

			return battleLog;
		}
//
//
		Handler* CreateOptionBox ()
		{
			Handler *optionBox = new Handler;


			return optionBox;
		}