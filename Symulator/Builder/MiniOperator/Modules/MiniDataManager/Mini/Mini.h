#pragma once

	#include "MiniModel//MiniModel.h"


	class Mini: public MiniModel
	{
		public:

			Mini (std::string name);
			Mini (std::string name, MiniModel_Data *model);
				private:
				inline void SetDefaultMiniCharacteristics    ();
				inline void SetDefaultBolleanCharacteristics ();
			public:

			~Mini ();

	/****************************************************************/

			void Render ();

			void DrawCircles ();

			void DrawSquadSpacing       ();
			void SetDrawingSquadSpacing (bool const &newValue);
			bool IsDrawingSquadSpacing  ();

			void DrawActivity       ();
			void SetDrawingActivity (bool const &newValue);
			bool IsDrawingActivity  ();

			void DrawSelection       ();
			void SetDrawingSelection (bool const &newValue);
			bool IsDrawingSelection  ();

			void DrawAssaultTarget       ();
			void SetDrawingAssaultTarget (bool const &newValue);
			bool IsDrawingAssaultTarget  ();

			void DrawRangedTarget        ();
			void SetDrawingRangedTarget  (bool const &newValue);
			bool IsDrawingRangedTarget   ();

			void DrawMovementRange       ();
			void SetDrawingMovementRange (bool const &newValue);
			bool IsDrawingMovementRange  ();

			void DrawShootingRange       ();
			void SetDrawingShootingRange (bool const &newValue);
			bool IsDrawingShootingRange  ();

			void DrawRapidFiringRange       ();
			void SetDrawingRapidFiringRange (bool const &newValue);
			bool IsDrawingRapidFiringRange  ();

			void DrawAssaultRange       ();
			void SetDrawingAssaultRange (bool const &newValue);
			bool IsDrawingAssaultRange  ();


			void ResetDrawingFlags ();

			private:
			void DrawCircle (bool const &polygon, float const &radius, Color &color);
			public:

	/****************************************************************/

			std::string GetMiniName ();


			void SetPoints (int const &newValue);
			int  GetPoints ();


			void GiveStandardWeapon ();
			bool HasStandardWeapon  ();

			void GivePowerWeapon ();
			bool HasPowerWeapon  ();

			void GivePowerFist ();
			bool HasPowerFist  ();

			void GiveRendingWeapon ();
			bool HasRendingWeapon  ();


			void SetWeaponStrength (int const &newValue);
			int  GetWeaponStrength ();

			void SetWeaponSkill (int const &newValue);
			int  GetWeaponSkill ();

			void SetInitiative (int const &newValue);
			int  GetInitiative ();

			void SetAttacks (int const &newValue);
			int  GetAttacks ();


			void GiveHeavyWeapon ();
			bool HasHeavyWeapon  ();

			void GiveAssaultWeapon ();
			bool HasAssaultWeapon  ();

			void GiveRapidFireWeapon ();
			bool HasRapidFireWeapon  ();

			void GivePistol ();
			bool HasPistol  ();

			
			void SetFirearmStrength (int const &newValue);
			int  GetFirearmStrength ();

			void SetBallisticSkill (int const &newValue);
			int  GetBallisticSkill ();

			void SetFiringRange (int const &newValue);
			int  GetFiringRange ();

			void SetRateOfFire (int const &newValue);
			int  GetRateOfFire ();

			void SetArmorPiercing (int const &newValue);
			int  GetArmorPiercing ();


			void SetToughness (int const &newValue);
			int  GetToughness ();

			void SetWounds (int const &newValue);
			int  GetWounds ();

			void SetSave (int const &newValue);
			int  GetSave ();


			void SetMovementSpeed (int const &newValue);
			int  GetMovementSpeed ();

			void SetRunningBonus (int const &value);

			float GetAssaultRange ();
	
			private:
			inline int CorrectNewValue (int const &newValue);
			public:

	/****************************************************************/

			void SetInMelee    (bool const &newValue);
			bool IsTiedInMelee ();

			void Deploy (Vector3 const &position);

			void SetDeployment (bool const &newValue);
			bool IsDeployed    ();
			bool IsNotDeployed ();

			void InflictWound ();

			void Kill   ();
			void Revive ();

			void SetDead (bool const &newValue);
			bool IsDead  ();
			bool IsAlive ();

			bool IsInAction    ();

		/****************************************************************/

			void SetFlag_Move    ();
			void SetFlag_Run     ();
			void SetFlag_Shoot   ();
			void SetFlag_Assault ();

			bool CanMove         ();
			bool CanShoot        ();
			bool CanShootRapidly ();
			bool CanAssault      ();

			bool IsRunning ();

			void ResetStateFlags ();

	/****************************************************************/

			bool InRangeOf_Fire      (Mini *possibleTarget);
			bool InRangeOf_RapidFire (Mini *possibleTarget);
			bool InRangeOf_Assault   (Mini *possibleTarget);

	/****************************************************************/

			std::string ComposeMiniInfo ();

	/****************************************************************/

			Vector3 CalculateHeight (Vector3 const &openGLposition);

			static void SetHeightMap(float *heightMap, int const &x, int const &z)
			{
				Mini::heightMap = heightMap;
				mapWidth_X = x;
				mapWidth_Z = z;
			}
	//
	/****************************************************************/

			void Save (char* filename);
				private:
				inline void SaveMiniName            (std::fstream &file);

				inline void SaveMiniCharacteristics (std::fstream &file);
				inline void SaveMiniPoints    (std::fstream &file);
				inline void SaveWeaponData    (std::fstream &file);
				inline void SaveFireArmData   (std::fstream &file);
				inline void SaveEnduranceData (std::fstream &file);
				inline void SaveMovementSpeed (std::fstream &file);
			public:

			void Load (char* filename);
				private:
				inline void LoadMiniName            (std::fstream &file);

				inline void LoadMiniCharacteristics (std::fstream &file);
				inline void LoadMiniPoints    (std::fstream &file);
				inline void LoadWeaponData    (std::fstream &file);
				inline void LoadFireArmData   (std::fstream &file);
				inline void LoadEnduranceData (std::fstream &file);
				inline void LoadMovementSpeed (std::fstream &file);

				inline void InformAboutFailedAttemptToLoadMiniFile (std::string filename);
			public:

		private:

			std::string miniName;


			int points;

			std::string weaponType;
			
				// - Standard_Weapon
				// - Power_Weapon:   Ignores save.
				// - Power_Fist:     Doubles strength, gives 1 inititative, ignores armor.
				// - Rending_Weapon: If hits, automatically causes a wound + ignores armour, if rolled 6.
			
			int weaponStrength;	// Instant-kill, when S >= 2*T.
			int weaponSkill;
			int initiative;
			int attacks;


			std::string fireArmType;

				// - Standard_FireArm
				// - Heavy:				Can't shoot, if moved.
				// - Assault:			Can shoot, if moved + can assault after shooting.
				// - RapidFire:			Can't normally shoot, if moved. Up to 12 inches it can always (even if moved) twice.
				// - Pistol:			Can shoot, before assaulting.

			int fireArmStrength;	// Instant-kill, when S >= 2*T.
			int ballisticSkill;
			int firingRange;
			int rateOfFire;
			int armorPiercing;	// If <= target's save, then it ignores save roll.


			int toughness;
			int wounds, woundsInflicted;
			int save;

			int movementSpeed;
			int runningRangeBonus;


			bool deployed, dead;

			bool tiedInMelee;

			bool didMoving, didRunning, didShooting, didAssaulting;

			bool drawingSquadSpacing;
			bool drawingActivity;

			bool drawingSelection;
			bool drawingAssaultTarget;
			bool drawingRangedTarget;

			bool drawingMovementRange;
			bool drawingShootingRange;
			bool drawingRapidFiringRange;
			bool drawingAssaultRange;


			static float *heightMap;
			static int mapWidth_X, mapWidth_Z;
	};