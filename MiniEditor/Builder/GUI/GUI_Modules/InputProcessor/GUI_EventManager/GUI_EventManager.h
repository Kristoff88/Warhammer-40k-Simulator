#pragma once

	class InputProcessor;
	class GUI;

	/******************************************************
		Structure holding pointers to a specific, 
		user-defined functions, that are used on 
		certain events.
	******************************************************/

	class GUI_EventManager
	{
		friend InputProcessor;
		friend GUI;

		public:

			static GUI_EventManager& Singleton ()
			{
				static GUI_EventManager singleton;
				return singleton;
			}
		//
		//
			void (*Ptr_KeyDown)             (int const &keyCode);
			void (*Ptr_KeyPressed)          (int const &keyCode);
			void (*Ptr_KeyUp)               (int const &keyCode);

			void (*Ptr_MouseLeftDown)       (POINT const &mousePosition);
			void (*Ptr_MouseMiddleDown)     (POINT const &mousePosition);
			void (*Ptr_MouseRightDown)      (POINT const &mousePosition);

			void (*Ptr_MouseLeftPressed)    (POINT const &mousePosition);
			void (*Ptr_MouseMiddlePressed)  (POINT const &mousePosition);
			void (*Ptr_MouseRightPressed)   (POINT const &mousePosition);

			void (*Ptr_MouseLeftUp)         (POINT const &mousePosition);
			void (*Ptr_MouseMiddleUp)       (POINT const &mousePosition);
			void (*Ptr_MouseRightUp)        (POINT const &mousePosition);

			void (*Ptr_MouseLeftDblClick)   (POINT const &mousePosition);
			void (*Ptr_MouseMiddleDblClick) (POINT const &mousePosition);
			void (*Ptr_MouseRightDblClick)  (POINT const &mousePosition);

			void (*Ptr_MouseMove)           (POINT const &mousePosition);
			void (*Ptr_MouseDrag)           (POINT const &mousePosition);

			void (*Ptr_Scroll)              (int const &scrollInfo);

			void (*Ptr_Moving)              (POINT const &mousePositionDelta);
			void (*Ptr_Scaling)             (float const &scaleMultiplier);

		private:

			GUI_EventManager  ();
			~GUI_EventManager ();

			void OnKeyDown             (int const &keyCode);
			void OnKeyPressed          (int const &keyCode);
			void OnKeyUp               (int const &keyCode);

			void OnMouseLeftDown       (POINT const &mousePosition);
			void OnMouseMiddleDown     (POINT const &mousePosition);
			void OnMouseRightDown      (POINT const &mousePosition);

			void OnMouseLeftPressed    (POINT const &mousePosition);
			void OnMouseMiddlePressed  (POINT const &mousePosition);
			void OnMouseRightPressed   (POINT const &mousePosition);

			void OnMouseLeftUp         (POINT const &mousePosition);
			void OnMouseMiddleUp       (POINT const &mousePosition);
			void OnMouseRightUp        (POINT const &mousePosition);

			void OnMouseLeftDblClick   (POINT const &mousePosition);
			void OnMouseMiddleDblClick (POINT const &mousePosition);
			void OnMouseRightDblClick  (POINT const &mousePosition);

			void OnMouseMoved          (POINT const &mousePosition);
			void OnMouseDragged        (POINT const &mousePosition);

			void OnScroll              (int const &scrollInfo);

			void OnMoving              (POINT const &mousePositionDelta);
			void OnScaling             (float const &scaleMultiplier);
	};
