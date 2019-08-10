#include "GlobalHeaders.h"

#include "Control_EventManager.h"


	Control_EventManager::Control_EventManager  ()
	{
		Ptr_Create = Ptr_Destroy = NULL;

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

		Ptr_Scroll = NULL;

		Ptr_FocusReceived = Ptr_FocusLost = NULL;

		Ptr_Collision = NULL;

		Ptr_Moving = Ptr_Resizing = NULL;
		Ptr_Scaling = NULL;

		Ptr_Rendering = NULL;
	}

	Control_EventManager::~Control_EventManager ()
	{}
//
//
	void Control_EventManager::OnCreate		         (Control *ctrl)
	{
		if( Ptr_Create )
		{
			Ptr_Create(ctrl);
		}
	}

	void Control_EventManager::OnDestroy		     (Control *ctrl)
	{
		if( Ptr_Destroy )
		{
			Ptr_Destroy(ctrl);
		}
	}


//
	void Control_EventManager::OnKeyDown             (Control *ctrl, int const &keyCode)
	{
		if( Ptr_KeyDown )
		{
			Ptr_KeyDown(ctrl, keyCode);
		}
	}

	void Control_EventManager::OnKeyPressed          (Control *ctrl, int const &keyCode)
	{
		if( Ptr_KeyPressed )
		{
			Ptr_KeyPressed(ctrl, keyCode);
		}
	}

	void Control_EventManager::OnKeyUp               (Control *ctrl, int const &keyCode)
	{
		if( Ptr_KeyUp )
		{
			Ptr_KeyUp(ctrl, keyCode);
		}
	}

//
	void Control_EventManager::OnMouseLeftDown       (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseLeftDown )
		{
			Ptr_MouseLeftDown(ctrl, mousePosition);	
		}
	}

	void Control_EventManager::OnMouseMiddleDown     (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseMiddleDown )
		{
			Ptr_MouseMiddleDown(ctrl, mousePosition);	
		}
	}

	void Control_EventManager::OnMouseRightDown      (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseRightDown )
		{
			Ptr_MouseRightDown(ctrl, mousePosition);	
		}
	}

//
	void Control_EventManager::OnMouseLeftPressed    (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseLeftPressed )
		{
			Ptr_MouseLeftPressed(ctrl, mousePosition);	
		}
	}

	void Control_EventManager::OnMouseMiddlePressed  (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseMiddlePressed )
		{
			Ptr_MouseMiddlePressed(ctrl, mousePosition);	
		}
	}

	void Control_EventManager::OnMouseRightPressed   (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseRightPressed )
		{
			Ptr_MouseRightPressed(ctrl, mousePosition);	
		}
	}

//
	void Control_EventManager::OnMouseLeftUp         (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseLeftUp )
		{
			Ptr_MouseLeftUp(ctrl, mousePosition);	
		}
	}

	void Control_EventManager::OnMouseMiddleUp       (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseMiddleUp )
		{
			Ptr_MouseMiddleUp(ctrl, mousePosition);	
		}
	}

	void Control_EventManager::OnMouseRightUp        (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseRightUp )
		{
			Ptr_MouseRightUp(ctrl, mousePosition);	
		}
	}

//
	void Control_EventManager::OnMouseLeftDblClick   (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseLeftDblClick )
		{
			Ptr_MouseLeftDblClick(ctrl, mousePosition);	
		}
	}

	void Control_EventManager::OnMouseMiddleDblClick (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseMiddleDblClick )
		{
			Ptr_MouseMiddleDblClick(ctrl, mousePosition);	
		}
	}

	void Control_EventManager::OnMouseRightDblClick  (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseRightDblClick )
		{
			Ptr_MouseRightDblClick(ctrl, mousePosition);	
		}
	}

//
	void Control_EventManager::OnMouseMoved          (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseMove )
		{
			Ptr_MouseMove(ctrl, mousePosition);	
		}
	}

	void Control_EventManager::OnMouseDragged        (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_MouseDrag)
		{
			Ptr_MouseDrag(ctrl, mousePosition);	
		}
	}

//
	void Control_EventManager::OnScroll              (Control *ctrl, int const &scrollInfo)
	{
		if( Ptr_Scroll )
		{
			Ptr_Scroll(ctrl, scrollInfo);
		}
	}

//
	void Control_EventManager::OnFocusReceived       (Control *ctrl)
	{
		if( Ptr_FocusReceived )
		{
			Ptr_FocusReceived(ctrl);
		}
	}

	void Control_EventManager::OnFocusLost           (Control *ctrl)
	{
		if( Ptr_FocusLost)
		{
			Ptr_FocusLost(ctrl);
		}
	}

//
	void Control_EventManager::OnCollision           (Control *ctrl)
	{
		if( Ptr_Collision )
		{
			Ptr_Collision(ctrl);
		}
	}
//
	void Control_EventManager::OnMoving              (Control *ctrl, POINT const &mousePositionDelta)
	{
		if( Ptr_Moving )
		{
			Ptr_Moving(ctrl, mousePositionDelta);
		}
	}

	void Control_EventManager::OnResizing            (Control *ctrl, POINT const &mousePosition)
	{
		if( Ptr_Resizing )
		{
			Ptr_Resizing(ctrl, mousePosition);
		}
	}

	void Control_EventManager::OnScaling             (Control *ctrl, float const &scaleMultiplier)
	{
		if( Ptr_Scaling )
		{
			Ptr_Scaling(ctrl, scaleMultiplier);
		}
	}

//
	void Control_EventManager::OnRendering           (Control *ctrl, Color const &GUIcolor, bool const &TransparentGUI)
	{
		if( Ptr_Rendering )
		{
			Ptr_Rendering(ctrl, GUIcolor, TransparentGUI);
		}
	}
