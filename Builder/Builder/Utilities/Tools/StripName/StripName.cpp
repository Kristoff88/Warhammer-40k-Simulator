#include "GlobalHeaders.h"

#include "StripName.h"



	std::string StripName(std::string arg, bool WithExtension)
	{
		bool trigger = false;
		std::string::size_type result;

		while(!trigger)
		{
			result = arg.find('\\');

			if(result != std::string::npos)
			{
				arg.erase(0, result+1);
			}
			else
			{
				if( !WithExtension )
				{
					result = arg.rfind('.');

					if(result != std::string::npos)
					{
						arg.erase(result, std::string::npos);
					}
				}

				trigger = true;
			}
		}

		return arg;
	}