	#include "GlobalHeaders.h"

	#include "Squad.h"


	#define SQUAD_SPACING_RANGE (4.f)




	Squad::Squad  (std::string const &squadName, Mini *mini, int const &miniCount)
	{
		currentMini = NULL;
		meleeOpponnent = NULL;

		this->squadName = squadName;


		assert( miniCount >= 1 );

		pointsValue       = miniCount * mini->GetPoints();
		originalSquadSize = miniCount;

		for(int i = 0 ; i < miniCount; i++)
		{
			Mini *newMini = new Mini("");
			*newMini = *mini;

			squadMembers.push_back(newMini);
		}

		currentMini = squadMembers[0];

		killCount = 0;
	}

	Squad::~Squad ()
	{}
//
	void Squad::operator= (Squad &squad)
	{
		squadName = squad.squadName;

		for(int i = 0; i < squad.squadMembers.size(); i++)
		{
			Mini *newMini = new Mini("");
			*newMini = *squad.squadMembers[i];

			squadMembers.push_back(newMini);
		}

		currentMini = NULL;

		pointsValue       = squad.pointsValue;
		originalSquadSize = squad.originalSquadSize;
		killCount         = squad.killCount;
	}
//
/*****************************************************/

	void Squad::Render ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->Render();
		}
	}
//
/*****************************************************/

	std::string Squad::GetSquadName ()
	{
		return squadName;
	}

	std::string Squad::GetMiniName  ()
	{
		return squadMembers[0]->GetMiniName();
	}
//
	int Squad::GetPointsValue ()
	{
		return pointsValue;
	}
//
	int Squad::GetOriginalSquadSize    ()
	{
		return originalSquadSize;
	}

	int Squad::GetDeployedMembersCount ()
	{
		int deployedMembersCount = 0;

		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( squadMembers[i]->IsDeployed() )
			{
				deployedMembersCount++;
			}
		}

		return deployedMembersCount;
	}

	int Squad::GetLiveMembersCount     ()
	{
		int liveMembersCount = 0;

		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( squadMembers[i]->IsAlive() )
			{
				liveMembersCount++;
			}
		}

		return liveMembersCount;
	}

	int Squad::GetMembersInAction      ()
	{
		int membersInAction = 0;

		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( squadMembers[i]->IsInAction () )
			{
				membersInAction++;
			}
		}

		return membersInAction;
	}
//
/*****************************************************/

	bool Squad::SquadCollidesWithThisMini (Mini *mini)
	{
		bool miniCollision = false;

		Vector3 miniPosition = mini->GetPosition();

		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( squadMembers[i] != mini       &&
			    squadMembers[i]->IsInAction() && 
				squadMembers[i]->PositionWithinModelProximity(miniPosition) )
			{
				miniCollision = true;
				break;
			}
		}

		return miniCollision;
	}
//
	bool Squad::SquadGotClickedOn (Vector3 const &cursor)
	{
		currentMini = NULL;


		bool squadGotClickedOn = false;

		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( squadMembers[i]->IsInAction() && squadMembers[i]->PositionWithinModelProximity(cursor) )
			{
				currentMini = squadMembers[i];
				squadGotClickedOn = true;
				break;
			}
		}

		return squadGotClickedOn;
	}
//
/*****************************************************/

	Mini* Squad::GetMini (int const &index)
	{
		if( GivenIndexIsCorrect(index) )
		{
			return squadMembers[index];
		}
		else
			InformAboutIncorrectIndex(index);
	}
//
	Mini* Squad::GetCurrentMini   ()
	{
		return currentMini;
	}

	int   Squad::GetCurrentMiniID ()
	{
		if( currentMini == NULL )
			return -1;


		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( squadMembers[i] == currentMini )
			{
				return i;
			}
		}


		bool GetCurrentMiniIDerror = true;
		assert( !GetCurrentMiniIDerror );
	}
//
	void Squad::SetCurrentMini (int const &index)
	{
		if( GivenIndexIsCorrect(index) )
		{
			currentMini = squadMembers[index];
		}
		else
			InformAboutIncorrectIndex(index);
	}
//
	void Squad::SetFirstMiniAsCurrent ()
	{
		assert( squadMembers.size() );

		currentMini = squadMembers[0];
	}

	void Squad::SetNextMiniAsCurrent  ()
	{
		assert( squadMembers.size() );

		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( squadMembers[i] == currentMini )
			{
				if( i < squadMembers.size()-1 )
				{
					currentMini = squadMembers[i+1];
				}

				return;
			}
		}
	}
//
		bool Squad::GivenIndexIsCorrect       (int const &index)
		{
			if( index >= 0 && index < squadMembers.size() )
			{
				return true;
			}
			else
				return false;
		}

		void Squad::InformAboutIncorrectIndex (int const &index) throw (std::string)
		{
			std::string error;

			error  = "Given mini index ";
			error += index;
			error += " is incorrect!";

			throw error;
		}
//
/*****************************************************/

	void Squad::DeployCurrentMini (Vector3 const &position)
	{
		assert( currentMini != NULL );
		currentMini->Deploy(position);

		AdjustCurrentMinisRotation();
	}
//
		void Squad::AdjustCurrentMinisRotation ()
		{
			if( currentMini != squadMembers[0] )
			{
				for(int i = 1; i < squadMembers.size(); i++)
				{
					if( squadMembers[i] == currentMini )
					{
						currentMini->SetRotation( squadMembers[i-1]->GetRotation() );
						break;
					}
				}
			}
		}
//
	void Squad::Undeploy  ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->SetDeployment(false);
		}
	}
//
	void Squad::ReviveAll ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->Revive();
		}
	}

	void Squad::KillAll   ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->Kill();
		}
	}
//
/*****************************************************/

	int  Squad::InitiateRun    ()
	{
		assert( HowManyMembersCanMove() );

		srand( time(NULL) );
		int extraInches = (rand()%6) + 1;

		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->SetRunningBonus(extraInches);
		}

		SetFlag_Run();

		return extraInches;
	}

	bool Squad::SquadIsRunning ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			if(	squadMembers[i]->IsRunning() )
			{
				return true;
			}
		}

		return false;
	}
//
/*****************************************************/

	bool Squad::TargetInShootingRange  (Squad *target)
	{
		assert( target != NULL );


		for(int i = 0; i < squadMembers.size();            i++)
		for(int j = 0; j < target->GetOriginalSquadSize(); j++)
		{
			Mini *mini = target->GetMini(j);

			if( squadMembers[i]->IsInAction() && mini->IsInAction() )
			{
				if( squadMembers[i]->InRangeOf_Fire(mini) )
				{
					return true;
				}
			}
		}


		return false;
	}

	bool Squad::TargetInRapidFireRange (Squad *target)
	{
		assert( target != NULL );


		for(int i = 0; i < squadMembers.size();            i++)
		for(int j = 0; j < target->GetOriginalSquadSize(); j++)
		{
			Mini *mini = target->GetMini(j);

			if( squadMembers[i]->IsInAction() && mini->IsInAction() )
			{
				if( squadMembers[i]->InRangeOf_RapidFire(mini) )
				{
					return true;
				}
			}
		}


		return false;
	}

	bool Squad::TargetInAssaultRange   (Squad *target)
	{
		assert( target != NULL );


		for(int i = 0; i < squadMembers.size();            i++)
		for(int j = 0; j < target->GetOriginalSquadSize(); j++)
		{
			Mini *mini = target->GetMini(j);

			if( squadMembers[i]->IsInAction() && mini->IsInAction() )
			{
				if( squadMembers[i]->InRangeOf_Assault(mini) )
				{
					return true;
				}
			}
		}


		return false;
	}
//
/*****************************************************/

	void   Squad::SetMeleeOpponnent (Squad *opponnent)
	{
		meleeOpponnent = opponnent;

		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( meleeOpponnent )
			{
				squadMembers[i]->SetInMelee(true);
			}
			else
				squadMembers[i]->SetInMelee(false);
		}
	}

	Squad* Squad::GetMeleeOpponnent ()
	{
		return meleeOpponnent;
	}
//
	bool Squad::IsTiedInMelee    ()
	{
		if( IsInAction() )
		{
			bool squadTiedInMelee = false;

			for(int i = 0; i < squadMembers.size(); i++)
			{
				if( squadMembers[i]->IsTiedInMelee() )
				{
					squadTiedInMelee = true;
					break;
				}
			}

			return squadTiedInMelee;
		}
		else
			return false;
	}

	bool Squad::IsNotTiedInMelee ()
	{
		return !IsTiedInMelee();
	}
//
/*****************************************************/

	bool Squad::IsDeployed    ()
	{
		bool SquadIsFullyDeployed = true;

		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( squadMembers[i]->IsNotDeployed() )
			{
				SquadIsFullyDeployed = false;
				break;
			}
		}

		return SquadIsFullyDeployed;
	}

	bool Squad::IsNotDeployed ()
	{
		return !IsDeployed();
	}
//
	bool Squad::IsWipedOut ()
	{
		bool SquadIsWipedOut = true;

		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( squadMembers[i]->IsAlive() )
			{
				SquadIsWipedOut = false;
				break;
			}
		}

		return SquadIsWipedOut;
	}

	bool Squad::IsAlive    ()
	{
		return !IsWipedOut();
	}
//
	bool Squad::IsInAction ()
	{
		return IsDeployed() && IsAlive();
	}
//
	bool Squad::SquadLostIntegrity ()
	{
		if( GetMembersInAction() <= 1 )
		{
			return false;
		}



		bool squadHasLostIntegrity = false;


		for(int i = 0; i < squadMembers.size(); i++)
		{
			if( !squadMembers[i]->IsInAction() )
			{
				break;
			}

			int squadMembersNearby = 0;

			for(int j = 0; j < squadMembers.size(); j++)
			{
				if( !squadMembers[j]->IsInAction() )
				{
					break;
				}

				if( i != j )
				{
					float distance = squadMembers[i]->GetDistance( squadMembers[j]->GetPosition() );
					distance -= squadMembers[j]->GetRadius();

					if( distance < SQUAD_SPACING_RANGE )
					{
						squadMembersNearby++;
					}
				}
			}

			if( squadMembersNearby == 0 )
			{
				squadHasLostIntegrity = true;
				break;
			}
		}


		return squadHasLostIntegrity;
	}
//
/*****************************************************/

	void Squad::SetFlag_Move    ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->SetFlag_Move();
		}
	}

	void Squad::SetFlag_Run     ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->SetFlag_Run();
		}
	}

	void Squad::SetFlag_Shoot   ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->SetFlag_Shoot();
		}
	}

	void Squad::SetFlag_Assault ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->SetFlag_Assault();
		}
	}
//
	int Squad::HowManyMembersCanMove         ()
	{
		if( IsInAction() )
		{
			int membersThatCanMove = 0;

			for(int i = 0; i < squadMembers.size(); i++)
			{
				if( squadMembers[i]->CanMove() )
				{
					membersThatCanMove++;
				}
			}

			return membersThatCanMove;
		}
		else
			return 0;
	}

	int Squad::HowManyMembersCanShoot        ()
	{
		if( IsInAction() )
		{
			int membersThatCanShoot = 0;

			for(int i = 0; i < squadMembers.size(); i++)
			{
				if( squadMembers[i]->CanShoot() )
				{
					membersThatCanShoot++;
				}
			}

			return membersThatCanShoot;
		}
		else
			return 0;
	}

	int Squad::HowManyMembersCanShootRapidly ()
	{
		if( IsInAction() )
		{
			int membersThatCanShootRapidly = 0;

			for(int i = 0; i < squadMembers.size(); i++)
			{
				if( squadMembers[i]->CanShootRapidly() )
				{
					membersThatCanShootRapidly++;
				}
			}

			return membersThatCanShootRapidly;
		}
		else
			return 0;
	}

	int Squad::HowManyMembersCanAssault      ()
	{
		if( IsInAction() )
		{
			int membersThatCanAssault = 0;

			for(int i = 0; i < squadMembers.size(); i++)
			{
				if( squadMembers[i]->CanAssault() )
				{
					membersThatCanAssault++;
				}
			}

			return membersThatCanAssault;
		}
		else
			return 0;
	}

	int Squad::HowManyMembersAreTiedInMelee  ()
	{
		if( IsInAction() )
		{
			int membersTiedInMelee = 0;

			for(int i = 0; i < squadMembers.size(); i++)
			{
				if( squadMembers[i]->IsInAction() && squadMembers[i]->IsTiedInMelee() )
				{
					membersTiedInMelee++;
				}
			}

			return membersTiedInMelee;
		}
		else
			return 0;
	}
//
	void Squad::ResetStateFlags ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->ResetStateFlags();
		}
	}
//
	void Squad::ResetDrawingFlags ()
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->ResetDrawingFlags();
		}
	}
//
/****************************************************************/

	void Squad::SetKillCount (int const &newKillCount)
	{
		killCount = newKillCount;
	}

	int  Squad::GetKillCount ()
	{
		return killCount;
	}
//
	void Squad::AddKills (int const &kills)
	{
		killCount += kills;
	}
//
	void Squad::ResetKillCount ()
	{
		killCount = 0;
	}
//
/****************************************************************/

	std::string Squad::ComposeSquadInfo ()
	{
		std::string squadInfo("");

		squadInfo  = "  ";
		squadInfo += squadName;
		squadInfo += "\n";

		squadInfo += "  ";
		squadInfo += "Original size: ";
		squadInfo += NumberToString( originalSquadSize );
		squadInfo += "\n";

		squadInfo += "  ";
		squadInfo += "Size: ";
		squadInfo += NumberToString( GetMembersInAction() );
		squadInfo += "\n";

		squadInfo += "  ";
		squadInfo += "Kill count: ";
		squadInfo += NumberToString( killCount );
		squadInfo += "\n";

		squadInfo += "  ";
		squadInfo += "Tied in melee: ";
		
		if( IsTiedInMelee() )
		{
			squadInfo += "TRUE";
		}
		else
			squadInfo += "FALSE";

		squadInfo += "\n\n";


		if( currentMini )
		{
			squadInfo += currentMini->ComposeMiniInfo();
		}

		return squadInfo;
	}
//
/*****************************************************/

	void Squad::SetHeightMap(float *heightMap, int const &x, int const &z)
	{
		for(int i = 0; i < squadMembers.size(); i++)
		{
			squadMembers[i]->SetHeightMap(heightMap, x, z);
		}
	}