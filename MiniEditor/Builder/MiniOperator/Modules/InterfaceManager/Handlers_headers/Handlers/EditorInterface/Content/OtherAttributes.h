#pragma once

	extern std::string NumberToString(int number);


	Label* Create_Label_Points ()
	{
		Label *pointsLabel = new Label;

		Control_SetDefaultSettings(pointsLabel);
		Label_SetDefaultSettings(pointsLabel);

		Label_SetBorder(330, 620, pointsLabel);
		pointsLabel->SetText("Points:");
		pointsLabel->AdjustWidthToCaption();
		
		return pointsLabel;
	}

	void Points_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newPointsValue = atoi( points->GetText().c_str() );
			currentMini->SetPoints( newPointsValue );
		}
	}

	Edit*  Create_Edit_Points  ()
	{
		Edit *points = new Edit;

		Control_SetDefaultSettings(points);
		Edit_SetDefaultSettings(points);
		points->SetMaxCharacterCount(3);

		Edit_SetBorder(330, 590, points);
		points->SetName(POINTS);

		points->EventManager.Ptr_KeyDown = Points_KeyDown;

		return points;
	}
//
/****************************************************************/

	Label* Create_Label_Toughness ()
	{
		Label *toughnessLabel = new Label;

		Control_SetDefaultSettings(toughnessLabel);
		Label_SetDefaultSettings(toughnessLabel);

		Label_SetBorder(10, 140, toughnessLabel);
		toughnessLabel->SetText("Toughness:");
		toughnessLabel->AdjustWidthToCaption();
		
		return toughnessLabel;
	}

	void Toughness_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newToughnessValue = atoi( points->GetText().c_str() );
			currentMini->SetToughness( newToughnessValue );
		}
	}
	Edit*  Create_Edit_Toughness  ()
	{
		Edit *toughness = new Edit;

		Control_SetDefaultSettings(toughness);
		Edit_SetDefaultSettings(toughness);

		Edit_SetBorder(10, 115, toughness);
		toughness->SetName(TOUGHNESS);

		toughness->EventManager.Ptr_KeyDown = Toughness_KeyDown;

		return toughness;
	}
//
	Label* Create_Label_Wounds ()
	{
		Label *woundsLabel = new Label;

		Control_SetDefaultSettings(woundsLabel);
		Label_SetDefaultSettings(woundsLabel);

		Label_SetBorder(160, 140, woundsLabel);
		woundsLabel->SetText("Wounds:");
		woundsLabel->AdjustWidthToCaption();
		
		return woundsLabel;
	}

	void Wounds_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newWoundsValue = atoi( points->GetText().c_str() );
			currentMini->SetWounds( newWoundsValue );
		}
	}

	Edit*  Create_Edit_Wounds  ()
	{
		Edit *wounds = new Edit;

		Control_SetDefaultSettings(wounds);
		Edit_SetDefaultSettings(wounds);

		Edit_SetBorder(160, 115, wounds);
		wounds->SetName(WOUNDS);

		wounds->EventManager.Ptr_KeyDown = Wounds_KeyDown;

		return wounds;
	}
//
	Label* Create_Label_Save ()
	{
		Label *saveLabel = new Label;

		Control_SetDefaultSettings(saveLabel);
		Label_SetDefaultSettings(saveLabel);

		Label_SetBorder(310, 140, saveLabel);
		saveLabel->SetText("Save:");
		saveLabel->AdjustWidthToCaption();
		
		return saveLabel;
	}

	void Save_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newSaveValue = atoi( points->GetText().c_str() );
			currentMini->SetSave( newSaveValue );
		}
	}
	Edit* Create_Edit_Save   ()
	{
		Edit *save = new Edit;

		Control_SetDefaultSettings(save);
		Edit_SetDefaultSettings(save);

		Edit_SetBorder(310, 115, save);
		save->SetName(SAVE);

		save->EventManager.Ptr_KeyDown = Save_KeyDown;

		return save;
	}
//
/****************************************************************/

	Label* Create_Label_MovementSpeed ()
	{
		Label *movementSpeedLabel = new Label;

		Control_SetDefaultSettings(movementSpeedLabel);
		Label_SetDefaultSettings(movementSpeedLabel);

		Label_SetBorder(10, 80, movementSpeedLabel);
		movementSpeedLabel->SetText("Movement speed:");
		movementSpeedLabel->AdjustWidthToCaption();

		return movementSpeedLabel;
	}

	void MovementSpeed_KeyDown        (Control *ctrl, int const &keyCode)
	{
		Edit_KeyManager(ctrl, keyCode);

		Edit *points = reinterpret_cast <Edit*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			int newMovementSpeedValue = atoi( points->GetText().c_str() );
			currentMini->SetMovementSpeed( newMovementSpeedValue );
		}
	}
	Edit*  Create_Edit_MovementSpeed  ()
	{
		Edit *movementSpeed = new Edit;

		Control_SetDefaultSettings(movementSpeed);
		Edit_SetDefaultSettings(movementSpeed);

		Edit_SetBorder(10, 55, movementSpeed);
		movementSpeed->SetName(MOVEMENT_SPEED);

		movementSpeed->EventManager.Ptr_KeyDown = MovementSpeed_KeyDown;

		return movementSpeed;
	}


	