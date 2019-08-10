#pragma once

// te biblioteki trzeba includowac, jezeli nie zostaly jeszcze zalaczone w aplikacji

//#include <string>
//
//#include <windows.h>
//#include <Commctrl.h>	// potrzebne do obslugi roznego rodzaju kontrolek WinApi



	class ProgressDialog
	{
		
		public:

			ProgressDialog(HINSTANCE instance, HWND hwnd, std::string name, unsigned int minRange, unsigned int maxRange, unsigned int incrementLevel);
			~ProgressDialog();

			void Increment();
			void SetCaption(std::string caption);

		private:

			HWND DialogBox,		// handler do dialog box'a
				 progressBar;	// handler do paska postepu


			unsigned int incrementLVL;	// stopien postepu progressBara

	};