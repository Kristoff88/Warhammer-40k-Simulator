	#include "GlobalHeaders.h"

	#include "Mini.h"


	#define STANDARD_WEAPON  ("Standard_Weapon")
	#define POWER_WEAPON     ("Power_Weapon")	// Ignores save.
	#define POWER_FIST       ("Power_Fist")		// Doubles strength, gives 1 inititative, ignores armor.
	#define RENDING_WEAPON   ("Rending_Weapon")	// If hits, automatically causes a wound + ignores armour, if rolled 6.
			
	#define HEAVY_WEAPON	 ("Heavy_Weapon")		// Can't shoot, if moved.
	#define ASSAULT_WEAPON	 ("Assault_Weapon")		// Can shoot, if moved + can assault after shooting.
	#define RAPIDFIRE_WEAPON ("RapidFiring_Weapon")	// Can't normally shoot, if moved. Up to 12 inches it can always (even if moved) twice.
	#define PISTOL			 ("Pistol")				// Can shoot, before assaulting.


	

	Mini::Mini  (std::string name)
	{
		miniName = name;
		SetDefaultMiniCharacteristics();
	}

	Mini::Mini  (std::string name, MiniModel_Data *model)
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

			movementSpeed = 1;
		}
//
	Mini::~Mini ()
	{}
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
//
	int  Mini::GetMovementSpeed ()
	{
		return movementSpeed;
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


