#pragma once

	class Control;

	class Control_EventManager
	{
		friend Control;

		public:

			void (*Ptr_Create)		        (Control *ctrl);
			void (*Ptr_Destroy)		        (Control *ctrl);

			void (*Ptr_KeyDown)             (Control *ctrl, int const &keyCode);
			void (*Ptr_KeyPressed)          (Control *ctrl, int const &keyCode);
			void (*Ptr_KeyUp)               (Control *ctrl, int const &keyCode);

			void (*Ptr_MouseLeftDown)       (Control *ctrl, POINT const &mousePosition);
			void (*Ptr_MouseMiddleDown)     (Control *ctrl, POINT const &mousePosition);
			void (*Ptr_MouseRightDown)      (Control *ctrl, POINT const &mousePosition);

			void (*Ptr_MouseLeftPressed)    (Control *ctrl, POINT const &mousePosition);
			void (*Ptr_MouseMiddlePressed)  (Control *ctrl, POINT const &mousePosition);
			void (*Ptr_MouseRightPressed)   (Control *ctrl, POINT const &mousePosition);

			void (*Ptr_MouseLeftUp)         (Control *ctrl, POINT const &mousePosition);
			void (*Ptr_MouseMiddleUp)       (Control *ctrl, POINT const &mousePosition);
			void (*Ptr_MouseRightUp)        (Control *ctrl, POINT const &mousePosition);

			void (*Ptr_MouseLeftDblClick)   (Control *ctrl, POINT const &mousePosition);
			void (*Ptr_MouseMiddleDblClick) (Control *ctrl, POINT const &mousePosition);
			void (*Ptr_MouseRightDblClick)  (Control *ctrl, POINT const &mousePosition);

			void (*Ptr_MouseMove)           (Control *ctrl, POINT const &mousePosition);
			void (*Ptr_MouseDrag)           (Control *ctrl, POINT const &mousePosition);

			void (*Ptr_Scroll)              (Control *ctrl, int const &scrollInfo);

			void (*Ptr_FocusReceived)       (Control *ctrl);
			void (*Ptr_FocusLost)           (Control *ctrl);

			void (*Ptr_Collision)           (Control *ctrl);

			void (*Ptr_Moving)              (Control *ctrl, POINT const &mousePositionDelta);
			void (*Ptr_Resizing)            (Control *ctrl, POINT const &mousePositionDelta);
			void (*Ptr_Scaling)             (Control *ctrl, float const &scaleMultiplier);

			void (*Ptr_Rendering)           (Control *ctrl, Color const &GUIcolor, bool const &TransparentGUI);

		protected:

			Control_EventManager  ();
			~Control_EventManager ();

			void OnCreate		       (Control *ctrl);
			void OnDestroy		       (Control *ctrl);

			void OnKeyDown             (Control *ctrl, int const &keyCode);
			void OnKeyPressed          (Control *ctrl, int const &keyCode);
			void OnKeyUp               (Control *ctrl, int const &keyCode);

			void OnMouseLeftDown       (Control *ctrl, POINT const &mousePosition);
			void OnMouseMiddleDown     (Control *ctrl, POINT const &mousePosition);
			void OnMouseRightDown      (Control *ctrl, POINT const &mousePosition);

			void OnMouseLeftPressed    (Control *ctrl, POINT const &mousePosition);
			void OnMouseMiddlePressed  (Control *ctrl, POINT const &mousePosition);
			void OnMouseRightPressed   (Control *ctrl, POINT const &mousePosition);

			void OnMouseLeftUp         (Control *ctrl, POINT const &mousePosition);
			void OnMouseMiddleUp       (Control *ctrl, POINT const &mousePosition);
			void OnMouseRightUp        (Control *ctrl, POINT const &mousePosition);

			void OnMouseLeftDblClick   (Control *ctrl, POINT const &mousePosition);
			void OnMouseMiddleDblClick (Control *ctrl, POINT const &mousePosition);
			void OnMouseRightDblClick  (Control *ctrl, POINT const &mousePosition);

			void OnMouseMoved          (Control *ctrl, POINT const &mousePosition);
			void OnMouseDragged        (Control *ctrl, POINT const &mousePosition);

			void OnScroll              (Control *ctrl, int const &scrollInfo);

			void OnFocusReceived       (Control *ctrl);
			void OnFocusLost           (Control *ctrl);

			void OnCollision           (Control *ctrl);

			void OnMoving              (Control *ctrl, POINT const &mousePositionDelta);
			void OnResizing            (Control *ctrl, POINT const &mousePosition);
			void OnScaling             (Control *ctrl, float const &scaleMultiplier);

			void OnRendering           (Control *ctrl, Color const &GUIcolor, bool const &TransparentGUI);

	};