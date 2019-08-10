#pragma once

	#include "Modules//MiniModules.h"

	



	class MiniOperator
	{
		public:

			MiniInputProcessor &input;


			static MiniOperator& Singleton ()
			{
				static MiniOperator s;
				return s;
			}
			//

	/****************************************************************/

			bool Init  (HDC &hdc, HWND &hwnd);
			void Close ();

	/****************************************************************/

			void Run ();


		private:

			MiniDataManager &data;
			InterfaceManager &gui;

	/****************************************************************/

			MiniOperator  ();
			~MiniOperator ();
	};