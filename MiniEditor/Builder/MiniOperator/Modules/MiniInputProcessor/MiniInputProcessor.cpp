	#include "GlobalHeaders.h"

	#include "MiniInputProcessor.h"



	bool MiniInputProcessor::Init  ()
	{
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
	}

	void MiniInputProcessor::KeyUp               (int const &keyCode)
	{
		gui.KeyUp(keyCode);
	}
//
	void MiniInputProcessor::LeftMouseDown       ()
	{
		gui.LeftMouseDown();
	}

	void MiniInputProcessor::MiddleMouseDown     ()
	{
		gui.MiddleMouseDown();
	}

	void MiniInputProcessor::RightMouseDown      ()
	{
		gui.RightMouseDown();
	}
//
	void MiniInputProcessor::LeftMouseUp         ()
	{
		gui.LeftMouseUp();
	}

	void MiniInputProcessor::MiddleMouseUp       ()
	{
		gui.MiddleMouseUp();
	}

	void MiniInputProcessor::RightMouseUp        ()
	{
		gui.RightMouseUp();
	}
//
	void MiniInputProcessor::LeftMouseDblClick   ()
	{
		gui.LeftMouseDblClick();
	}

	void MiniInputProcessor::MiddleMouseDblClick ()
	{
		gui.MiddleMouseDblClick();
	}

	void MiniInputProcessor::RightMouseDblClick  ()
	{
		gui.RightMouseDblClick();
	}
//
	void MiniInputProcessor::Scroll              (int const &scrollInfo)
	{
		gui.Scroll(scrollInfo);
	}
//
	void MiniInputProcessor::MoveCursor          (POINT const &mousePosition)
	{
		gui.MoveCursor(mousePosition);
	}
//
/****************************************************************/

	MiniInputProcessor::MiniInputProcessor  (): gui( InterfaceManager::Singleton() )
	{}

	MiniInputProcessor::~MiniInputProcessor ()
	{}