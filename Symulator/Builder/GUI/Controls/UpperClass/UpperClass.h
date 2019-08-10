#pragma once

	#include "..//LowerClasses//LowerClasses.h"

	#define BACKGROUND (0)


	 class UpperClass: public Control
	 {
		public:

			UpperClass& operator= (UpperClass const &upper);


	/******************************************************
			Basic managment functions.
	******************************************************/

			Control* GetFocusedContent ();

			void SetBackground     (unsigned int const &textureID);

			void SetBorders        (RECT const &Border);
			void SetColor          (Color const &color);

			void SetMaxHeight      (float const &height);
			void SetMaxWidth       (float const &width);

			void SetTransparency   (bool const &IsTransparent);


		protected:

			std::vector <Control*> contentArray;
			Control	*focusedContent;


			virtual void AdjustContentColor () {};
			virtual void AdjustContentSize  () {};
			virtual void UpdateContent ();

	/******************************************************
			Standard functions redefinitions.
	******************************************************/

			void SetTextures (std::vector <unsigned int> const &textures);
			void SetCurrentTexture (const int &TextureState);


			void KeyDown             (int const &keyCode);
			void KeyPressed          (int const &keyCode);
			void KeyUp               (int const &keyCode);

			void MouseLeftDown       (POINT const &mousePosition);
			void MouseMiddleDown     (POINT const &mousePosition);
			void MouseRightDown      (POINT const &mousePosition);

			void MouseLeftPressed    (POINT const &mousePosition);
			void MouseMiddlePressed  (POINT const &mousePosition);
			void MouseRightPressed   (POINT const &mousePosition);

			void MouseLeftUp         (POINT const &mousePosition);
			void MouseMiddleUp       (POINT const &mousePosition);
			void MouseRightUp        (POINT const &mousePosition);

			void MouseLeftDblClick   (POINT const &mousePosition);
			void MouseMiddleDblClick (POINT const &mousePosition);
			void MouseRightDblClick  (POINT const &mousePosition);

			void MouseMoved          (POINT const &mousePosition);
			void MouseDragged        (POINT const &mousePosition);

			void Scroll              (int const &scrollInfo);

			void Collision		     ();
			
			public:
			void Move                (POINT const &mousePositionDelta);
			protected:

			void Resize              (POINT const &mousePosition);

			public:
			void Scale               (float const &scaleMultiplier);
			protected:

			void Render              (Color const &GUIColor, bool const &TransparentGUI);

	/*****************************************************/

			void Save (std::fstream &file);
			void Load (std::fstream &file);

	/*****************************************************/
	 
			UpperClass();
			~UpperClass();

	 };