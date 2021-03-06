#pragma once


/*****************************************************************************
	Tworzy okno, kontekst urządzenia oraz kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL SetupWindow(const char *title, int width, int height, int bits, bool isFullscreen);

/*****************************************************************************
	Przywraca wyjściowy tryb wyświetlania. Usuwa okno, kontekst urządzenia 
	i kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL KillWindow();