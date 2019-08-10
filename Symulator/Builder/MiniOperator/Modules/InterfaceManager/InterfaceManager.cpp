	#include "GlobalHeaders.h"

	#include "InterfaceManager.h"

	#include "Handlers_headers//Handlers_headers.h"


	extern PFNGLBINDBUFFERARBPROC glBindBufferARB;



	bool InterfaceManager::Init  (HDC &hdc, HWND &hwnd)
	{
		try
		{
			if( gui.Init(hdc, hwnd) && GenerateInterface() )
			{
				return true;
			}
			else
				return false;
		}
		catch(std::string error)
		{
			MessageBox(NULL, error.c_str(), "InterfaceManager initialization error.", MB_OK);
			return false;
		}
	}
//
		bool InterfaceManager::GenerateInterface ()
		{
			try
			{
				AddHandler( CreateBattleCreator() );
				AddHandler( CreateDeploymentWindow() );
				AddHandler( CreateSimulationGUI() );
			}
			catch(std::string error)
			{
				MessageBox(NULL, error.c_str(), "GenerateInterface() - error.", MB_OK);
				return false;
			}

			return true;
		}

		void InterfaceManager::AddHandler (Handler *handler)
		{
			gui.Controls.AddControl(handler);

			for(int i = 0; i < handler->GetHandlingsCount(); i++)
			{
				Control *handling = handler->GetHandling(i);

				if( handling->GetType() == "Handler" )
				{
					AddHandler( reinterpret_cast <Handler*> (handling) );
				}
				else
					gui.Controls.AddControl( handling );
			}
		}
//
	void InterfaceManager::Close ()
	{}
//
/****************************************************************/

	void InterfaceManager::Run ()
	{
		gui.Run();
	}
//
/*****************************************************/

	Control* InterfaceManager::GetCurrentFocus ()
	{
		return gui.Controls.GetCurrentFocus();
	}
//
/*****************************************************/

	void InterfaceManager::KeyDown             (int const &keyCode)
	{
		gui.Input.KeyDown(keyCode);
	}

	void InterfaceManager::KeyUp               (int const &keyCode)
	{
		gui.Input.KeyUp(keyCode);
	}
//
	void InterfaceManager::LeftMouseDown       ()
	{
		gui.Input.LeftMouseDown();
	}

	void InterfaceManager::MiddleMouseDown     ()
	{
		gui.Input.MiddleMouseDown();
	}

	void InterfaceManager::RightMouseDown      ()
	{
		gui.Input.RightMouseDown();
	}
//
	void InterfaceManager::LeftMouseUp         ()
	{
		gui.Input.LeftMouseUp();
	}

	void InterfaceManager::MiddleMouseUp       ()
	{
		gui.Input.MiddleMouseUp();
	}

	void InterfaceManager::RightMouseUp        ()
	{
		gui.Input.RightMouseUp();
	}
//
	void InterfaceManager::LeftMouseDblClick   ()
	{
		gui.Input.LeftMouseDblClick();
	}

	void InterfaceManager::MiddleMouseDblClick ()
	{
		gui.Input.MiddleMouseDblClick();
	}

	void InterfaceManager::RightMouseDblClick  ()
	{
		gui.Input.RightMouseDblClick();
	}
//
	void InterfaceManager::Scroll              (int const &scrollInfo)
	{
		gui.Input.Scroll(scrollInfo);
	}
//
	void InterfaceManager::MoveCursor          (POINT const &mousePosition)
	{
		gui.Input.MoveCursor(mousePosition);
	}
//
/****************************************************************/

	InterfaceManager::InterfaceManager  (): gui( GUI::Singleton() ),
										    data( MiniDataManager::Singleton() )
	{}

	InterfaceManager::~InterfaceManager ()
	{}