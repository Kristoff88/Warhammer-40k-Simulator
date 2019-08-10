#pragma once

	#include "Squad//Squad.h"


	class BattleForce
	{
		public:

			BattleForce  (std::string const &battleForceName);
			~BattleForce ();


	/*****************************************************/

			void Render ();

	/*****************************************************/

			std::string GetBattleForceName ();

			int GetPointsValue ();

	/*****************************************************/

			void InsertNewSquad (Squad *newSquad);
			void RemoveSquad	(int const &index);
			void BattleForce::RemoveSquad	 (std::string const &squadName);
				private:
				inline void CorrigateCurrentSquad  (int const &index);
				inline void PerformSquadRemovement (int const &index);
			public:

			std::vector <std::string> GetSquadList          ();
			std::vector <std::string> GetDeployedSquadList  ();

			int GetSquadCount         ();
			int GetDeployedSquadCount ();
			int GetAliveSquadCount    ();
			int GetInActionSquadCount ();
  
			void   SetCurrentSquad (int const &index);
			Squad* GetCurrentSquad ();

			void SetFirstSquadAsCurrent ();
			void SetNextSquadAsCurrent  ();

			Squad* GetSquad (int const &index);
			Squad* GetSquad (std::string const &squadName);

				private:
				inline bool GivenIndexIsCorrect       (int const &index);
				inline void InformAboutIncorrectIndex (int const &index) throw (std::string);
				inline void InformAboutIncorrectName  (std::string const &squadName) throw (std::string);
			public:
	
	/*****************************************************/

			void ResetStateFlags   ();
			void ResetDrawingFlags ();

	/*****************************************************/

			bool BattleForceGotClickedOn (Vector3 const &cursor);

	/*****************************************************/

			std::string ComposeBattleForceInfo ();

	/*****************************************************/

			void SetHeightMap(float *heightMap, int const &x, int const &z);	

		private:

			std::string battleForceName;

			std::vector <Squad*> squadList;
			Squad *currentSquad;

			int pointsValue;
	};