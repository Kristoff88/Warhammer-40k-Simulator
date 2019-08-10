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
				AddHandler( Create_EditorInterface() );
				FillAllControlsWithCurrentMiniData();

				AddHandler( Create_MiniCreator() );
			}
			catch(std::string error)
			{
				MessageBox(NULL, error.c_str(), "Error while generating user interface.", MB_OK);
				return false;
			}

			return true;
		}

		void InterfaceManager::AddHandler (Handler *handler)
		{
			gui.Controls.AddControl(handler);

			for(int i = 0; i < handler->GetHandlingsCount(); i++)
			{
				gui.Controls.AddControl( handler->GetHandling(i) );
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

		RenderGreenNetUnderCurrentMini();

		if( currentMini )
		{
			RenderCurrentMini();
		}
	}
//
	void InterfaceManager::RenderGreenNetUnderCurrentMini ()
	{
		glPushMatrix();

		glBegin(GL_LINES);

			glColor4f(0, 0.8f, 0.1f, 0.5f);

			int shift = 0;
			for(int i = 0; i < 101; i++)
			{
				glVertex3f( -50.5f + shift, 0.f, -50.5f );
				glVertex3f( -50.5f + shift, 0.f,  50.5f );

				shift++;
			}

			shift = 0;
			for(int i = 0; i < 101; i++)
			{
				glVertex3f( -50.5f, 0.f, -50.5f + shift );
				glVertex3f(  50.5f, 0.f, -50.5f + shift );

				shift++;
			}

		glEnd();

		glPopMatrix();
	}
//
	void InterfaceManager::RenderCurrentMini ()
	{
		PreRendering();
		currentMini->Render();
		PostRendering();
	}
//
		void InterfaceManager::PreRendering      ()
		{
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST)	;

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
		}

		void InterfaceManager::PostRendering     ()
		{
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST)	;

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		}
//
/*****************************************************/

	void  InterfaceManager::SetCurrentMini (std::string miniName)
	{
		int miniCount = data.GetMiniCount();
		
		try
		{
			if( miniName == "" )
			{
				currentMini = NULL;
			}
			else
			{
				assert( miniCount );
				currentMini = data.GetMini(miniName);
			}
		}
		catch(std::string error)
		{
			MessageBox(NULL, error.c_str(), "SetCurrentMini(miniName)", MB_OK);
		}
	}

	Mini* InterfaceManager::GetCurrentMini ()
	{
		return currentMini;
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
										    data( MiniDataManager::Singleton() ),
											
											currentMini(NULL)
	{}

	InterfaceManager::~InterfaceManager ()
	{}