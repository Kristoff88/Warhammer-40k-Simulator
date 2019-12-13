#pragma once


/*****************************************************************************
	Tworzy okno, kontekst urzadzenia oraz kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL SetupWindow(const char *title, int width, int height, BYTE bits, bool isFullscreen);

/*****************************************************************************
	Przywraca wyj�ciowy tryb wy�wietlania. Usuwa okno, kontekst urzadzenia 
	i kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL KillWindow();