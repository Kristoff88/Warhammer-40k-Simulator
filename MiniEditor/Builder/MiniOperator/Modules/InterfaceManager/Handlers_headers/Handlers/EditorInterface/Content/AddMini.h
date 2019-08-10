#pragma once


	void AddMini_MouseLeftDown     (Control *ctrl, POINT const &mousePosition)
	{
		Button *addMini = reinterpret_cast <Button*> (ctrl);


		Handler *editorInterface = reinterpret_cast <Handler*> ( GUI::Singleton().Controls.GetControl(EDITOR_INTERFACE) );
		editorInterface->SetReadingInput(false);

		Handler *miniCreator = reinterpret_cast <Handler*> ( GUI::Singleton().Controls.GetControl(MINI_CREATOR) );
		miniCreator->SetRendering(true);


		addMini->Unpress();
	}

	inline void AddMini_SetTexture (Button *addMini)
	{
		std::vector <unsigned int> textures;

		textures.push_back( GUI::Singleton().Data.GetTexture("plus") );
		textures.push_back( GUI::Singleton().Data.GetTexture("plus_pressed") );

		addMini->SetTextures(textures);
		addMini->SetCurrentTexture(1);
		addMini->SetPressTexture(2);
		addMini->SetUnpressTexture(1);
	}

	Button* Create_Button_AddMini  ()
	{
		Button *addMini = new Button;

		Control_SetDefaultSettings(addMini);

		Button_SetBorder(330, 680, addMini);
		AddMini_SetTexture(addMini);

		addMini->EventManager.Ptr_MouseLeftDown = AddMini_MouseLeftDown;

		return addMini;
	}