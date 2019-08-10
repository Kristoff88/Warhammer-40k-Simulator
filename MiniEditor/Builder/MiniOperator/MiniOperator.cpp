	#include "GlobalHeaders.h"

	#include "MiniOperator.h"


	



	bool MiniOperator::Init  (HDC &hdc, HWND &hwnd)
	{
		try
		{
			if( data.Init()			 && 
				gui.Init(hdc, hwnd)  && 
				input.Init() )
			{
				return true;
			}
			else
				return false;
		}
		catch(std::string error)
		{
			MessageBox(NULL, error.c_str(), "Error, while initializing MiniOperator.", MB_OK);
		}
	}

	void MiniOperator::Close ()
	{
		data.Close();
		gui.Close();
		input.Close();
	}
//
/****************************************************************/

	void MiniOperator::Run ()
	{
		gui.Run();
	}
//
/****************************************************************/

	MiniOperator::MiniOperator  (): data( MiniDataManager::Singleton() ),
									gui( InterfaceManager::Singleton() ),
									input( MiniInputProcessor::Singleton() )
	{}

	MiniOperator::~MiniOperator ()
	{
		Close();
	}
