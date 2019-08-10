#pragma once

	#include "..//Utilities//Handler_utilities.h"


	class BattleCreator: public Handler_Utilities
	{
		public:
			

			Handler* CreateBattleCreator ();
				private:
				inline void BattleCreator_SetBorder      (Handler *battleCreator);
				inline void BattleCreator_SetLabel       (Handler *battleCreator);
				inline void BattleCreator_SetStartButton (Handler *battleCreator);

				inline Handler* CreateSection_MapList ();

					inline void MapList_SetBorder  (Handler *mapSection);
					inline void MapList_SetLabel   (Handler *mapSection);
					inline void MapList_SetMapList (Handler *mapSection);

				inline Handler* CreateSection_BattleForceCreator ();

					inline void BattleForceCreator_SetBorder    (Handler *battleForceCreator);
					inline void BattleForceCreator_SetLabel     (Handler *battleForceCreator);
					inline void BattleForceCreator_SetEdit      (Handler *battleForceCreator);
					inline void BattleForceCreator_SetAddButton (Handler *battleForceCreator);

				inline Handler* CreateSection_BattleForceList ();

					inline void BattleForceList_SetBorder       (Handler *battleForceList);
					inline void BattleForceList_SetLabel        (Handler *battleForceList);
					inline void BattleForceList_SetComboBox     (Handler *battleForceList);
					inline void BattleForceList_SetRemoveButton (Handler *battleForceList);

				inline Handler* CreateSection_MiniAdder ();

					inline void MiniAdder_SetBorder    (Handler *miniAdder);
					inline void MiniAdder_SetLabel     (Handler *miniAdder);
					inline void MiniAdder_SetComboBox  (Handler *miniAdder);
					inline void MiniAdder_SetComboBox2 (Handler *miniAdder);
					inline void MiniAdder_SetAddButton (Handler *miniAdder);

				inline Handler* CreateSection_MiniSelections ();

					inline void MiniSelections_SetBorder       (Handler *miniSelections);
					inline void MiniSelections_SetLabel        (Handler *miniSelections);
					inline void MiniSelections_SetListBox      (Handler *miniSelections);
					inline void MiniSelections_SetRemoveButton (Handler *miniSelections);
			public:
	};