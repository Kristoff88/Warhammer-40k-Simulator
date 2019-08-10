	#include "GlobalHeaders.h"

	#include "LineReader.h"


	std::string ReadNewValue (std::fstream &file, char const &commentChar)
	{
		std::string line("");

		do
		{
			file >> line;

			if( file.eof() )
			{
				break;
			}
		}
		while( line[0] == commentChar );

		return line;
	}