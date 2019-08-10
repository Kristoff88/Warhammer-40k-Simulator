	#include "GlobalHeaders.h"

	#include "MiniInputProcessor.h"



	bool MiniInputProcessor::Init  (HWND const &hwnd)
	{
		this->hwnd = hwnd;
		return true;
	}

	void MiniInputProcessor::Close ()
	{
	}
//
/****************************************************************/

	void MiniInputProcessor::KeyDown             (int const &keyCode)
	{
		gui.KeyDown(keyCode);

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.KeyDown(keyCode);
		}
	}

	void MiniInputProcessor::KeyUp               (int const &keyCode)
	{
		gui.KeyUp(keyCode);

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.KeyUp(keyCode);
		}
	}
//
	void MiniInputProcessor::LeftMouseDown       ()
	{
		gui.LeftMouseDown();

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.LeftMouseDown();
		}
	}

	void MiniInputProcessor::MiddleMouseDown     ()
	{
		gui.MiddleMouseDown();

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.MiddleMouseDown();
		}
	}

	void MiniInputProcessor::RightMouseDown      ()
	{
		gui.RightMouseDown();

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.RightMouseDown();
		}
	}
//
	void MiniInputProcessor::LeftMouseUp         ()
	{
		gui.LeftMouseUp();

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.LeftMouseUp();
		}
	}

	void MiniInputProcessor::MiddleMouseUp       ()
	{
		gui.MiddleMouseUp();

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.MiddleMouseUp();
		}
	}

	void MiniInputProcessor::RightMouseUp        ()
	{
		gui.RightMouseUp();

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.RightMouseUp();
		}
	}
//
	void MiniInputProcessor::LeftMouseDblClick   ()
	{
		gui.LeftMouseDblClick();

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.LeftMouseDblClick();
		}
	}

	void MiniInputProcessor::MiddleMouseDblClick ()
	{
		gui.MiddleMouseDblClick();

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.MiddleMouseDblClick();
		}
	}

	void MiniInputProcessor::RightMouseDblClick  ()
	{
		gui.RightMouseDblClick();

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.RightMouseDblClick();
		}
	}
//
	void MiniInputProcessor::Scroll              (int const &scrollInfo)
	{
		gui.Scroll(scrollInfo);

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.Scroll(scrollInfo);
		}
	}
//
	void MiniInputProcessor::MoveCursor          (POINT const &mousePosition)
	{
		gui.MoveCursor( ReverseMousePositionByY(mousePosition) );

		if( gui.GetCurrentFocus() == NULL )
		{
			battleForceManager.MoveCursor(mousePosition);
		}
	}
//
		POINT MiniInputProcessor::ReverseMousePositionByY (POINT const &mousePosition)
		{
			POINT poisitionInvertedByY;
		
			WINDOWINFO info;
			GetWindowInfo(hwnd, &info); 
			int clientAreaHeight = info.rcClient.bottom-info.rcClient.top;

			poisitionInvertedByY.x =  mousePosition.x;
			poisitionInvertedByY.y = -mousePosition.y + clientAreaHeight;

			return poisitionInvertedByY;
		}
//
/****************************************************************/

	MiniInputProcessor::MiniInputProcessor  (): gui( InterfaceManager::Singleton() ),
												battleForceManager( BattleForceManager::Singleton() )
	{}

	MiniInputProcessor::~MiniInputProcessor ()
	{}