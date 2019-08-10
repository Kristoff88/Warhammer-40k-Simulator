	#include "GlobalHeaders.h"

	#include "BattleForce.h"




	BattleForce::BattleForce  (std::string const &battleForceName): currentSquad(NULL), pointsValue(0)
	{
		this->battleForceName = battleForceName;
	}

	BattleForce::~BattleForce ()
	{}
//
/*****************************************************/

	void BattleForce::Render ()
	{
		for(int i = 0; i < squadList.size(); i++)
		{
			squadList[i]->Render();
		}
	}
//
/*****************************************************/

	std::string BattleForce::GetBattleForceName ()
	{
		return battleForceName;
	}
//
	int BattleForce::GetPointsValue ()
	{
		return pointsValue;
	}
//
/*****************************************************/

	void BattleForce::InsertNewSquad (Squad *newSquad)
	{
		pointsValue += newSquad->GetPointsValue();
		squadList.insert(squadList.begin(), newSquad);

		if( currentSquad == NULL )
		{
			currentSquad = newSquad;
		}
	}

	void BattleForce::RemoveSquad	 (int const &index)
	{
		if( GivenIndexIsCorrect(index) )
		{
			PerformSquadRemovement(index);
		}
		else
			InformAboutIncorrectIndex(index);
	}

	void BattleForce::RemoveSquad	 (std::string const &squadName)
	{
		for(int i = 0; i < squadList.size(); i++)
		{
			if( squadList[i]->GetSquadName() == squadName )
			{
				PerformSquadRemovement(i);
				return;
			}
		}
	
		InformAboutIncorrectName(squadName);
	}
//
		void BattleForce::CorrigateCurrentSquad  (int const &index)
		{
			if( currentSquad == squadList[index] )
			{
				if( squadList.size() == 1 )
				{
					currentSquad = NULL;
				}
				else if( index == 1 )
				{
					currentSquad = squadList[1];
				}
				else
					currentSquad = squadList[0];
			}
		}

		void BattleForce::PerformSquadRemovement (int const &index)
		{
			CorrigateCurrentSquad(index);

			pointsValue -= squadList[index]->GetPointsValue();
			squadList.erase( squadList.begin()+index );
		}
//
	std::vector <std::string> BattleForce::GetSquadList          ()
	{
		std::vector <std::string> squadNames;

		for(int i = 0; i < squadList.size(); i++)
		{
			squadNames.push_back( squadList[i]->GetSquadName() );
		}

		return squadNames;
	}

	std::vector <std::string> BattleForce::GetDeployedSquadList  ()
	{
		std::vector <std::string> deployedSquadNames;

		for(int i = 0; i < squadList.size(); i++)
		{
			if( squadList[i]->IsDeployed() )
			{
				deployedSquadNames.push_back( squadList[i]->GetSquadName() );
			}
		}

		return deployedSquadNames;
	}
//
	int BattleForce::GetSquadCount         ()
	{
		return squadList.size();
	}

	int BattleForce::GetDeployedSquadCount ()
	{
		int deployedSquadCount = 0;

		for(int i = 0; i < squadList.size(); i++)
		{
			if( squadList[i]->IsDeployed() )
			{
				deployedSquadCount++;
			}
		}

		return deployedSquadCount;
	}

	int BattleForce::GetAliveSquadCount    ()
	{
		int aliveSquadCount = 0;

		for(int i = 0; i < squadList.size(); i++)
		{
			if( squadList[i]->IsAlive() )
			{
				aliveSquadCount++;
			}
		}

		return aliveSquadCount;
	}

	int BattleForce::GetInActionSquadCount ()
	{
		int inActionSquadCount = 0;

		for(int i = 0; i < squadList.size(); i++)
		{
			if( squadList[i]->IsInAction() )
			{
				inActionSquadCount++;
			}
		}

		return inActionSquadCount;
	}
//
	void   BattleForce::SetCurrentSquad (int const &index)
	{
		if( GivenIndexIsCorrect(index) )
		{
			currentSquad = squadList[index];
		}
		else
			InformAboutIncorrectIndex(index);
	}

	Squad* BattleForce::GetCurrentSquad ()
	{
		return currentSquad;
	}
//
	void BattleForce::SetFirstSquadAsCurrent ()
	{
		assert( squadList.size() );

		currentSquad = squadList[0];
	}

	void BattleForce::SetNextSquadAsCurrent  ()
	{
		assert( squadList.size() );

		for(int i = 0; i < squadList.size(); i++)
		{
			if( squadList[i] == currentSquad )
			{
				if( i < squadList.size()-1 )
				{
					currentSquad = squadList[i+1];
				}

				return;
			}
		}
	}
//
	Squad* BattleForce::GetSquad (int const &index)
	{
		if( GivenIndexIsCorrect(index) )
		{
			return squadList[index];
		}
		else
			InformAboutIncorrectIndex(index);
	}

	Squad* BattleForce::GetSquad (std::string const &squadName)
	{
		for(int i = 0; i < squadList.size(); i++)
		{
			if( squadList[i]->GetSquadName() == squadName )
			{
				return squadList[i];
			}
		}

		InformAboutIncorrectName(squadName);
	}
//
		bool BattleForce::GivenIndexIsCorrect       (int const &index)
		{
			if( index >= 0 && index < squadList.size() )
			{
				return true;
			}
			else
				return false;
		}

		void BattleForce::InformAboutIncorrectIndex (int const &index) throw (std::string)
		{
			std::string error;

			error  = "Given squad index ";
			error += index;
			error += " is incorrect!";

			throw error;
		}

		void BattleForce::InformAboutIncorrectName  (std::string const &squadName) throw (std::string)
		{
			std::string error;

			error  = "Given squad name ";
			error += squadName;
			error += " is incorrect!";

			throw error;
		}
//
/*****************************************************/

	void BattleForce::ResetStateFlags   ()
	{
		for(int i = 0; i < squadList.size(); i++)
		{
			squadList[i]->ResetStateFlags();
		}
	}

	void BattleForce::ResetDrawingFlags ()
	{
		for(int i = 0; i < squadList.size(); i++)
		{
			squadList[i]->ResetDrawingFlags();
		}
	}
//
/*****************************************************/

	bool BattleForce::BattleForceGotClickedOn (Vector3 const &cursor)
	{
		bool battleForceGotClickedOn = false;

		currentSquad = NULL;

		for(int i = 0; i < squadList.size(); i++)
		{
			if( squadList[i]->SquadGotClickedOn(cursor) )
			{
				currentSquad = squadList[i];
				battleForceGotClickedOn = true;
				break;
			}
		}

		return battleForceGotClickedOn;
	}
//
/*****************************************************/

	std::string BattleForce::ComposeBattleForceInfo ()
	{
		std::string battleForceInfo("");

		battleForceInfo  = "  ";
		battleForceInfo += "BattleForce: ";
		battleForceInfo += battleForceName;
		battleForceInfo += "\n\n";

		/*if( currentSquad )
		{
			battleForceInfo += currentSquad->ComposeSquadInfo();
		}*/

		return battleForceInfo;
	}
//
/*****************************************************/

	void BattleForce::SetHeightMap(float *heightMap, int const &x, int const &z)
	{
		for(int i = 0; i < squadList.size(); i++)
		{
			squadList[i]->SetHeightMap(heightMap, x, z);
		}
	}
//
/*****************************************************/