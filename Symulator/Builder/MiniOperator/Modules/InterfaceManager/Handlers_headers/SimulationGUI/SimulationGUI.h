#pragma once

	#include "..//Utilities//Handler_utilities.h"


	class SimulationGUI: public Handler_Utilities
	{
		public:

			Handler* CreateSimulationGUI ();
				private:
				Label* CreateInfoBox       ();
				Label* CreateTargetInfoBox ();

				Edit*  CreateBattleLog ();

				Handler* CreateOptionBox ();
			public:
	};
