#pragma once


/*****************************************************************************
	Tworzy okno, kontekst urz¹dzenia oraz kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL SetupWindow(const char *title, int width, int height, int bits, bool isFullscreen);

/*****************************************************************************
	Przywraca wyjœciowy tryb wyœwietlania. Usuwa okno, kontekst urz¹dzenia 
	i kontekst tworzenia grafiki.
*****************************************************************************/

	BOOL KillWindow();