#pragma once


	inline std::string ComposeRemovementConfirmation (std::string const &miniName)
	{
		std::string removementConfirmation;

		removementConfirmation  = "Are you sure you want to remove \"";
		removementConfirmation += miniName.c_str();
		removementConfirmation += "\"?";

		return removementConfirmation;
	}
//
//
	inline void RemoveMiniNameFromMiniList (std::string const &miniName)
	{
		ComboBox *miniList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(MINI_LIST) );

		miniList->RemoveElement(miniName);
		
		if( miniList->GetElementCount() )
		{
			miniList->SetSelection(0);
		}
	}

	inline void RemoveMiniFromDataManager  (std::string const &miniName)
	{
		MiniDataManager::Singleton().RemoveMini(miniName);
	}
//
	inline void PerformMiniRemovement (std::string const &miniName)
	{
		RemoveMiniNameFromMiniList(miniName);
		RemoveMiniFromDataManager(miniName);
	}
//
//
	void RemoveMini_MouseLeftDown     (Control *ctrl, POINT const &mousePosition)
	{
		Button *removeMini = reinterpret_cast <Button*> (ctrl);
		ComboBox *miniList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(MINI_LIST) );


		try
		{
			std::string miniName = miniList->GetSelection();

			if( !miniName.empty() )
			{
				int result = MessageBox(NULL, ComposeRemovementConfirmation(miniName).c_str(), "Removing mini.", MB_YESNO);

				if( result == IDYES )
				{
					PerformMiniRemovement(miniName);
					FillAllControlsWithCurrentMiniData();
				}
			}
			else
				throw std::string("There is nothing to remove!");
		}
		catch(std::string error)
		{
			MessageBox(NULL, error.c_str(), "Remove mini - error.", MB_OK);
		}


		removeMini->Unpress();
	}
//
//
	inline void RemoveMini_SetTexture (Button *removeMini)
	{
		std::vector <unsigned int> textures;

		textures.push_back( GUI::Singleton().Data.GetTexture("bin") );
		textures.push_back( GUI::Singleton().Data.GetTexture("bin_pressed") );

		removeMini->SetTextures(textures);
		removeMini->SetCurrentTexture(1);
		removeMini->SetPressTexture(2);
		removeMini->SetUnpressTexture(1);
	}

	Button* Create_Button_RemoveMini  ()
	{
		Button *removeMini = new Button;

		Control_SetDefaultSettings(removeMini);

		Button_SetBorder(390, 680, removeMini);
		RemoveMini_SetTexture(removeMini);

		removeMini->EventManager.Ptr_MouseLeftDown = RemoveMini_MouseLeftDown;

		return removeMini;
	}