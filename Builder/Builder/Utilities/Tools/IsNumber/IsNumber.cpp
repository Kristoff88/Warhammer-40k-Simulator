#include "GlobalHeaders.h"

#include "IsNumber.h"



	bool IsNumber(std::string String, bool IsInteger)
	{

		bool isNumber = true,
			 foundDot = false,
			 foundE	   = false,
			 foundSign = false;

		for(int i = 0; i < String.size(); i++)
		{
		// jezeli jest to pierwszy znak, moze on byc znakiem dodatnim/ujemnym

			if( i == 0 )
			{
				if( String[i] == '+' || String[i] == '-' ||
				   ( String[i] >= 48 && String[i] <= 57 ) )
				   ;
				else if(  String[i] == '.' )
				{
				// max 1 kropek :)

					if( !foundDot ) 
					{
						foundDot = true;
					}
					else
					{
						isNumber = false;
						break;
					}
				}
				else
				{
					isNumber = false;
					break;
				}
			}
			else
			{
				if( String[i] >= 48 &&  String[i] <= 57 )
					;// cyfra
				else if(  String[i] == '.' && !foundDot && !IsInteger)
				{
				// max 1 kropek :)

					if( !foundDot ) 
					{
						foundDot = true;
					}
					else
					{
						isNumber = false;
						break;
					}
				}
				else if( (String[i] == 'e' || String[i] == 'E') && !foundE )
				{
					foundE = true;
				}
				else if( (String[i] == '-' || String[i] == '+') && foundE && !foundSign )
				{
					foundSign = true;
				}
				else
				{
					isNumber = false;
					break;
				}
			}

		}// while( cString[index] != NULL )


		return isNumber;
	}