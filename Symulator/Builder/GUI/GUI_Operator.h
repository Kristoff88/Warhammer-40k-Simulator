#pragma once


// Required headers (if not included somewhere else in the project).

	// #include <windows>

	// #include <string>
	// #include <vector>

	// #include <gl/gl.h>
	// #include <gl/glu.h>
	// #include <gl/glaux.h>


	
	#include "GUI_Modules//GUI_modules.h"
	#include "Utilities//GUI_Utilities.h"

	

	class GUI
	{
		public:

			GUI_DataManager &Data;
			ControlManager  &Controls;

			GUI_EventManager &EventManager;
			InputProcessor &Input;


			static GUI& Singleton ()
			{
				static GUI singleton;
				return singleton;
			}
		//
		//
			bool Init  (HDC hdc, HWND hwnd) throw( std::string );

	/******************************************************
			GUI properties managment.
	******************************************************/

			void SetTransparency (bool const &IsTransparent);
			void SetStatic		 (bool const &IsStatic);
			void SetScalable	 (bool const &IsScalable);
			void SetRendering    (bool const &IsRendering);

			bool IsTransparent ();
			bool IsStatic      ();
			bool IsScalable    ();
			bool IsRendering   ();

	/*******************************************************/

			void MoveGUI  (POINT const &mousePositionDelta);
			void ScaleGUI (float const &scaleMultiplier);

	/******************************************************
			Core function
	******************************************************/

			void Run ();

	/*****************************************************/

			void Save (char *filename);
			void Load (char *filename) throw( std::string );

		private:

	/******************************************************
			Hidden consructor and destructor.
	******************************************************/

			GUI  ();
		    ~GUI ();

	/*****************************************************/

			void Render ();

				inline void RenderControls     ();
				inline void RenderCurrentFocus ();



			bool Transparent;

			bool Static;		

			bool Scalable;	

			bool Rendering;
	};