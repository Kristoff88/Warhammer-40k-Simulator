#pragma once

	#include "..//InterfaceManager//InterfaceManager.h"


	class MiniInputProcessor
	{
		public:

			static MiniInputProcessor& Singleton ()
			{
				static MiniInputProcessor s;
				return s;
			}
			//

			bool Init  ();
			void Close ();

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

			InterfaceManager &gui;

			MiniInputProcessor  ();
			~MiniInputProcessor ();
	};