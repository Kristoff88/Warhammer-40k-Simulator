#pragma once

/*****************************************************************************
	Tworzy okno, kontekst urzadzenia oraz kontekst tworzenia grafiki.
*****************************************************************************/
BOOL SetupWindow(const char* title, int width, int height, BYTE bits, bool isFullscreen);

/*****************************************************************************
	Przywraca wyjściowy tryb wyświetlania. Usuwa okno, kontekst urzadzenia
	i kontekst tworzenia grafiki.
*****************************************************************************/
BOOL KillWindow();