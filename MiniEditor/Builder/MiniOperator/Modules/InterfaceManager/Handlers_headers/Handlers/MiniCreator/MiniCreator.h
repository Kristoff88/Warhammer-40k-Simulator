#pragma once



	


	void HideMiniCreator ()
	{
		Handler *miniCreator = reinterpret_cast <Handler*> ( GUI::Singleton().Controls.GetControl(MINI_CREATOR) );
		miniCreator->SetRendering(false);

		Handler *editorInterface = reinterpret_cast <Handler*> ( GUI::Singleton().Controls.GetControl(EDITOR_INTERFACE) );
		editorInterface->SetReadingInput(true);
	}
//
//


	Label* Create_Label_MiniName ()
	{
		Label *miniNameLabel = new Label;

		Control_SetDefaultSettings(miniNameLabel);
		Label_SetDefaultSettings(miniNameLabel);

		Label_SetBorder(30, 140, miniNameLabel);
		miniNameLabel->SetText("Mini name:");
		miniNameLabel->AdjustWidthToCaption();
		
		return miniNameLabel;
	}

	void MiniName_KeyDown        (Control *ctrl, int const &keyCode)
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

	Edit*  Create_Edit_MiniName ()
	{
		Edit *miniName = new Edit;

		Control_SetDefaultSettings(miniName);

		Font font = GenerateFont_Edit();
		miniName->SetFont(font);

		miniName->SetColor( Color(0,0.45,0.9,1) );

		RECT border;

		border.left = 30;
		border.top = 110;

		border.right = border.left + 325;
		border.bottom = border.top - 30;

		miniName->SetBorders(border);

		miniName->SetName(MINI_NAME);

		miniName->EventManager.Ptr_KeyDown = MiniName_KeyDown;

		return miniName;
	}
//
//
	inline std::string GetNewMiniName ()
	{
		Edit *miniName = reinterpret_cast <Edit*> ( GUI::Singleton().Controls.GetControl(MINI_NAME) );

		return miniName->GetText();
	}
//
	inline void GetAndCopyModelDataToNewMini (std::string const &newMiniName)
	{
		ComboBox *modelList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(MODEL_LIST) );

		std::string modelName = modelList->GetSelection();

		if( !modelName.empty() )
		{
			Mini           *mini      = MiniDataManager::Singleton().GetMini(newMiniName);
			MiniModel_Data *modelData = MiniDataManager::Singleton().GetModel(modelName);

			mini->CopyModelData(modelData);
		}
	}

	inline void InsertNewMiniNameToMiniList  (std::string const &newMiniName)
	{
		ComboBox *miniList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(MINI_LIST) );

		miniList->AddElement(newMiniName);
	}
//
	void Accept_MouseLeftDown     (Control *ctrl, POINT const &mousePosition)
	{
		Button *accept = reinterpret_cast <Button*> (ctrl);

		try
		{
			std::string newMiniName = GetNewMiniName();

			if( newMiniName.empty() )
			{
				MessageBox(NULL, "New name is empty!", "Can't create mini.", MB_OK);
				accept->Unpress();
			}
			else
			{
				MiniDataManager::Singleton().AddMini(newMiniName);
				GetAndCopyModelDataToNewMini(newMiniName);
				InsertNewMiniNameToMiniList(newMiniName);
				
				HideMiniCreator();
				accept->Unpress();
			}
		}
		catch(std::string error)
		{
			MessageBox(NULL, error.c_str(), "Creating new mini - error.", MB_OK);

			HideMiniCreator();
			accept->Unpress();
		}
	}
//
//
	inline void Accept_SetTexture (Button *accept)
	{
		std::vector <unsigned int> textures;

		textures.push_back( GUI::Singleton().Data.GetTexture("accept") );
		textures.push_back( GUI::Singleton().Data.GetTexture("accept_pressed") );

		accept->SetTextures(textures);
		accept->SetCurrentTexture(1);
		accept->SetPressTexture(2);
		accept->SetUnpressTexture(1);
	}

	Button* Create_Button_Accept  ()
	{
		Button *accept = new Button;

		Control_SetDefaultSettings(accept);

		Button_SetBorder(30, 70, accept);
		Accept_SetTexture(accept);

		accept->EventManager.Ptr_MouseLeftDown = Accept_MouseLeftDown;

		return accept;
	}
//
//
	void Cancel_MouseLeftDown     (Control *ctrl, POINT const &mousePosition)
	{
		Button *cancel = reinterpret_cast <Button*> (ctrl);
		
		HideMiniCreator();
		cancel->Unpress();
	}

	inline void Cancel_SetTexture (Button *cancel)
	{
		std::vector <unsigned int> textures;

		textures.push_back( GUI::Singleton().Data.GetTexture("cancel") );
		textures.push_back( GUI::Singleton().Data.GetTexture("cancel_pressed") );

		cancel->SetTextures(textures);
		cancel->SetCurrentTexture(1);
		cancel->SetPressTexture(2);
		cancel->SetUnpressTexture(1);
	}

	Button* Create_Button_Cancel  ()
	{
		Button *cancel = new Button;

		Control_SetDefaultSettings(cancel);

		Button_SetBorder(90, 70, cancel);
		Cancel_SetTexture(cancel);

		cancel->EventManager.Ptr_MouseLeftDown = Cancel_MouseLeftDown;

		return cancel;
	}
//
//
	inline void MiniCreator_SetBorder       (Handler *miniCreator)
	{
		RECT border;

		border.left = 0;
		border.top = 150;
		border.right = 400;
		border.bottom = 0;
		
		miniCreator->SetBorders(border);
	}

	inline void MiniCreator_SetTexture      (Handler *miniCreator)
	{
		std::vector <unsigned int> textures;

		textures.push_back( GUI::Singleton().Data.GetTexture("Element_Unpressed") );

		miniCreator->SetTextures(textures);
		miniCreator->SetCurrentTexture(1);
	}

	inline void MiniCreator_ShiftToPosition (Handler *miniCreator)
	{
		POINT shift;

		shift.x = 450;
		shift.y = 500;

		miniCreator->Move(shift);
	}
//
	Handler* Create_MiniCreator ()
	{
		Handler *miniCreator = new Handler;

		Control_SetDefaultSettings(miniCreator);
		MiniCreator_SetBorder(miniCreator);
		MiniCreator_SetTexture(miniCreator);

		miniCreator->SetName(MINI_CREATOR);


		miniCreator->AddHandling( Create_Label_MiniName() );
		miniCreator->AddHandling( Create_Edit_MiniName() );

		miniCreator->AddHandling( Create_Button_Accept() );
		miniCreator->AddHandling( Create_Button_Cancel() );


		MiniCreator_ShiftToPosition(miniCreator);
		miniCreator->SetRendering(false);
		
		return miniCreator;
	}