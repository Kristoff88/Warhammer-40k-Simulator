#pragma once

	#include "..//..//..//GUI//GUI_Operator.h"
	#include "..//MiniDataManager//MiniDataManager.h"

	#include "Handlers_headers//Handlers_headers.h"


	class InterfaceManager: public BattleCreator, public DeploymentWindow, public SimulationGUI
	{
		public:

			static InterfaceManager& Singleton ()
			{
				static InterfaceManager s;
				return s;
			}
			//

			bool Init  (HDC &hdc, HWND &hwnd);
				private:
				inline bool GenerateInterface ();
				inline void AddHandler (Handler *handler);
			public:	

			void Close ();

	/*****************************************************/

			void Run ();

	/*****************************************************/
			
			Control* GetCurrentFocus ();
	
	/*****************************************************/

			void KeyDown             (int const &keyCode);
			void KeyUp               (int const &keyCode);

			void LeftMouseDown       ();
			void MiddleMouseDown     ();
			void RightMouseDown      ();

			void LeftMouseUp         ();
			void MiddleMouseUp       ();
			void RightMouseUp        ();

			void LeftMouseDblClick   ();
			void MiddleMouseDblClick ();
			void RightMouseDblClick  ();

			void Scroll              (int const &scrollInfo);

			void MoveCursor          (POINT const &mousePosition);

		private:

			GUI &gui;
			MiniDataManager &data;


			InterfaceManager  ();
			~InterfaceManager ();
	};