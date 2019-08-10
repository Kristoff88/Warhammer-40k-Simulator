	#include "GlobalHeaders.h"
	
	#include "UpperClass.h"


	UpperClass& UpperClass::operator= (UpperClass const &upper)
	{
		Control::operator =(upper);
		return *this;
	}
//
//
/******************************************************
		Basic managment functions.
******************************************************/

	Control* UpperClass::GetFocusedContent ()
	{
		return focusedContent;
	}
//
	void UpperClass::SetBackground     (unsigned int const &textureID)
	{
		textures[BACKGROUND] = textureID;
		UpdateContent();
	}
//
	void UpperClass::SetBorders        (RECT const &Border)
	{
		Control::SetBorders(Border);
		AdjustContentSize();
	}

	void UpperClass::SetColor          (Color const &color)
	{
		Control::SetColor(color);
		AdjustContentColor();
	}
//
	void UpperClass::SetMaxHeight      (float const &height)
	{
		Control::SetMaxHeight(height);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->SetMaxHeight(height);
		}

		AdjustContentSize();
	}
	void UpperClass::SetMaxWidth       (float const &width)
	{
		Control::SetMaxWidth(width);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->SetMaxWidth(width);
		}

		AdjustContentSize();
	}
//
	void UpperClass::SetTransparency   (bool const &IsTransparent)
	{
		Control::SetTransparency(IsTransparent);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->SetTransparency(IsTransparent);
		}
	}
//
/*****************************************************/

	void UpperClass::UpdateContent ()
	{
		SetCurrentTexture(BACKGROUND+1);
	}
//
/******************************************************
		Standard functions redefinitions.
******************************************************/

	void UpperClass::SetTextures (std::vector <unsigned int> const &textures)
	{
		Control::SetTextures(textures);
		UpdateContent();
	}
	void UpperClass::SetCurrentTexture (const int &TextureState)
	{
		Control::SetCurrentTexture(TextureState);
	}
//
//
	void UpperClass::KeyDown             (int const &keyCode)
	{
		Control::KeyDown(keyCode);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnKeyDown(keyCode);
		}
	}

	void UpperClass::KeyPressed          (int const &keyCode)
	{
		Control::KeyPressed(keyCode);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnKeyPressed(keyCode);
		}
	}

	void UpperClass::KeyUp               (int const &keyCode)
	{
		Control::KeyUp(keyCode);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnKeyUp(keyCode);
		}
	}

//
	void UpperClass::MouseLeftDown       (POINT const &mousePosition)
	{
		Control::MouseLeftDown(mousePosition);

		focusedContent = NULL;

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );

			if( contentArray[i]->OnMouseLeftDown(mousePosition) )
			{
				focusedContent = contentArray[i];
			}
		}
	}

	void UpperClass::MouseMiddleDown     (POINT const &mousePosition)
	{
		Control::MouseMiddleDown(mousePosition);

		focusedContent = NULL;

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );

			if( contentArray[i]->OnMouseMiddleDown(mousePosition) )
			{
				focusedContent = contentArray[i];
			}
		}
	}

	void UpperClass::MouseRightDown      (POINT const &mousePosition)
	{
		Control::MouseRightDown(mousePosition);

		focusedContent = NULL;

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );

			if( contentArray[i]->OnMouseRightDown(mousePosition) )
			{
				focusedContent = contentArray[i];
			}
		}
	}

//
	void UpperClass::MouseLeftPressed    (POINT const &mousePosition)
	{
		Control::MouseLeftPressed(mousePosition);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnMouseLeftPressed(mousePosition);
		}
	}

	void UpperClass::MouseMiddlePressed  (POINT const &mousePosition)
	{
		Control::MouseMiddlePressed(mousePosition);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnMouseMiddlePressed(mousePosition);
		}
	}

	void UpperClass::MouseRightPressed   (POINT const &mousePosition)
	{
		Control::MouseRightPressed(mousePosition);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnMouseRightPressed(mousePosition);
		}
	}

//
	void UpperClass::MouseLeftUp         (POINT const &mousePosition)
	{
		Control::MouseLeftUp(mousePosition);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnMouseLeftUp(mousePosition);
		}
	}

	void UpperClass::MouseMiddleUp       (POINT const &mousePosition)
	{
		Control::MouseMiddleUp(mousePosition);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnMouseMiddleUp(mousePosition);
		}
	}

	void UpperClass::MouseRightUp        (POINT const &mousePosition)
	{
		Control::MouseRightUp(mousePosition);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnMouseRightUp(mousePosition);
		}
	}

//
	void UpperClass::MouseLeftDblClick   (POINT const &mousePosition)
	{
		Control::MouseLeftDblClick(mousePosition);

		focusedContent = NULL;

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );

			if( contentArray[i]->OnMouseLeftDblClick(mousePosition) )
			{
				focusedContent = contentArray[i];
			}
		}
	}

	void UpperClass::MouseMiddleDblClick (POINT const &mousePosition)
	{
		Control::MouseMiddleDblClick(mousePosition);

		focusedContent = NULL;

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );

			if( contentArray[i]->OnMouseMiddleDblClick(mousePosition) )
			{
				focusedContent = contentArray[i];
			}
		}
	}

	void UpperClass::MouseRightDblClick  (POINT const &mousePosition)
	{
		Control::MouseRightDblClick(mousePosition);

		focusedContent = NULL;

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );

			if( contentArray[i]->OnMouseRightDblClick(mousePosition) )
			{
				focusedContent = contentArray[i];
			}
		}
	}

//
	void UpperClass::MouseMoved          (POINT const &mousePosition)
	{
		Control::MouseMoved(mousePosition);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnMouseMoved(mousePosition);
		}
	}

	void UpperClass::MouseDragged        (POINT const &mousePosition)
	{
		Control::MouseDragged(mousePosition);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnMouseDragged(mousePosition);
		}

		AdjustContentSize();
	}

//
	void UpperClass::Scroll              (int const &scrollInfo)
	{
		Control::Scroll(scrollInfo);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnScroll(scrollInfo);
		}
	}
//
	void UpperClass::Collision		     ()
	{
		Control::Collision();
		AdjustContentSize();
	}

//
	void UpperClass::Move                (POINT const &mousePositionDelta)
	{
		Control::Move(mousePositionDelta);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnMoving(mousePositionDelta);
		}
	}

//
	void UpperClass::Resize              (POINT const &mousePosition)
	{
		Control::Resize(mousePosition);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnResizing(mousePosition);
		}

		AdjustContentSize();
	}

//
	void UpperClass::Scale               (float const &scaleMultiplier)
	{
		Control::Scale(scaleMultiplier);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnScaling(scaleMultiplier);
		}

		AdjustContentSize();
	}

//
	void UpperClass::Render              (Color const &GUIColor, bool const &TransparentGUI)
	{
		Control::Render(GUIColor, TransparentGUI);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL );
			contentArray[i]->OnRendering(GUIColor, TransparentGUI);
		}
	}

//
/*****************************************************/

	void UpperClass::Save (std::fstream &file)
	{
		Control::Save(file);

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL);

			char indexAsChar[MAX_PATH];
			_itoa_s(i, indexAsChar, MAX_PATH, 10);

			file << "#__Content_" << indexAsChar << "__#\n\n";
			contentArray[i]->Save(file);
		}
	}

	void UpperClass::Load (std::fstream &file)
	{
		Control::Load(file);

		std::string line;

		for(int i = 0; i < contentArray.size(); i++)
		{
			assert( contentArray[i] != NULL);

			// taking type name
			line = ReadNewValue(file, '#');

			contentArray[i]->Load(file);
		}
	}
//
/*****************************************************/

	UpperClass::UpperClass(): focusedContent(NULL)
	{
		// Pushing empty slot for background texture.
		textures.push_back(0);
	}

	UpperClass::~UpperClass()
	{
		for(int i = 0; i < contentArray.size(); i++)
		{
			delete contentArray[i];
		}

		contentArray.clear();
	}
