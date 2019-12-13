#pragma once

#include <GlobalHeaders.h>
#include "Font.h"


unsigned int CreateBitmapFont(char* fontName, bool isBold, int fontSize, HDC g_hdc)
{
	unsigned int    base;
	HFONT  hFont;

	int bold;

	if (isBold)
	{
		bold = FW_BOLD;
	}
	else
	{
		bold = FW_NORMAL;
	}

	base = glGenLists(96);

	if (_stricmp(fontName, "symbol") == 0)
	{
		hFont = CreateFont(fontSize, 0, 0, 0, bold, FALSE, FALSE, FALSE,
			SYMBOL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
			fontName);
	}
	else
	{
		hFont = CreateFont(fontSize, 0, 0, 0, bold, FALSE, FALSE, FALSE,
			ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
			fontName);
	}

	if (!hFont)
		return 0;

	SelectObject(g_hdc, hFont);
	wglUseFontBitmaps(g_hdc, 32, 96, base);

	return base;
}

void PrintString(unsigned int base, const char* str)
{
	if ((base == 0) || (str == NULL))
		return;

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(static_cast<GLsizei>(strlen(str)), GL_UNSIGNED_BYTE, str);
	glPopAttrib();
}

void ClearFont(unsigned int base)
{
	if (base != 0)
		glDeleteLists(base, 96);
}