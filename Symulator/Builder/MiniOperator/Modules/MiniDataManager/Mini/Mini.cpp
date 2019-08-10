	#include "GlobalHeaders.h"

	#include "Mini.h"



	float* Mini::heightMap  = NULL;
	int    Mini::mapWidth_X = 0;
	int    Mini::mapWidth_Z = 0;


	#define STANDARD_WEAPON  ("Standard_Weapon")
	#define POWER_WEAPON     ("Power_Weapon")	// Ignores save.
	#define POWER_FIST       ("Power_Fist")		// Doubles strength, gives 1 inititative, ignores armor.
	#define RENDING_WEAPON   ("Rending_Weapon")	// If hits, automatically causes a wound + ignores armour, if rolled 6.
			
	#define HEAVY_WEAPON	 ("Heavy_Weapon")		// Can't shoot, if moved.
	#define ASSAULT_WEAPON	 ("Assault_Weapon")		// Can shoot, if moved + can assault after shooting.
	#define RAPIDFIRE_WEAPON ("RapidFiring_Weapon")	// Can't normally shoot, if moved. Up to 12 inches it can always (even if moved) twice.
	#define PISTOL			 ("Pistol")				// Can shoot, before assaulting.



	#define SELECTED_UNIT_COLOR  (Color(0.90, 0.90, 1.00f, 0.90f))
	#define ASSAULT_TARGET_COLOR (Color(1.00, 0.00, 0.00f, 0.90f))
	#define RANGED_TARGET_COLOR  (Color(0.20, 0.20, 0.90f, 0.90f))

	#define MOVE_COLOR       (Color(0.00, 1.00, 0.00f, 0.90f))
	#define SHOOT_COLOR      (Color(0.50, 0.50, 1.00f, 0.90f))
	#define RAPID_FIRE_COLOR (Color(0.00, 0.00, 1.00f, 0.90f))
	#define ASSAULT_COLOR    (Color(1.00, 0.50, 0.50f, 0.90f))

	#define SQUAD_SPACING_RANGE (4.f)
	#define RAPID_FIRING_RANGE	(12.f)
	#define ASSAULT_RANGE		(6.f)



	

	Mini::Mini (std::string name)
	{
		miniName = name;
		SetDefaultMiniCharacteristics();
		SetDefaultBolleanCharacteristics();
	}

	Mini::Mini (std::string name, MiniModel_Data *model)
	{
		Mini::Mini(name);
		CopyModelData(model);	
	}
//
		void Mini::SetDefaultMiniCharacteristics ()
		{
			points = 1;

			weaponType = STANDARD_WEAPON;

			weaponStrength = 
			weaponSkill    = 
			initiative     = 
			attacks        = 1;


			fireArmType = ASSAULT_WEAPON;

			fireArmStrength = 
			ballisticSkill  = 
			firingRange     = 
			rateOfFire      = 
			armorPiercing   = 1;


			toughness = 
			wounds    = 
			save      = 1;

			woundsInflicted = 0;

			movementSpeed = 1;
			runningRangeBonus = 0;
		}

		void Mini::SetDefaultBolleanCharacteristics ()
		{
			deployed = dead = false;

			tiedInMelee = false;

			didMoving     = 
			didRunning    = 
			didShooting   = 
			didAssaulting = false;

			drawingSquadSpacing = 
			drawingActivity     = 
			drawingSelection    = false;

			drawingAssaultTarget =
			drawingRangedTarget  = false;

			drawingMovementRange    = 
			drawingShootingRange    = 
			drawingRapidFiringRange =
			drawingAssaultRange     = false;
		}

//
	Mini::~Mini ()
	{}
//
/****************************************************************/

	void Mini::Render ()
	{
		if( deployed && !dead )
		{
			DrawCircles();
			MiniModel::Render();
		}
	}
//
//
	void Mini::DrawCircles ()
	{
		if( drawingSquadSpacing )
		{
			DrawSquadSpacing();
		}

		if( drawingActivity )
		{
			DrawActivity();
		}

		if( drawingSelection )
		{
			DrawSelection();
		}

		if( drawingAssaultTarget )
		{
			DrawAssaultTarget();
		}

		if( drawingRangedTarget )
		{
			DrawRangedTarget();
		}

		if( drawingMovementRange )
		{
			DrawMovementRange();
		}

		if( drawingShootingRange )
		{
			DrawShootingRange();
		}

		if( drawingRapidFiringRange )
		{
			DrawRapidFiringRange();
		}

		if( drawingAssaultRange )
		{
			DrawAssaultRange();
		}
	}
//
	void Mini::DrawSquadSpacing       ()
	{
		DrawCircle(false, SQUAD_SPACING_RANGE + GetRadius(), SELECTED_UNIT_COLOR);
	}

	void Mini::SetDrawingSquadSpacing (bool const &newValue)
	{
		drawingSquadSpacing = newValue;
	}

	bool Mini::IsDrawingSquadSpacing  ()
	{
		return drawingSquadSpacing;
	}
//
	void Mini::DrawActivity	      ()
	{
		DrawCircle(false, GetRadius(), SELECTED_UNIT_COLOR);
	}

	void Mini::SetDrawingActivity (bool const &newValue)
	{
		drawingActivity = newValue;
	}

	bool Mini::IsDrawingActivity  ()
	{
		return drawingActivity;
	}
//
	void Mini::DrawSelection       ()
	{
		DrawCircle(false, GetRadius()*0.9f, SELECTED_UNIT_COLOR);
	}

	void Mini::SetDrawingSelection (bool const &newValue)
	{
		drawingSelection = newValue;
	}

	bool Mini::IsDrawingSelection  ()
	{
		return drawingSelection;
	}
//
	void Mini::DrawAssaultTarget       ()
	{
		DrawCircle(false, GetRadius(), ASSAULT_TARGET_COLOR);
	}

	void Mini::SetDrawingAssaultTarget (bool const &newValue)
	{
		drawingAssaultTarget = newValue;
	}

	bool Mini::IsDrawingAssaultTarget  ()
	{
		return drawingAssaultTarget;
	}
//
	void Mini::DrawRangedTarget        ()
	{
		DrawCircle(false, GetRadius(), RANGED_TARGET_COLOR);
	}

	void Mini::SetDrawingRangedTarget  (bool const &newValue)
	{
		drawingRangedTarget = newValue;
	}

	bool Mini::IsDrawingRangedTarget   ()
	{
		return drawingRangedTarget;
	}
//
	void Mini::DrawMovementRange       ()
	{
		DrawCircle(false, movementSpeed+runningRangeBonus + GetRadius(), MOVE_COLOR);
	}

	void Mini::SetDrawingMovementRange (bool const &newValue)
	{
		drawingMovementRange = newValue;
	}

	bool Mini::IsDrawingMovementRange  ()
	{
		return drawingMovementRange;
	}
//
	void Mini::DrawShootingRange       ()
	{
		DrawCircle(false, firingRange + GetRadius(), SHOOT_COLOR);
	}

	void Mini::SetDrawingShootingRange (bool const &newValue)
	{
		drawingShootingRange = newValue;
	}

	bool Mini::IsDrawingShootingRange  ()
	{
		return drawingShootingRange;
	}
//
	void Mini::DrawRapidFiringRange       ()
	{
		DrawCircle(false, RAPID_FIRING_RANGE + GetRadius(), RAPID_FIRE_COLOR);
	}

	void Mini::SetDrawingRapidFiringRange (bool const &newValue)
	{
		drawingRapidFiringRange = newValue;
	}

	bool Mini::IsDrawingRapidFiringRange  ()
	{
		return drawingRapidFiringRange;
	}
//
	void Mini::DrawAssaultRange       ()
	{
		DrawCircle(false, ASSAULT_RANGE + GetRadius(), ASSAULT_COLOR);
	}

	void Mini::SetDrawingAssaultRange (bool const &newValue)
	{
		drawingAssaultRange = newValue;
	}

	bool Mini::IsDrawingAssaultRange  ()
	{
		return drawingAssaultRange;
	}
//
	void Mini::ResetDrawingFlags ()
	{
		drawingSquadSpacing = 
		drawingActivity     = 
		drawingSelection    =

		drawingAssaultTarget =
		drawingRangedTarget  = 

		drawingMovementRange    = 
		drawingShootingRange    =
		drawingRapidFiringRange =
		drawingAssaultRange     = false;
	}
//
//
	void Mini::DrawCircle (bool const &polygon, float const &radius, Color &color)
	{
		glPushMatrix();
		glDisable(GL_LIGHTING);
	
			if( polygon )
			{
				glBegin(GL_POLYGON);
			}
			else
				glBegin(GL_LINE_LOOP);
			
			//glColor4fv(color);

			glColor4f(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());


			for(int angle = 360; angle >= 0; angle -= 10)
			{
				float angleInRadians = (angle * 3.14159f) / 180.f;
				float x = position.x + radius * (float)cos(angleInRadians);
				float z = position.z + radius * (float)sin(angleInRadians);

				float height = CalculateHeight(Vector3(x,0,z)).y + .1f;


				if( x < 0 )
				{
					x = 0;
					height = 0;
				}
				else if( x > mapWidth_X )
				{
					x = mapWidth_X;
					height = 0;
				}
				else if( z > 0 )
				{
					z = 0;
					height = 0;
				}
				else if( z < -mapWidth_Z )
				{
					z = -mapWidth_Z;
					height = 0;
				}
			

				glVertex3f(x, height, z);
			}

			glEnd();

		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
//
/****************************************************************/

	std::string Mini::GetMiniName ()
	{
		return miniName;
	}
//
//
	void Mini::SetPoints (int const &newValue)
	{
		if( newValue > 999 )
		{
			points = 999;
		}
		else if( newValue < 1 )
		{
			points = 1;
		}
		else
			points = newValue;
	}

	int  Mini::GetPoints ()
	{
		return points;
	}
//
	void Mini::GiveStandardWeapon ()
	{
		weaponType = STANDARD_WEAPON;
	}

	bool Mini::HasStandardWeapon  ()
	{
		return weaponType == STANDARD_WEAPON;
	}
//
	void Mini::GivePowerWeapon ()
	{
		weaponType = POWER_WEAPON;
	}

	bool Mini::HasPowerWeapon  ()
	{
		return weaponType == POWER_WEAPON;
	}
//
	void Mini::GivePowerFist ()
	{
		weaponType = POWER_FIST;
	}

	bool Mini::HasPowerFist  ()
	{
		return weaponType == POWER_FIST;;
	}
//
	void Mini::GiveRendingWeapon ()
	{
		weaponType = RENDING_WEAPON;
	}

	bool Mini::HasRendingWeapon  ()
	{
		return weaponType == RENDING_WEAPON;
	}
//
//
	void Mini::SetWeaponStrength (int const &newValue)
	{
		weaponStrength = CorrectNewValue(newValue);
	}

	int  Mini::GetWeaponStrength ()
	{
		return weaponStrength;
	}
//
	void Mini::SetWeaponSkill (int const &newValue)
	{
		weaponSkill = CorrectNewValue(newValue);
	}

	int  Mini::GetWeaponSkill ()
	{
		return weaponSkill;
	}
//
	void Mini::SetInitiative (int const &newValue)
	{
		initiative = CorrectNewValue(newValue);
	}

	int  Mini::GetInitiative ()
	{
		return initiative;
	}
//
	void Mini::SetAttacks (int const &newValue)
	{
		attacks = CorrectNewValue(newValue);
	}

	int  Mini::GetAttacks ()
	{
		return attacks;
	}
//
//
	void Mini::GiveHeavyWeapon ()
	{
		fireArmType = HEAVY_WEAPON;
	}

	bool Mini::HasHeavyWeapon  ()
	{
		return fireArmType == HEAVY_WEAPON;
	}
//
	void Mini::GiveAssaultWeapon ()
	{
		fireArmType = ASSAULT_WEAPON;
	}

	bool Mini::HasAssaultWeapon  ()
	{
		return fireArmType == ASSAULT_WEAPON;
	}
//
	void Mini::GiveRapidFireWeapon ()
	{
		fireArmType = RAPIDFIRE_WEAPON;
	}

	bool Mini::HasRapidFireWeapon  ()
	{
		return fireArmType == RAPIDFIRE_WEAPON;
	}
//
	void Mini::GivePistol ()
	{
		fireArmType = PISTOL;
	}

	bool Mini::HasPistol  ()
	{
		return fireArmType == PISTOL;
	}
//
//
	void Mini::SetFirearmStrength (int const &newValue)
	{
		fireArmStrength = CorrectNewValue(newValue);
	}

	int  Mini::GetFirearmStrength ()
	{
		return fireArmStrength;
	}
//
	void Mini::SetBallisticSkill (int const &newValue)
	{
		ballisticSkill = CorrectNewValue(newValue);

		if( ballisticSkill > 6 )
		{
			ballisticSkill = 6;
		}
	}

	int  Mini::GetBallisticSkill ()
	{
		return ballisticSkill;
	}
//
	void Mini::SetFiringRange (int const &newValue)
	{
		firingRange = CorrectNewValue(newValue);
	}

	int  Mini::GetFiringRange ()
	{
		return firingRange;
	}
//
	void Mini::SetRateOfFire (int const &newValue)
	{
		rateOfFire = CorrectNewValue(newValue);
	}

	int  Mini::GetRateOfFire ()
	{
		return rateOfFire;
	}
//
	void Mini::SetArmorPiercing (int const &newValue)
	{
		armorPiercing = CorrectNewValue(newValue);
	}

	int  Mini::GetArmorPiercing ()
	{
		return armorPiercing;
	}
//
//
	void Mini::SetToughness (int const &newValue)
	{
		toughness = CorrectNewValue(newValue);
	}

	int  Mini::GetToughness ()
	{
		return toughness;
	}
//
	void Mini::SetWounds (int const &newValue)
	{
		wounds = CorrectNewValue(newValue);
	}

	int  Mini::GetWounds ()
	{
		return wounds;
	}
//
	void Mini::SetSave (int const &newValue)
	{
		save = CorrectNewValue(newValue);
	}

	int  Mini::GetSave ()
	{
		return save;
	}
//
//
	void Mini::SetMovementSpeed (int const &newValue)
	{
		if( newValue < 0 )
		{
			movementSpeed = 0;
		}
		else
			movementSpeed = newValue;
	}
	int  Mini::GetMovementSpeed ()
	{
		return movementSpeed + runningRangeBonus;
	}
//
	void Mini::SetRunningBonus (int const &value)
	{
		assert( value >= 0 );

		runningRangeBonus = value;
	}
//
	float Mini::GetAssaultRange ()
	{
		return ASSAULT_RANGE;
	}
//
//
	int Mini::CorrectNewValue (int const &newValue)
	{
		if( newValue < 1 )
		{
			return 1;
		}
		else if( newValue > 99 )
		{
			return 99;
		}
		else
			return newValue;
	}
//
/****************************************************************/

	void Mini::SetInMelee    (bool const &newValue)
	{
		tiedInMelee = newValue;
	}

	bool Mini::IsTiedInMelee ()
	{
		return tiedInMelee;
	}
//
	void Mini::Deploy (Vector3 const &position)
	{
		SetPosition(position);
		SetDeployment(true);
	}
//
	void Mini::SetDeployment (bool const &newValue)
	{
		deployed = newValue;
	}

	bool Mini::IsDeployed    ()
	{
		return deployed;
	}

	bool Mini::IsNotDeployed ()
	{
		return !IsDeployed();
	}
//
	void Mini::InflictWound ()
	{
		woundsInflicted++;

		if( woundsInflicted == wounds )
		{
			Kill();
		}
	}
//
	void Mini::Kill   ()
	{
		dead = true;
		woundsInflicted = wounds;
	}

	void Mini::Revive ()
	{
		dead = false;
		woundsInflicted = 0;
	}
//
	void Mini::SetDead (bool const &newValue)
	{
		dead = newValue;
	}

	bool Mini::IsDead  ()
	{
		return dead;
	}

	bool Mini::IsAlive ()
	{
		return !IsDead();
	}
//
	bool Mini::IsInAction ()
	{
		return IsDeployed() && IsAlive();
	}
//
/****************************************************************/

	void Mini::SetFlag_Move    ()
	{
		didMoving = true;
	}

	void Mini::SetFlag_Run     ()
	{
		didRunning = true;
	}

	void Mini::SetFlag_Shoot   ()
	{
		didShooting = true;
	}

	void Mini::SetFlag_Assault ()
	{
		didAssaulting = true;
	}
//
	bool Mini::CanMove         ()
	{
		if( IsInAction() )
		{
			if( tiedInMelee )
			{
				return false;
			}
			else
				return !didMoving;
		}
		else
			return false;
	}

	bool Mini::CanShoot        ()
	{
		if( IsInAction() )
		{
			if( didRunning || didShooting || tiedInMelee )
			{
				return false;
			}
			else
			{
				if( didMoving && ( HasHeavyWeapon() || HasRapidFireWeapon() ) )
				{
					return false;
				}
				else
					return true;
			}
		}
		else
			return false;
	}

	bool Mini::CanShootRapidly ()
	{
		if( IsInAction() )
		{
			if( didRunning || didShooting || tiedInMelee )
			{
				return false;
			}
			else
			{
				if( HasRapidFireWeapon() )
				{
					return true;
				}
				else
					return false;
			}
		}
		else
			return false;
	}

	bool Mini::CanAssault      ()
	{
		if( IsInAction() )
		{
			if( //didRunning || 
				didAssaulting )
			{
				return false;
			}
			else if( didShooting )
			{
				if( HasAssaultWeapon() || HasPistol() )
				{
					return true;
				}
				else
					return false;
			}
			else
				return true;
		}
		else
			return false;
	}
//
	bool Mini::IsRunning ()
	{
		if( runningRangeBonus > 0 )
		{
			return true;
		}
		else
			return false;
	}
//
	void Mini::ResetStateFlags ()
	{
		didMoving = didRunning = didShooting = didAssaulting = false;

		runningRangeBonus = 0;
	}
//
/****************************************************************/

	bool Mini::InRangeOf_Fire      (Mini *possibleTarget)
	{
		float distance = GetDistance(possibleTarget->GetPosition()) - GetRadius() - possibleTarget->GetRadius();

		if( distance <= firingRange )
		{
			return true;
		}
		else
			return false;
	}

	bool Mini::InRangeOf_RapidFire (Mini *possibleTarget)
	{
		float distance = GetDistance(possibleTarget->GetPosition()) - GetRadius() - possibleTarget->GetRadius();

		if( distance <= RAPID_FIRING_RANGE )
		{
			return true;
		}
		else
			return false;
	}

	bool Mini::InRangeOf_Assault   (Mini *possibleTarget)
	{
		float distance = GetDistance(possibleTarget->GetPosition()) - (float)GetRadius() - (float)possibleTarget->GetRadius();

		if( distance <= ASSAULT_RANGE )
		{
			return true;
		}
		else
			return false;
	}
//
/****************************************************************/

	std::string Mini::ComposeMiniInfo ()
	{
		std::string miniInfo("");

		miniInfo  = "  ";
		miniInfo += "Mini: ";
		miniInfo += miniName;
		miniInfo += "\n\n";


		miniInfo += "  ";
		miniInfo += "Weapon type: ";
		
		if( HasStandardWeapon() )
		{
			miniInfo += STANDARD_WEAPON;
			miniInfo += "\n";
		}
		else if( HasPowerWeapon() )
		{
			miniInfo += POWER_WEAPON;
			miniInfo += "\n";
		}
		else if( HasPowerFist() )
		{
			miniInfo += POWER_FIST;
			miniInfo += "\n";
		}
		else if( HasRendingWeapon() )
		{
			miniInfo += RENDING_WEAPON;
			miniInfo += "\n";
		}
			

		miniInfo += "  ";
		miniInfo += "Weapon strength: ";
		miniInfo += NumberToString( weaponStrength );
		miniInfo += "\n";

		miniInfo += "  ";
		miniInfo += "Weapon skill: ";
		miniInfo += NumberToString( weaponSkill );
		miniInfo += "\n";

		miniInfo += "  ";
		miniInfo += "Initiative: ";
		miniInfo += NumberToString( initiative );
		miniInfo += "\n";

		miniInfo += "  ";
		miniInfo += "Attacks: ";
		miniInfo += NumberToString( attacks );
		miniInfo += "\n\n";


		miniInfo += "  ";
		miniInfo += "Firearm type: ";

		if( HasAssaultWeapon() )
		{
			miniInfo += ASSAULT_WEAPON;
			miniInfo += "\n";
		}
		else if( HasHeavyWeapon() )
		{
			miniInfo += HEAVY_WEAPON;
			miniInfo += "\n";
		}
		else if( HasRapidFireWeapon() )
		{
			miniInfo += RAPIDFIRE_WEAPON;
			miniInfo += "\n";
		}
		else if( HasPistol() )
		{
			miniInfo += PISTOL;
			miniInfo += "\n";
		}
			
		miniInfo += "  ";
		miniInfo += "Firearm strength: ";
		miniInfo += NumberToString( fireArmStrength );
		miniInfo += "\n";

		miniInfo += "  ";
		miniInfo += "Ballistic skill: ";
		miniInfo += NumberToString( ballisticSkill );
		miniInfo += "\n";

		miniInfo += "  ";
		miniInfo += "Firing range: ";
		miniInfo += NumberToString( firingRange );
		miniInfo += "\n";

		miniInfo += "  ";
		miniInfo += "Rate of Fire: ";
		miniInfo += NumberToString( rateOfFire );
		miniInfo += "\n";

		miniInfo += "  ";
		miniInfo += "Armor piercing: ";
		miniInfo += NumberToString( armorPiercing );
		miniInfo += "\n\n";


		miniInfo += "  ";
		miniInfo += "Toughness: ";
		miniInfo += NumberToString( toughness );
		miniInfo += "\n";

		miniInfo += "  ";
		miniInfo += "Wounds: ";
		miniInfo += NumberToString( wounds- woundsInflicted ) + std::string(" / " ) + NumberToString( wounds );
		miniInfo += "\n";

		miniInfo += "  ";
		miniInfo += "Save: ";
		miniInfo += NumberToString( save );
		miniInfo += "\n\n";


		miniInfo += "  ";
		miniInfo += "Movement speed: ";
		miniInfo += NumberToString( movementSpeed+runningRangeBonus );
		miniInfo += "\n\n";

		return miniInfo;
	}
//
/****************************************************************/

	Vector3 Mini::CalculateHeight (Vector3 const &openGLposition)
	{
		assert( heightMap != NULL );


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
/****************************************************************/

	void Mini::Save (char* filename)
	{
		std::fstream file;
		file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);

		SaveMiniName(file);
		SaveMiniCharacteristics(file);
	
		MiniModel::Save(file);

		file.close();
	}
//
		void Mini::SaveMiniName            (std::fstream &file)
		{
			file << "#Mini_name:" << "\t" << miniName << "\n\n";
		}
//
		void Mini::SaveMiniCharacteristics (std::fstream &file)
		{
			SaveMiniPoints(file);
			SaveWeaponData(file);
			SaveFireArmData(file);
			SaveEnduranceData(file);
			SaveMovementSpeed(file);
		}

		void Mini::SaveMiniPoints    (std::fstream &file)
		{
			file << "#Points:" << "\t" << points << "\n\n";
		}

		void Mini::SaveWeaponData    (std::fstream &file)
		{
			file << "#Weapon_type:"     << "\t" << weaponType     << "\n"
				 << "#Weapon_strength:" << "\t" << weaponStrength << "\n"
				 << "#Weapon_skill:"    << "\t" << weaponSkill    << "\n"
				 << "#Initiative:"      << "\t" << initiative     << "\n"
				 << "#Attacks:"         << "\t" << attacks        << "\n\n";
		}

		void Mini::SaveFireArmData   (std::fstream &file)
		{
			file << "#FireArm_type:"     << "\t" << fireArmType     << "\n"
				 << "#FireArm_strength:" << "\t" << fireArmStrength << "\n"
				 << "#Ballistic_skill:"  << "\t" << ballisticSkill  << "\n"
				 << "#Firing_range:"     << "\t" << firingRange     << "\n"
				 << "#Rate_of_fire:"     << "\t" << rateOfFire      << "\n"
				 << "#Armor_piercing:"   << "\t" << armorPiercing   << "\n\n";
		}

		void Mini::SaveEnduranceData (std::fstream &file)
		{
			file << "#Toughness:" << "\t" << toughness << "\n"
				 << "#Wounds:"    << "\t" << wounds    << "\n"
				 << "#Save:"      << "\t" << save      << "\n\n";
		}

		void Mini::SaveMovementSpeed (std::fstream &file)
		{
			file << "#Movement_speed:" << "\t" << movementSpeed << "\n\n";
		}
//
	void Mini::Load (char* filename)
	{
		std::fstream file;
		file.open(filename, std::ios::in | std::ios::binary);
		
		if( file )
		{
			LoadMiniName(file);
			LoadMiniCharacteristics(file);

			MiniModel::Load(file);
		}
		else
		{
			InformAboutFailedAttemptToLoadMiniFile(filename);
		}

		file.close();
		file.clear();
	}
//
		void Mini::LoadMiniName            (std::fstream &file)
		{
			miniName = ReadNewValue(file, '#');
		}
//
		void Mini::LoadMiniCharacteristics (std::fstream &file)
		{
			LoadMiniPoints(file);
			LoadWeaponData(file);
			LoadFireArmData(file);
			LoadEnduranceData(file);
			LoadMovementSpeed(file);
		}

		void Mini::LoadMiniPoints    (std::fstream &file)
		{
			points = atoi( ReadNewValue(file, '#').c_str() );
		}

		void Mini::LoadWeaponData    (std::fstream &file)
		{
			weaponType = ReadNewValue(file, '#');

			weaponStrength = atoi( ReadNewValue(file, '#').c_str() );
			weaponSkill    = atoi( ReadNewValue(file, '#').c_str() );
			initiative     = atoi( ReadNewValue(file, '#').c_str() );
			attacks        = atoi( ReadNewValue(file, '#').c_str() );
		}

		void Mini::LoadFireArmData   (std::fstream &file)
		{
			fireArmType = ReadNewValue(file, '#');

			fireArmStrength = atoi( ReadNewValue(file, '#').c_str() );
			ballisticSkill  = atoi( ReadNewValue(file, '#').c_str() );
			firingRange     = atoi( ReadNewValue(file, '#').c_str() );
			rateOfFire      = atoi( ReadNewValue(file, '#').c_str() );
			armorPiercing   = atoi( ReadNewValue(file, '#').c_str() );
		}

		void Mini::LoadEnduranceData (std::fstream &file)
		{
			toughness = atoi( ReadNewValue(file, '#').c_str() );
			wounds    = atoi( ReadNewValue(file, '#').c_str() );
			save      = atoi( ReadNewValue(file, '#').c_str() );
		}

		void Mini::LoadMovementSpeed (std::fstream &file)
		{
			movementSpeed = atoi( ReadNewValue(file, '#').c_str() );
		}
//
		void Mini::InformAboutFailedAttemptToLoadMiniFile (std::string filename)
		{
			char errorMessage[MAX_PATH];

			strcpy_s( errorMessage, MAX_PATH, "Failed to load a file containing mini data." );
			strcat_s( errorMessage, MAX_PATH, "\n\n" );
			strcat_s( errorMessage, MAX_PATH, "Path to file:" );
			strcat_s( errorMessage, MAX_PATH, "\t" );
			strcat_s( errorMessage, MAX_PATH, filename.c_str() );

			MessageBox(NULL, errorMessage, "Mini::Load(char *filename) error!", MB_OK);
		}
//
//

	#undef STANDARD_WEAPON
	#undef POWER_WEAPON
	#undef POWER_FIST
	#undef RENDING_WEAPON
			
	#undef HEAVY_WEAPON
	#undef ASSAULT_WEAPON
	#undef RAPIDFIRE_WEAPON
	#undef PISTOL


