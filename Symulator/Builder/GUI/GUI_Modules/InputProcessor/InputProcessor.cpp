#include "GlobalHeaders.h"

#include "InputProcessor.h"

class Control;

	void InputProcessor::SetReadingInput (bool const &IsReadingInput)
	{
		ReadingInput = IsReadingInput;

		if( !ReadingInput )
		{
			ClearInputValues();	
		}
	}
	bool InputProcessor::IsReadingInput  ()
	{
		return ReadingInput;
	}

//
	POINT InputProcessor::GetCursor ()
	{
		return cursor;
	}
//
/******************************************************
		Functions that set input flags
******************************************************/

	void InputProcessor::KeyDown             (int const &keyCode)
	{
		if( ReadingInput && !ProcessingInput && keyCode )
		{
			keyDown = keyPressed = true;
			key = keyCode;
			ReceivedInput = true;
		}
	}

	void InputProcessor::KeyUp               (int const &keyCode)
	{
		if( ReadingInput && !ProcessingInput && keyCode )
		{
			keyUp = true;
			keyPressed = false;
			key = keyCode;
			ReceivedInput = true;
		}
	}

//
	void InputProcessor::LeftMouseDown       ()
	{
		if( ReadingInput && !ProcessingInput )
		{
			leftDown = leftPressed = true;
			ReceivedInput = true;
		}
	}

	void InputProcessor::MiddleMouseDown     ()
	{
		if( ReadingInput && !ProcessingInput )
		{
			middleDown = middlePressed = true;
			ReceivedInput = true;
		}
	}

	void InputProcessor::RightMouseDown      ()
	{	
		if( ReadingInput && !ProcessingInput )
		{
			rightDown = rightPressed = true;
			ReceivedInput = true;
		}
	}

//
	void InputProcessor::LeftMouseUp         ()
	{
		if( ReadingInput && !ProcessingInput )
		{
			leftUp = true;
			leftPressed = false;
			ReceivedInput = true;
		}
	}

	void InputProcessor::MiddleMouseUp       ()
	{
		if( ReadingInput && !ProcessingInput )
		{
			middleUp = true;
			middlePressed = false;
			ReceivedInput = true;
		}
	}

	void InputProcessor::RightMouseUp        ()
	{	
		if( ReadingInput && !ProcessingInput )
		{
			rightUp = true;
			rightPressed = false;
			ReceivedInput = true;
		}
	}
//	
	void InputProcessor::LeftMouseDblClick   ()
	{
		if( ReadingInput && !ProcessingInput )
		{
			leftDblClick = true;
			ReceivedInput = true;
		}
	}

	void InputProcessor::MiddleMouseDblClick ()
	{
		if( ReadingInput && !ProcessingInput )
		{
			middleDblClick = true;
			ReceivedInput = true;
		}
	}

	void InputProcessor::RightMouseDblClick  ()
	{
		if( ReadingInput && !ProcessingInput )
		{
			rightDblClick = true;
			ReceivedInput = true;
		}
	}

	void InputProcessor::Scroll              (int const &Info)
	{
		if( ReadingInput && !ProcessingInput )
		{
			scrollInfo = Info;
			ReceivedInput = true;
		}
	}
	void InputProcessor::MoveCursor          (POINT const &mousePosition)
	{
		if( ReadingInput && !ProcessingInput )
		{
			oldCursor = cursor;
			cursor    = mousePosition;
		}
	}

//
/******************************************************
		Input "digestion" functions.
******************************************************/

	void InputProcessor::ProcessInput ()
	{
		ProcessingInput = true;

		ProcessKey();
		ProcessMouse();
		ProcessScroll();

		if( CursorHasChangedPosition() )
		{
			ProcessMouseMovement();
		}

		assert( CheckInputValuesCorrectness() );

		if( NoKeysPressed() )
		{
			ReceivedInput = false;
		}

		ProcessingInput = false;
	}
//
		void InputProcessor::ProcessKey ()
		{
			if( keyDown )
			{
				ProcessKeyDown();
			}

			if( keyPressed )
			{
				ProcessKeyPressed();
			}

			if( keyUp )
			{
				ProcessKeyUp();
			}
		}
//
			void InputProcessor::ProcessKeyDown    ()
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnKeyDown(key);	
				}
				else
				{
					EventManager.OnKeyDown(key);
				}

				keyDown = false;
			}

			void InputProcessor::ProcessKeyPressed ()
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnKeyPressed(key);	
				}
				else 
				{
					EventManager.OnKeyPressed(key);
				}
			}

			void InputProcessor::ProcessKeyUp      ()
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnKeyUp(key);
				}
				else
				{
					EventManager.OnKeyUp(key);
				}

				key = 0;
				keyUp = false;
			}


//
		void InputProcessor::ProcessMouse ()
		{
			ProcessMouseDown();
			ProcessMousePressed();
			ProcessMouseUp();
			ProcessMouseDblClick();
		}
//
		void InputProcessor::ProcessMouseDown ()
		{
			if( leftDown )
			{
				ProcessMouseLeftDown();
			}
			if( middleDown )
			{
				ProcessMouseMiddleDown();
			}
			if( rightDown )
			{
				ProcessMouseRightDown();
			}
		}
//
			void InputProcessor::ProcessMouseLeftDown ()
			{
				if( Controls.currentFocus )
				{
					if( Controls.currentFocus->OnMouseLeftDown(cursor) )
						;
					else
					{
						if( Controls.ScanForNewFocus(cursor) )
						{
							//Scanning already clicked the new focus.
						}
						else
						{
							EventManager.OnMouseLeftDown(cursor);
						}
					}
				}
				else if( Controls.ScanForNewFocus(cursor) )
				{
					//Scanning already clicked the new focus.
				}
				else
				{
					EventManager.OnMouseLeftDown(cursor);
				}

				leftDown = false;
			}
			void InputProcessor::ProcessMouseMiddleDown ()
			{
				if( Controls.currentFocus )
				{
					if( Controls.currentFocus->OnMouseMiddleDown(cursor) )
						;
					else
					{
						if( Controls.ScanForNewFocus(cursor) )
						{
							//Scanning already clicked the new focus.
						}
						else
						{
							EventManager.OnMouseMiddleDown(cursor);
						}
					}

				}
				else if( Controls.ScanForNewFocus(cursor) )
				{
					//Scanning already clicked the new focus.
				}
				else
				{
					EventManager.OnMouseMiddleDown(cursor);
				}

				middleDown = false;
			}

			void InputProcessor::ProcessMouseRightDown  ()
			{
				if( Controls.currentFocus )
				{
					if( Controls.currentFocus->OnMouseRightDown(cursor) )
						;
					else
					{
						if( Controls.ScanForNewFocus(cursor) )
						{
							//Scanning already clicked the new focus.
						}
						else
						{
							EventManager.OnMouseRightDown(cursor);
						}
					}

				}
				else if( Controls.ScanForNewFocus(cursor) )
				{
					//Scanning already clicked the new focus.
				}
				else
				{
					EventManager.OnMouseRightDown(cursor);
				}

				rightDown = false;
			}

//
		void InputProcessor::ProcessMousePressed ()
		{
			if( leftPressed )
			{
				ProcessMouseLeftPressed();
			}
			if( middlePressed )
			{
				ProcessMouseMiddlePressed();
			}
			if( rightPressed )
			{
				ProcessMouseRightPressed();
			}
		}
//
			void InputProcessor::ProcessMouseLeftPressed   ()
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseLeftPressed(cursor);
				}
				else
				{
					EventManager.OnMouseLeftPressed(cursor);
				}
			}

			void InputProcessor::ProcessMouseMiddlePressed ()
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseMiddlePressed(cursor);
				}
				else
				{
					EventManager.OnMouseMiddlePressed(cursor);
				}
			}

			void InputProcessor::ProcessMouseRightPressed  ()
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseRightPressed(cursor);
				}
				else
				{
					EventManager.OnMouseRightPressed(cursor);
				}
			}
//
		void InputProcessor::ProcessMouseUp ()
		{
			if( leftUp )
			{
				ProcessMouseLeftUp();
			}
			if( middleUp )
			{
				ProcessMouseMiddleUp();
			}
			if( rightUp )
			{
				ProcessMouseRightUp();
			}
		}
//
			void InputProcessor::ProcessMouseLeftUp   ()
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseLeftUp(cursor);
				}
				else
				{
					EventManager.OnMouseLeftUp(cursor);
				}

				leftUp = false;
			}

			void InputProcessor::ProcessMouseMiddleUp ()
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseMiddleUp(cursor);
				}
				else
				{
					EventManager.OnMouseMiddleUp(cursor);
				}

				middleUp = false;
			}

			void InputProcessor::ProcessMouseRightUp  ()
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseRightUp(cursor);
				}
				else
				{
					EventManager.OnMouseRightUp(cursor);
				}

				rightUp = false;
			}


//
		void InputProcessor::ProcessMouseDblClick ()
		{
			if( leftDblClick )
			{
				ProcessMouseLeftDblClick();
			}
			if( middleDblClick )
			{
				ProcessMouseMiddleDblClick();
			}
			if( rightDblClick )
			{
				ProcessMouseRightDblClick();
			}
		}
//
			void InputProcessor::ProcessMouseLeftDblClick   ()
			{
				if( Controls.currentFocus )
				{
					if( Controls.currentFocus->OnMouseLeftDblClick(cursor) )
						;
					else
					{
						if( Controls.ScanForNewFocus(cursor) )
						{
							//Scanning already clicked the new focus.
						}
						else
						{
							EventManager.OnMouseLeftDblClick(cursor);
						}
					}
				}
				else if( Controls.ScanForNewFocus(cursor) )
				{
					//Scanning already clicked the new focus.
				}
				else
				{
					EventManager.OnMouseLeftDblClick(cursor);
				}

				/*if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseLeftDblClick(cursor);
				}
				else
				{
					EventManager.OnMouseLeftDblClick(cursor);
				}*/

				leftDblClick = false;
			}

			void InputProcessor::ProcessMouseMiddleDblClick ()
			{
				if( Controls.currentFocus )
				{
					if( Controls.currentFocus->OnMouseMiddleDblClick(cursor) )
						;
					else
					{
						if( Controls.ScanForNewFocus(cursor) )
						{
							//Scanning already clicked the new focus.
						}
						else
						{
							EventManager.OnMouseMiddleDblClick(cursor);
						}
					}
				}
				else if( Controls.ScanForNewFocus(cursor) )
				{
					//Scanning already clicked the new focus.
				}
				else
				{
					EventManager.OnMouseMiddleDblClick(cursor);
				}

				/*if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseMiddleDblClick(cursor);
				}
				else
				{
					EventManager.OnMouseMiddleDblClick(cursor);
				}*/

				middleDblClick = false;
			}

			void InputProcessor::ProcessMouseRightDblClick  ()
			{
				if( Controls.currentFocus )
				{
					if( Controls.currentFocus->OnMouseRightDblClick(cursor) )
						;
					else
					{
						if( Controls.ScanForNewFocus(cursor) )
						{
							//Scanning already clicked the new focus.
						}
						else
						{
							EventManager.OnMouseRightDblClick(cursor);
						}
					}
				}
				else if( Controls.ScanForNewFocus(cursor) )
				{
					//Scanning already clicked the new focus.
				}
				else
				{
					EventManager.OnMouseRightDblClick(cursor);
				}

				/*if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseRightDblClick(cursor);
				}
				else
				{
					EventManager.OnMouseRightDblClick(cursor);
				}*/

				rightDblClick = false;
			}
//
		void InputProcessor::ProcessScroll ()
		{
			if( scrollInfo )
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnScroll(scrollInfo);
				}
				else
				{
					EventManager.OnScroll(scrollInfo);
				}

				scrollInfo = 0;
			}
		}

//
		bool InputProcessor::CursorHasChangedPosition ()
		{
			return cursor.x != oldCursor.x || cursor.y != oldCursor.y;
		}

		void InputProcessor::ProcessMouseMovement	   ()
		{
			if( MouseKeysPressed() )
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseDragged(cursor);

					if( Controls.currentFocus->IsCurrentlyResized() )
					{
						ProcessControlResizing();
					}
					else if( !Controls.currentFocus->IsStatic() )
					{
						ProcessControlDragging();
					}
				}
				else
				{
					EventManager.OnMouseDragged(cursor);
				}
			}
			else
			{
				if( Controls.currentFocus )
				{
					Controls.currentFocus->OnMouseMoved(cursor);
				}
				else
				{
					EventManager.OnMouseMoved(cursor);
				}
			}

			oldCursor = cursor;
		}
//
			void InputProcessor::ProcessControlResizing ()
			{
				assert( Controls.currentFocus != NULL );

				PerformControlResizing(Controls.currentFocus);

				if( CheckForCollisionWithGuiBorder(Controls.currentFocus) )
				{
					CorrectCollisionWithGUIBorder(Controls.currentFocus);
					Controls.currentFocus->OnCollision();
				}

				Controls.SortControls();
			}

			void InputProcessor::PerformControlResizing (Control *ctrl)
			{
				Control *ctrlBeforeResizing = new Control;
				*ctrlBeforeResizing = *ctrl;

				Controls.currentFocus->OnResizing(cursor);
				ScanForAndManagePossibleCollision(ctrl, ctrlBeforeResizing);

				delete ctrlBeforeResizing;
			}
//
			void InputProcessor::ProcessControlDragging ()
			{
				assert( Controls.currentFocus != NULL );

				PerformControlDragging(Controls.currentFocus);

				if( CheckForCollisionWithGuiBorder(Controls.currentFocus) )
				{
					CorrectCollisionWithGUIBorder(Controls.currentFocus);
					Controls.currentFocus->OnCollision();
				}

				Controls.SortControls();
			}

			void InputProcessor::PerformControlDragging (Control *ctrl)
			{
				Control *ctrlBeforeDragging = new Control;
				*ctrlBeforeDragging = *ctrl;

				POINT delta;
				delta.x = cursor.x - oldCursor.x;
				delta.y = cursor.y - oldCursor.y;

				ctrl->OnMoving(delta);
				ScanForAndManagePossibleCollision(ctrl, ctrlBeforeDragging);

				delete ctrlBeforeDragging;
			}
//
			void InputProcessor::ScanForAndManagePossibleCollision(Control *ctrlAfterMovement, Control *ctrlBeforeMovement)
			{
				for(int i = 0; i < Controls.controlArray.size(); i++)
				{
					assert( Controls.controlArray[i] != NULL );

					if( ctrlAfterMovement != Controls.controlArray[i] )
					{
						Controls.controlArray[i]->ManagePossibleBorderCollision(ctrlAfterMovement, ctrlBeforeMovement);
					}
				}
			}

//
			bool InputProcessor::CheckForCollisionWithGuiBorder (Control *ctrl)
			{
				return CollisionWithTop(ctrl)    ||
					   CollisionWithBottom(ctrl) ||
					   CollisionWithLeft(ctrl)   ||
					   CollisionWithRight(ctrl);
			}
//
				bool InputProcessor::CollisionWithTop    (Control *ctrl)
				{
					if( ctrl->controlBorder.top > Data.GUIborder.top )
					{
						return true;
					}
					else
						return false;
				}

				bool InputProcessor::CollisionWithBottom (Control *ctrl)
				{
					if( ctrl->controlBorder.bottom < Data.GUIborder.bottom )
					{
						return true;
					}
					else
						return false;
				}

				bool InputProcessor::CollisionWithLeft   (Control *ctrl)
				{
					if( ctrl->controlBorder.left < Data.GUIborder.left )
					{
						return true;
					}
					else
						return false;
				}

				bool InputProcessor::CollisionWithRight  (Control *ctrl)
				{
					if( ctrl->controlBorder.right > Data.GUIborder.right )
					{
						return true;
					}
					else
						return false;
				}
//
			void InputProcessor::CorrectCollisionWithGUIBorder  (Control *ctrl)
			{
				if( CollisionWithTop(ctrl) )
				{
					AdjustByTop(ctrl);
				}
				if( CollisionWithBottom(ctrl) )
				{
					AdjustByBottom(ctrl);
				}
				if( CollisionWithLeft(ctrl) )
				{
					AdjustByLeft(ctrl);
				}
				if( CollisionWithRight(ctrl) )
				{
					AdjustByRight(ctrl);
				}
			}
//
				void InputProcessor::AdjustByTop    (Control *ctrl)
				{
					POINT adjustment;

					adjustment.x = 0;
					adjustment.y = Data.GUIborder.top - ctrl->controlBorder.top;

					ctrl->Move(adjustment);
				}

				void InputProcessor::AdjustByBottom (Control *ctrl)
				{
					POINT adjustment;

					adjustment.x = 0;
					adjustment.y = Data.GUIborder.bottom - ctrl->controlBorder.bottom;

					ctrl->Move(adjustment);
				}

				void InputProcessor::AdjustByLeft   (Control *ctrl)
				{
					POINT adjustment;

					adjustment.x = Data.GUIborder.left - ctrl->controlBorder.left;
					adjustment.y = 0;
					
					ctrl->Move(adjustment);
				}

				void InputProcessor::AdjustByRight  (Control *ctrl)
				{
					POINT adjustment;

					adjustment.x = Data.GUIborder.right - ctrl->controlBorder.right;
					adjustment.y = 0;

					ctrl->Move(adjustment);
				}
//
	bool InputProcessor::MouseKeysPressed			  ()
	{
		return leftPressed || middlePressed || rightPressed;
	}

	bool InputProcessor::NoKeysPressed			      ()
	{
		return keyPressed && leftPressed && middlePressed && rightPressed == false;
	}

	bool InputProcessor::CheckInputValuesCorrectness ()
	{
	// At the end of input processing, only "pressed" flags can be true.

		bool FlagsCorrect 
			= (keyDown      && keyUp          &&
			   leftDown     && middleDown     && rightDown &&
			   leftUp       && middleUp       && rightUp   &&
			   leftDblClick && middleDblClick && rightDblClick ) == false;

		bool KeyStateCorrect;

		if( key )
		{
			KeyStateCorrect = (keyPressed == true);
		}
		else
		{
 			KeyStateCorrect = (keyPressed == false);
		}

		bool CursorValuesCorrect = (cursor.x == oldCursor.x) && (cursor.y == oldCursor.y);


		/*if( !( FlagsCorrect && KeyStateCorrect && (scrollInfo == 0) && CursorValuesCorrect ) )
		{
			int lol = 1;
		}*/

		return FlagsCorrect && KeyStateCorrect && (scrollInfo == 0) && CursorValuesCorrect;
	}

	void InputProcessor::ClearInputValues            ()
	{
		key = scrollInfo = 0;

		keyDown      = keyUp          = 
		leftDown     = middleDown     = rightDown     =
		leftUp       = middleUp       = rightUp       =
		leftDblClick = middleDblClick = rightDblClick = false;
	}
//
/*****************************************************/

	void InputProcessor::Save (std::fstream &file)
	{
		file << "\t" << "#Reading_Input:" << "\t" << ReadingInput << "\n\n";
	}

	void InputProcessor::Load (std::fstream &file)
	{
		std::string line;

		line = ReadNewValue(file, '#');
		ReadingInput = atoi( line.c_str() );
	}
//
/******************************************************
		Hidden consructor and destructor.
******************************************************/

	InputProcessor::InputProcessor()

		: Data( GUI_DataManager::Singleton() ),
	      Controls( ControlManager::Singleton() ),

		  EventManager( GUI_EventManager::Singleton() ),
	      
	      key(0), scrollInfo(0),
    	  keyDown(false), keyPressed(false), keyUp(false),

		  leftDown(false),     middleDown(false),     rightDown(false),
		  leftPressed(false),  middlePressed(false),  rightPressed(false),
		  leftUp(false),       middleUp(false),       rightUp(false),
		  leftDblClick(false), middleDblClick(false), rightDblClick(false),

		  ProcessingInput(false),
		  ReceivedInput(false),

		  ReadingInput(true)
	{}

	InputProcessor::~InputProcessor()
	{}