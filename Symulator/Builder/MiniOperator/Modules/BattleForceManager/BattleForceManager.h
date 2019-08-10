#pragma once

	#include "BattleForce//BattleForce.h"



	class BattleForceManager
	{
		public:

			static BattleForceManager& Singleton ()
			{
				static BattleForceManager s;
				return s;
			}
		//

	/*****************************************************/

			bool Init  ();
			void Close ();

			void Reset ();
				private:
				inline void ResetAllSquads ();
			public:

			bool Run ();
				private:
				inline void RenderGhostSquad   ();
				inline void RenderBattleForces ();

				inline void PreRendering  ();
				inline void PostRendering ();
			public:

	/*****************************************************/

			void AddBattleForce (std::string const &battleForceName)	throw(std::string);
				private:
				inline bool BattleForceWithThisNameAlreadyExists (std::string const &battleForceName);
				inline void InformAboutDuplicateBattleForceName  (std::string const &battleForceName)	throw(std::string);
				inline void AddNewBattleForce (std::string const &battleForceName);
			public:

			void RemoveBattleForce (std::string const &battleForceName)	throw(std::string);
				private:
				inline void InformAboutAttemptToRemoveNonexistentBattleForce (std::string const &battleForceName)	throw(std::string);
				inline void PerformBattleForceRemovement (int const &index);
				inline void CorrigateCurrentBattleForce  (int const &index);
			public:

			void RemoveAllBattleForces ();

			std::vector <std::string> GetBattleForceList  ();
			int                       GetBattleForceCount ();

			void         SetCurrentBattleForce (BattleForce *battleForce);
			void         SetCurrentBattleForce (int const &index);
			BattleForce* GetCurrentBattleForce ();

			void SetFirstBattleForce ();
			void SetNextBattleForce  ();

			BattleForce* GetBattleForce (int const &index);
			BattleForce* GetBattleForce (std::string const battleForceName);

				private:
				inline bool GivenIndexIsCorrect       (int const &index);
				inline void InformAboutIncorrectIndex (int const &index) throw (std::string);
				inline void InformAboutIncorrectName  (std::string const &battleForceName) throw (std::string);
			public:

	/*****************************************************/

			void StartDeploymentPhase () throw (std::string);
			void StartSimulationPhase () throw (std::string);

				private:
				inline bool AtLeastTwoOpposingSquadsAreSet      ();
				inline bool AtLeastTwoOpposingSquadsAreDeployed ();
				inline void InformAboutSquadsNotBeeingSet       () throw (std::string);

				inline void FilterOutBattleForcesWithNoSquadsDeployed ();
				inline void InformThatSimulationAlreadyStarted        () throw (std::string);
			public:

			void NextPhase () throw (std::string);
				private:
				inline bool VictoryCheck ();
				inline std::vector <std::string> ScanForBattleForcesInAction      ();
				inline void                      ProclaimVictoryAndResetSimulator (std::string const &victoriusBattleForce);

				inline bool MovementPhase_Check (); 
				inline void MovementPhase_SetUp ();
				inline void MovementPhase_SetActiveMinis ();

				inline bool ShootingPhase_Check ();
				inline void ShootingPhase_SetUp ();
				inline void ShootingPhase_SetActiveMinis ();

				inline bool AssaultPhase_Check ();
				inline void AssaultPhase_SetUp ();
				inline void AssaultPhase_SetActiveMinis  ();

				inline void ResetDrawingFlags ();
				inline void ResetStateFlags   ();

				inline void InformThatSimulationFlagsAreIncorrect () throw (std::string);
				inline void InformThatSimulationHasntStarted      () throw (std::string);
			public:

			void ResetPhaseFlags ();

	/*****************************************************/

			void SetNextTurn ();
				private:
				inline void RefreshCurrentTurnLabel ();
			public:
			int  GetCurrentTurn ();

	/*****************************************************/

			void SetPreviousLog ();
			void SetNextLog     ();

			std::string GetCurrentLog ();

			private:
			void ClearBattleLog ();
			public:

	/*****************************************************/

			void ClearCurrentSelection ();
			void SetCurrentSelection   (Squad *squad);

	/*****************************************************/

			void SetHeightMap (float *heightMap, int const &mapWidth_X, int  const &mapWidth_Z);

			float* GetHeightMap  ();
			int    GetMapWidth_X ();
			int    GetMapWidth_Z ();

			Vector3 CalculateHeight (int const &x, int const &y);

	/*****************************************************/

			void KeyDown             (int const &keyCode);
			void KeyUp               (int const &keyCode);

			void LeftMouseDown       ();
			void MiddleMouseDown     ();
			void RightMouseDown      ();

			void LeftMouseUp         ();
			void MiddleMouseUp       ();
			void RightMouseUp        ();

			void LeftMouseDblClick   ();
			void MiddleMouseDblClick ();
			void RightMouseDblClick  ();

			void Scroll              (int const &Info);

			void MoveCursor          (POINT const &mousePosition);


			private:
			Vector3 CalculateHeight (Vector3 const &openGLposition);
			public:

		private:

			std::vector <BattleForce*> battleForces;

			BattleForce *currentBattleForce;
			Squad *currentSelection;
			Squad *currentTarget;

			bool movementInitiated;
			bool assaultInitiated;
			Squad *ghostSquad;

			std::vector <std::string> battleLog;
			int currentLog;

			float *heightMap;
			int mapWidth_X, mapWidth_Z;
			

			bool deploymentPhase;
			bool simulationPhase;
			bool movementPhase, shootingPhase, assaultPhase;

			int turnTicker;


			Vector3 cursor, oldCursor;
			int key;
			int scrollInfo;

		// Flags that determine nature of the input.

			bool  keyDown, keyPressed, keyUp,

			      leftDown,     middleDown,     rightDown,
				  leftPressed,  middlePressed,  rightPressed,
				  leftUp,       middleUp,       rightUp,
				  leftDblClick, middleDblClick, rightDblClick;

			bool receivedInput;
			bool processingInput;

			bool exitSimulator;


	/******************************************************
			Input "digestion" functions.
	******************************************************/

			void ProcessInput ();
			
				void ProcessKey ();

				inline void ProcessKeyDown    ();
				inline void ProcessKeyPressed ();
				inline void ProcessKeyUp      ();

				void ProcessMouse ();

				inline void ProcessMouseDown ();

					inline void ProcessMouseLeftDown   ();	
					inline void ProcessMouseMiddleDown ();
					inline void ProcessMouseRightDown  ();

				inline void ProcessMousePressed ();

					inline void ProcessMouseLeftPressed   ();
					inline void ProcessMouseMiddlePressed ();
					inline void ProcessMouseRightPressed  ();

				inline void ProcessMouseUp ();

					inline void ProcessMouseLeftUp   ();
					inline void ProcessMouseMiddleUp ();
					inline void ProcessMouseRightUp  ();

				inline void ProcessMouseDblClick ();

					inline void ProcessMouseLeftDblClick   ();
					inline void ProcessMouseMiddleDblClick ();
					inline void ProcessMouseRightDblClick  ();

				void ProcessScroll ();

				inline bool CursorHasChangedPosition ();
				void        ProcessMouseMovement	 ();

			inline bool MouseKeysPressed			();
			inline bool NoKeysPressed				();
			inline void CheckInputValuesCorrectness ();
			inline void ClearInputValues            ();

	/******************************************************
		Phase specific input "digestion" functions.
	******************************************************/

			void DeploymentPhase_KeyDown ();
			void DeploymentPhase_LeftMouseDown ();
			void DeploymentPhase_ProcessMouseMovement ();

			void MovementPhase_KeyDown ();
			void MovementPhase_LeftMouseDown ();
			void MovementPhase_ProcessMouseMovement ();

			void ShootingPhase_KeyDown ();
			void ShootingPhase_LeftMouseDown ();

			void AssaultPhase_KeyDown ();
			void AssaultPhase_LeftMouseDown ();
			void AssaultPhase_ProcessMouseMovement ();

	/*****************************************************/

			void StopDrawingRangesOnPreviousMini ();
			bool SelectNewMiniAndDrawItsRanges   ();
				inline void StartDrawingMovementRangeOnSelectedMini ();
				inline void StartDrawingShootingRangeOnSelectedMini ();
				inline void StartDrawingAssaultRangeOnSelectedMini  ();

			bool SelectNewTarget ();

	/*****************************************************/

			void RefreshInfoBoxes ();
				inline void RefreshSelectionInfoBox ();
				inline void RefreshTargetInfoBox    ();

	/*****************************************************/

			void BattleLog_InsertHeaderInfo ();


			void BattleLog_SquadIsRunning (std::string const &squadName, int const &extraInches);

			void BattleLog_SquadShoots_SquadnAndTargetInfo  (std::string const &squadName, int const &shooterCount, std::string const &targetName, int const &targetCount);
			void BattleLog_SquadShoots_NumberOfShotsAndHits (int const &numberOfShots, int const &numberOfHits);
			void BattleLog_SquadShoots_NumberOfWounds       (int const &numberOfHits, int const &numberOfWounds);
			void BattleLog_SquadShoots_NumberOfSaves        (int const &numberOfWounds, int const &numberOfSaves, bool const &negateArmorSave);
			void BattleLog_SquadShoots_Result               (int const &targetCount, int const &numberOfDead, bool const &instantKill);

			void BattleLog_SquadAssaults_SquadnAndTargetInfo    (std::string const &squadName, int const &attackerCount, std::string const &targetName, int const &targetCount);
			void BattleLog_SquadRetaliates_SquadnAndTargetInfo  (std::string const &squadName, int const &attackerCount, std::string const &targetName, int const &targetCount);

			void BattleLog_SquadAssaults_NumberOfAttacksAndHits (int const &numberOfAttacks, int const &numberOfHits, int const &savesNegatedBySixesOnHitWithRending);
			void BattleLog_SquadAssaults_NumberOfWounds         (int const &numberOfHits, int const &numberOfWounds, bool const &rendingAttack);
			void BattleLog_SquadAssaults_NumberOfSaves          (int const &numberOfWounds, int const &numberOfSaves, bool const &ignoreArmorSave, int const &savesNegatedBySixesOnHitWithRending);
			void BattleLog_SquadAssaults_Result                 (int const &targetCount, int const &numberOfDead, bool const &instantKill);


			inline void RefreshBattleLog_EditControl ();


	/*****************************************************/

			void ClearCurrentTarget ();
			void SetCurrentTarget   (Squad *target);
				inline void HiglightCurrentTargets ();

	/*****************************************************/

			void InitiateMovement ();
			void CancelMovement   ();

			void PerformMovement ();
				inline void MoveSelectedSquadToGhostPositions ();

	/*****************************************************/

			void InitiateRunOnSelectedSquad ();
				inline bool SelectedSquadCantRun ();
				inline void InformThatSelectedSquadCantRun ();

				inline bool ConfirmRunInitiation ();
				inline void InformAboutExtraInchesThatSelectedSquadReceived (int const &extraInches) throw (std::string);

	/*****************************************************/

			void ShootAtTarget ();
				inline void PerformShooting ();

				inline int  Calculate_ShooterCount         ();
				inline int  Calculate_TargetCount          ();
				inline int  Calculate_NumberOfShots        (int const &shooterCount);
				inline int  Calculate_RollToHit_WithShot   ();
				inline int  Calculate_RollToWound_WithShot ();

				inline bool Calculate_ArmorPiercing        ();
				inline bool Calculate_InstantKill_WithShot ();

				inline void InformThatThereIsNoSquadSelectedThatCanShoot     ();
				inline void InformThatThereIsNoTargetSelectedThatCanBeShotAt ();
				
	/*****************************************************/

			void InitiateAssault ();
			void CancelAssault   ();

			void AssaultTarget ();
				inline void PerformAssault ();

				inline bool Calculate_AssaulterIsCharging    ();

				inline void ExecuteAssaultAndInsertBattleLogData  (bool const &assaulterIsCharging, bool const &retaliation);
				inline void SwapCurrentSelectionWithCurrentTarget ();

				inline int  Calculate_AssaulterInitiative    ();
				inline int  Calculate_TargetInitiative       ();
				inline int  Calculate_NumberOfAttacks        (int const &attackerCount);
				inline int  Calculate_RollToHit_WithAttack   ();
				inline int  Calculate_RollToWound_WithAttack ();

				inline bool Calculate_RendingAttack          ();
				inline bool Calculate_IgnoreArmorSave        ();
				inline bool Calculate_InstantKill_WithAttack ();

				inline void SetNewMeleeOpponentsIfCurrentAreNULL ();
				inline void ClearCurrentMeleeOpponentsIfWipedOut ();

				inline void InformThatThereIsNoSquadSelectedThatCanAssault      ();
				inline void InformThatThereIsNoTargetSelectedThatCanBeAssaulted ();

			int  HowManySquadMembersReachEnemyForMelee (Squad *squad, Squad *enemies);
				inline bool MemberIsNearEnemies     (Mini *member, Squad *enemies);

			int  HighlightPossibleAssaultTargets    ();
			bool TargetIsInAssaultReachOfGhostSquad (Mini *target);

	/*****************************************************/

			inline int Calculate_Hits   (int const &numberOfAttacks, int const &rollToHit);
			inline int Calculate_Wounds (int const &numberOfHits, int const &rollToWound);
			inline int Calculate_Saves  (int const &numberOfWounds, int const &armorSave);

			inline int InflictWounds (int const &numberOfInflictedWounds, int const &numberOfTargets, bool const &instantKill);

	/*****************************************************/

			void StopDrawingActivityOnCurrentSelection  ();
			void StartDrawingActivityOnCurrentSelection ();

	/*****************************************************/

			void RemoveGhostSquad ();
			void CreateGhostSquad (Squad *squad);

			void StopDrawingMovementRangeOfPreviousGhost ();
			void StopDrawingAssaultRangeOfPreviousGhost  ();

			bool SelectGhost ();
				inline void DrawMovementRangeFromOriginator ();
				inline void DrawAssaultRangeFromOriginator  ();

			void MoveGhost (Vector3 const &cursor);
				inline float GetMaxDistance (Mini *mini);

	/*****************************************************/

			void StopDrawingPossibleTargets         ();
			void ScanBattleForcesForPossibleTargets (Mini *selectedMini);

				inline void ScanBattleForceForPossibleTargets (Mini *selectedMini, BattleForce *hostileBattleForce);
				inline void ScanSquadForPossibleTargets       (Mini *selectedMini, Squad *hostileSquad);

				inline void DrawPossibleRangedTargets  (Squad *hostileSquad);
				inline void DrawPossibleAssaultTargets (Squad *hostileSquad);

	/*****************************************************/

			bool MiniIsInWrongPosition (Mini *movedMini);
				inline bool MiniGoesOverboard         (Mini *movedMini);
				inline bool MiniCollidesWithObstacle  (Mini *movedMini);
				inline bool MiniCollidesWithWater     (Mini *movedMini);
				inline bool MiniCollidesWithOtherMini (Mini *movedMini);

	/*****************************************************/


			BattleForceManager  ();
			~BattleForceManager ();
	};