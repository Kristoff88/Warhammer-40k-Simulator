	#include "GlobalHeaders.h"

	#include "NumberToString.h"



	std::string NumberToString(int number)
	{
		std::string numberAsString;

		char numberAsChar[MAX_PATH];
		_itoa_s(number, numberAsChar, MAX_PATH, 10);

		numberAsString = numberAsChar;

		return numberAsString;
	}