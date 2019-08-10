#pragma once

	#include "..//..//..//MiniDataManager//Mini//Mini.h"


	class Squad
	{
		public:
			
			Squad  (std::string const &squadName, Mini *mini, int const &miniCount);
			~Squad ();

			void operator= (Squad &squad);

	/*****************************************************/

			void Render ();

	/*****************************************************/

			std::string GetSquadName ();
			std::string GetMiniName  ();

			int GetPointsValue ();

			int GetOriginalSquadSize    ();
			int GetDeployedMembersCount ();
			int GetLiveMembersCount     ();
			int GetMembersInAction      ();

	/*****************************************************/

			bool SquadCollidesWithThisMini (Mini *mini);

			bool SquadGotClickedOn (Vector3 const &cursor);

	/*****************************************************/

			Mini* GetMini (int const &index);
				
			Mini* GetCurrentMini   ();
			int   GetCurrentMiniID ();

			void SetCurrentMini (int const &index);

			void SetFirstMiniAsCurrent ();
			void SetNextMiniAsCurrent  ();

				private:
				inline bool GivenIndexIsCorrect       (int const &index);
				inline void InformAboutIncorrectIndex (int const &index) throw (std::string);
			public:

	/*****************************************************/

			void DeployCurrentMini (Vector3 const &position);
				private:
				inline void AdjustCurrentMinisRotation ();
			public:
			void Undeploy  ();

			void ReviveAll ();
			void KillAll   ();

	/*****************************************************/

			int  InitiateRun    ();
			bool SquadIsRunning ();

	/*****************************************************/

			bool TargetInShootingRange  (Squad *target);
			bool TargetInRapidFireRange (Squad *target);
			bool TargetInAssaultRange   (Squad *target);

	/*****************************************************/

			void   SetMeleeOpponnent (Squad *opponnent);
			Squad* GetMeleeOpponnent ();

			bool IsTiedInMelee    ();
			bool IsNotTiedInMelee ();

	/*****************************************************/

			bool IsDeployed    ();
			bool IsNotDeployed ();

			bool IsWipedOut ();
			bool IsAlive    ();

			bool IsInAction ();

			bool SquadLostIntegrity ();

	/*****************************************************/

			void SetFlag_Move    ();
			void SetFlag_Run     ();
			void SetFlag_Shoot   ();
			void SetFlag_Assault ();

			int HowManyMembersCanMove         ();
			int HowManyMembersCanShoot        ();
			int HowManyMembersCanShootRapidly ();
			int HowManyMembersCanAssault      ();
			int HowManyMembersAreTiedInMelee  ();

			void ResetStateFlags ();
			void ResetDrawingFlags ();

	/*****************************************************/

			void SetKillCount (int const &newKillCount);
			int  GetKillCount ();

			void AddKills (int const &kills);

			void ResetKillCount ();

	/*****************************************************/

			std::string ComposeSquadInfo ();

	/*****************************************************/

			void SetHeightMap(float *heightMap, int const &x, int const &z);

		private:

			std::string squadName;

			std::vector <Mini*> squadMembers;
			Mini *currentMini;

			Squad *meleeOpponnent;

			int pointsValue;
			int originalSquadSize;
			int killCount;
	};