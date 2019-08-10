	#include "GlobalHeaders.h"

	#include "GUI_Operator.h"


	#define DEFAULT_FONT	("Courier")



	bool GUI::Init  (HDC hdc, HWND hwnd) throw( std::string )
	{
		Data.AdjustGUIToClientArea(hwnd);
		
		ProgressDialog progressDlg(NULL, hwnd, "", 0, 2, 1);

		progressDlg.SetCaption("Loading textures...");
		Data.LoadTextures();
		progressDlg.Increment();

		return true;
	}
//
/******************************************************
		GUI properties managment.
******************************************************/

	void GUI::SetTransparency (bool const &IsTransparent)
	{
		Transparent = IsTransparent;
	}

	void GUI::SetStatic		  (bool const &IsStatic)
	{
		Static = IsStatic;
	}

	void GUI::SetScalable	  (bool const &IsScalable)
	{
		Scalable = IsScalable;
	}

	void GUI::SetRendering    (bool const &IsRendering)
	{
		Rendering = IsRendering;
	}


//
	bool GUI::IsTransparent ()
	{
		return Transparent;
	}

	bool GUI::IsStatic		()
	{
		return Static;
	}

	bool GUI::IsScalable	()
	{
		return Scalable;
	}

	bool GUI::IsRendering   ()
	{
		return Rendering;
	}
//
/*****************************************************/

	void GUI::MoveGUI  (POINT const &mousePositionDelta)
	{
		for(int i = 0; i < Controls.controlArray.size(); i++)
		{
			assert( Controls.controlArray[i] != NULL );
			Controls.controlArray[i]->OnMoving(mousePositionDelta);
		}
	}

	void GUI::ScaleGUI (float const &scaleMultiplier)
	{
		for(int i = 0; i < Controls.controlArray.size(); i++)
		{
			assert( Controls.controlArray[i] != NULL );
			Controls.controlArray[i]->OnScaling(scaleMultiplier);
		}
	}
//
/******************************************************
		Core Function
******************************************************/

	void GUI::Run ()
	{
		if( Input.ReceivedInput )
		{
			Input.ProcessInput();
		}

		if( Rendering )
		{
			Render();
		}
	}
//
//
/*****************************************************/

	void GUI::Save (char *filename)
	{
		std::fstream file;

		file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);


		file << "#GUIstate:\n\n"

			 << "\t" << "#Transparent:" << "\t" << Transparent << "\n"
			 << "\t" << "#Static:"      << "\t" << Static      << "\n"
			 << "\t" << "#Scalable:"    << "\t" << Scalable    << "\n"
			 << "\t" << "#Rendering:"   << "\t" << Rendering   << "\n"

			 << "\n";

		Input.Save(file);
		Data.Save(file);
		Controls.Save(file);
		

		file.close();
	}

	void GUI::Load (char *filename) throw( std::string )
	{
		std::fstream file;
		std::string  line;

		file.open(filename, std::ios::in | std::ios::binary);


		line = ReadNewValue(file, '#');		Transparent = atoi(line.c_str());
		line = ReadNewValue(file, '#');		Static      = atoi(line.c_str());
		line = ReadNewValue(file, '#');		Scalable    = atoi(line.c_str());
		line = ReadNewValue(file, '#');		Rendering   = atoi(line.c_str());

		Input.Load(file);
		Data.Load(file);
		Controls.Load(file);
	

		file.close();
	}
//
/******************************************************
		Hidden consructor and destructor.
******************************************************/

	GUI::GUI  (): Data( GUI_DataManager::Singleton() ),
				  Controls( ControlManager::Singleton() ),

				  EventManager( GUI_EventManager::Singleton() ),
				  Input( InputProcessor::Singleton() ),

			      Transparent(true), 
				  Static(false),
				  Scalable(true),
				  Rendering(true)
	{}

	GUI::~GUI ()
	{}
//
/*****************************************************/

	void GUI::Render()
	{
		glPushAttrib(GL_ENABLE_BIT);

		glEnable(GL_TEXTURE_2D);


	// It assumes, that there is used a modelview matrix.

		glPushMatrix();

			glLoadIdentity();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();

				glLoadIdentity();
			
		
				GLint viewport[4];
				glGetIntegerv(GL_VIEWPORT, viewport);
				gluOrtho2D(0, viewport[2], 0, viewport[3]);
	

				glDisable(GL_LIGHTING);
				glEnable(GL_DEPTH_TEST);

				RenderControls();
				RenderCurrentFocus();

			glPopMatrix();

		glMatrixMode(GL_MODELVIEW);

		glPopMatrix();



		glPopAttrib();
	}
//
	void GUI::RenderControls     ()
	{
		for(int i = 0; i < Controls.controlArray.size(); i++)
		{
			if( Controls.controlArray[i] != Controls.currentFocus || Controls.controlArray[i]->controlType == "Handler" )
			{
				Controls.controlArray[i]->OnRendering(Data.GUIcolor, Transparent);
			}
		}
	}

	void GUI::RenderCurrentFocus ()
	{
		if( Controls.currentFocus && Controls.currentFocus->controlType != "Handler" )
		{
			Controls.currentFocus->OnRendering(Data.GUIcolor, Transparent);
		}
	}
