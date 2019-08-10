	#include "GlobalHeaders.h"

	#include "MiniOperator.h"


	extern PFNGLBINDBUFFERARBPROC glBindBufferARB;




	bool MiniOperator::Init  (HDC &hdc, HWND &hwnd)
	{
		try
		{
			if( data.Init()			      && 
				gui.Init(hdc, hwnd)       && 
				battleForceManager.Init() &&
				input.Init(hwnd) )
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
		battleForceManager.Close();
		input.Close();
	}
//
/****************************************************************/

	bool MiniOperator::Run ()
	{
		if( !battleForceManager.Run() )
		{
			return false;
		}

		gui.Run();

		return true;
	}
//
/****************************************************************/

	MiniOperator::MiniOperator  (): data( MiniDataManager::Singleton() ),

									gui( InterfaceManager::Singleton() ),
									battleForceManager( BattleForceManager::Singleton() ),

									input( MiniInputProcessor::Singleton() )
	{}

	MiniOperator::~MiniOperator ()
	{
		Close();
	}
