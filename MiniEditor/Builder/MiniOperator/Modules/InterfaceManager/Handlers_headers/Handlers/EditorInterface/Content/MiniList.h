#pragma once



	Label* Create_Label_MiniList ()
	{
		Label *miniListLabel = new Label;

		Control_SetDefaultSettings(miniListLabel);
		Label_SetDefaultSettings(miniListLabel);

		Label_SetBorder(10, 700, miniListLabel);
		miniListLabel->SetText("Mini list:");		
		miniListLabel->AdjustWidthToCaption();

		return miniListLabel;
	}
//
	void MiniList_MouseLeftDown        (Control *ctrl, POINT const &mousePosition)
	{
		ComboBox *miniList = reinterpret_cast <ComboBox*> (ctrl);

		std::string miniName = miniList->GetSelection();

		if( !miniName.empty() )
		{
			InterfaceManager::Singleton().SetCurrentMini(miniName);
			FillAllControlsWithCurrentMiniData();
		}
		else
			InterfaceManager::Singleton().SetCurrentMini("");
	}

	inline void LoadMinisFromMiniArray (ComboBox *miniList)
	{
		int miniCount = MiniDataManager::Singleton().GetMiniCount();

		for(int i = 0; i < miniCount; i++)
		{
			miniList->AddElement( MiniDataManager::Singleton().GetMini(i)->GetMiniName() );
		}

		InterfaceManager::Singleton().SetCurrentMini( miniList->GetSelection() );
	}

	ComboBox* Create_ComboBox_MiniList ()
	{
		ComboBox *miniList = new ComboBox;

		Control_SetDefaultSettings(miniList);
		ComboBox_SetDefaultSettings(miniList);

		ComboBox_SetBorder(10, 670, miniList);
		LoadMinisFromMiniArray(miniList);

		miniList->EventManager.Ptr_MouseLeftDown = MiniList_MouseLeftDown;
		miniList->SetName(MINI_LIST);

		return miniList;
	}