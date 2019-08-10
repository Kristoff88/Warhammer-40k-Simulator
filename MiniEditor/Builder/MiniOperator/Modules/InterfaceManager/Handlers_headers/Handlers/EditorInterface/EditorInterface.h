#pragma once


	



	#include "Content//EditorUpdater.h"

	#include "Content//MiniList.h"
	#include "Content//ModelList.h"

	#include "Content//AddMini.h"
	#include "Content//RemoveMini.h"

	#include "Content//WeaponAttributes.h"
	#include "Content//FirearmAttributes.h"
	#include "Content//OtherAttributes.h"

	#include "Content//SaveButton.h"




	inline void EditorInterface_SetBorder (Handler *editorInterface)
	{
		RECT border;

		border.left = 0;
		border.top = 700;
		border.right = 450;
		border.bottom = 0;
		
		editorInterface->SetBorders(border);
	}
//
	Handler* Create_EditorInterface ()
	{
		Handler *editorInterface = new Handler;

		Control_SetDefaultSettings(editorInterface);
		EditorInterface_SetBorder(editorInterface);

		editorInterface->SetName(EDITOR_INTERFACE);



		editorInterface->AddHandling( Create_Label_MiniList() );
		editorInterface->AddHandling( Create_ComboBox_MiniList() );

		editorInterface->AddHandling( Create_Button_AddMini() );
		editorInterface->AddHandling( Create_Button_RemoveMini() );

		editorInterface->AddHandling( Create_Label_ModelList() );
		editorInterface->AddHandling( Create_ComboBox_ModelList() );


		editorInterface->AddHandling( Create_Label_Points() );
		editorInterface->AddHandling( Create_Edit_Points() );


		editorInterface->AddHandling( Create_Label_WeaponType() );
		editorInterface->AddHandling( Create_ComboBox_WeaponType() );

		editorInterface->AddHandling( Create_Label_WeaponStrength() );
		editorInterface->AddHandling( Create_Edit_WeaponStrength() );

		editorInterface->AddHandling( Create_Label_WeaponSkill() );
		editorInterface->AddHandling( Create_Edit_WeaponSkill() );

		editorInterface->AddHandling( Create_Label_Initiative() );
		editorInterface->AddHandling( Create_Edit_Initiative() );

		editorInterface->AddHandling( Create_Label_Attacks() );
		editorInterface->AddHandling( Create_Edit_Attacks() );


		editorInterface->AddHandling( Create_Label_FirearmType() );
		editorInterface->AddHandling( Create_ComboBox_FirearmType() );

		editorInterface->AddHandling( Create_Label_FirearmStrength() );
		editorInterface->AddHandling( Create_Edit_FirearmStrength() );

		editorInterface->AddHandling( Create_Label_BallisticSkill() );
		editorInterface->AddHandling( Create_Edit_BallisticSkill() );

		editorInterface->AddHandling( Create_Label_FiringRange() );
		editorInterface->AddHandling( Create_Edit_FiringRange() );

		editorInterface->AddHandling( Create_Label_RateOfFire() );
		editorInterface->AddHandling( Create_Edit_RateOfFire() );

		editorInterface->AddHandling( Create_Label_ArmorPiercing() );
		editorInterface->AddHandling( Create_Edit_ArmorPiercing() );

		
		editorInterface->AddHandling( Create_Label_Toughness() );
		editorInterface->AddHandling( Create_Edit_Toughness() );

		editorInterface->AddHandling( Create_Label_Wounds() );
		editorInterface->AddHandling( Create_Edit_Wounds() );

		editorInterface->AddHandling( Create_Label_Save() );
		editorInterface->AddHandling( Create_Edit_Save() );


		editorInterface->AddHandling( Create_Label_MovementSpeed() );
		editorInterface->AddHandling( Create_Edit_MovementSpeed() );


		editorInterface->AddHandling( Create_Button_SaveMini() );


		return editorInterface;
	}