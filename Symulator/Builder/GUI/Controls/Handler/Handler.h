#pragma once

	#include "..//BaseControl//Control.h"


	class Handler: public Control
	{
		public:

			Handler  ();
				private:
			public:

			Handler  (Handler const &handler);
			Handler& operator= (Handler const &handler);
				private:
				void CopyHandlerData (Handler const &handler);
			public:

			~Handler ();

	/******************************************************
			Handlings managment functions.
	******************************************************/

			void AddHandling        (Control *ctrl)             throw( std::string );
				private:
				inline void PerformAddition				  (Control *ctrl);

				inline void InformAboutNullPointer       ()		throw( std::string );
				inline void InformAboutThisPointer       ()		throw( std::string );
				inline bool DuplicatePointerHasBeenGiven (Control *ctrl);
				inline void InformAboutDuplicatePointer  ()		throw( std::string );
			public:

			Control* GetHandling	(unsigned int const &index) throw( std::string );

			void RemoveHandling     (Control *ctrl)             throw( std::string );
				private:
				inline void InformAboutNoMatchToPointer ()		throw( std::string );
			public:
			void RemoveHandling     (unsigned int const &index) throw( std::string );

			void RemoveAllHandlings ();

			int GetHandlingsCount   ();

			private:
			bool GivenIndexIsCorrect       (unsigned int const &index);
			void InformAboutIncorrectIndex (unsigned int const &index) throw( std::string );
			public:

	/******************************************************
			Basic managment functions.
	******************************************************/

			void SetBorders        (RECT const &Border);

			void SetColor          (Color const &color);


			void SetFrameThickness (float const &width);

			void SetMaxWidth       (float const &width)   throw( std::string );
			void SetMaxHeight      (float const &height)  throw( std::string );


			void SetTransparency (bool const &IsTransparent);
			void SetFocusable    (bool const &IsFocusable);
			void SetObstacle     (bool const &IsObstacle);
			void SetStatic		 (bool const &IsStatic);
			void SetScalable	 (bool const &IsScalable);
			void SetResizability (bool const &Resizability);
			void SetReadingInput (bool const &inputReadiness);
			void SetRendering    (bool const &IsRendering);

		protected:

			std::vector <Control*> handlings;


	/******************************************************
			Standard functions redefinitions.
	******************************************************/

			public:
			void Move   (POINT const &mousePositionDelta);
				private:
				inline void ShiftHandlings (POINT const &mousePositionDelta);
			public:

			void Scale  (float const &scaleMultiplier);
				private:
				inline void ScaleHandlings (float const &scaleMultiplier);
			protected:


			bool OnMouseLeftDown       (POINT const &mousePosition);
			bool OnMouseMiddleDown     (POINT const &mousePosition);
			bool OnMouseRightDown      (POINT const &mousePosition);

			bool OnMouseLeftDblClick   (POINT const &mousePosition);
			bool OnMouseMiddleDblClick (POINT const &mousePosition);
			bool OnMouseRightDblClick  (POINT const &mousePosition);

				private:
				bool ClickedAnyHandling (POINT const &mousePosition);
			protected:

	/*****************************************************/

			void CorrigateHandlingsPositions ();

				inline void AdjustHandlingsBoundries	  (Control *ctrl);
				inline void CorrigateHandlingsBorders     (Control *ctrl);
				inline void CorrigateHorizontally		  (RECT &handlingBorder);
				inline void CorrigateVeritcally			  (RECT &handlingBorder);

	/*****************************************************/

			void Save (std::fstream &file);
			inline void SaveHandlings      (std::fstream &file);

			void Load (std::fstream &file);
			inline void LoadHandlings (std::fstream &file);
	};