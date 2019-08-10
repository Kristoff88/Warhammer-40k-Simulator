#pragma once

	#include "..//Utilities//Handler_utilities.h"


	class DeploymentWindow: public Handler_Utilities
	{
		public:

			Handler* CreateDeploymentWindow ();
				private:
				inline void DeploymentWindow_SetBorder      (Handler *deploymentWindow);
				inline void DeploymentWindow_SetLabel       (Handler *deploymentWindow);
				inline void DeploymentWindow_SetStartButton (Handler *deploymentWindow);

				inline Handler* CreateSection_BattleForceList ();

					inline void BattleForceList_SetBorder       (Handler *battleForceList);
					inline void BattleForceList_SetLabel        (Handler *battleForceList);
					inline void BattleForceList_SetComboBox     (Handler *battleForceList);

				inline Handler* CreateSection_MiniSelections ();

					inline void MiniSelections_SetBorder       (Handler *miniSelections);
					inline void MiniSelections_SetLabel        (Handler *miniSelections);
					inline void MiniSelections_SetListBox      (Handler *miniSelections);
					inline void MiniSelections_SetDeployButton (Handler *miniSelections);
			public:
			
	};