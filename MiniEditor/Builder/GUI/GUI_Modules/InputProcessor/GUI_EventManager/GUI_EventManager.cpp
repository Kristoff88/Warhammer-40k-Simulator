#include "GlobalHeaders.h"

#include "GUI_EventManager.h"

	GUI_EventManager::GUI_EventManager  ()
	{
		Ptr_KeyDown = Ptr_KeyPressed = Ptr_KeyUp = NULL;

		Ptr_MouseLeftDown   = 
		Ptr_MouseMiddleDown = 
		Ptr_MouseRightDown  =

		Ptr_MouseLeftPressed   = 
		Ptr_MouseMiddlePressed = 
		Ptr_MouseRightPressed  =

		Ptr_MouseLeftUp   = 
		Ptr_MouseMiddleUp = 
		Ptr_MouseRightUp  = 

		Ptr_MouseLeftDblClick   = 
		Ptr_MouseMiddleDblClick = 
		Ptr_MouseRightDblClick  = 

		Ptr_MouseMove = 
		Ptr_MouseDrag = NULL;

		Ptr_Scroll  = NULL;

		Ptr_Moving  = NULL;
		Ptr_Scaling = NULL;
	}

	GUI_EventManager::~GUI_EventManager ()
	{}
//
//
	void GUI_EventManager::OnKeyDown             (int const &keyCode)
	{
		if( Ptr_KeyDown )
		{
			Ptr_KeyDown(keyCode);
		}
	}

	void GUI_EventManager::OnKeyPressed          (int const &keyCode)
	{
		if( Ptr_KeyPressed )
		{
			Ptr_KeyPressed(keyCode);
		}
	}

	void GUI_EventManager::OnKeyUp               (int const &keyCode)
	{
		if( Ptr_KeyUp )
		{
			Ptr_KeyUp(keyCode);
		}
	}

//
	void GUI_EventManager::OnMouseLeftDown       (POINT const &mousePosition)
	{
		if( Ptr_MouseLeftDown )
		{
			Ptr_MouseLeftDown(mousePosition);
		}
	}

	void GUI_EventManager::OnMouseMiddleDown     (POINT const &mousePosition)
	{
		if( Ptr_MouseMiddleDown )
		{
			Ptr_MouseMiddleDown(mousePosition);
		}
	}

	void GUI_EventManager::OnMouseRightDown      (POINT const &mousePosition)
	{
		if( Ptr_MouseRightDown )
		{
			Ptr_MouseRightDown(mousePosition);
		}
	}

//
	void GUI_EventManager::OnMouseLeftPressed    (POINT const &mousePosition)
	{
		if( Ptr_MouseLeftPressed )
		{
			Ptr_MouseLeftPressed(mousePosition);
		}
	}

	void GUI_EventManager::OnMouseMiddlePressed  (POINT const &mousePosition)
	{
		if( Ptr_MouseMiddlePressed )
		{
			Ptr_MouseMiddlePressed(mousePosition);
		}
	}

	void GUI_EventManager::OnMouseRightPressed   (POINT const &mousePosition)
	{
		if( Ptr_MouseRightPressed )
		{
			Ptr_MouseRightPressed(mousePosition);
		}
	}

//
	void GUI_EventManager::OnMouseLeftUp         (POINT const &mousePosition)
	{
		if( Ptr_MouseLeftUp )
		{
			Ptr_MouseLeftUp(mousePosition);
		}
	}

	void GUI_EventManager::OnMouseMiddleUp       (POINT const &mousePosition)
	{
		if( Ptr_MouseMiddleUp )
		{
			Ptr_MouseMiddleUp(mousePosition);
		}
	}

	void GUI_EventManager::OnMouseRightUp        (POINT const &mousePosition)
	{
		if( Ptr_MouseRightUp )
		{
			Ptr_MouseRightUp(mousePosition);
		}
	}

//
	void GUI_EventManager::OnMouseLeftDblClick   (POINT const &mousePosition)
	{
		if( Ptr_MouseLeftDblClick  )
		{
			Ptr_MouseLeftDblClick(mousePosition);
		}
	}

	void GUI_EventManager::OnMouseMiddleDblClick (POINT const &mousePosition)
	{
		if( Ptr_MouseMiddleDblClick )
		{
			Ptr_MouseMiddleDblClick(mousePosition);
		}
	}

	void GUI_EventManager::OnMouseRightDblClick  (POINT const &mousePosition)
	{
		if( Ptr_MouseRightDblClick )
		{
			Ptr_MouseRightDblClick(mousePosition);
		}
	}

//
	void GUI_EventManager::OnMouseMoved          (POINT const &mousePosition)
	{
		if( Ptr_MouseMove )
		{
			Ptr_MouseMove(mousePosition);
		}
	}

	void GUI_EventManager::OnMouseDragged        (POINT const &mousePosition)
	{
		if( Ptr_MouseDrag )
		{
			Ptr_MouseDrag(mousePosition);
		}
	}

//
	void GUI_EventManager::OnScroll              (int const &scrollInfo)
	{
		if( Ptr_Scroll )
		{
			Ptr_Scroll(scrollInfo);
		}
	}

//
	void GUI_EventManager::OnMoving              (POINT const &mousePositionDelta)
	{
		if( Ptr_Moving )
		{
			Ptr_Moving(mousePositionDelta);
		}
	}

	void GUI_EventManager::OnScaling             (float const &scaleMultiplier)
	{
		if( Ptr_Scaling )
		{
			Ptr_Scaling(scaleMultiplier);
		}
	}


	