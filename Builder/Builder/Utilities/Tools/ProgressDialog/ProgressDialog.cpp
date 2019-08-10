#include "GlobalHeaders.h"

#include "ProgressDialog.h"




	ProgressDialog::ProgressDialog(HINSTANCE instance, HWND hwnd, std::string name, unsigned int minRange, unsigned int maxRange, unsigned int incrementLevel)
	 : incrementLVL(incrementLevel)
	{

		// stworzenie dialog box'a pokazujacego postep plantowania trawy

			DialogBox = CreateDialog(instance, MAKEINTRESOURCE(IDD_PROGRESS_DIALOG), hwnd, NULL);

		// ustawienie paska tytulowego

			SetWindowText(DialogBox, name.c_str());

		// pokazanie okna

			ShowWindow(DialogBox, SW_SHOW);

		// ustawienie handlera paska postepu

			progressBar = GetDlgItem(DialogBox, IDC_PROGRESS_BAR);

		// przyszykowanie progress bara

			SendMessageA(progressBar, PBM_SETRANGE32,  minRange, maxRange);
			SendMessageA(progressBar, PBM_SETPOS, TRUE, 0);

	}


	ProgressDialog::~ProgressDialog(void)
	{
		EndDialog(DialogBox, TRUE);
	}





	// Increment

	void ProgressDialog::Increment()
	{
		SendMessageA(progressBar, PBM_DELTAPOS, incrementLVL, 0);
	}

	void ProgressDialog::SetCaption(std::string caption)
	{
		SetWindowText(DialogBox, caption.c_str());
	}