#pragma once


	Label* Create_Label_ModelList ()
	{
		Label *modelListLabel = new Label;

		Control_SetDefaultSettings(modelListLabel);
		Label_SetDefaultSettings(modelListLabel);

		Label_SetBorder(10, 620, modelListLabel);
		modelListLabel->SetText("Model list:");	
		modelListLabel->AdjustWidthToCaption();

		return modelListLabel;
	}
//
	void ModelList_MouseLeftDown         (Control *ctrl, POINT const &mousePosition)
	{
		ComboBox *modelList = reinterpret_cast <ComboBox*> (ctrl);
		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( currentMini )
		{
			std::string modelName = modelList->GetSelection();

			if( !modelName.empty() )
			{
				MiniModel_Data *modelData = MiniDataManager::Singleton().GetModel(modelName);
				currentMini->CopyModelData( modelData );
			}
		}
	}

	inline void LoadModelsFromModelArray (ComboBox *modelList)
	{
		int modelCount = MiniDataManager::Singleton().GetModelCount();

		for(int i = 0; i < modelCount; i++)
		{
			modelList->AddElement( MiniDataManager::Singleton().GetModel(i)->GetModelName() );
		}
	}

	ComboBox* Create_ComboBox_ModelList  ()
	{
		ComboBox *modelList = new ComboBox;

		Control_SetDefaultSettings(modelList);
		ComboBox_SetDefaultSettings(modelList);


		ComboBox_SetBorder(10, 590, modelList);
		LoadModelsFromModelArray(modelList);

		modelList->SetName(MODEL_LIST);

		modelList->EventManager.Ptr_MouseLeftDown = ModelList_MouseLeftDown;

		return modelList;
	}