#pragma once

	//#include "DataManager.h"

	#include "..//ControlManager//ControlManager.h"

	#include "GUI_EventManager//GUI_EventManager.h"


	class Control;
	class GUI;

	class InputProcessor
	{
		friend GUI;

		public:
			
			static InputProcessor& Singleton ()
			{
				static InputProcessor singleton;
				return singleton;
			}
//
//
			void SetReadingInput (bool const &IsReadingInput);
			bool IsReadingInput  ();

			POINT GetCursor  ();

	/******************************************************
			Functions, that set input flags.
	******************************************************/

			void KeyDown             (int const &keyCode);
			void KeyUp               (int const &keyCode);

			void LeftMouseDown       ();
			void MiddleMouseDown     ();
			void RightMouseDown      ();

			void LeftMouseUp         ();
			void MiddleMouseUp       ();
			void RightMouseUp        ();

			void LeftMouseDblClick   ();
			void MiddleMouseDblClick ();
			void RightMouseDblClick  ();

			void Scroll              (int const &scrollInfo);

			void MoveCursor          (POINT const &mousePosition);

		private:

			GUI_DataManager &Data;
			ControlManager &Controls;

			GUI_EventManager &EventManager;


			POINT cursor, oldCursor;

			int   key;

			int   scrollInfo;

		// Flags that determine nature of the input.

			bool  keyDown, keyPressed, keyUp,

			      leftDown,     middleDown,     rightDown,
				  leftPressed,  middlePressed,  rightPressed,
				  leftUp,       middleUp,       rightUp,
				  leftDblClick, middleDblClick, rightDblClick;

			bool ReceivedInput;
			bool ProcessingInput;

			bool ReadingInput;


	/******************************************************
			Input "digestion" functions.
	******************************************************/

			void ProcessInput ();
			
				void ProcessKey ();

				inline void ProcessKeyDown    ();
				inline void ProcessKeyPressed ();
				inline void ProcessKeyUp      ();

				void ProcessMouse ();

				inline void ProcessMouseDown ();

					inline void ProcessMouseLeftDown   ();	
					inline void ProcessMouseMiddleDown ();
					inline void ProcessMouseRightDown  ();

				inline void ProcessMousePressed ();

					inline void ProcessMouseLeftPressed   ();
					inline void ProcessMouseMiddlePressed ();
					inline void ProcessMouseRightPressed  ();

				inline void ProcessMouseUp ();

					inline void ProcessMouseLeftUp   ();
					inline void ProcessMouseMiddleUp ();
					inline void ProcessMouseRightUp  ();

				inline void ProcessMouseDblClick ();

					inline void ProcessMouseLeftDblClick   ();
					inline void ProcessMouseMiddleDblClick ();
					inline void ProcessMouseRightDblClick  ();

				void ProcessScroll ();

				inline bool CursorHasChangedPosition ();
				void        ProcessMouseMovement	 ();

					inline void ProcessControlResizing ();
					inline void PerformControlResizing (Control *ctrl);

					inline void ProcessControlDragging ();
					inline void PerformControlDragging (Control *ctrl);


					inline void ScanForAndManagePossibleCollision(Control *ctrlAfterMovement, Control *ctrlBeforeMovement);

					inline bool CheckForCollisionWithGuiBorder (Control *ctrl);
						inline bool CollisionWithTop    (Control *ctrl);
						inline bool CollisionWithBottom (Control *ctrl);
						inline bool CollisionWithLeft   (Control *ctrl);
						inline bool CollisionWithRight  (Control *ctrl);

					inline void CorrectCollisionWithGUIBorder  (Control *ctrl);
						inline void AdjustByTop    (Control *ctrl);
						inline void AdjustByBottom (Control *ctrl);
						inline void AdjustByLeft   (Control *ctrl);
						inline void AdjustByRight  (Control *ctrl);

			inline bool MouseKeysPressed			();
			inline bool NoKeysPressed				();
			inline bool CheckInputValuesCorrectness ();
			inline void ClearInputValues            ();

	/*****************************************************/

			void Save (std::fstream &file);
			void Load (std::fstream &file);

	/******************************************************
			Hidden consructor and destructor.
	******************************************************/

			InputProcessor();
			~InputProcessor();

	};

