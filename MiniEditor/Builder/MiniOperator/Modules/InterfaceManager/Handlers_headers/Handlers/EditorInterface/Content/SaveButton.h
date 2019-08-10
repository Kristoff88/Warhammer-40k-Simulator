#pragma once


	inline bool MiniHasBeenSetInMiniList ()
	{
		ComboBox *miniList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(MINI_LIST) );

		if( miniList->GetSelection() != "" )
		{
			return true;
		}
		else
		{
			assert( miniList->GetElementCount() == 0 );
			return false;
		}
	}

	inline void InformAboutEmptyMiniList ()
	{
		MessageBox(NULL, "There is nothing to save. Mini list is empty.", "Saving operation error.", MB_OK);
	}
//
	inline bool ModelHasBeenSetInModelList ()
	{
		ComboBox *modelList = reinterpret_cast <ComboBox*> ( GUI::Singleton().Controls.GetControl(MODEL_LIST) );

		if( modelList->GetSelection() != "" )
		{
			return true;
		}
		else
		{
			assert( modelList->GetElementCount() == 0 );
			return false;
		}
	}

	inline void InformAboutEmptyModelList  ()
	{
		MessageBox(NULL, "Model hasn't been set in Model list.", "Saving operation error.", MB_OK);
	}
//
	void SaveMini_MouseLeftDown (Control *ctrl, POINT const &mousePosition)
	{
		Button *saveMini = reinterpret_cast <Button*> (ctrl);


		Mini *currentMini = InterfaceManager::Singleton().GetCurrentMini();

		if( !currentMini )
		{
			MessageBox(NULL, "There is nothing to save!", "Saving operation error.", MB_OK);
			return;
		}


		try
		{
			if( MiniHasBeenSetInMiniList() )
			{
				assert( currentMini->GetMiniName() == reinterpret_cast <ComboBox*> (GUI::Singleton().Controls.GetControl(MINI_LIST))->GetSelection() );
			}
			else
			{
				assert( currentMini == NULL );
				InformAboutEmptyMiniList();
				saveMini->Unpress();
				return;
			}

			if( ModelHasBeenSetInModelList() )
			{
				assert( currentMini->GetModelName() == reinterpret_cast <ComboBox*> (GUI::Singleton().Controls.GetControl(MODEL_LIST))->GetSelection() );
			}
			else
			{
				InformAboutEmptyModelList();
				saveMini->Unpress();
				return;
			}


			char locationPath[MAX_PATH];

			strcpy_s( locationPath, MAX_PATH, MiniDataManager::Singleton().GetMinisFolderPath().c_str() );
			strcat_s( locationPath, MAX_PATH, currentMini->GetMiniName().c_str() );
			strcat_s( locationPath, MAX_PATH, ".mini" );

			currentMini->Save(locationPath);
			MessageBox(NULL, "Mini saved correctly!", "Saving operation.", MB_OK);
			saveMini->Unpress();
		}
		catch(std::string error)
		{
			MessageBox(NULL, error.c_str(), "Saving operation error.", MB_OK);
			saveMini->Unpress();
		}
	}
//
//
	inline void SaveMini_SetTexture (Button *saveMini)
	{
		std::vector <unsigned int> textures;

		textures.push_back( GUI::Singleton().Data.GetTexture("save") );
		textures.push_back( GUI::Singleton().Data.GetTexture("save_pressed") );

		saveMini->SetTextures(textures);
		saveMini->SetCurrentTexture(1);
		saveMini->SetPressTexture(2);
		saveMini->SetUnpressTexture(1);
	}

	Button* Create_Button_SaveMini  ()
	{
		Button *saveMini = new Button;

		Control_SetDefaultSettings(saveMini);

		Button_SetBorder(390, 60, saveMini);
		SaveMini_SetTexture(saveMini);

		saveMini->EventManager.Ptr_MouseLeftDown = SaveMini_MouseLeftDown;

		return saveMini;
	}