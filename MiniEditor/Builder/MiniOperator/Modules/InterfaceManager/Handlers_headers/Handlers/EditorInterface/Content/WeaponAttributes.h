#pragma once



	Label* Create_Label_WeaponType ()
	{
		Label *weaponTypeLabel = new Label;

		Control_SetDefaultSettings(weaponTypeLabel);
		Label_SetDefaultSettings(weaponTypeLabel);

		Label_SetBorder(10, 540, weaponTypeLabel);
		weaponTypeLabel->SetText("Weapon type:");	
		weaponTypeLabel->AdjustWidthToCaption();

		return weaponTypeLabel;
	}
//
	void WeaponType_MouseLeftDown        (Control *ctrl, POINT const &mousePosition)
	{
		ComboBox *weaponTypeComboBox = reinterpret_cast <ComboBox*> (ctrl);


		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( !currentMini )
		{
			return;
		}



		std::string weaponType = weaponTypeComboBox->GetSelection();

		if( !weaponType.empty() )
		{
			if( weaponType == "Standard_Weapon" )
			{
				currentMini->GiveStandardWeapon();
			}
			else if( weaponType == "Power_Weapon" )
			{
				currentMini->GivePowerWeapon();
			}
			else if( weaponType == "Power_Fist" )
			{
				currentMini->GivePowerFist();
				currentMini->SetInitiative(1);
				FillAllControlsWithCurrentMiniData();
			}
			else if( weaponType == "Rending_Weapon" )
			{
				currentMini->GiveRendingWeapon();
			}
			else
			{
				bool WeaponTypeFromComboBoxIsIncorrect = true;
				assert( !WeaponTypeFromComboBoxIsIncorrect );
			}
		}
	}

	inline void SetDefaultWeaponTypes    (ComboBox *weaponType)
	{
		weaponType->AddElement("Rending_Weapon");
		weaponType->AddElement("Power_Fist");
		weaponType->AddElement("Power_Weapon");
		weaponType->AddElement("Standard_Weapon");

		weaponType->SetSelection(0);
	}

	ComboBox* Create_ComboBox_WeaponType ()
	{
		ComboBox *weaponType = new ComboBox;

		Control_SetDefaultSettings(weaponType);
		ComboBox_SetDefaultSettings(weaponType);

		ComboBox_SetBorder(10, 510, weaponType);
		SetDefaultWeaponTypes(weaponType);

		weaponType->SetName(WEAPON_TYPE);

		weaponType->EventManager.Ptr_MouseLeftDown = WeaponType_MouseLeftDown;

		return weaponType;
	}
//
//
	Label* Create_Label_WeaponStrength ()
	{
		Label *weaponStrengthLabel = new Label;

		Control_SetDefaultSettings(weaponStrengthLabel);
		Label_SetDefaultSettings(weaponStrengthLabel);

		Label_SetBorder(10, 465, weaponStrengthLabel);
		weaponStrengthLabel->SetText("Weapon strength:");
		weaponStrengthLabel->AdjustWidthToCaption();
		
		return weaponStrengthLabel;
	}

	void WeaponStrength_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newWeaponStrengthValue = atoi( points->GetText().c_str() );
			currentMini->SetWeaponStrength( newWeaponStrengthValue );
		}
	}
	Edit*  Create_Edit_WeaponStrength  ()
	{
		Edit *weaponStrength = new Edit;

		Control_SetDefaultSettings(weaponStrength);
		Edit_SetDefaultSettings(weaponStrength);

		Edit_SetBorder(10, 440, weaponStrength);
		weaponStrength->SetName(WEAPON_STRENGTH);

		weaponStrength->EventManager.Ptr_KeyDown = WeaponStrength_KeyDown;

		return weaponStrength;
	}
//
	Label* Create_Label_WeaponSkill ()
	{
		Label *weaponSkillLabel = new Label;

		Control_SetDefaultSettings(weaponSkillLabel);
		Label_SetDefaultSettings(weaponSkillLabel);

		Label_SetBorder(160, 465, weaponSkillLabel);
		weaponSkillLabel->SetText("Weapon skill:");
		weaponSkillLabel->AdjustWidthToCaption();
		
		return weaponSkillLabel;
	}

	void WeaponSkill_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newWeaponSkillValue = atoi( points->GetText().c_str() );
			currentMini->SetWeaponSkill( newWeaponSkillValue );
		}
	}
	Edit*  Create_Edit_WeaponSkill  ()
	{
		Edit *weaponSkill = new Edit;

		Control_SetDefaultSettings(weaponSkill);
		Edit_SetDefaultSettings(weaponSkill);

		Edit_SetBorder(160, 440, weaponSkill);
		weaponSkill->SetName(WEAPON_SKILL);

		weaponSkill->EventManager.Ptr_KeyDown = WeaponSkill_KeyDown;

		return weaponSkill;
	}
//
	Label* Create_Label_Initiative ()
	{
		Label *initiativeLabel = new Label;

		Control_SetDefaultSettings(initiativeLabel);
		Label_SetDefaultSettings(initiativeLabel);

		Label_SetBorder(10, 405, initiativeLabel);
		initiativeLabel->SetText("Initiative:");
		initiativeLabel->AdjustWidthToCaption();
		
		return initiativeLabel;
	}

	void Initiative_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			if( !currentMini->HasPowerFist() )
			{
				int newInitiativeValue = atoi( points->GetText().c_str() );
				currentMini->SetInitiative( newInitiativeValue );
			}
			else
				points->SetText("1");
		}
	}
	Edit*  Create_Edit_Initiative  ()
	{
		Edit *initiative = new Edit;

		Control_SetDefaultSettings(initiative);
		Edit_SetDefaultSettings(initiative);

		Edit_SetBorder(10, 380, initiative);
		initiative->SetName(INITIATIVE);

		initiative->EventManager.Ptr_KeyDown = Initiative_KeyDown;

		return initiative;
	}
//
	Label* Create_Label_Attacks ()
	{
		Label *attacksLabel = new Label;

		Control_SetDefaultSettings(attacksLabel);
		Label_SetDefaultSettings(attacksLabel);

		Label_SetBorder(160, 405, attacksLabel);
		attacksLabel->SetText("Attacks:");
		attacksLabel->AdjustWidthToCaption();
		
		return attacksLabel;
	}

	void Attacks_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newAttacksValue = atoi( points->GetText().c_str() );
			currentMini->SetAttacks( newAttacksValue );
		}
	}
	Edit*  Create_Edit_Attacks  ()
	{
		Edit *attacks = new Edit;

		Control_SetDefaultSettings(attacks);
		Edit_SetDefaultSettings(attacks);

		Edit_SetBorder(160, 380, attacks);
		attacks->SetName(ATTACKS);

		attacks->EventManager.Ptr_KeyDown = Attacks_KeyDown;

		return attacks;
	}