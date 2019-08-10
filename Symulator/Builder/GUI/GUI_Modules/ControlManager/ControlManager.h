#pragma once

	#include "..//..//Controls//ControlHierarchy.h"

	#include "..//DataManager//DataManager.h"

	class InputProcessor;
	class GUI;


	class ControlManager
	{
		friend InputProcessor;
		friend GUI;

		public:

			GUI_DataManager &Data;

			static ControlManager& Singleton ()
			{
				static ControlManager singleton;
				return singleton;
			}
		//
	/******************************************************
			Control managment.
	******************************************************/

			Control* GetControl    (std::string const &name) throw( std::string );
			Control* GetControl    (int const &index)        throw( std::string );

			void     AddControl    (Control *newCtrl)	     throw( std::string );
				private:
				inline void CheckForControlCollision (Control *ctrl)  throw( std::string );
			public:

			void     RemoveControl (Control *(&ctrl))        throw( std::string );
			void     RemoveControl (std::string const &name) throw( std::string );
			void     RemoveControl (int const &index)        throw( std::string );
				private:
					inline void PerformControlRemovement (int index);
			public:

			void RemoveAllControls () throw( std::string );

			int GetControlCount ();

			void SetToTheFront (Control *pointer) throw( std::string );
			void SetToTheBack  (Control *pointer) throw( std::string );


			void SetVeritcalSegregation   ();
			void SetHorizontalSegregation ();

			void Tab        ();
			void ReverseTab ();

	/******************************************************
			Focus managment.
	******************************************************/

			void     SetCurrentFocus (Control *ctrl)           throw( std::string );
			void     SetCurrentFocus (std::string const &name) throw( std::string );
			void     SetCurrentFocus (int const &index)        throw( std::string );
			
			Control* GetCurrentFocus ();

			private:
			bool        ScanForNewFocus (POINT &mousePosition);
			inline void SetNewFocus     (int index);
			public:

		private:

			std::vector <Control*> controlArray;
			Control *currentFocus;

			bool VerticalSegregation;

	/******************************************************
			Functions used both by control
			and focus managment.
	******************************************************/

			int	FindControlIndexByPointer (Control *ctrl)           throw( std::string );
			int	FindControlIndexByName    (std::string const &name) throw( std::string );

	/*****************************************************/

			void SortControls ();
	//
				static bool SegregateVertically    (Control *A, Control *B)
				{
					POINT centerA = A->GetPosition(),
						  centerB = B->GetPosition();

					return centerA.y > centerB.y;
				}

				static bool SegregateHorizontally  (Control *A, Control *B)
				{
					POINT centerA = A->GetPosition(),
						  centerB = B->GetPosition();

					return centerA.x > centerB.x;
				}

				static bool SortHandlersToTheFront (Control *A, Control *B)
				{
					std::string typeA = A->GetType(),
						        typeB = B->GetType();

					if( typeA == "Handler" && typeB != "Handler" )
					{
						return true;
					}
					else
						return false;
				}

				static bool SortHandlersToTheBack  (Control *A, Control *B)
				{
					std::string typeA = A->GetType(),
						        typeB = B->GetType();

					if( typeA != "Handler" && typeB == "Handler" )
					{
						return true;
					}
					else
						return false;
				}
	//
	/*****************************************************/

			void Save (std::fstream &file);
			void Load (std::fstream &file) throw( std::string );

				inline int  LoadControlCount     (std::fstream &file);
				inline void LoadControls         (std::fstream &file, int const &controlCount);
				inline int  LoadControlType      (std::fstream &file);
				inline bool LoadSpecificControl  (std::fstream &file, int const &controlType);

				inline void AdjustControlTextures (Control *loadedControl);


	/******************************************************
			Hidden consructor and destructor.
	******************************************************/

			ControlManager  ();
			~ControlManager ();
	};