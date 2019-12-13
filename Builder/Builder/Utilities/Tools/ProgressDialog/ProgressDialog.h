#pragma once

#include <windows.h>
#include <string>

class ProgressDialog
{
public:
	ProgressDialog(HINSTANCE instance, HWND hwnd, std::string name, size_t minRange, size_t maxRange, size_t incrementLevel);
	~ProgressDialog();

	void Increment();
	void SetCaption(std::string caption);

private:
	HWND DialogBox;		// handler do dialog box'a
	HWND progressBar;	// handler do paska postepu

	size_t incrementLVL;	// stopien postepu progressBara
};