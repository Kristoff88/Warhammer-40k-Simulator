#pragma once



	Label* Create_Label_FirearmType ()
	{
		Label *fireArmTypeLabel = new Label;

		Control_SetDefaultSettings(fireArmTypeLabel);
		Label_SetDefaultSettings(fireArmTypeLabel);

		Label_SetBorder(10, 340, fireArmTypeLabel);
		fireArmTypeLabel->SetText("Firearm type:");	
		fireArmTypeLabel->AdjustWidthToCaption();

		return fireArmTypeLabel;
	}
//
	void FirearmType_MouseLeftDown        (Control *ctrl, POINT const &mousePosition)
	{
		ComboBox *firearmTypeComboBox = reinterpret_cast <ComboBox*> (ctrl);


		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( !currentMini )
		{
			return;
		}


		std::string firearmType = firearmTypeComboBox->GetSelection();

		if( !firearmType.empty() )
		{
			if( firearmType == "Heavy_Weapon" )
			{
				currentMini->GiveHeavyWeapon();
			}
			else if( firearmType == "Assault_Weapon" )
			{
				currentMini->GiveAssaultWeapon();
			}
			else if( firearmType == "RapidFiring_Weapon" )
			{
				currentMini->GiveRapidFireWeapon();
			}
			else if( firearmType == "Pistol" )
			{
				currentMini->GivePistol();
			}
			else
			{
				bool FirearmTypeFromComboBoxIsIncorrect = true;
				assert( !FirearmTypeFromComboBoxIsIncorrect );
			}
		}
	}

	inline void SetDefaultFirearmTypes    (ComboBox *fireArmType)
	{
		fireArmType->AddElement("Pistol");
		fireArmType->AddElement("RapidFiring_Weapon");
		fireArmType->AddElement("Assault_Weapon");
		fireArmType->AddElement("Heavy_Weapon");

		fireArmType->SetSelection(0);
	}

	ComboBox* Create_ComboBox_FirearmType ()
	{
		ComboBox *fireArmType = new ComboBox;

		Control_SetDefaultSettings(fireArmType);
		ComboBox_SetDefaultSettings(fireArmType);

		ComboBox_SetBorder(10, 310, fireArmType);
		SetDefaultFirearmTypes(fireArmType);

		fireArmType->SetName(FIREARM_TYPE);

		fireArmType->EventManager.Ptr_MouseLeftDown = FirearmType_MouseLeftDown;

		return fireArmType;
	}
//
//
	Label* Create_Label_FirearmStrength ()
	{
		Label *fireArmStrengthLabel = new Label;

		Control_SetDefaultSettings(fireArmStrengthLabel);
		Label_SetDefaultSettings(fireArmStrengthLabel);

		Label_SetBorder(10, 265, fireArmStrengthLabel);
		fireArmStrengthLabel->SetText("Firearm strength:");
		fireArmStrengthLabel->AdjustWidthToCaption();
		
		return fireArmStrengthLabel;
	}

	void FirearmStrength_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newFirearmStrengthValue = atoi( points->GetText().c_str() );
			currentMini->SetFirearmStrength( newFirearmStrengthValue );
		}
	}

	Edit*  Create_Edit_FirearmStrength  ()
	{
		Edit *fireArmStrength = new Edit;

		Control_SetDefaultSettings(fireArmStrength);
		Edit_SetDefaultSettings(fireArmStrength);

		Edit_SetBorder(10, 240, fireArmStrength);
		fireArmStrength->SetName(FIREARM_STRENGTH);

		fireArmStrength->EventManager.Ptr_KeyDown = FirearmStrength_KeyDown;

		return fireArmStrength;
	}
//
	Label* Create_Label_BallisticSkill ()
	{
		Label *ballisticSkillLabel = new Label;

		Control_SetDefaultSettings(ballisticSkillLabel);
		Label_SetDefaultSettings(ballisticSkillLabel);

		Label_SetBorder(160, 265, ballisticSkillLabel);
		ballisticSkillLabel->SetText("Ballistic skill:");
		ballisticSkillLabel->AdjustWidthToCaption();
		
		return ballisticSkillLabel;
	}

	void BallisticSkill_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newBallisticSkillValue = atoi( points->GetText().c_str() );
			currentMini->SetBallisticSkill( newBallisticSkillValue );
		}
	}

	Edit*  Create_Edit_BallisticSkill  ()
	{
		Edit *ballisticSkill = new Edit;

		Control_SetDefaultSettings(ballisticSkill);
		Edit_SetDefaultSettings(ballisticSkill);

		Edit_SetBorder(160, 240, ballisticSkill);
		ballisticSkill->SetName(BALLISTIC_SKILL);

		ballisticSkill->EventManager.Ptr_KeyDown = BallisticSkill_KeyDown;

		return ballisticSkill;
	}
//
	Label* Create_Label_FiringRange ()
	{
		Label *firingRangeLabel = new Label;

		Control_SetDefaultSettings(firingRangeLabel);
		Label_SetDefaultSettings(firingRangeLabel);

		Label_SetBorder(320, 265, firingRangeLabel);
		firingRangeLabel->SetText("Firing range:");
		firingRangeLabel->AdjustWidthToCaption();
		
		return firingRangeLabel;
	}

	void FiringRange_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newFiringRangeValue = atoi( points->GetText().c_str() );
			currentMini->SetFiringRange( newFiringRangeValue );
		}
	}

	Edit*  Create_Edit_FiringRange  ()
	{
		Edit *firingRange = new Edit;

		Control_SetDefaultSettings(firingRange);
		Edit_SetDefaultSettings(firingRange);

		Edit_SetBorder(320, 240, firingRange);
		firingRange->SetName(FIRING_RANGE);

		firingRange->EventManager.Ptr_KeyDown = FiringRange_KeyDown;

		return firingRange;
	}
//
	Label* Create_Label_RateOfFire ()
	{
		Label *rateOfFireLabel = new Label;

		Control_SetDefaultSettings(rateOfFireLabel);
		Label_SetDefaultSettings(rateOfFireLabel);

		Label_SetBorder(10, 205, rateOfFireLabel);
		rateOfFireLabel->SetText("Rate of fire:");
		rateOfFireLabel->AdjustWidthToCaption();
		
		return rateOfFireLabel;
	}

	void RateOfFire_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newRateOfFireValue = atoi( points->GetText().c_str() );
			currentMini->SetRateOfFire( newRateOfFireValue );
		}
	}

	Edit*  Create_Edit_RateOfFire  ()
	{
		Edit *rateOfFire = new Edit;

		Control_SetDefaultSettings(rateOfFire);
		Edit_SetDefaultSettings(rateOfFire);

		Edit_SetBorder(10, 180, rateOfFire);
		rateOfFire->SetName(RATE_OF_FIRE);

		rateOfFire->EventManager.Ptr_KeyDown = RateOfFire_KeyDown;

		return rateOfFire;
	}
//
	Label* Create_Label_ArmorPiercing ()
	{
		Label *armorPiercingLabel = new Label;

		Control_SetDefaultSettings(armorPiercingLabel);
		Label_SetDefaultSettings(armorPiercingLabel);

		Label_SetBorder(160, 205, armorPiercingLabel);
		armorPiercingLabel->SetText("Armor piercing:");
		armorPiercingLabel->AdjustWidthToCaption();
		
		return armorPiercingLabel;
	}

	void ArmorPiercing_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newArmorPiercingValue = atoi( points->GetText().c_str() );
			currentMini->SetArmorPiercing( newArmorPiercingValue );
		}
	}

	Edit*  Create_Edit_ArmorPiercing  ()
	{
		Edit *armorPiercing = new Edit;

		Control_SetDefaultSettings(armorPiercing);
		Edit_SetDefaultSettings(armorPiercing);

		Edit_SetBorder(160, 180, armorPiercing);
		armorPiercing->SetName(ARMOR_PIERCING);

		armorPiercing->EventManager.Ptr_KeyDown = ArmorPiercing_KeyDown;

		return armorPiercing;
	}