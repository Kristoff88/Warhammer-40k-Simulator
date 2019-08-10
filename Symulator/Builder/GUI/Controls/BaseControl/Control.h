#pragma once

	#include "Control_EventManager.h"


/******************************************************
	Base class for all higher-class controls.
******************************************************/
	
	class UpperClass;
	class Handler;

	class ControlManager;
	class InputProcessor;
	class GUI;


	class Control
	{
		friend UpperClass;
		friend Handler;

		friend ControlManager;
		friend InputProcessor;
		friend GUI;

		public:

			Control_EventManager EventManager;


			Control  ();
				private:
				inline void SetDefaultControlData  ();
				inline void SetDefaultBooleanFlags ();
			public:

			Control  (Control const &ctrl);
			virtual Control& operator= (Control const &ctrl);
				private:
				std::string CreateNameCopy   (std::string name);
				inline void CopyControlData  (Control const &ctrl);
				inline void CopyBooleanFlags (Control const &ctrl);
			public:

			~Control ();

	/******************************************************
			Basic managment functions.
	******************************************************/

		/*		Mutators		*/

			virtual void SetName           (std::string const &newName)  throw( std::string );
				private:
				void RemoveNameFromRegistry (std::string name);
				void CheckForNameCollision  (std::string name)  throw( std::string );
			public:

			virtual void SetBorders        (RECT const &Border);
  
			virtual void SetColor          (Color const &color);

			virtual void SetTextures (std::vector <unsigned int> const &Textures);
			virtual void SetCurrentTexture (int const &TextureState);

			virtual void SetFrameThickness (float const &width);

			virtual void SetMinWidth       (float const &width);
			virtual void SetMaxWidth       (float const &width);

			virtual void SetMinHeight      (float const &height);
			virtual void SetMaxHeight      (float const &height);

			virtual void SetTransparency   (bool const &IsTransparent);
			virtual void SetFocusable      (bool const &IsFocusable);
			virtual void SetObstacle       (bool const &IsObstacle);
			virtual void SetStatic		   (bool const &IsStatic);
			virtual void SetScalable	   (bool const &IsScalable);
			virtual void SetResizability   (bool const &Resizability);
			virtual void SetReadingInput   (bool const &inputReadiness);
			virtual void SetRendering      (bool const &IsRendering);

		/*		Akccessors		*/
			
			std::string					GetName           ();
			std::string					GetType           ();

			RECT						GetBorders        ();

			Color						GetColor          ();

			std::vector <unsigned int>	GetTextures       ();
			int							GetCurrentTexture ();

			float						GetFrameThickness ();

			float						GetMinWidth       ();
			float						GetMaxWidth       ();
			float						GetWidth		  ();

			float						GetMinHeight      ();
			float						GetMaxHeight      ();
			float						GetHeight		  ();

			POINT						GetPosition		  ();
			POINT						GetPositionDelta  (POINT const &anotherPosition);
			POINT						GetPositionDelta  (Control *ctrl);

		/*		Predicators		*/

			bool IsTransparent      ();
			bool IsFocused          ();
			bool IsFocusable        ();
			bool IsObstacle			();
			bool IsStatic           ();
			bool IsScalable         ();
			bool IsCurrentlyResized ();
			bool IsResizable        ();
			bool IsReadingInput     ();
			bool IsRendering		();

		protected:

	/*****************************************************/

			virtual std::string InsertAdditionalErrorInfo ();

				inline std::string InsertControlName ();
				inline std::string InsertControlType ();

	/*****************************************************/

			void ControlSizeCorrection   ();

			inline void WidthCorrection  ();
			inline bool WidthIsTooBig    ();
			inline bool WidthIsTooSmall  ();

			inline void HeightCorrection ();
			inline bool HeightIsTooBig   ();
			inline bool HeightIsTooSmall ();

	/******************************************************
			Functions that determine, whether control
			was hit by other control.
	******************************************************/

			void ManagePossibleBorderCollision (Control *ctrlAfterMovement, Control *ctrlBeforeMovement);

				inline bool CheckForBorderCollision (Control *ctrl);
				inline void AdjustCollidingBorders  (Control *ctrlAfterMovement, Control *ctrlBeforeMovement);

				inline void AdjustVertically   (Control *ctrlAfterMovement, Control *ctrlBeforeMovement);
					inline void AdjustByTop    (Control *ctrl);
					inline void AdjustByBottom (Control *ctrl);

				inline void AdjustHorizontally (Control *ctrlAfterMovement, Control *ctrlBeforeMovement);
					inline void AdjustByLeft   (Control *ctrl);
					inline void AdjustByRight  (Control *ctrl);

				inline bool VerticalCollision       (Control *ctrl);
					inline bool CollisionWithTop    (Control *ctrl);
					inline bool CollisionWithBottom (Control *ctrl);
					inline bool HorizontalAlignment (Control *ctrl);

				inline bool HorizontalCollision     (Control *ctrl);
					inline bool CollisionWithLeft   (Control *ctrl);
					inline bool CollisionWithRight  (Control *ctrl);
					inline bool VerticalAlignment   (Control *ctrl);


	/******************************************************
			Functions that determine, whether control
			or it's frame has been hit.
	******************************************************/

			bool ControlHit (POINT const &mousePosition);

			bool FrameHit   (POINT const &mousePosition);

				inline bool FrameHitOn_TopLeft     (POINT const &mousePosition);
				inline bool FrameHitOn_BottomLeft  (POINT const &mousePosition);
				inline bool FrameHitOn_TopRight    (POINT const &mousePosition);
				inline bool FrameHitOn_BottomRight (POINT const &mousePosition);
				inline bool FrameHitOn_Left        (POINT const &mousePosition);
				inline bool FrameHitOn_Top         (POINT const &mousePosition);
				inline bool FrameHitOn_Right       (POINT const &mousePosition);
				inline bool FrameHitOn_Bottom      (POINT const &mousePosition);

				inline void CheckForMultipleResizingFlags ();
				inline void ClearResizingFlags            ();

	/******************************************************
			Standard utility functions.
			(possible redefinition along the heritage)
	******************************************************/

			virtual void Create				 ()					          {}
			virtual void Destroy			 ()					          {}

			virtual void KeyDown             (int const &keyCode)         {}
			virtual void KeyPressed          (int const &keyCode)         {}
			virtual void KeyUp               (int const &keyCode)         {}

			virtual void MouseLeftDown       (POINT const &mousePosition) {}
			virtual void MouseMiddleDown     (POINT const &mousePosition) {}
			virtual void MouseRightDown      (POINT const &mousePosition) {}

			virtual void MouseLeftPressed    (POINT const &mousePosition) {}
			virtual void MouseMiddlePressed  (POINT const &mousePosition) {}
			virtual void MouseRightPressed   (POINT const &mousePosition) {}

			virtual void MouseLeftUp         (POINT const &mousePosition) {}
			virtual void MouseMiddleUp       (POINT const &mousePosition) {}
			virtual void MouseRightUp        (POINT const &mousePosition) {}

			virtual void MouseLeftDblClick   (POINT const &mousePosition) {}
			virtual void MouseMiddleDblClick (POINT const &mousePosition) {}
			virtual void MouseRightDblClick  (POINT const &mousePosition) {}

			virtual void MouseMoved          (POINT const &mousePosition) {}
			virtual void MouseDragged        (POINT const &mousePosition) {}

			virtual void Scroll              (int const &scrollInfo)      {}

			virtual void FocusReceived       ();
			virtual void FocusLost           ();

			virtual void Collision		     () {}
			
			public:
			virtual void Move                (POINT const &mousePositionDelta);
			protected:

			virtual void Resize              (POINT const &mousePosition);
				inline void ResizeBy_TopLeft     (POINT const &mousePosition);
				inline void ResizeBy_Top         (POINT const &mousePosition);
				inline void ResizeBy_TopRight    (POINT const &mousePosition);
				inline void ResizeBy_Left        (POINT const &mousePosition);
				inline void ResizeBy_Right       (POINT const &mousePosition);
				inline void ResizeBy_BottomLeft  (POINT const &mousePosition);
				inline void ResizeBy_Bottom      (POINT const &mousePosition);
				inline void ResizeBy_BottomRight (POINT const &mousePosition);

			public:
			virtual void Scale               (float const &scaleMultiplier);
			protected:

			virtual void Render              (Color const &GUIColor, bool const &TransparentGUI);

	/******************************************************
			Event managment functions.
			( call standard and specific functions )
	******************************************************/

			virtual void OnCreate			   ();
			virtual void OnDestroy			   ();

			virtual void OnKeyDown             (int const &keyCode);
			virtual void OnKeyPressed          (int const &keyCode);
			virtual void OnKeyUp               (int const &keyCode);

			virtual bool OnMouseLeftDown       (POINT const &mousePosition);
			virtual bool OnMouseMiddleDown     (POINT const &mousePosition);
			virtual bool OnMouseRightDown      (POINT const &mousePosition);

			virtual void OnMouseLeftPressed    (POINT const &mousePosition);
			virtual void OnMouseMiddlePressed  (POINT const &mousePosition);
			virtual void OnMouseRightPressed   (POINT const &mousePosition);

			virtual void OnMouseLeftUp         (POINT const &mousePosition);
			virtual void OnMouseMiddleUp       (POINT const &mousePosition);
			virtual void OnMouseRightUp        (POINT const &mousePosition);

			virtual bool OnMouseLeftDblClick   (POINT const &mousePosition);
			virtual bool OnMouseMiddleDblClick (POINT const &mousePosition);
			virtual bool OnMouseRightDblClick  (POINT const &mousePosition);

			virtual void OnMouseMoved          (POINT const &mousePosition);
			virtual void OnMouseDragged        (POINT const &mousePosition);

			virtual void OnScroll              (int const &scrollInfo);

			virtual void OnFocusReceived       ();
			virtual void OnFocusLost           ();

			virtual void OnCollision	       ();

			virtual void OnMoving              (POINT const &mousePositionDelta);
			virtual void OnResizing            (POINT const &mousePosition);
			virtual void OnScaling             (float const &scaleMultiplier);

			virtual void OnRendering           (Color const &GUIColor, bool const &TransparentGUI);

	/*****************************************************/

			virtual void Save (std::fstream &file);
				inline void SaveControlBorder   (std::fstream &file);
				inline void SaveControlColor    (std::fstream &file);
				inline void SaveControlTextures (std::fstream &file);
				inline void SaveControlBooleans (std::fstream &file);
			protected:

			virtual void Load (std::fstream &file);
				private:
				inline void LoadControlBorder   (std::fstream &file);
				inline void LoadControlColor    (std::fstream &file);
				inline void LoadControlTextures (std::fstream &file);
				inline void LoadControlBooleans (std::fstream &file);
			protected:

	/******************************************************
			Variables, flags, etc.
	******************************************************/

			std::string controlName;
			static std::vector <std::string> nameRegistry;

			std::string controlType;	

			RECT  controlBorder;

			float frameThickness;

			float minControlWidth,  maxControlWidth,
				  minControlHeight, maxControlHeight;

			Color controlColor;

			std::vector <unsigned int> textures;
			int	currentTexture;


		// Reisizng flags (only one can be set as true)

			bool ResizingBy_TopLeft,    ResizingBy_Top,    ResizingBy_TopRight,
				 ResizingBy_Left,						   ResizingBy_Right,
				 ResizingBy_BottomLeft, ResizingBy_Bottom, ResizingBy_BottomRight;

			bool Focused;	


			bool Transparent;		
			bool Focusable;		
			bool Obstacle;
			bool Static;		
			bool Scalable;		
			bool Resizable;		
			bool ReadingInput;	
			bool Rendering;
	};
