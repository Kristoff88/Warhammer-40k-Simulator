#pragma once


/*****************************************************************************
	Tworzy okno, kontekst urz�dzenia oraz kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL SetupWindow(const char *title, int width, int height, int bits, bool isFullscreen);

/*****************************************************************************
	Przywraca wyj�ciowy tryb wy�wietlania. Usuwa okno, kontekst urz�dzenia 
	i kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL KillWindow();