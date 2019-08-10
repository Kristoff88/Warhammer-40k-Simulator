#pragma once


	#define GetControl GUI::Singleton().Controls.GetControl




	inline void RefreshModelList (Mini *currentMini)
	{
		if( currentMini )
		{
			ComboBox *modelList = reinterpret_cast <ComboBox*> (GetControl(MODEL_LIST));

			int index = 0, 
				elementCount = modelList->GetElementCount();

			while( index < elementCount )
			{
				if( modelList->GetElement(index) == currentMini->GetModelName() )
				{
					modelList->SetSelection(index);
					break;
				}

				index++;
			}
		}
	}

	inline void RefreshPoints    (Mini *currentMini)
	{
		Edit *points = reinterpret_cast <Edit*> (GetControl(POINTS));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetPoints(), integerAsChar, 10 );

			points->SetText( integerAsChar );
		}
		else
			points->SetText("");
	}
//
	inline void RefreshWeaponTypeList (Mini *currentMini)
	{
		if( currentMini )
		{
			ComboBox *weaponType = reinterpret_cast <ComboBox*> (GetControl(WEAPON_TYPE));

			if( currentMini->HasStandardWeapon() )
			{
				weaponType->SetSelection(0);
			}
			else if( currentMini->HasPowerWeapon() )
			{
				weaponType->SetSelection(1);
			}
			else if( currentMini->HasPowerFist() )
			{
				weaponType->SetSelection(2);
			}
			else if( currentMini->HasRendingWeapon() )
			{
				weaponType->SetSelection(3);
			}
			else
			{
				bool MiniWeaponStateIsIncorrect = true;
				assert( !MiniWeaponStateIsIncorrect );
			}
		}
	}

	inline void RefreshWeaponStrength (Mini *currentMini)
	{
		Edit *weaponStrength = reinterpret_cast <Edit*> (GetControl(WEAPON_STRENGTH));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetWeaponStrength(), integerAsChar, 10 );

			weaponStrength->SetText( integerAsChar );
		}
		else
			weaponStrength->SetText("");
	}

	inline void RefreshWeaponSkill    (Mini *currentMini)
	{
		Edit *weaponSkill = reinterpret_cast <Edit*> (GetControl(WEAPON_SKILL));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetWeaponSkill(), integerAsChar, 10 );

			weaponSkill->SetText( integerAsChar );
		}
		else
			weaponSkill->SetText("");
	}

	inline void RefreshInitiative     (Mini *currentMini)
	{
		Edit *initiative = reinterpret_cast <Edit*> (GetControl(INITIATIVE));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetInitiative(), integerAsChar, 10 );

			initiative->SetText( integerAsChar );
		}
		else
			initiative->SetText("");
	}

	inline void RefreshAttacks        (Mini *currentMini)
	{
		Edit *attacks = reinterpret_cast <Edit*> (GetControl(ATTACKS));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetAttacks(), integerAsChar, 10 );

			attacks->SetText( integerAsChar );
		}
		else
			attacks->SetText("");
	}
//
	inline void RefreshFirearmTypeList (Mini *currentMini)
	{
		if( currentMini )
		{
			ComboBox *firearmType = reinterpret_cast <ComboBox*> (GetControl(FIREARM_TYPE));

			if( currentMini->HasHeavyWeapon() )
			{
				firearmType->SetSelection(0);
			}
			else if( currentMini->HasAssaultWeapon() )
			{
				firearmType->SetSelection(1);
			}
			else if( currentMini->HasRapidFireWeapon() )
			{
				firearmType->SetSelection(2);
			}
			else if( currentMini->HasPistol() )
			{
				firearmType->SetSelection(3);
			}
			else
			{
				bool MiniFirearmStateIsIncorrect = true;
				assert( !MiniFirearmStateIsIncorrect );
			}
		}
	}

	inline void RefreshFirearmStrength (Mini *currentMini)
	{
		Edit *firearmStrength = reinterpret_cast <Edit*> (GetControl(FIREARM_STRENGTH));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetFirearmStrength(), integerAsChar, 10 );

			firearmStrength->SetText( integerAsChar );
		}
		else
			firearmStrength->SetText("");
	}

	inline void RefreshBallistickSkill (Mini *currentMini)
	{
		Edit *ballisticSkill = reinterpret_cast <Edit*> (GetControl(BALLISTIC_SKILL));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetBallisticSkill(), integerAsChar, 10 );

			ballisticSkill->SetText( integerAsChar );
		}
		else
			ballisticSkill->SetText("");
	}

	inline void RefreshFiringRange     (Mini *currentMini)
	{
		Edit *firingRange = reinterpret_cast <Edit*> (GetControl(FIRING_RANGE));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetFiringRange(), integerAsChar, 10 );

			firingRange->SetText( integerAsChar );
		}
		else
			firingRange->SetText("");
	}

	inline void RefreshRateOfFire      (Mini *currentMini)
	{
		Edit *rateOfFire = reinterpret_cast <Edit*> (GetControl(RATE_OF_FIRE));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetRateOfFire(), integerAsChar, 10 );

			rateOfFire->SetText( integerAsChar );
		}
		else
			rateOfFire->SetText("");
	}

	inline void RefreshArmorPiercing   (Mini *currentMini)
	{
		Edit *armorPiercing = reinterpret_cast <Edit*> (GetControl(ARMOR_PIERCING));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetArmorPiercing(), integerAsChar, 10 );

			armorPiercing->SetText( integerAsChar );
		}
		else
			armorPiercing->SetText("");
	}
//
	inline void RefreshToughness (Mini *currentMini)
	{
		Edit *toughness = reinterpret_cast <Edit*> (GetControl(TOUGHNESS));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetToughness(), integerAsChar, 10 );

			toughness->SetText( integerAsChar );
		}
		else
			toughness->SetText("");
	}

	inline void RefreshWounds    (Mini *currentMini)
	{
		Edit *wounds = reinterpret_cast <Edit*> (GetControl(WOUNDS));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetWounds(), integerAsChar, 10 );

			wounds->SetText( integerAsChar );
		}
		else
			wounds->SetText("");
	}

	inline void RefreshSave      (Mini *currentMini)
	{
		Edit *save = reinterpret_cast <Edit*> (GetControl(SAVE));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetSave(), integerAsChar, 10 );

			save->SetText( integerAsChar );
		}
		else
			save->SetText("");
	}
//
	inline void RefreshMovementSpeed (Mini *currentMini)
	{
		Edit *movementSpeed = reinterpret_cast <Edit*> (GetControl(MOVEMENT_SPEED));

		if( currentMini )
		{
			char integerAsChar[MAX_PATH];
			_itoa_s( currentMini->GetMovementSpeed(), integerAsChar, 10 );

			movementSpeed->SetText( integerAsChar );
		}
		else
			movementSpeed->SetText("");
	}
//
//
	void FillAllControlsWithCurrentMiniData ()
	{
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		try
		{
			RefreshModelList(currentMini);
			RefreshPoints(currentMini);

			RefreshWeaponTypeList(currentMini);
			RefreshWeaponStrength(currentMini);
			RefreshWeaponSkill(currentMini);
			RefreshInitiative(currentMini);
			RefreshAttacks(currentMini);

			RefreshFirearmTypeList(currentMini);
			RefreshFirearmStrength(currentMini);
			RefreshBallistickSkill(currentMini);
			RefreshFiringRange(currentMini);
			RefreshRateOfFire(currentMini);
			RefreshArmorPiercing(currentMini);

			RefreshToughness(currentMini);
			RefreshWounds(currentMini);
			RefreshSave(currentMini);

			RefreshMovementSpeed(currentMini);
		}
		catch(std::string error)
		{
			MessageBox(NULL, error.c_str(), "FillAllControlsWithCurrentMiniData() - error...", MB_OK);
		}
	}
//
//


	#undef GetControl