#pragma once

	#include "..//..//..//GUI//GUI_Operator.h"
	#include "..//MiniDataManager//MiniDataManager.h"


	class InterfaceManager
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
				private:
				inline void RenderGreenNetUnderCurrentMini ();

				inline void RenderCurrentMini ();
					inline void PreRendering      ();
					inline void PostRendering     ();
			public:

	/*****************************************************/
			
			void  SetCurrentMini (std::string miniName);
			Mini* GetCurrentMini ();
	
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

			Mini *currentMini;



			InterfaceManager  ();
			~InterfaceManager ();
	};