	#include "GlobalHeaders.h"

	#include "BattleForceManager.h"

	#include "..//MiniDataManager//MiniDataManager.h"
	MiniDataManager &miniData = MiniDataManager::Singleton();



	#include "..//..//..//ObjectOperator//ObjectOperator.h"
	ObjectOperator &objects = ObjectOperator::Singleton();

	#include "..//..//..//TerrainEditor//ModelsOperator//Model//Model.h"
	#include "..//..//..//LiquidObject//LiquidObject.h"



	#include "..//InterfaceManager//Handlers_headers//HANDLER_MACROS.h"


	extern PFNGLBINDBUFFERARBPROC glBindBufferARB;



	#define KEY_LEFT	  ('<')
	#define KEY_RIGHT	  ('>')

	#define KEY_SPACE	  ('_')
	#define KEY_BACKSPACE ('/')

	#define KEY_0 ('0')
	#define KEY_1 ('1')
	#define KEY_2 ('2')
	#define KEY_3 ('3')
	#define KEY_4 ('4')
	#define KEY_5 ('5')
	#define KEY_6 ('6')
	#define KEY_7 ('7')
	#define KEY_8 ('8')
	#define KEY_9 ('9')

	#define KEY_A ('A')
	#define KEY_B ('B')
	#define KEY_C ('C')
	#define KEY_D ('D')
	#define KEY_E ('E')
	#define KEY_F ('F')
	#define KEY_G ('G')
	#define KEY_H ('H')
	#define KEY_I ('I')
	#define KEY_J ('J')
	#define KEY_K ('K')
	#define KEY_L ('L')
	#define KEY_M ('M')
	#define KEY_N ('N')
	#define KEY_O ('O')
	#define KEY_P ('P')
	#define KEY_R ('R')
	#define KEY_S ('S')
	#define KEY_T ('T')
	#define KEY_U ('U')
	#define KEY_W ('W')
	#define KEY_X ('X')
	#define KEY_Y ('Y')
	#define KEY_Z ('Z')


	#define TAB_SPACE ("  ")

	#define GHOST_COLOR (Color(0.5f, 0.5f, 1.0f, 0.5f))





	bool BattleForceManager::Init  ()
	{
		//Mini *mini1 = MiniDataManager::Singleton().GetMini("ASSAULT");
		//Mini *mini2 = MiniDataManager::Singleton().GetMini("HEAVY");
		//Mini *mini3 = MiniDataManager::Singleton().GetMini("PISTOL");
		//Mini *mini4 = MiniDataManager::Singleton().GetMini("RAPID");
		//Mini *mini5 = MiniDataManager::Singleton().GetMini("POWERFIST");
		//Mini *mini6 = MiniDataManager::Singleton().GetMini("RENDING");

		//// assault, heavy, pistol, rapid, powerfist, rending

		//Squad *assault   = new Squad("ASSAULT",   mini1, 1);
		//Squad *heavy     = new Squad("HEAVY",     mini2, 3);
		//Squad *pistol    = new Squad("PISTOL",    mini3, 3);
		//Squad *rapid     = new Squad("RAPID",     mini4, 3);
		//Squad *powerfist = new Squad("POWERFIST", mini5, 3);
		//Squad *rending   = new Squad("RENDING",   mini6, 3);

		//Squad *assault2   = new Squad("ASSAULT",   mini1, 1);
		//Squad *heavy2     = new Squad("HEAVY",     mini2, 3);
		//Squad *pistol2    = new Squad("PISTOL",    mini3, 3);
		//Squad *rapid2     = new Squad("RAPID",     mini4, 3);
		//Squad *powerfist2 = new Squad("POWERFIST", mini5, 3);
		//Squad *rending2   = new Squad("RENDING",   mini6, 3);

		//AddBattleForce("BATTLEFORCE_1");
		//AddBattleForce("BATTLEFORCE_2");

		//GetBattleForce(0)->InsertNewSquad(assault);
		///*GetBattleForce(0)->InsertNewSquad(heavy);
		//GetBattleForce(0)->InsertNewSquad(pistol);
		//GetBattleForce(0)->InsertNewSquad(rapid);
		//GetBattleForce(0)->InsertNewSquad(powerfist);
		//GetBattleForce(0)->InsertNewSquad(rending);*/

		//GetBattleForce(1)->InsertNewSquad(assault2);
		///*GetBattleForce(1)->InsertNewSquad(heavy2);
		//GetBattleForce(1)->InsertNewSquad(pistol2);
		//GetBattleForce(1)->InsertNewSquad(rapid2);
		//GetBattleForce(1)->InsertNewSquad(powerfist2);
		//GetBattleForce(1)->InsertNewSquad(rending2);*/

		return true;
	}

	void BattleForceManager::Close ()
	{
		RemoveAllBattleForces();
	}
//
//
	void BattleForceManager::Reset ()
	{
		ResetAllSquads();
		ResetPhaseFlags();

		RefreshInfoBoxes();
		ClearBattleLog();

		turnTicker = 0;

		StartDeploymentPhase();

		GUI::Singleton().Controls.GetControl(SIMULATION_GUI)->SetRendering(false);
		GUI::Singleton().Controls.GetControl(SIMULATION_GUI)->SetReadingInput(false);

		GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetRendering(true);
		GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetReadingInput(true);

		currentBattleForce = battleForces[0];
	}
//
		void BattleForceManager::ResetAllSquads ()
		{
			for(int i = 0; i < battleForces.size();              i++)
			for(int j = 0; j < battleForces[i]->GetSquadCount(); j++)
			{
				Squad *squad = battleForces[i]->GetSquad(j);

				squad->ReviveAll();
				squad->Undeploy();

				squad->ResetStateFlags();
				squad->ResetDrawingFlags();

				squad->ResetKillCount();

				squad->SetMeleeOpponnent(NULL);
			}
		}
//
//
	bool BattleForceManager::Run ()
	{
		if( exitSimulator )
		{
			return false;
		}

		if( receivedInput )
		{
			assert( processingInput == false );
			ProcessInput();
		}

		PreRendering();

		RenderGhostSquad();
		RenderBattleForces();

		PostRendering();

		return true;
	}
//
		void BattleForceManager::RenderGhostSquad   ()
		{
			if( ghostSquad )
			{
				ghostSquad->Render();
			}
		}

		void BattleForceManager::RenderBattleForces ()
		{
			for(int i = 0; i < battleForces.size(); i++)
			{
				battleForces[i]->Render();
			}
		}
//
		void BattleForceManager::PreRendering  ()
		{
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
		}

		void BattleForceManager::PostRendering ()
		{
			glDisable(GL_TEXTURE_2D);
			//glDisable(GL_CULL_FACE);
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		}
//
/*****************************************************/

	void BattleForceManager::AddBattleForce (std::string const &battleForceName)	throw(std::string)
	{
		if( BattleForceWithThisNameAlreadyExists(battleForceName) )
		{
			InformAboutDuplicateBattleForceName(battleForceName);
		}
		else
			AddNewBattleForce(battleForceName);
	}
//
		bool BattleForceManager::BattleForceWithThisNameAlreadyExists (std::string const &battleForceName)
		{
			bool FoundBattleForceNameDuplicate = false;

			for(int i = 0; i < battleForces.size(); i++)
			{
				if( battleForces[i]->GetBattleForceName() == battleForceName )
				{
					FoundBattleForceNameDuplicate = true;
					break;
				}
			}

			return FoundBattleForceNameDuplicate;
		}

		void BattleForceManager::InformAboutDuplicateBattleForceName  (std::string const &battleForceName)	throw(std::string)
		{
			std::string error;

			error  = "Failed to add new BattleForce.";
			error += "\n\n";

			error += "BattleForce with name ";
			error += battleForceName;
			error += " already exists!";

			throw error;
		}

		void BattleForceManager::AddNewBattleForce (std::string const &battleForceName)
		{
			BattleForce *newBattleForce = new BattleForce(battleForceName);

			battleForces.push_back( newBattleForce );

			if( currentBattleForce == NULL )
			{
				currentBattleForce = newBattleForce;
			}
		}
//
	void BattleForceManager::RemoveBattleForce (std::string const &battleForceName)	throw(std::string)
	{
		for(int i = 0; i < battleForces.size(); i++)
		{
			if( battleForces[i]->GetBattleForceName() == battleForceName )
			{
				PerformBattleForceRemovement(i);
				return;
			}
		}

		InformAboutAttemptToRemoveNonexistentBattleForce(battleForceName);
	}
//
		void BattleForceManager::InformAboutAttemptToRemoveNonexistentBattleForce (std::string const &battleForceName)	throw(std::string)
		{
			std::string error;

			error  = "Failed to remove BattleForce.";
			error += "\n\n";

			error += "BattleForce with name ";
			error += battleForceName;
			error += " doesn't exist!";

			throw error;
		}

		void BattleForceManager::PerformBattleForceRemovement (int const &index)
		{
			CorrigateCurrentBattleForce(index);
			battleForces.erase( battleForces.begin()+index );
		}

		void BattleForceManager::CorrigateCurrentBattleForce  (int const &index)
		{
			if( currentBattleForce == battleForces[index] )
			{
				if( battleForces.size() == 1 )
				{
					currentBattleForce = NULL;
				}
				else if( index == 1 )
				{
					currentBattleForce = battleForces[1];
				}
				else
					currentBattleForce = battleForces[0];
			}
		}
//
	void BattleForceManager::RemoveAllBattleForces ()
	{
		battleForces.clear();
	}
//
	std::vector <std::string> BattleForceManager::GetBattleForceList ()
	{
		std::vector <std::string> battleForceList;

		for(int i = 0; i < battleForces.size(); i++)
		{
			battleForceList.push_back( battleForces[i]->GetBattleForceName() );
		}

		return battleForceList;
	}

	int                       BattleForceManager::GetBattleForceCount ()
	{
		return battleForces.size();
	}
//
	void         BattleForceManager::SetCurrentBattleForce (BattleForce *battleForce)
	{
		currentBattleForce = battleForce;
	}

	void         BattleForceManager::SetCurrentBattleForce (int const &index)
	{
		if( GivenIndexIsCorrect(index) )
		{
			currentBattleForce = battleForces[index];
		}
		else
			InformAboutIncorrectIndex(index);
	}

	BattleForce* BattleForceManager::GetCurrentBattleForce ()
	{
		return currentBattleForce;
	}
//
	void BattleForceManager::SetFirstBattleForce ()
	{
		assert( battleForces.size() );

		currentBattleForce = battleForces[0];
	}

	void BattleForceManager::SetNextBattleForce  ()
	{
		assert( battleForces.size() );

		for(int i = 0; i < battleForces.size(); i++)
		{
			if( battleForces[i] == currentBattleForce )
			{
				if( i < battleForces.size()-1 )
				{
					currentBattleForce = battleForces[i+1];
				}
				else
					SetFirstBattleForce();

				return;
			}
		}
	}
//
	BattleForce* BattleForceManager::GetBattleForce (int const &index)
	{
		if( GivenIndexIsCorrect(index) )
		{
			return battleForces[index];
		}
		else
			InformAboutIncorrectIndex(index);
	}

	BattleForce* BattleForceManager::GetBattleForce (std::string const battleForceName)
	{
		for(int i = 0; i < battleForces.size(); i++)
		{
			if( battleForces[i]->GetBattleForceName() == battleForceName )
			{
				return battleForces[i];
			}
		}

		InformAboutIncorrectName(battleForceName);
	}
//
		bool BattleForceManager::GivenIndexIsCorrect       (int const &index)
		{
			if( index >= 0 && index < battleForces.size() )
			{
				return true;;
			}
			else
				return false;
		}

		void BattleForceManager::InformAboutIncorrectIndex (int const &index) throw (std::string)
		{
			std::string error;

			error  = "Given index ";
			error += index;
			error += " is incorrect!";

			throw error;
		}

		void BattleForceManager::InformAboutIncorrectName  (std::string const &battleForceName) throw (std::string)
		{
			std::string error;

			error  = "Given battleForce name ";
			error += battleForceName;
			error += " is incorrect!";

			throw error;
		}
//
/*****************************************************/

	void BattleForceManager::StartDeploymentPhase () throw (std::string)
	{
		if( AtLeastTwoOpposingSquadsAreSet() )
		{
			ResetPhaseFlags();
			deploymentPhase = true;
		}
		else
			InformAboutSquadsNotBeeingSet();
	}

	void BattleForceManager::StartSimulationPhase () throw (std::string)
	{
		if( simulationPhase != true )
		{
			if( AtLeastTwoOpposingSquadsAreDeployed() )
			{
				ResetPhaseFlags();
				simulationPhase = movementPhase = true;

				FilterOutBattleForcesWithNoSquadsDeployed();

				SetFirstBattleForce();
				ResetDrawingFlags();
				MovementPhase_SetActiveMinis();

				RefreshInfoBoxes();
			}
			else
				InformAboutSquadsNotBeeingSet();
		}
		else
			InformThatSimulationAlreadyStarted();
	}
//
		bool BattleForceManager::AtLeastTwoOpposingSquadsAreSet      ()
		{
			if( battleForces.size() < 2 )
			{
				return false;
			}
			else
			{
				int battleForcesWithAtLeastOneSquadSet = 0;

				for(int i = 0; i < battleForces.size(); i++)
				{
					if( battleForces[i]->GetSquadCount() > 0 )
					{
						battleForcesWithAtLeastOneSquadSet++;
					}
				}

				if( battleForcesWithAtLeastOneSquadSet >= 2 )
				{
					return true;
				}
				else
					return false;
			}
		}

		bool BattleForceManager::AtLeastTwoOpposingSquadsAreDeployed ()
		{
			if( battleForces.size() < 2 )
			{
				return false;
			}
			else
			{
				int battleForcesWithAtLeastOneSquadDeployed = 0;

				for(int i = 0; i < battleForces.size(); i++)
				{
					if( battleForces[i]->GetDeployedSquadCount() > 0 )
					{
						battleForcesWithAtLeastOneSquadDeployed++;
					}
				}

				if( battleForcesWithAtLeastOneSquadDeployed >= 2 )
				{
					return true;
				}
				else
					return false;
			}
		}

		void BattleForceManager::InformAboutSquadsNotBeeingSet       () throw (std::string)
		{
			throw std::string("There need to be at least two opposing squads set!");
		}
//
		void BattleForceManager::FilterOutBattleForcesWithNoSquadsDeployed ()
		{
			for(int i = battleForces.size()-1; i >= 0; i--)
			{
				if( battleForces[i]->GetDeployedSquadCount() == 0 )
				{
					RemoveBattleForce( battleForces[i]->GetBattleForceName() );
				}
			}
		}

		void BattleForceManager::InformThatSimulationAlreadyStarted        () throw (std::string)
		{
			throw std::string("Simulation already has started!");
		}
//
//
	void BattleForceManager::NextPhase ()
	{
		if( simulationPhase )
		{
			if( VictoryCheck() )
			{
				return;
			}


			if( movementPhase )
			{
				if( MovementPhase_Check() )
				{
					ShootingPhase_SetUp();

					ClearCurrentSelection();
					ClearCurrentTarget();
				}
			}
			else if( shootingPhase )
			{
				if( ShootingPhase_Check() )
				{
					AssaultPhase_SetUp();

					ClearCurrentSelection();
					ClearCurrentTarget();
				}
			}
			else if( assaultPhase )
			{
				if( AssaultPhase_Check() )
				{
					SetNextBattleForce();

					if( currentBattleForce == battleForces[0] )
					{
						ResetStateFlags();
						SetNextTurn();
					}

					MovementPhase_SetUp();

					ClearCurrentSelection();
					ClearCurrentTarget();
				}
			}
			else
				InformThatSimulationFlagsAreIncorrect();
		}
		else
			InformThatSimulationHasntStarted();
	}
//
		bool BattleForceManager::VictoryCheck ()
		{
			std::vector <std::string> battleForcesLeft = ScanForBattleForcesInAction();

			if( battleForcesLeft.size() == 1 )
			{
				ProclaimVictoryAndResetSimulator( battleForcesLeft[0] );
				return true;
			}
			else
				return false;
		}
//
		std::vector <std::string> BattleForceManager::ScanForBattleForcesInAction      ()
		{
			std::vector <std::string> battleForcesInAction;

			for(int i = 0; i < battleForces.size(); i++)
			{
				if( battleForces[i]->GetInActionSquadCount() )
				{
					battleForcesInAction.push_back( battleForces[i]->GetBattleForceName() );
				}
			}

			assert( battleForcesInAction.size() > 0 );
			
			return battleForcesInAction;
		}

		void                      BattleForceManager::ProclaimVictoryAndResetSimulator (std::string const &victoriusBattleForce)
		{
			std::string victoryInfo;

			victoryInfo  = victoriusBattleForce;
			victoryInfo += " has won the battle!";
			victoryInfo += "\n\n";
			victoryInfo += "Do you wish to restart the simulation?";


			int result = MessageBox(NULL, victoryInfo.c_str(), "Victory!", MB_YESNO);

			if( result == IDYES )
			{
				Reset();
			}
			else
				exitSimulator = true;
		}
//
		bool BattleForceManager::MovementPhase_Check ()
		{
			int minisThatCanMove = 0;

			for(int i = 0; i < currentBattleForce->GetSquadCount(); i++)
			{
				minisThatCanMove += currentBattleForce->GetSquad(i)->HowManyMembersCanMove();
			}

			if( minisThatCanMove > 0 )
			{
				std::string question;
				
				question  = NumberToString(minisThatCanMove);
				question += " minis can move.";
				question += "\n";

				question += "Are you sure you want to proceed to the next phase?";

				int result = MessageBox(NULL, question.c_str(), "Warning.", MB_YESNO);

				if( result == IDYES )
				{
					return true;
				}
				else
					return false;
			}
			else
				return true;
		}

		void BattleForceManager::MovementPhase_SetUp ()
		{
			assaultPhase = false;
			movementPhase = true;

			ResetDrawingFlags();
			MovementPhase_SetActiveMinis();

			MessageBox(NULL, "Movement phase.", "Next phase!", MB_OK);
		}

		void BattleForceManager::MovementPhase_SetActiveMinis ()
		{
			for(int i = 0; i < currentBattleForce->GetSquadCount(); i++)
			{
				Squad *currentSquad = currentBattleForce->GetSquad(i);
				currentSquad->SetFirstMiniAsCurrent();

				for(int j = 0; j < currentSquad->GetOriginalSquadSize(); j++)
				{
					Mini *mini = currentSquad->GetMini(j);
					
					if( mini->IsInAction() && mini->CanMove() )
					{
						mini->SetDrawingActivity(true);
					}
				}
			}
		}
//
		bool BattleForceManager::ShootingPhase_Check ()
		{
			int minisThatCanShoot = 0;
			int minisThatCanShootRapidly = 0;

			for(int i = 0; i < currentBattleForce->GetSquadCount(); i++)
			{
				minisThatCanShoot        += currentBattleForce->GetSquad(i)->HowManyMembersCanShoot();
				minisThatCanShootRapidly += currentBattleForce->GetSquad(i)->HowManyMembersCanShootRapidly();
			}

			if( minisThatCanShoot > 0 || minisThatCanShootRapidly > 0 )
			{
				std::string question;
				
				question  = NumberToString(minisThatCanShoot);
				question += " minis can shoot";

				if( minisThatCanShootRapidly > 0 )
				{
					question += " and ";
					question += NumberToString(minisThatCanShootRapidly);
					question += " minis can rapid fire";
				}
			
				question += ".";
				question += "\n";

				question += "Are you sure you want to proceed to the next phase?";

				int result = MessageBox(NULL, question.c_str(), "Warning.", MB_YESNO);

				if( result == IDYES )
				{
					return true;
				}
				else
					return false;
			}
			else
				return true;
		}

		void BattleForceManager::ShootingPhase_SetUp ()
		{
			movementPhase = false;
			shootingPhase = true;

			ResetDrawingFlags();
			ShootingPhase_SetActiveMinis();

			MessageBox(NULL, "Shooting phase.", "Next phase!", MB_OK);
		}

		void BattleForceManager::ShootingPhase_SetActiveMinis ()
		{
			currentBattleForce->SetFirstSquadAsCurrent();

			for(int i = 0; i < currentBattleForce->GetSquadCount(); i++)
			{
				Squad *currentSquad = currentBattleForce->GetCurrentSquad();
				currentSquad->SetFirstMiniAsCurrent();

				for(int j = 0; j < currentSquad->GetOriginalSquadSize(); j++)
				{
					Mini *currentMini = currentSquad->GetCurrentMini();
					
					if( currentMini->IsInAction() && (currentMini->CanShoot() || currentMini->CanShootRapidly()) )
					{
						currentMini->SetDrawingActivity(true);
					}

					currentSquad->SetNextMiniAsCurrent();
				}

				currentBattleForce->SetNextSquadAsCurrent();
			}
		}
//
		bool BattleForceManager::AssaultPhase_Check ()
		{
			int minisThatCanAssault = 0;

			for(int i = 0; i < currentBattleForce->GetSquadCount(); i++)
			{
				minisThatCanAssault += currentBattleForce->GetSquad(i)->HowManyMembersCanAssault();
			}

			if( minisThatCanAssault > 0 )
			{
				std::string question;
				
				question  = NumberToString(minisThatCanAssault);
				question += " minis can assault.";
				question += "\n";

				question += "Are you sure you want to proceed to the next phase?";

				int result = MessageBox(NULL, question.c_str(), "Warning.", MB_YESNO);

				if( result == IDYES )
				{
					return true;
				}
				else
					return false;
			}
			else
				return true;
		}

		void BattleForceManager::AssaultPhase_SetUp ()
		{
			shootingPhase = false;
			assaultPhase = true;

			ResetDrawingFlags();
			AssaultPhase_SetActiveMinis();

			MessageBox(NULL, "Assault phase.", "Next phase!", MB_OK);
		}

		void BattleForceManager::AssaultPhase_SetActiveMinis  ()
		{
			currentBattleForce->SetFirstSquadAsCurrent();

			for(int i = 0; i < currentBattleForce->GetSquadCount(); i++)
			{
				Squad *currentSquad = currentBattleForce->GetCurrentSquad();
				currentSquad->SetFirstMiniAsCurrent();

				for(int j = 0; j < currentSquad->GetOriginalSquadSize(); j++)
				{
					Mini *currentMini = currentSquad->GetCurrentMini();
					
					if( currentMini->IsInAction() && currentMini->CanAssault() )
					{
						currentMini->SetDrawingActivity(true);
					}

					currentSquad->SetNextMiniAsCurrent();
				}

				currentBattleForce->SetNextSquadAsCurrent();
			}
		}
//
		void BattleForceManager::ResetDrawingFlags            ()
		{
			for(int i = 0; i < battleForces.size(); i++)
			{
				battleForces[i]->ResetDrawingFlags();
			}
		}

		void BattleForceManager::ResetStateFlags   ()
		{
			for(int i = 0; i < battleForces.size(); i++)
			{
				battleForces[i]->ResetStateFlags();
			}
		}
//
		void BattleForceManager::InformThatSimulationFlagsAreIncorrect () throw (std::string)
		{
			throw std::string("Simulation phase flags are incorrect (all are false)!");
		}

		void BattleForceManager::InformThatSimulationHasntStarted      () throw (std::string)
		{
			throw std::string("Simulation hasn't started yet!");
		}
//
//
	void BattleForceManager::ResetPhaseFlags ()
	{
		deploymentPhase = false;

		simulationPhase = false;
		movementPhase = shootingPhase = assaultPhase = false;
	}
//
/*****************************************************/

	void BattleForceManager::SetNextTurn ()
	{
		turnTicker++;
		RefreshCurrentTurnLabel();
	}
//
		void BattleForceManager::RefreshCurrentTurnLabel ()
		{
			// blablabla
		}
//
	int  BattleForceManager::GetCurrentTurn ()
	{
		return turnTicker;
	}
//
/*****************************************************/

	void BattleForceManager::SetPreviousLog ()
	{
		if( currentLog > 0 )
		{
			currentLog--;
		}
	}

	void BattleForceManager::SetNextLog     ()
	{
		if( currentLog < battleLog.size()-1 )
		{
			currentLog++;
		}
	}
//
	std::string BattleForceManager::GetCurrentLog ()
	{
		if( battleLog.size() )
		{
			assert( currentLog >= 0 && currentLog < battleLog.size() );
			return battleLog[currentLog];
		}
		else
			return std::string("");
	}
//
	void BattleForceManager::ClearBattleLog ()
	{
		battleLog.clear();
		currentLog = 0;

		Edit *battleLogEdit = reinterpret_cast <Edit*> ( GUI::Singleton().Controls.GetControl(SIMULATION_GUI_BATTLE_LOG) );
		battleLogEdit->SetText("");
	}
//
/*****************************************************/

	void BattleForceManager::ClearCurrentSelection ()
	{
		currentSelection = NULL;
		RefreshInfoBoxes();
	}

	void BattleForceManager::SetCurrentSelection   (Squad *squad)
	{
		currentSelection = squad;
		currentSelection->GetCurrentMini()->SetDrawingSelection(true);

		RefreshInfoBoxes();
	}
//
/*****************************************************/

	void BattleForceManager::SetHeightMap (float *heightMap, int const &mapWidth_X, int  const &mapWidth_Z)
	{
		if( this->heightMap != NULL )
		{
			delete [] this->heightMap;
		}

		assert( heightMap != NULL );

		this->heightMap  = heightMap;
		this->mapWidth_X = mapWidth_X;
		this->mapWidth_Z = mapWidth_Z;

		
		Mini::SetHeightMap(this->heightMap, this->mapWidth_X, this->mapWidth_Z);
	}
//
	float* BattleForceManager::GetHeightMap  ()
	{
		return heightMap;
	}

	int    BattleForceManager::GetMapWidth_X ()
	{
		return mapWidth_X;
	}

	int    BattleForceManager::GetMapWidth_Z ()
	{
		return mapWidth_Z;
	}
//
	Vector3 BattleForceManager::CalculateHeight (Vector3 const &openGLposition)
	{
		if( heightMap == NULL )
		{
			return Vector3(0,0,0);
		}

		float projCameraX =  openGLposition.x,
		      projCameraZ = -openGLposition.z;

		int col0 = openGLposition.x;
		int col1 = col0 + 1;
		int row0 = -openGLposition.z;
		int row1 = row0 + 1;

		if (col1 > mapWidth_X)
			col1 = 0;
	
		if (row1 > mapWidth_Z)
			row1 = 0;

		Vector3 positionOnHeightMap = openGLposition;

		if( col0 < mapWidth_X && row1 < mapWidth_Z )
		{
			float h00 = (float)heightMap[col0 + row0*mapWidth_X];
			float h01 = (float)heightMap[col1 + row0*mapWidth_X];
			float h11 = (float)heightMap[col1 + row1*mapWidth_X];
			float h10 = (float)heightMap[col0 + row1*mapWidth_X];

			float tx = projCameraX - float(col0);
			float ty = projCameraZ - float(row0);

			float txty = tx * ty;

			float final_height = h00 * (1.0f - ty - tx + txty)
							+ h01 * (tx - txty)
							+ h11 * txty
							+ h10 * (ty - txty);

			
			positionOnHeightMap.y = final_height;
		}
		else
			positionOnHeightMap.y = 0;

		return positionOnHeightMap;
	}
//
/*****************************************************/

	void BattleForceManager::KeyDown             (int const &keyCode)
	{
		if( currentBattleForce && !processingInput && keyCode )
		{
			keyDown = keyPressed = true;
			key = keyCode;
			receivedInput = true;
		}
	}

	void BattleForceManager::KeyUp               (int const &keyCode)
	{
		if( currentBattleForce && !processingInput && keyCode )
		{
			keyUp = true;
			keyPressed = false;
			key = keyCode;
			receivedInput = true;
		}
	}
//
	void BattleForceManager::LeftMouseDown       ()
	{
		if( currentBattleForce && !processingInput )
		{
			leftDown = leftPressed = true;
			receivedInput = true;
		}
	}
	void BattleForceManager::MiddleMouseDown     ()
	{
		if( currentBattleForce && !processingInput )
		{
			middleDown = middlePressed = true;
			receivedInput = true;
		}
	}

	void BattleForceManager::RightMouseDown      ()
	{
		if( currentBattleForce && !processingInput )
		{
			rightDown = rightPressed = true;
			receivedInput = true;
		}
	}
//
	void BattleForceManager::LeftMouseUp         ()
	{
		if( currentBattleForce && !processingInput )
		{
			leftUp = true;
			leftPressed = false;
			receivedInput = true;
		}
	}

	void BattleForceManager::MiddleMouseUp       ()
	{
		if( currentBattleForce && !processingInput )
		{
			middleUp = true;
			middlePressed = false;
			receivedInput = true;
		}
	}

	void BattleForceManager::RightMouseUp        ()
	{
		if( currentBattleForce && !processingInput )
		{
			rightUp = true;
			rightPressed = false;
			receivedInput = true;
		}
	}
//
	void BattleForceManager::LeftMouseDblClick   ()
	{
		if( currentBattleForce && !processingInput )
		{
			leftDblClick = true;
			receivedInput = true;
		}
	}

	void BattleForceManager::MiddleMouseDblClick ()
	{
		if( currentBattleForce && !processingInput )
		{
			middleDblClick = true;
			receivedInput = true;
		}
	}

	void BattleForceManager::RightMouseDblClick  ()
	{
		if( currentBattleForce && !processingInput )
		{
			rightDblClick = true;
			receivedInput = true;
		}
	}
//
	void BattleForceManager::Scroll              (int const &Info)
	{
		if( currentBattleForce && !processingInput && scrollInfo )
		{
			scrollInfo = Info;
			receivedInput = true;
		}
	}
//
	void BattleForceManager::MoveCursor          (POINT const &mousePosition)
	{
		if( currentBattleForce && !processingInput )
		{
			oldCursor = cursor;
			cursor = CalculateHeight( GetOGLPos(mousePosition.x, mousePosition.y) );
		}
	}
//
/******************************************************
		Input "digestion" functions.
******************************************************/

	void BattleForceManager::ProcessInput ()
	{
		processingInput = true;

		ProcessKey();
		ProcessMouse();
		ProcessScroll();

		if( CursorHasChangedPosition() )
		{
			ProcessMouseMovement();
		}

		CheckInputValuesCorrectness();


		if( NoKeysPressed() )
		{
			receivedInput = false;
		}

		processingInput = false;
	}
//
		void BattleForceManager::ProcessKey ()
		{
			if( keyDown )
			{
				ProcessKeyDown();
			}

			if( keyPressed )
			{
				ProcessKeyPressed();
			}

			if( keyUp )
			{
				ProcessKeyUp();
			}
		}
//
			void BattleForceManager::ProcessKeyDown    ()
			{
				if( deploymentPhase )
				{
					DeploymentPhase_KeyDown();
				}
				else if( movementPhase )
				{
					MovementPhase_KeyDown();
				}
				else if( shootingPhase )
				{
					ShootingPhase_KeyDown();
				}
				else if( assaultPhase )
				{
					AssaultPhase_KeyDown();
				}

				keyDown = false;
			}

			void BattleForceManager::ProcessKeyPressed ()
			{
				
			}

			void BattleForceManager::ProcessKeyUp      ()
			{
				

				key = 0;
				keyUp = false;
			}


//
		void BattleForceManager::ProcessMouse ()
		{
			ProcessMouseDown();
			ProcessMousePressed();
			ProcessMouseUp();
			ProcessMouseDblClick();
		}
//
		void BattleForceManager::ProcessMouseDown ()
		{
			if( leftDown )
			{
				ProcessMouseLeftDown();
			}
			if( middleDown )
			{
				ProcessMouseMiddleDown();
			}
			if( rightDown )
			{
				ProcessMouseRightDown();
			}
		}
//
			void BattleForceManager::ProcessMouseLeftDown   ()
			{
				if( deploymentPhase )
				{
					DeploymentPhase_LeftMouseDown();
				}
				else if( movementPhase )
				{
					MovementPhase_LeftMouseDown();
				}
				else if( shootingPhase )
				{
					ShootingPhase_LeftMouseDown();
				}
				else if( assaultPhase )
				{
					AssaultPhase_LeftMouseDown();
				}

				leftDown = false;
			}
			void BattleForceManager::ProcessMouseMiddleDown ()
			{
				if( deploymentPhase )
				{}
				else if( movementPhase )
				{}
				else if( shootingPhase )
				{}
				else if( assaultPhase )
				{}

				middleDown = false;
			}

			void BattleForceManager::ProcessMouseRightDown  ()
			{
				if( deploymentPhase )
				{}
				else if( movementPhase )
				{}
				else if( shootingPhase )
				{}
				else if( assaultPhase )
				{}

				rightDown = false;
			}

//
		void BattleForceManager::ProcessMousePressed ()
		{
			if( leftPressed )
			{
				ProcessMouseLeftPressed();
			}
			if( middlePressed )
			{
				ProcessMouseMiddlePressed();
			}
			if( rightPressed )
			{
				ProcessMouseRightPressed();
			}
		}
//
			void BattleForceManager::ProcessMouseLeftPressed   ()
			{
				
			}

			void BattleForceManager::ProcessMouseMiddlePressed ()
			{
				
			}

			void BattleForceManager::ProcessMouseRightPressed  ()
			{
				
			}
//
		void BattleForceManager::ProcessMouseUp ()
		{
			if( leftUp )
			{
				ProcessMouseLeftUp();
			}
			if( middleUp )
			{
				ProcessMouseMiddleUp();
			}
			if( rightUp )
			{
				ProcessMouseRightUp();
			}
		}
//
			void BattleForceManager::ProcessMouseLeftUp   ()
			{
				if( deploymentPhase )
				{
					
				}
				else if( movementPhase )
				{
				}
				else if( shootingPhase )
				{
				}
				else if( assaultPhase )
				{
				}

				leftUp = false;
			}

			void BattleForceManager::ProcessMouseMiddleUp ()
			{
				if( deploymentPhase )
				{
					
				}
				else if( movementPhase )
				{
				}
				else if( shootingPhase )
				{
				}
				else if( assaultPhase )
				{
				}

				middleUp = false;
			}

			void BattleForceManager::ProcessMouseRightUp  ()
			{
				if( deploymentPhase )
				{
				}
				else if( movementPhase )
				{
				}
				else if( shootingPhase )
				{
				}
				else if( assaultPhase )
				{
				}

				rightUp = false;
			}


//
		void BattleForceManager::ProcessMouseDblClick ()
		{
			if( leftDblClick )
			{
				ProcessMouseLeftDblClick();
			}
			if( middleDblClick )
			{
				ProcessMouseMiddleDblClick();
			}
			if( rightDblClick )
			{
				ProcessMouseRightDblClick();
			}
		}
//
			void BattleForceManager::ProcessMouseLeftDblClick   ()
			{
				

				leftDblClick = false;
			}

			void BattleForceManager::ProcessMouseMiddleDblClick ()
			{
				
				middleDblClick = false;
			}

			void BattleForceManager::ProcessMouseRightDblClick  ()
			{
				

				rightDblClick = false;
			}
//
		void BattleForceManager::ProcessScroll ()
		{
			if( scrollInfo )
			{
				

				scrollInfo = 0;
			}
		}

//
		bool BattleForceManager::CursorHasChangedPosition ()
		{
			return cursor != oldCursor;
		}

		void BattleForceManager::ProcessMouseMovement	  ()
		{
			if( MouseKeysPressed() )
			{
				if( deploymentPhase )
				{
					DeploymentPhase_ProcessMouseMovement();				
				}
				else if( movementPhase )
				{
					MovementPhase_ProcessMouseMovement();
				}
				else if( assaultPhase )
				{
					AssaultPhase_ProcessMouseMovement();
				}
			}
			else
			{}

			oldCursor = cursor;
		}
//
	bool BattleForceManager::MouseKeysPressed			 ()
	{
		return leftPressed || middlePressed || rightPressed;
	}

	bool BattleForceManager::NoKeysPressed			     ()
	{
		return keyPressed && leftPressed && middlePressed && rightPressed == false;
	}

	void BattleForceManager::CheckInputValuesCorrectness ()
	{
	   keyDown      = keyUp          =
	   leftDown     = middleDown     = rightDown =
	   leftUp       = middleUp       = rightUp   =
	   leftDblClick = middleDblClick = rightDblClick  = false;

		key = 0;
		
		oldCursor = cursor;
	}

	void BattleForceManager::ClearInputValues            ()
	{
		key = scrollInfo = 0;

		keyDown      = keyUp          = 
		leftDown     = middleDown     = rightDown     =
		leftUp       = middleUp       = rightUp       =
		leftDblClick = middleDblClick = rightDblClick = false;
	}
//
/******************************************************
	Phase specific input "digestion" functions.
******************************************************/

	void BattleForceManager::DeploymentPhase_KeyDown  ()
	{
		switch( key )
		{
			case KEY_SPACE:
			{
				if( currentSelection )
				{
					Mini  *currentMini  = currentSelection->GetCurrentMini();

					if( currentSelection->IsDeployed() )
					{
						for(int i = 0; i < currentSelection->GetOriginalSquadSize(); i++)
						{
							currentSelection->GetMini(i)->SetDrawingSquadSpacing(false);					
						}

						GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetReadingInput(true);
						GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetRendering(true);

						ClearCurrentSelection();
					}
					else if( currentMini->IsDeployed() )
					{
						currentSelection->SetNextMiniAsCurrent();
					}
				}
			}
			break;

			case KEY_BACKSPACE:
			{
				if( currentSelection )
				{
					currentSelection->Undeploy();
					ClearCurrentSelection();

					GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetRendering(true);
					GUI::Singleton().Controls.GetControl(DEPLOYMENT_WINDOW)->SetReadingInput(true);
				}
			}
			break;


			case 'N':
			{
				NextPhase();
			}
			break;
		}
	}

	void BattleForceManager::DeploymentPhase_LeftMouseDown  ()
	{
		if( currentSelection )
		{
			Mini  *currentMini  = currentSelection->GetCurrentMini();

			if( currentSelection->IsNotDeployed() )
			{
				currentSelection->DeployCurrentMini(cursor);
				currentMini->SetDrawingSquadSpacing(true);

				if( currentSelection->SquadLostIntegrity() ||
					MiniIsInWrongPosition(currentMini) )
				{
					currentMini->SetDeployment(false);
				}
			}
		}
	}

	void BattleForceManager::DeploymentPhase_ProcessMouseMovement ()
	{
		if( currentSelection )
		{
			Mini  *currentMini  = currentSelection->GetCurrentMini();

			if( leftPressed )
			{
				Vector3 previousPosition = currentMini->GetPosition();
				currentMini->SetPosition(cursor);

				if( currentSelection->SquadLostIntegrity() || 
					MiniIsInWrongPosition(currentMini) )
				{
					currentMini->SetPosition(previousPosition);
				}
			}
			else if( rightPressed )
			{
				currentSelection->GetCurrentMini()->LookAt(cursor);
			}
		}
	}
//
	void BattleForceManager::MovementPhase_KeyDown ()
	{
		switch( key )
		{
			case KEY_SPACE:
			{
				PerformMovement();
			}
			break;

			case KEY_BACKSPACE:
			{
				CancelMovement();
			}
			break;

			case 'I':
			{
				InitiateMovement();
			}
			break;

			case 'R':
			{
				InitiateRunOnSelectedSquad();
			}
			break;


			case 'N':
			{
				if( movementInitiated == false )
				{
					NextPhase();
				}
			}
			break;
		}
	}

	void BattleForceManager::MovementPhase_LeftMouseDown ()
	{
		StopDrawingRangesOnPreviousMini();
		StopDrawingPossibleTargets();

		if( movementInitiated )
		{
			StopDrawingMovementRangeOfPreviousGhost();

			if( SelectGhost() )
			{
				ScanBattleForcesForPossibleTargets(ghostSquad->GetCurrentMini());
			}
		}
		else if( SelectNewMiniAndDrawItsRanges() )
		{
			ScanBattleForcesForPossibleTargets(currentSelection->GetCurrentMini());
		}
	}

	void BattleForceManager::MovementPhase_ProcessMouseMovement ()
	{
		if( movementInitiated )
		{
			Mini *ghost = ghostSquad->GetCurrentMini();

			if( ghost )
			{
				if( leftPressed )
				{
					MoveGhost(cursor);

					StopDrawingPossibleTargets();
					ScanBattleForcesForPossibleTargets(ghost);
				}
				else if( rightPressed )
				{
					ghost->LookAt(cursor);
				}
			}
		}
	}
//
	void BattleForceManager::ShootingPhase_KeyDown ()
	{
		switch( key )
		{
			case KEY_SPACE:
			{
				ShootAtTarget();
			}
			break;


			case 'N':
			{
				NextPhase();
			}
			break;
		}
	}

	void BattleForceManager::ShootingPhase_LeftMouseDown ()
	{
		if( currentSelection && SelectNewTarget() )
		{}
		else 
		{
			StopDrawingRangesOnPreviousMini();
			StopDrawingPossibleTargets();
			
			if( SelectNewMiniAndDrawItsRanges() )
			{
				ScanBattleForcesForPossibleTargets(currentSelection->GetCurrentMini());
			}
		}
	}
//
	void BattleForceManager::AssaultPhase_KeyDown ()
	{
		switch( key )
		{
			case 'I':
			{
				InitiateAssault();
			}
			break;

			case KEY_SPACE:
			{
				AssaultTarget();
			}
			break;

			case KEY_BACKSPACE:
			{
				CancelAssault();
			}
			break;


			case 'N':
			{
				if( assaultInitiated == false )
				{
					NextPhase();
				}
			}
			break;
		}
	}

	void BattleForceManager::AssaultPhase_LeftMouseDown ()
	{
		StopDrawingRangesOnPreviousMini();
		StopDrawingPossibleTargets();

		if( assaultInitiated )
		{
			StopDrawingAssaultRangeOfPreviousGhost();

			if( SelectGhost() )
			{
				HighlightPossibleAssaultTargets();
			}
		}
		else if( currentSelection && SelectNewTarget() )
		{}
		else if( SelectNewMiniAndDrawItsRanges() )
		{
			ScanBattleForcesForPossibleTargets(currentSelection->GetCurrentMini());
		}
	}

	void BattleForceManager::AssaultPhase_ProcessMouseMovement ()
	{
		if( assaultInitiated )
		{
			Mini *ghost = ghostSquad->GetCurrentMini();

			if( ghost )
			{
				if( leftPressed )
				{
					MoveGhost(cursor);

					StopDrawingPossibleTargets();
					HighlightPossibleAssaultTargets();
				}
				else if( rightPressed )
				{
					ghostSquad->GetCurrentMini()->LookAt(cursor);
				}
			}
		}
	}
//
/*****************************************************/

	void BattleForceManager::StopDrawingRangesOnPreviousMini ()
	{
		if( currentSelection )
		{
			Mini *currentMini = currentSelection->GetCurrentMini();

			if( currentMini )
			{
				currentMini->ResetDrawingFlags();

				if( movementPhase && !movementInitiated && currentMini->CanMove() )
				{
					currentMini->SetDrawingActivity(true);
				}
				else if( shootingPhase && (currentMini->CanShoot() || currentMini->CanShootRapidly()) )
				{
					currentMini->SetDrawingActivity(true);
				}
				else if( assaultPhase && !assaultInitiated && currentMini->CanAssault() )
				{
					currentMini->SetDrawingActivity(true);
				}
				else
					currentMini->SetDrawingActivity(false);
			}
		}
	}

	bool BattleForceManager::SelectNewMiniAndDrawItsRanges   ()
	{
		assert( movementPhase || shootingPhase || assaultPhase );


		if( currentBattleForce->BattleForceGotClickedOn(cursor) )
		{
			Squad *clickedSquad = currentBattleForce->GetCurrentSquad();

			if( movementPhase && clickedSquad->HowManyMembersCanMove() )
			{
				SetCurrentSelection(clickedSquad);
				StartDrawingMovementRangeOnSelectedMini();

				return true;
			}
			else if( shootingPhase &&
			        (clickedSquad->HowManyMembersCanShoot() || 
				     clickedSquad->HowManyMembersCanShootRapidly()) )
			{
				SetCurrentSelection(clickedSquad);
				StartDrawingShootingRangeOnSelectedMini();

				return true;
			}
			else if( assaultPhase && clickedSquad->HowManyMembersCanAssault() )
			{
				SetCurrentSelection(clickedSquad);
				StartDrawingAssaultRangeOnSelectedMini();

				return true;
			}
			else
			{
				ClearCurrentSelection();
				return false;
			}
		}
		else
		{
			ClearCurrentSelection();
			return false;
		}
	}
//
		void BattleForceManager::StartDrawingMovementRangeOnSelectedMini ()
		{
			Mini *currentMini = currentSelection->GetCurrentMini();

			if( currentMini->CanMove() )
			{
				currentMini->SetDrawingMovementRange(true);
			}
		}

		void BattleForceManager::StartDrawingShootingRangeOnSelectedMini ()
		{
			Mini *currentMini = currentSelection->GetCurrentMini();

			if( currentMini->CanShoot() )
			{
				currentMini->SetDrawingShootingRange(true);
			}

			if( currentMini->CanShootRapidly() )
			{
				currentMini->SetDrawingRapidFiringRange(true);
			}
		}

		void BattleForceManager::StartDrawingAssaultRangeOnSelectedMini  ()
		{
			Mini *currentMini = currentSelection->GetCurrentMini();

			if( currentMini->CanAssault() )
			{
				currentMini->SetDrawingAssaultRange(true);
			}
		}
//
//
	bool BattleForceManager::SelectNewTarget ()
	{
		assert( shootingPhase || assaultPhase );
		assert( currentSelection != NULL );


		ClearCurrentTarget();

		for(int i = 0; i < battleForces.size(); i++)
		{
			if( battleForces[i] == currentBattleForce )
			{
				continue;
			}

			if( battleForces[i]->BattleForceGotClickedOn(cursor) )
			{
				Squad *clickedSquad = battleForces[i]->GetCurrentSquad();

				if( shootingPhase )
				{
					if( clickedSquad->IsTiedInMelee() )
					{
						return false;
					}
					else if( currentSelection->TargetInShootingRange(clickedSquad) || 
						    (currentSelection->TargetInRapidFireRange(clickedSquad) &&
							 currentSelection->HowManyMembersCanShootRapidly()) )
					{
						SetCurrentTarget(clickedSquad);
						return true;
					}
					else
						return false;
				}
				else if( assaultPhase )
				{
					Squad *currentMeleeOpponnent = currentSelection->GetMeleeOpponnent();

					if( currentMeleeOpponnent != NULL )
					{
						SetCurrentTarget(currentMeleeOpponnent);
						return true;
					}
					else if( currentSelection->TargetInAssaultRange(clickedSquad) )
					{
						SetCurrentTarget(clickedSquad);
						return true;
					}
					else
						return false;
				}				
			}
		}

		return false;
	}
//
/*****************************************************/

	void BattleForceManager::RefreshInfoBoxes ()
	{
		assert( currentBattleForce != NULL );

		RefreshSelectionInfoBox();
		RefreshTargetInfoBox();		
	}
//
		void BattleForceManager::RefreshSelectionInfoBox ()
		{
			Label *infoBox       = reinterpret_cast <Label*> ( GUI::Singleton().Controls.GetControl(SIMULATION_GUI_INFO_BOX) );

			std::string selectionInfo = currentBattleForce->ComposeBattleForceInfo();

			if( currentSelection )
			{
				selectionInfo += currentSelection->ComposeSquadInfo();
			}

			infoBox->SetText(selectionInfo);
		}

		void BattleForceManager::RefreshTargetInfoBox    ()
		{
			Label *targetInfoBox = reinterpret_cast <Label*> ( GUI::Singleton().Controls.GetControl(SIMULATION_GUI_TARGET_INFO_BOX) );

			
			if( currentTarget )
			{
				std::string targetInfo;

				targetInfo  = "Current target:";
				targetInfo += "\n\n";

				targetInfo += currentTarget->ComposeSquadInfo();

				targetInfoBox->SetText(targetInfo);
			}
			else
				targetInfoBox->SetText("No target selected.");
		}
//
/*****************************************************/

	void BattleForceManager::BattleLog_InsertHeaderInfo ()
	{
		std::string registry("");

		registry += TAB_SPACE;
		registry += "#";
		registry += NumberToString(battleLog.size()+1);
		registry += " Turn: ";
		registry += NumberToString(turnTicker+1);
		registry += " Current BattleForce: ";
		registry += currentBattleForce->GetBattleForceName();
		registry += "\n\n";

		battleLog.push_back(registry);
		SetNextLog();
	}
//
//
	void BattleForceManager::BattleLog_SquadIsRunning (std::string const &squadName, int const &extraInches)
	{
		BattleLog_InsertHeaderInfo();

		std::string registry = battleLog.back();

		registry += TAB_SPACE;
		registry += squadName;
		registry += " has initiated a run!";
		registry += std::string(" Gained ") + NumberToString(extraInches) + std::string(" extra inches.");
		registry += "\n";

		battleLog[currentLog] = registry;
		
		RefreshBattleLog_EditControl();
	}
//
	void BattleForceManager::BattleLog_SquadShoots_SquadnAndTargetInfo  (std::string const &squadName, int const &shooterCount, std::string const &targetName, int const &targetCount)
	{
		BattleLog_InsertHeaderInfo();

		std::string registry = battleLog.back();

		registry += TAB_SPACE;
		registry += squadName  + std::string(" (") + NumberToString(shooterCount) + std::string(")");
		registry += " shoots at ";
		registry += targetName + std::string(" (") + NumberToString(targetCount)  + std::string(").");
		registry += "\n\n";

		battleLog[currentLog] = registry;
	}

	void BattleForceManager::BattleLog_SquadShoots_NumberOfShotsAndHits (int const &numberOfShots, int const &numberOfHits)
	{
		std::string registry = battleLog.back();

		float percentage = ((float)numberOfHits/(float)numberOfShots) * 100.f;

		registry += TAB_SPACE;
		registry += " - ";
		registry += NumberToString(numberOfShots);
		registry += " shots were fired, of which ";
		registry += NumberToString(numberOfHits);
		registry += " (";
		registry += NumberToString(percentage);
		registry += "%) hit the target.";
		registry += "\n";

		battleLog[currentLog] = registry;
	}

	void BattleForceManager::BattleLog_SquadShoots_NumberOfWounds       (int const &numberOfHits, int const &numberOfWounds)
	{
		std::string registry = battleLog.back();

		float percentage = ((float)numberOfWounds/(float)numberOfHits) * 100.f;

		registry += TAB_SPACE;
		registry += " - ";
		registry += NumberToString(numberOfWounds);
		registry += " (";
		registry += NumberToString(percentage);
		registry += "%) hits caused a wound.";
		registry += "\n";

		battleLog[currentLog] = registry;
	}

	void BattleForceManager::BattleLog_SquadShoots_NumberOfSaves        (int const &numberOfWounds, int const &numberOfSaves, bool const &negateArmorSave)
	{
		std::string registry = battleLog.back();

		registry += TAB_SPACE;
		registry += " - ";

		if( negateArmorSave )
		{
			registry += "Shooters' armor piercing value negates target's armor saves.";
		}
		else
		{
			float percentage = ((float)numberOfSaves/(float)numberOfWounds) * 100.f;
			
			registry += NumberToString(numberOfSaves);
			registry += " (";
			registry += NumberToString(percentage);
			registry += "%) of these wounds have been ignored due to armor saves.";
		}

		registry += "\n";

		battleLog[currentLog] = registry;
	}

	void BattleForceManager::BattleLog_SquadShoots_Result               (int const &targetCount, int const &numberOfDead, bool const &instantKill)
	{
		std::string registry = battleLog.back();

		float percentage = ((float)numberOfDead/(float)targetCount) * 100.f;

		registry += TAB_SPACE;
		registry += "Result: ";
		registry += NumberToString(numberOfDead);
		registry += " (";
		registry += NumberToString(percentage);
		registry += "%) targets have been eliminated";
		
		if( instantKill )
		{
			registry += " with Instant Kill rule";
		}

		registry += ".";
		registry += "\n";

		battleLog[currentLog] = registry;

		RefreshBattleLog_EditControl();
	}
//
	void BattleForceManager::BattleLog_SquadAssaults_SquadnAndTargetInfo    (std::string const &squadName, int const &attackerCount, std::string const &targetName, int const &targetCount)
	{
		std::string registry = battleLog.back();

		registry += TAB_SPACE;
		registry += squadName  + std::string(" (") + NumberToString(attackerCount) + std::string(")");
		registry += " attacks ";
		registry += targetName + std::string(" (") + NumberToString(targetCount) + std::string(")");
		registry += " in melee!";
		registry += "\n\n";

		battleLog[currentLog] = registry;
	}

	void BattleForceManager::BattleLog_SquadRetaliates_SquadnAndTargetInfo  (std::string const &squadName, int const &attackerCount, std::string const &targetName, int const &targetCount)
	{
		std::string registry = battleLog.back();

		registry += TAB_SPACE;
		registry += squadName  + std::string(" (") + NumberToString(attackerCount) + std::string(")");
		registry += " survived, to retaliate against ";
		registry += targetName + std::string(" (") + NumberToString(targetCount) + std::string(")");
		registry += "!";
		registry += "\n\n";

		battleLog[currentLog] = registry;
	}
//
	void BattleForceManager::BattleLog_SquadAssaults_NumberOfAttacksAndHits (int const &numberOfAttacks, int const &numberOfHits, int const &savesNegatedBySixesOnHitWithRending)
	{
		std::string registry = battleLog.back();

		float percentage = ((float)numberOfHits/(float)numberOfAttacks) * 100.f;

		registry += TAB_SPACE;
		registry += " - ";
		registry += NumberToString(numberOfAttacks);
		registry += " attacks were were made, of which ";
		registry += NumberToString(numberOfHits);
		registry += " (";
		registry += NumberToString(percentage);
		registry += "%) hit the opponnent.";
		registry += "\n";

		if( savesNegatedBySixesOnHitWithRending )
		{
			registry += TAB_SPACE;
			registry += "  ";
			registry += "Also got ";
			registry += NumberToString(savesNegatedBySixesOnHitWithRending);
			registry += " sixes with Rending Weapon!";
			registry += "\n";
		}

		battleLog[currentLog] = registry;
	}

	void BattleForceManager::BattleLog_SquadAssaults_NumberOfWounds         (int const &numberOfHits, int const &numberOfWounds, bool const &rendingAttack)
	{
		std::string registry = battleLog.back();

		float percentage = ((float)numberOfWounds/(float)numberOfHits) * 100.f;

		registry += TAB_SPACE;
		registry += " - ";

		if( rendingAttack )
		{
			registry += "Automatically caused ";
			registry += NumberToString(numberOfWounds);
			registry += " (";
			registry += NumberToString(percentage);
			registry += "%) due do rending weapons.";
		}
		else
		{
			registry += NumberToString(numberOfWounds);
			registry += " (";
			registry += NumberToString(percentage);
			registry += "%) hits caused a wound.";
		}

		registry += "\n";

		battleLog[currentLog] = registry;
	}

	void BattleForceManager::BattleLog_SquadAssaults_NumberOfSaves          (int const &numberOfWounds, int const &numberOfSaves, bool const &ignoreArmorSave, int const &savesNegatedBySixesOnHitWithRending)
	{
		std::string registry = battleLog.back();

		registry += TAB_SPACE;
		registry += " - ";

		if( ignoreArmorSave )
		{
			registry += "Armor saves ignored by power weapons.";
			registry += "\n";
		}
		else
		{
			if( numberOfSaves && savesNegatedBySixesOnHitWithRending )
			{
				float percentage = (((float)numberOfSaves+(float)savesNegatedBySixesOnHitWithRending)/(float)numberOfWounds) * 100.f;
			
				registry += NumberToString(numberOfSaves);
				registry += " (";
				registry += NumberToString(percentage);
				registry += "%) successful armor saves.";
				registry += "\n";

				registry += TAB_SPACE;
				registry += NumberToString(savesNegatedBySixesOnHitWithRending);
				registry += " of saves have been negated, ";
				registry += "\n";

				registry += TAB_SPACE;
				registry += "by rolling sixes on hits with rending weapons.";
				registry += "\n";
			}
			else
			{
				float percentage = ((float)numberOfSaves/(float)numberOfWounds) * 100.f;
			
				registry += NumberToString(numberOfSaves);
				registry += " (";
				registry += NumberToString(percentage);
				registry += "%) of these wounds have been ignored due to armor saves.";
				registry += "\n";
			}
		}


		battleLog[currentLog] = registry;
	}

	void BattleForceManager::BattleLog_SquadAssaults_Result                 (int const &targetCount, int const &numberOfDead, bool const &instantKill)
	{
		std::string registry = battleLog.back();

		float percentage = ((float)numberOfDead/(float)targetCount) * 100.f;

		registry += TAB_SPACE;
		registry += "Result: ";
		registry += NumberToString(numberOfDead);
		registry += " (";
		registry += NumberToString(percentage);
		registry += "%) opponnents have been slain";
		
		if( instantKill )
		{
			registry += " with Instant Kill rule";
		}

		registry += ".";
		registry += "\n\n";

		battleLog[currentLog] = registry;

		RefreshBattleLog_EditControl();
	}
//
//
	void BattleForceManager::RefreshBattleLog_EditControl ()
	{
		Edit *battleLogEdit = reinterpret_cast <Edit*> ( GUI::Singleton().Controls.GetControl(SIMULATION_GUI_BATTLE_LOG) );

		battleLogEdit->SetText( battleLog[currentLog] );
	}
//
/*****************************************************/

	void BattleForceManager::ClearCurrentTarget ()
	{
		if( currentTarget )
		{
			currentTarget->GetCurrentMini()->SetDrawingSelection(false);
		}

		currentTarget = NULL;
		RefreshInfoBoxes();
	}

	void BattleForceManager::SetCurrentTarget   (Squad *target)
	{
		assert( target != NULL );

		currentTarget = target;
		HiglightCurrentTargets();

		RefreshInfoBoxes();
	}
//
		void BattleForceManager::HiglightCurrentTargets ()
		{
			Mini *currentMini = currentTarget->GetCurrentMini();
			assert( currentMini != NULL );

			currentMini->SetDrawingSelection(true);
		}
//
/*****************************************************/

	void BattleForceManager::InitiateMovement ()
	{
		assert( movementPhase );
		
		if( movementInitiated )
			return;
		

		if( currentSelection != NULL && 
		    currentSelection->HowManyMembersCanMove() )
		{
			movementInitiated = true;

			CreateGhostSquad(currentSelection);
			StopDrawingActivityOnCurrentSelection();
		}
		else
			MessageBox(NULL, "First select a squad, that can move!", "InititateMovement() - error.", MB_OK); 
	}
//
	void BattleForceManager::CancelMovement   ()
	{
		assert( movementPhase );

		if( movementInitiated )
		{
			assert( currentSelection != NULL && ghostSquad != NULL );

			movementInitiated = false;

			RemoveGhostSquad();
			StartDrawingActivityOnCurrentSelection();
		}
	}
//
	void BattleForceManager::PerformMovement ()
	{
		assert( movementPhase );

		if( movementInitiated )
		{
			assert( currentSelection != NULL && ghostSquad != NULL );

			if( ghostSquad->SquadLostIntegrity() )
			{
				MessageBox(NULL, "Moved squad doesn't have integrity!", "PerformMovement() - error.", MB_OK);
				return;
			}
			else
			{
				StopDrawingPossibleTargets();
				StopDrawingMovementRangeOfPreviousGhost();
				MoveSelectedSquadToGhostPositions ();

				movementInitiated = false;
				RemoveGhostSquad();
			}
		}
	}
//
		void BattleForceManager::MoveSelectedSquadToGhostPositions ()
		{
			for(int i = 0; i < currentSelection->GetOriginalSquadSize(); i++)
			{
				Mini *mini  = currentSelection->GetMini(i);
				Mini *ghost = ghostSquad->GetMini(i);

				mini->SetPosition( ghost->GetPosition() );
				mini->SetRotation( ghost->GetRotation() );

				mini->SetRunningBonus(0);
				mini->SetDrawingActivity(false);
				mini->SetFlag_Move();
			}
		}
//
/*****************************************************/

	void BattleForceManager::InitiateRunOnSelectedSquad ()
	{
		if( movementPhase && currentSelection != NULL )
		{
			if( SelectedSquadCantRun() )
			{
				InformThatSelectedSquadCantRun();
				return;
			}

			if( ConfirmRunInitiation() )
			{
				int extraInches = currentSelection->InitiateRun();

				InformAboutExtraInchesThatSelectedSquadReceived(extraInches);
				BattleLog_SquadIsRunning(currentSelection->GetSquadName(), extraInches);
			}
		}
	}
//
		bool BattleForceManager::SelectedSquadCantRun ()
		{
			assert( currentSelection != NULL );

			if( currentSelection->SquadIsRunning() )
			{
				return true;
			}
			else
				return false;
		}

		void BattleForceManager::InformThatSelectedSquadCantRun ()
		{
			MessageBox(NULL, "Selected squad can't run!", "Run initiation - error.", MB_OK);
		}
//
		bool BattleForceManager::ConfirmRunInitiation ()
		{
			int result = MessageBox(NULL, "Initiate run?", "Confirmation...", MB_YESNO);

			if( result == IDYES )
			{
				return true;
			}
			else
				return false;
		}

		void BattleForceManager::InformAboutExtraInchesThatSelectedSquadReceived (int const &extraInches)
		{
			std::string info;

			info  = currentSelection->GetSquadName();
			info += " received ";
			info += NumberToString(extraInches);
			info += " extra inches.";

			MessageBox(NULL, info.c_str(), "Run initiation.", MB_OK);
		}
//
/*****************************************************/

	void BattleForceManager::ShootAtTarget ()
	{
		assert( shootingPhase );
		
		if( currentSelection && 
		   (currentSelection->HowManyMembersCanShoot() ||
		    currentSelection->HowManyMembersCanShootRapidly()) )
		{
			if( currentTarget )//&& currentTarget->IsNotTiedInMelee() )
			{
				PerformShooting();
				RefreshInfoBoxes();
			}
			else
				InformThatThereIsNoTargetSelectedThatCanBeShotAt();
		}
		else
			InformThatThereIsNoSquadSelectedThatCanShoot();
	}
//
		void BattleForceManager::PerformShooting ()
		{
			int shooterCount = Calculate_ShooterCount();
			int targetCount  = Calculate_TargetCount();
  
			int  numberOfShots   = Calculate_NumberOfShots(shooterCount);
			int  rollToHit       = Calculate_RollToHit_WithShot();
			int  rollToWound     = Calculate_RollToWound_WithShot();

			bool negateArmorSave = Calculate_ArmorPiercing();
			bool instantKill     = Calculate_InstantKill_WithShot();


			int numberOfHits   = Calculate_Hits(numberOfShots, rollToHit);
			int numberOfWounds = Calculate_Wounds(numberOfHits, rollToWound);
			int numberOfSaves;

			if( negateArmorSave )
			{
				numberOfSaves = 0;
			}
			else
				numberOfSaves = Calculate_Saves(numberOfWounds, currentTarget->GetMini(0)->GetSave());

			int numberOfInflictedWounds = numberOfWounds - numberOfSaves;

			int numberOfDead = InflictWounds(numberOfInflictedWounds, targetCount, instantKill);



			BattleLog_SquadShoots_SquadnAndTargetInfo(currentSelection->GetSquadName(), shooterCount, currentTarget->GetSquadName(), targetCount);
			BattleLog_SquadShoots_NumberOfShotsAndHits(numberOfShots, numberOfHits);

			if( numberOfHits > 0 )
			{
				BattleLog_SquadShoots_NumberOfWounds(numberOfHits, numberOfWounds);
			}

			if( numberOfWounds > 0 )
			{
				BattleLog_SquadShoots_NumberOfSaves(numberOfWounds, numberOfSaves, negateArmorSave);
			}

			BattleLog_SquadShoots_Result(targetCount, numberOfDead, instantKill);



			currentSelection->SetFlag_Shoot();
			currentSelection->ResetDrawingFlags();
			currentSelection->AddKills(numberOfDead);

			StopDrawingPossibleTargets();
		}
//
		int  BattleForceManager::Calculate_ShooterCount         ()
		{
			int shooterCount = currentSelection->HowManyMembersCanShoot();

			if( shooterCount == 0 )
			{
				currentSelection->HowManyMembersCanShootRapidly();
			}

			return shooterCount;
		}

		int  BattleForceManager::Calculate_TargetCount          ()
		{
			return currentTarget->GetMembersInAction();
		}

		int  BattleForceManager::Calculate_NumberOfShots        (int const &shooterCount)
		{
			int numberOfShots = shooterCount * currentSelection->GetMini(0)->GetRateOfFire();

			if( currentSelection->HowManyMembersCanShootRapidly() && 
				currentSelection->TargetInRapidFireRange(currentTarget) )
			{
				numberOfShots *= 2;
			}

			return numberOfShots;
		}

		int  BattleForceManager::Calculate_RollToHit_WithShot   ()
		{
			int rollToHit = 7 - currentSelection->GetMini(0)->GetBallisticSkill();

			if( rollToHit < 2 )
			{
				rollToHit = 2;
			}

			return rollToHit;
		}

		int  BattleForceManager::Calculate_RollToWound_WithShot ()
		{
			int ShooterStrength = currentSelection->GetMini(0)->GetFirearmStrength();
			int TargetToughness  = currentTarget->GetMini(0)->GetToughness();

			int difference = ShooterStrength - TargetToughness;
			int rollToWound;

			
			if( difference >= 2 )
			{
				rollToWound = 2;
			}
			else if( difference == 1 )
			{
				rollToWound = 3;
			}
			else if( difference == 0 )
			{
				rollToWound = 4;
			}
			else if( difference == -1 )
			{
				rollToWound = 5;
			}
			else if( difference == -2 || difference == -3 )
			{
				rollToWound = 6;
			}
			else if( difference <= -4 )
			{
				rollToWound = 0;
			}
			

			return rollToWound;
		}
//
		bool BattleForceManager::Calculate_ArmorPiercing        ()
		{
			int armorPiercingValue = currentSelection->GetMini(0)->GetArmorPiercing();
			int armorSave          = currentTarget->GetMini(0)->GetSave();

			if( armorPiercingValue <= armorSave )
			{
				return true;
			}
			else
				return false;
		}

		bool BattleForceManager::Calculate_InstantKill_WithShot ()
		{
			int attackerStrength   = currentSelection->GetMini(0)->GetFirearmStrength();
			int opponentsToughness = currentTarget->GetMini(0)->GetToughness();

			if( attackerStrength >= opponentsToughness*2 )
			{
				return true;
			}
			else
				return false;
		}
//
		void BattleForceManager::InformThatThereIsNoSquadSelectedThatCanShoot     ()
		{
			MessageBox(NULL, "First select some squad, that can shoot!", "Shoot at target - error.", MB_OK);
		}

		void BattleForceManager::InformThatThereIsNoTargetSelectedThatCanBeShotAt ()
		{
			MessageBox(NULL, "First select some target, to can be shot at!", "Shoot at target - error.", MB_OK);
		}
//
/*****************************************************/

	void BattleForceManager::InitiateAssault ()
	{
		assert( assaultPhase );

		if( assaultInitiated )
			return;
	

		if( currentSelection != NULL                     && 
		    currentSelection->HowManyMembersCanAssault() )
		{
			if(	currentTarget )
			{
				assaultInitiated = true;

				CreateGhostSquad(currentSelection);
				StopDrawingActivityOnCurrentSelection();
			}
			else
				MessageBox(NULL, "First select a target in range of a selected squad!", "InititateMovement() - error.", MB_OK); 
		}
		else
			MessageBox(NULL, "First select a squad, that can assault!", "InititateMovement() - error.", MB_OK); 
	}

	void BattleForceManager::CancelAssault   ()
	{
		assert( assaultPhase );

		if( assaultInitiated )
		{
			assert( currentSelection != NULL && ghostSquad != NULL && currentTarget != NULL );

			assaultInitiated = false;

			RemoveGhostSquad();

			StopDrawingPossibleTargets();
			ScanBattleForcesForPossibleTargets(currentSelection->GetCurrentMini());

			StartDrawingActivityOnCurrentSelection();
		}
	}
//
	void BattleForceManager::AssaultTarget ()
	{
		assert( assaultPhase );

		if( assaultInitiated )
		{
			assert( currentSelection != NULL && ghostSquad != NULL && currentTarget != NULL );

			if( ghostSquad->SquadLostIntegrity() )
			{
				InformThatThereIsNoSquadSelectedThatCanAssault();	
			}
			else if( HighlightPossibleAssaultTargets() == 0 )
			{
				InformThatThereIsNoTargetSelectedThatCanBeAssaulted();
			}
			else
			{
				assaultInitiated = false;
		
				MoveSelectedSquadToGhostPositions ();
				
				PerformAssault();
				RefreshInfoBoxes();

				StopDrawingPossibleTargets();
				StopDrawingAssaultRangeOfPreviousGhost();

				RemoveGhostSquad();

				ClearCurrentSelection();
				ClearCurrentTarget();
			}
		}
	}
//
		void BattleForceManager::PerformAssault ()
		{
			int assaulterInitiative = Calculate_AssaulterInitiative();
			int targetInitiative    = Calculate_TargetInitiative();

			bool assaulterIsCharging  = Calculate_AssaulterIsCharging();



			BattleLog_InsertHeaderInfo();

			if(  assaulterInitiative >= targetInitiative )
			{
				ExecuteAssaultAndInsertBattleLogData(assaulterIsCharging, false);
				SwapCurrentSelectionWithCurrentTarget();

				ExecuteAssaultAndInsertBattleLogData(false, true);
				SwapCurrentSelectionWithCurrentTarget();
			}
			else if( assaulterInitiative < targetInitiative )
			{
				SwapCurrentSelectionWithCurrentTarget();
				ExecuteAssaultAndInsertBattleLogData(false, false);

				SwapCurrentSelectionWithCurrentTarget();
				ExecuteAssaultAndInsertBattleLogData(assaulterIsCharging, true);
			}


			SetNewMeleeOpponentsIfCurrentAreNULL();
			ClearCurrentMeleeOpponentsIfWipedOut();
			currentSelection->SetFlag_Assault();
		}
//
		bool BattleForceManager::Calculate_AssaulterIsCharging    ()
		{
			if( currentSelection->IsTiedInMelee() )
			{
				return false;
			}
			else
				return true;
		}
//
		void BattleForceManager::ExecuteAssaultAndInsertBattleLogData  (bool const &assaulterIsCharging, bool const &retaliation)
		{
			if( currentSelection->IsWipedOut() )
				return;


			int assaulterCount = HowManySquadMembersReachEnemyForMelee(currentSelection, currentTarget);
			int targetCount    = HowManySquadMembersReachEnemyForMelee(currentTarget, currentSelection);


			int numberOfAttacks  = Calculate_NumberOfAttacks(assaulterCount);

			if( assaulterIsCharging )
			{
				numberOfAttacks += assaulterCount;
			}

			int rollToHit        = Calculate_RollToHit_WithAttack();
			int rollToWound      = Calculate_RollToWound_WithAttack();

			bool rendingAttack   = Calculate_RendingAttack();
			bool ignoreArmorSave = Calculate_IgnoreArmorSave();
			bool instantKill     = Calculate_InstantKill_WithAttack();
								   
			int numberOfHits   = 0;
			int numberOfWounds = 0;
			int numberOfSaves  = 0;

			int savesNegatedBySixesOnHitWithRending = 0;

			int inflictedWounds = 0;
			

			if( rendingAttack )
			{
				assert( ignoreArmorSave == false );

				srand(time(NULL));

				for(int i = 0; i < numberOfAttacks; i++)
				{
					int roll = (rand()%6) + 1;

					if( roll >= rollToHit )
					{
						numberOfHits++;
					}

					if( roll == 6 )
					{
						savesNegatedBySixesOnHitWithRending++;
					}
				}

				numberOfWounds = numberOfHits;
				numberOfSaves = Calculate_Saves(numberOfWounds, currentTarget->GetMini(0)->GetSave());

				if( numberOfSaves == 0 )
				{
					inflictedWounds = numberOfWounds;
				}
				else
				{
					if( savesNegatedBySixesOnHitWithRending > numberOfSaves )
					{
						savesNegatedBySixesOnHitWithRending = numberOfSaves;
					}

					int decreasedSaves = numberOfSaves - savesNegatedBySixesOnHitWithRending;

					inflictedWounds = numberOfWounds - decreasedSaves;
				}
			}
			else
			{
				assert( rendingAttack == false );

				numberOfHits   = Calculate_Hits(numberOfAttacks, rollToHit);
				numberOfWounds = Calculate_Wounds(numberOfHits, rollToWound);
			
				if( ignoreArmorSave )
				{
					numberOfSaves = 0;
				}
				else
					numberOfSaves = Calculate_Saves(numberOfWounds, currentTarget->GetMini(0)->GetSave());

				inflictedWounds = numberOfWounds - numberOfSaves;
			}


			int numberOfDead = InflictWounds(inflictedWounds, targetCount, instantKill);



			if( retaliation )
			{
				BattleLog_SquadRetaliates_SquadnAndTargetInfo(currentSelection->GetSquadName(), assaulterCount, currentTarget->GetSquadName(), targetCount);
			}
			else
				BattleLog_SquadAssaults_SquadnAndTargetInfo(currentSelection->GetSquadName(), assaulterCount, currentTarget->GetSquadName(), targetCount);


			BattleLog_SquadAssaults_NumberOfAttacksAndHits(numberOfAttacks, numberOfHits, savesNegatedBySixesOnHitWithRending);

			if( numberOfHits > 0 )
			{
				BattleLog_SquadAssaults_NumberOfWounds(numberOfHits, numberOfWounds, rendingAttack);
			}

			if( numberOfWounds > 0 )
			{
				BattleLog_SquadAssaults_NumberOfSaves(numberOfWounds, numberOfSaves, ignoreArmorSave, savesNegatedBySixesOnHitWithRending);
			}

			BattleLog_SquadAssaults_Result(targetCount, numberOfDead, instantKill);



			currentSelection->ResetDrawingFlags();
			currentSelection->AddKills(numberOfDead);
		}

		void BattleForceManager::SwapCurrentSelectionWithCurrentTarget ()
		{
			Squad *swapper = currentSelection;

			currentSelection = currentTarget;
			currentTarget = swapper;
		}
//
		int  BattleForceManager::Calculate_AssaulterInitiative    ()
		{
			Mini *assaulter = currentSelection->GetMini(0);

			return assaulter->GetInitiative();
		}

		int  BattleForceManager::Calculate_TargetInitiative       ()
		{
			Mini *target = currentTarget->GetMini(0);

			return target->GetInitiative();
		}

		int  BattleForceManager::Calculate_NumberOfAttacks        (int const &attackerCount)
		{
			Mini *attacker = currentSelection->GetMini(0);


			int numberOfAttacks = attacker->GetAttacks();

			if( attacker->HasPistol() )
			{
				numberOfAttacks++;
			}

			numberOfAttacks *= attackerCount;


			return numberOfAttacks;
		}

		int  BattleForceManager::Calculate_RollToHit_WithAttack   ()
		{
			int assaulterWeaponSkill = currentSelection->GetMini(0)->GetWeaponSkill();
			int opponnentWeaponSkill = currentTarget->GetMini(0)->GetWeaponSkill();

			int weaponSkillDifference = assaulterWeaponSkill - opponnentWeaponSkill;
			

			int rollToHit;

			if( weaponSkillDifference < 0 )
			{
				rollToHit = 3;
			}
			else if( weaponSkillDifference >= 0 && weaponSkillDifference <= assaulterWeaponSkill )
			{
				rollToHit = 4;
			}
			else
				rollToHit = 5;


			return rollToHit;
		}

		int  BattleForceManager::Calculate_RollToWound_WithAttack ()
		{
			int attackerStrength   = currentSelection->GetMini(0)->GetWeaponStrength();
			int opponnentToughness = currentTarget->GetMini(0)->GetToughness();

			if( currentSelection->GetMini(0)->HasPowerFist() )
			{
				attackerStrength *= 2.f;
			}

			int difference = attackerStrength - opponnentToughness;



			int rollToWound;

			
			if( difference >= 2 )
			{
				rollToWound = 2;
			}
			else if( difference == 1 )
			{
				rollToWound = 3;
			}
			else if( difference == 0 )
			{
				rollToWound = 4;
			}
			else if( difference == -1 )
			{
				rollToWound = 5;
			}
			else if( difference == -2 || difference == -3 )
			{
				rollToWound = 6;
			}
			else if( difference <= -4 )
			{
				rollToWound = 0;
			}
			

			return rollToWound;
		}
//
		bool BattleForceManager::Calculate_RendingAttack          ()
		{
			if( currentSelection->GetMini(0)->HasRendingWeapon() )
			{
				return true;
			}
			else
				return false;
		}

		bool BattleForceManager::Calculate_IgnoreArmorSave        ()
		{
			if( currentSelection->GetMini(0)->HasPowerWeapon() || currentSelection->GetMini(0)->HasPowerFist() )
			{
				return true;
			}
			else
				return false;
		}

		bool BattleForceManager::Calculate_InstantKill_WithAttack ()
		{
			int attackerStrength   = currentSelection->GetMini(0)->GetWeaponStrength();
			int opponentsToughness = currentTarget->GetMini(0)->GetToughness();

			if( currentSelection->GetMini(0)->HasPowerFist() )
			{
				attackerStrength *= 2;
			}

			if( attackerStrength >= opponentsToughness*2 )
			{
				return true;
			}
			else
				return false;
		}
//
		void BattleForceManager::SetNewMeleeOpponentsIfCurrentAreNULL ()
		{
			if( currentSelection->GetMeleeOpponnent() == NULL )
			{
				currentSelection->SetMeleeOpponnent(currentTarget);
			}

			if( currentTarget->GetMeleeOpponnent() == NULL )
			{
				currentTarget->SetMeleeOpponnent(currentSelection);
			}
		}

		void BattleForceManager::ClearCurrentMeleeOpponentsIfWipedOut ()
		{
			if( currentTarget->IsWipedOut() )
			{
				currentSelection->SetMeleeOpponnent(NULL);
			}
			else if( currentSelection->GetMeleeOpponnent() == currentSelection &&
				     currentSelection->IsWipedOut() )
			{
				currentTarget->SetMeleeOpponnent(NULL);
			}
		}
//
//
		void BattleForceManager::InformThatThereIsNoSquadSelectedThatCanAssault      ()
		{
			MessageBox(NULL, "Assaulting squad needs to remain integrity!", "Assault target - error.", MB_OK);
		}

		void BattleForceManager::InformThatThereIsNoTargetSelectedThatCanBeAssaulted ()
		{
			MessageBox(NULL, "There need to be at least two opposing minis in contact range!", "Assault target - error.", MB_OK);
		}
//
//
	int  BattleForceManager::HowManySquadMembersReachEnemyForMelee (Squad *squad, Squad *enemies)
	{
		int membersNearEnemies = 0;

		for(int i = 0; i < squad->GetOriginalSquadSize(); i++)
		{
			Mini *member = currentSelection->GetMini(i);

			if( member->IsInAction() && MemberIsNearEnemies(member, enemies) )
			{
				membersNearEnemies++;
			}
		}

		return membersNearEnemies;
	}
//
		bool BattleForceManager::MemberIsNearEnemies     (Mini *member, Squad *enemies)
		{
			Vector3 memberPosition = member->GetPosition();
			float   memberRadius   = member->GetRadius();

			for(int i = 0; i < enemies->GetOriginalSquadSize(); i++)
			{
				Mini *enemy = enemies->GetMini(i);

				float distance = enemy->GetDistance(memberPosition) - enemy->GetRadius() - memberRadius;

				if( distance <= 2.f )
				{
					return true;
				}
			}

			return false;
		}
//
	int  BattleForceManager::HighlightPossibleAssaultTargets    ()
	{
		assert( currentSelection != NULL && ghostSquad != NULL && currentTarget != NULL );

		int possibleTargets = 0;

		for(int i = 0; i < currentTarget->GetOriginalSquadSize(); i++)
		{
			Mini *target = currentTarget->GetMini(i);
			
			target->SetDrawingAssaultTarget(true);

			if( target->IsInAction() && TargetIsInAssaultReachOfGhostSquad(target) )
			{
				target->SetDrawingSelection(true);
				possibleTargets++;
			}
		}

		return possibleTargets;
	}

	bool BattleForceManager::TargetIsInAssaultReachOfGhostSquad (Mini *target)
	{
		bool targetIsNear = false;

		Vector3 targetsPosition = target->GetPosition();
		float targetsRadius = target->GetRadius();

		for(int i = 0; i < ghostSquad->GetOriginalSquadSize(); i++)
		{
			Mini *ghost = ghostSquad->GetMini(i);

			if( ghost->IsInAction() )
			{
				float distance = ghost->GetDistance(targetsPosition) - ghost->GetRadius() - targetsRadius;

				if( distance <= 2.f )
				{
					targetIsNear = true;
					break;
				}
			}
		}

		return targetIsNear;
	}
//
/*****************************************************/

	int BattleForceManager::Calculate_Hits   (int const &numberOfAttacks, int const &rollToHit)
	{
		srand(time(NULL));

		int numberOfHits = 0;

		for(int i = 0; i < numberOfAttacks; i++)
		{
			int roll = (rand()%6)+1;

			if( roll >= rollToHit )
			{
				numberOfHits++;
			}
		}

		return numberOfHits;
	}

	int BattleForceManager::Calculate_Wounds (int const &numberOfHits, int const &rollToWound)
	{
		srand(time(NULL));

		int numberOfWounds = 0;

		if( rollToWound == 0 )
		{
			return numberOfWounds;
		}


		for(int i = 0; i < numberOfHits; i++)
		{
			int roll = (rand()%6)+1;

			if( roll >= rollToWound )
			{
				numberOfWounds++;
			}
		}

		return numberOfWounds;
	}

	int BattleForceManager::Calculate_Saves  (int const &numberOfWounds, int const &armorSave)
	{
		srand(time(NULL));

		int numberOfSaves = 0;

		for(int i = 0; i < numberOfWounds; i++)
		{
			int roll = (rand()%6)+1;

			if( roll >= armorSave )
			{
				numberOfSaves++;
			}
		}

		return numberOfSaves;
	}
//
	int BattleForceManager::InflictWounds (int const &numberOfInflictedWounds, int const &numberOfTargets, bool const &instantKill)
	{
		int inflictedWounds = numberOfInflictedWounds;
		int numberOfDead = 0;


		while( inflictedWounds )
		{
			for(int i = 0; i < currentTarget->GetOriginalSquadSize(); i++)
			{
				Mini *target = currentTarget->GetMini(i);

				if( target->IsInAction() && inflictedWounds )
				{
					if( instantKill )
					{
						target->Kill();
					}
					else
						target->InflictWound();

					inflictedWounds--;


					if( target->IsDead() )
					{
						numberOfDead++;
					}

					if( numberOfDead >= numberOfTargets )
					{
						numberOfDead = numberOfTargets;
						break;
					}
				}
			}


			if( currentTarget->GetMembersInAction() == 0 )
			{
				break;
			}
		}


		return numberOfDead;
	}
//
/*****************************************************/

	void BattleForceManager::StopDrawingActivityOnCurrentSelection  ()
	{
		assert( currentSelection != NULL );

		for(int i = 0; i < currentSelection->GetOriginalSquadSize(); i++)
		{
			currentSelection->GetMini(i)->SetDrawingActivity(false);
		}
	}

	void BattleForceManager::StartDrawingActivityOnCurrentSelection ()
	{
		assert( currentSelection != NULL );

		for(int i = 0; i < currentSelection->GetOriginalSquadSize(); i++)
		{
			currentSelection->GetMini(i)->SetDrawingActivity(true);
		}
	}
//
/*****************************************************/

	void BattleForceManager::RemoveGhostSquad ()
	{
		assert( ghostSquad != NULL );

		delete ghostSquad;
		ghostSquad = NULL;
	}

	void BattleForceManager::CreateGhostSquad (Squad *squad)
	{
		assert( ghostSquad == NULL && squad != NULL && squad->GetMembersInAction() > 0 );

		ghostSquad = new Squad("ghostSquad", squad->GetMini(0), squad->GetOriginalSquadSize());


		ghostSquad->ResetDrawingFlags();

		for(int i = 0; i < ghostSquad->GetOriginalSquadSize(); i++)
		{
			Mini *currentGhost = ghostSquad->GetMini(i);
			Mini *squadMember  = squad->GetMini(i);


			if( squadMember->IsDeployed() )
			{
				currentGhost->SetDeployment(true);
			}
			else
				currentGhost->SetDeployment(false);

			if( squadMember->IsAlive() )
			{
				currentGhost->SetDead(false);
			}
			else
				currentGhost->SetDead(true);
			

			currentGhost->SetPosition( squadMember->GetPosition() );
			currentGhost->SetRotation( squadMember->GetRotation() );

			currentGhost->SetColor(GHOST_COLOR);
			currentGhost->SetDrawingSquadSpacing(true);
		}
	}
//
	void BattleForceManager::StopDrawingMovementRangeOfPreviousGhost ()
	{
		assert( movementPhase );
		assert( currentSelection != NULL && ghostSquad != NULL );

		int ghostID = ghostSquad->GetCurrentMiniID();

		if( ghostID >= 0 )
		{
			ghostSquad->GetCurrentMini()->SetDrawingSelection(false);
			currentSelection->GetMini(ghostID)->SetDrawingMovementRange(false);
		}
	}

	void BattleForceManager::StopDrawingAssaultRangeOfPreviousGhost  ()
	{
		assert( assaultPhase );
		assert( currentSelection != NULL && ghostSquad != NULL );

		int ghostID = ghostSquad->GetCurrentMiniID();

		if( ghostID >= 0 )
		{
			ghostSquad->GetCurrentMini()->SetDrawingSelection(false);
			currentSelection->GetMini(ghostID)->SetDrawingAssaultRange(false);
		}
	}
//
	bool BattleForceManager::SelectGhost ()
	{
		assert( currentSelection != NULL && ghostSquad != NULL );

		if( ghostSquad->SquadGotClickedOn(cursor) )
		{
			ghostSquad->GetCurrentMini()->SetDrawingSelection(true);

			if( movementPhase )
			{
				DrawMovementRangeFromOriginator();
			}
			else if( assaultPhase )
			{
				DrawAssaultRangeFromOriginator();
			}

			return true;
		}
		else
			return false;
	}
//
		void BattleForceManager::DrawMovementRangeFromOriginator ()
		{
			currentSelection->GetMini( ghostSquad->GetCurrentMiniID() )->SetDrawingMovementRange(true);
		}

		void BattleForceManager::DrawAssaultRangeFromOriginator  ()
		{
			currentSelection->GetMini( ghostSquad->GetCurrentMiniID() )->SetDrawingAssaultRange(true);
		}
//
	void BattleForceManager::MoveGhost (Vector3 const &cursor)
	{
		assert( movementPhase || assaultPhase );
		assert( currentSelection != NULL && ghostSquad != NULL && ghostSquad->GetCurrentMini() != NULL );

	
		float maxDistance = GetMaxDistance(currentSelection->GetMini(0));	

		Mini *currentGhost    = ghostSquad->GetCurrentMini();
		Mini *ghostOriginator = currentSelection->GetMini( ghostSquad->GetCurrentMiniID() );


		Vector3 previousPosition = currentGhost->GetPosition();
		currentGhost->SetPosition(cursor);

		if( currentGhost->GetDistance( ghostOriginator->GetPosition() ) > maxDistance ||
			MiniIsInWrongPosition(currentGhost) )
		{
			currentGhost->SetPosition(previousPosition);
		}
	}
//
		float BattleForceManager::GetMaxDistance (Mini *mini)
		{
			float maxDistance;

			if( movementPhase )
			{
				maxDistance = mini->GetMovementSpeed();
			}
			else if( assaultPhase )
			{
				maxDistance = mini->GetAssaultRange();
			}

			return maxDistance;
		}
//
/*****************************************************/

	void BattleForceManager::StopDrawingPossibleTargets         ()
	{
		for(int i = 0; i < battleForces.size(); i++)
		{
			if( battleForces[i] == currentBattleForce )
			{
				continue;
			}

			battleForces[i]->ResetDrawingFlags();
		}
	}

	void BattleForceManager::ScanBattleForcesForPossibleTargets (Mini *selectedMini)
	{
		for(int i = 0; i < battleForces.size(); i++)
		{
			if( battleForces[i] == currentBattleForce )
			{
				continue;
			}

			ScanBattleForceForPossibleTargets(selectedMini, battleForces[i]);
		}
	}
//
		void BattleForceManager::ScanBattleForceForPossibleTargets (Mini *selectedMini, BattleForce *hostileBattleForce)
		{
			for(int i = 0; i < hostileBattleForce->GetSquadCount(); i++)
			{
				ScanSquadForPossibleTargets(selectedMini, hostileBattleForce->GetSquad(i));
			}
		}

		void BattleForceManager::ScanSquadForPossibleTargets       (Mini *selectedMini, Squad *hostileSquad)
		{
			if( hostileSquad->IsInAction() )
			{
				bool selectedMiniCanShoot        = selectedMini->CanShoot();
				bool selectedMiniCanShootRapidly = selectedMini->CanShootRapidly();
				bool selectedMiniCanAssault      = selectedMini->CanAssault();


				bool squadCanBeShotAt    = false;
				bool squadCanBeAssaulted = false;


				for(int i = 0; i < hostileSquad->GetOriginalSquadSize(); i++)
				{
					Mini *hostileMini = hostileSquad->GetMini(i);

					if( hostileMini->IsInAction() )
					{
						if( selectedMiniCanAssault && selectedMini->InRangeOf_Assault(hostileMini) )
						{
							squadCanBeAssaulted = true;
							squadCanBeShotAt = false;
							//break;
						}
						else if( selectedMiniCanShoot        && selectedMini->InRangeOf_Fire(hostileMini)      ||
								 selectedMiniCanShootRapidly && selectedMini->InRangeOf_RapidFire(hostileMini) &&
								 !hostileMini->IsTiedInMelee() )
						{
							squadCanBeShotAt = true;
							//break;
						}
					}
				}

				if( squadCanBeAssaulted )
				{
					DrawPossibleAssaultTargets(hostileSquad);
				}
				else if( squadCanBeShotAt )
				{
					DrawPossibleRangedTargets(hostileSquad);
				}
			}
		}
//
		void BattleForceManager::DrawPossibleRangedTargets  (Squad *hostileSquad)
		{
			for(int i = 0; i < hostileSquad->GetOriginalSquadSize(); i++)
			{
				Mini *mini = hostileSquad->GetMini(i);

				if( mini->IsInAction() )
				{
					mini->SetDrawingRangedTarget(true);
				}
			}
		}

		void BattleForceManager::DrawPossibleAssaultTargets (Squad *hostileSquad)
		{
			for(int i = 0; i < hostileSquad->GetOriginalSquadSize(); i++)
			{
				Mini *mini = hostileSquad->GetMini(i);

				if( mini->IsInAction() )
				{
					mini->SetDrawingAssaultTarget(true);
				}
			}
		}
//
/*****************************************************/

	bool BattleForceManager::MiniIsInWrongPosition (Mini *movedMini)
	{
		if( MiniGoesOverboard(movedMini)        || 
			MiniCollidesWithObstacle(movedMini) || 
			MiniCollidesWithWater(movedMini)    || 
			MiniCollidesWithOtherMini(movedMini) )
		{
			return true;
		}
		else
			return false;
	}
//
		bool BattleForceManager::MiniGoesOverboard         (Mini *movedMini)
		{
			Vector3 position = movedMini->GetPosition();
			Vector3 size     = movedMini->GetSize();

			if( position.x-size.x < 0 || position.x+size.x > mapWidth_X )
			{
				return true;
			}

			if( position.z+size.z > 0 || position.z-size.z < -mapWidth_Z )
			{
				return true;
			}

			return false;
		}

		bool BattleForceManager::MiniCollidesWithObstacle  (Mini *movedMini)
		{
			ObjectOperator &objectOperator = ObjectOperator::Singleton();

			Vector3 position = movedMini->GetPosition();
			Vector3 size     = movedMini->GetSize();

			float minX = position.x - size.x,
				  maxX = position.x + size.x,

				  minY = position.y - size.y,
				  maxY = position.y + size.y,

				  minZ = position.z - size.z,
				  maxZ = position.z + size.z;


			for(int i = 0; i < objectOperator.objectArray.size(); i++)
			{
				ObjectOperator::Object object = objectOperator.objectArray[i];
				Model *model = reinterpret_cast <Model*> ( object.GetObject() );

				if( object.GetType() == MODEL_NAME && model->IsObstacle )
				{
					for(float x = minX; x <= maxX; x += 1)
					for(float y = minY; y <= maxY; y += 1)
					for(float z = minZ; z <= maxZ; z += 1)
					{
						if( object.CoordinatesAreInsideObject(x, y, z) )
						{
							return true;
						}
					}
				}
			}

			return false;
		}

		bool BattleForceManager::MiniCollidesWithWater     (Mini *movedMini)
		{
			ObjectOperator &objectOperator = ObjectOperator::Singleton();

			Vector3 position = movedMini->GetPosition();
			Vector3 size     = movedMini->GetSize();

			float minX = position.x - size.x,
				  maxX = position.x + size.x,

				  minY = position.y - size.y,
				  maxY = position.y + size.y,

				  minZ = position.z - size.z,
				  maxZ = position.z + size.z;



			for(int i = 0; i < objectOperator.objectArray.size(); i++)
			{
				ObjectOperator::Object object = objectOperator.objectArray[i];
				liquidObject *liquid = reinterpret_cast <liquidObject*> ( object.GetObject() );

				if( object.GetType() == LIQUID_NAME )
				{
					float waterLevel = liquid->maxLevel + object.GetPosition().y;

					if( position.y < waterLevel )
					{
						for(float x = minX; x <= maxX; x += 1)
						for(float y = minY; y <= maxY; y += 1)
						for(float z = minZ; z <= maxZ; z += 1)
						{
							if( object.CoordinatesAreInsideObject(x, y, z) )
							{
								return true;
							}
						}
					}
				}
			}
		
			
			return false;
		}

		bool BattleForceManager::MiniCollidesWithOtherMini (Mini *movedMini)
		{
			float miniRadius = movedMini->GetRadius();

			for(int i = 0; i < battleForces.size(); i++)
			for(int j = 0; j < battleForces[i]->GetSquadCount(); j++)
			{
				Squad *squad = battleForces[i]->GetSquad(j);

				if( movedMini->GetColor() == GHOST_COLOR && squad == currentSelection )
				{
					continue;
				}

				if( squad->GetMembersInAction() &&
				    squad->SquadCollidesWithThisMini(movedMini) )
				{
					return true;
				}
				
			}
			
			return false;
		}
//
/*****************************************************/

	BattleForceManager::BattleForceManager  ():	currentBattleForce(NULL), 
												currentSelection(NULL), currentTarget(NULL),
												movementInitiated(false), assaultInitiated(false),
												ghostSquad(NULL),
												currentLog(0),
												heightMap(NULL), mapWidth_X(0), mapWidth_Z(0),
												deploymentPhase(false), 
												simulationPhase(false),
												movementPhase(false), shootingPhase(false), assaultPhase(false), 
												turnTicker(0),
												receivedInput(false), processingInput(false),
												exitSimulator(false)
	{}

	BattleForceManager::~BattleForceManager ()
	{
		Close();
	}