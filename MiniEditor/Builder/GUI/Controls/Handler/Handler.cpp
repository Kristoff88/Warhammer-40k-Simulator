	#include "GlobalHeaders.h"

	#include "Handler.h"

	#include "..//..//GUI_Operator.h"


	#define controlManager (GUI::Singleton().Controls)



	Handler::Handler  ()
	{
		controlType = "Handler";
	}
//
	Handler::Handler  (Handler const &handler)
	{
		Control::operator =(handler);
		CopyHandlerData(handler);
	}

	Handler& Handler::operator= (Handler const &handler)
	{
		Control::operator =(handler);
		CopyHandlerData(handler);
		return *this;
	}
//
		void Handler::CopyHandlerData (Handler const &handler)
		{
			handlings = handler.handlings;
		}
//
	Handler::~Handler ()
	{}
//
/******************************************************
		Handlings managment functions.
******************************************************/

	void Handler::AddHandling        (Control *ctrl)             throw( std::string )
	{
		if( ctrl == NULL )
		{
			InformAboutNullPointer();
		}
		else if( ctrl == this )
		{
		}
		else if( DuplicatePointerHasBeenGiven(ctrl) )
		{
			InformAboutDuplicatePointer();
		}
		else
			PerformAddition(ctrl);
	}
//
		void Handler::PerformAddition				(Control *ctrl)
		{
			AdjustHandlingsBoundries(ctrl);
			CorrigateHandlingsBorders(ctrl);

			handlings.push_back(ctrl);
		}

//
		void Handler::InformAboutNullPointer       ()		     throw( std::string )
		{
			std::string error;

			error  = "Given pointer to a Control, is NULL.";
			error += "\n\n";

			error += "Operation:\tAddHandling(Control *ctrl).";
			error += "\n\n";

			error += InsertAdditionalErrorInfo();

			throw error;
		}

		void Handler::InformAboutThisPointer       ()		throw( std::string )
		{
			std::string error;

			error  = "Given pointer to a Control, is \"this\" pointer.";
			error += "\n\n";

			error += "Operation:\tAddHandling(Control *ctrl).";
			error += "\n\n";

			error += InsertAdditionalErrorInfo();

			throw error;
		}

		bool Handler::DuplicatePointerHasBeenGiven (Control *ctrl)
		{
			bool DuplicateFound = false;

			for(int i = 0; i < handlings.size(); i++)
			{
				if( handlings[i] == ctrl )
				{
					assert( DuplicateFound == false );
					DuplicateFound = true;
				}
			}

			return DuplicateFound;
		}

		void Handler::InformAboutDuplicatePointer  ()		     throw( std::string )
		{
			std::string error;

			error  = "Given pointer to a Control, is already in Handler registry.";
			error += "\n\n";

			error += "Operation:\tAddHandling(Control *ctrl).";
			error += "\n\n";

			error += InsertAdditionalErrorInfo();

			throw error;
		}

//
	Control* Handler::GetHandling	 (unsigned int const &index) throw( std::string )
	{
		if( GivenIndexIsCorrect(index) )
		{
			return handlings[index];
		}
		else
		{
			try
			{
				InformAboutIncorrectIndex(index);
			}
			catch(std::string error)
			{
				error += "Operation: GetHandling(unsigned int const &index).";
				throw error;
			}
		}
	}
//
	void Handler::RemoveHandling     (Control *ctrl)             throw( std::string )
	{
		bool HaventFoundMatchYet = true;

		for(int i = 0; i < handlings.size(); i++)
		{
			if( handlings[i] == ctrl )
			{
				assert( HaventFoundMatchYet );

				handlings.erase( handlings.begin()+i );

				HaventFoundMatchYet = false;
			}
		}

		if( HaventFoundMatchYet )
		{
			InformAboutNoMatchToPointer();
		}
	}
//
		void Handler::InformAboutNoMatchToPointer ()			 throw( std::string )
		{
			std::string error;

			error  = "There hasn't been found a match to a pointer.";
			error += "\n\n";
			error += "Operation: RemoveHandling(Control *ctrl).";

			throw error;
		}
//
	void Handler::RemoveHandling     (unsigned int const &index) throw( std::string )
	{
		if( GivenIndexIsCorrect(index) )
		{
			handlings.erase( handlings.begin()+index );
		}
		else
		{
			try
			{
				InformAboutIncorrectIndex(index);
			}
			catch(std::string error)
			{
				error += "Operation: RemoveHandling(unsigned int const &index).";
				throw error;
			}
		}
	}
//
	void Handler::RemoveAllHandlings ()
	{
		handlings.clear();
	}
//
	int Handler::GetHandlingsCount   ()
	{
		return handlings.size();
	}
//
//
	bool Handler::GivenIndexIsCorrect       (unsigned int const &index) throw( std::string )
	{
		if( index < 0 || index >= handlings.size() )
		{
			return false;
		}
		else
			return true;
	}
	void Handler::InformAboutIncorrectIndex (unsigned int const &index) throw( std::string )
	{
		std::string error;

		char indexAsChar[MAX_PATH];
		char handlingsSizeAsChar[MAX_PATH];

		_itoa_s(index,            indexAsChar,         MAX_PATH, 10);
		_itoa_s(handlings.size(), handlingsSizeAsChar, MAX_PATH, 10);

		error  = "Given index '";
		error += indexAsChar;
		error += "' is incorrect";
		error += "\n\n";

		error += "Control name:\t";
		error += controlName;
		error += "\n\n";

		error += "Handligs vector size:\t";
		error += handlingsSizeAsChar;
		error += "\n\n";

		throw error;
	}
//
/******************************************************
		Basic managment functions.
******************************************************/

	void Handler::SetBorders        (RECT const &Border)
	{
		Control::SetBorders(Border);
		CorrigateHandlingsPositions();
	}
//
	void Handler::SetColor          (Color const &color)
	{
		Control::SetColor(color);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetColor(color);
		}
	}
//
//
	void Handler::SetFrameThickness (float const &width)
	{
		Control::SetFrameThickness(width);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetFrameThickness(width);
		}
	}
//
	void Handler::SetMaxWidth       (float const &width)   throw( std::string )
	{
		Control::SetMaxWidth(width);

		for(int i = 0; i < handlings.size(); i++)
		{
			assert( handlings[i] != NULL );
			handlings[i]->SetMaxWidth(width);
		}
	}
	void Handler::SetMaxHeight      (float const &height)  throw( std::string )
	{
		Control::SetMaxHeight(height);

		for(int i = 0; i < handlings.size(); i++)
		{
			assert( handlings[i] != NULL );
			handlings[i]->SetMaxHeight(height);
		}
	}
//
//
	void Handler::SetTransparency (bool const &IsTransparent)
	{
		Control::SetTransparency(IsTransparent);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetTransparency(IsTransparent);
		}
	}	

	void Handler::SetFocusable    (bool const &IsFocusable)
	{
		Control::SetFocusable(IsFocusable);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetFocusable(IsFocusable);
		}
	}

	void Handler::SetObstacle     (bool const &IsObstacle)
	{
		Control::SetObstacle(IsObstacle);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetObstacle(IsObstacle);
		}
	}

	void Handler::SetStatic		  (bool const &IsStatic)
	{
		Control::SetStatic(IsStatic);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetStatic(IsStatic);
		}
	}

	void Handler::SetScalable	  (bool const &IsScalable)
	{
		Control::SetScalable(IsScalable);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetScalable(IsScalable);
		}
	}

	void Handler::SetResizability (bool const &Resizability)
	{
		Control::SetResizability(Resizability);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetResizability(Resizability);
		}
	}

	void Handler::SetReadingInput (bool const &inputReadiness)
	{
		Control::SetReadingInput(inputReadiness);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetReadingInput(inputReadiness);
		}
	}

	void Handler::SetRendering    (bool const &IsRendering)
	{
		Control::SetRendering(IsRendering);

		for(int i = 0; i < handlings.size(); i++)
		{
			handlings[i]->SetRendering(IsRendering);
		}
	}

//
/******************************************************
		Standard functions redefinitions.
******************************************************/

	void Handler::Move   (POINT const &mousePositionDelta)
	{
		Control::Move(mousePositionDelta);
		ShiftHandlings(mousePositionDelta);
		CorrigateHandlingsPositions();
	}
//
		void Handler::ShiftHandlings (POINT const &mousePositionDelta)
		{
			bool previousStaticState;

			for(int i = 0; i < handlings.size(); i++)
			{
				assert( handlings[i] != NULL );

				previousStaticState = handlings[i]->IsStatic();

				handlings[i]->SetStatic( false );
				handlings[i]->OnMoving(mousePositionDelta);
				handlings[i]->SetStatic( previousStaticState );
			}
		}
//
	void Handler::Scale  (float const &scaleMultiplier)
	{
		Control::Scale(scaleMultiplier);
		ScaleHandlings(scaleMultiplier);
		
	}
//
		void Handler::ScaleHandlings (float const &scaleMultiplier)
		{
			for(int i = 0; i < handlings.size(); i++)
			{
				assert( handlings[i] != NULL );
				handlings[i]->OnScaling(scaleMultiplier);
			}
		}
//
//
	bool Handler::OnMouseLeftDown       (POINT const &mousePosition)
	{
		if( ClickedAnyHandling(mousePosition) )
		{
			return false;
		}

		return Control::OnMouseLeftDown(mousePosition);
	}

	bool Handler::OnMouseMiddleDown     (POINT const &mousePosition)
	{
		if( ClickedAnyHandling(mousePosition) )
		{
			return false;
		}

		return Control::OnMouseMiddleDown(mousePosition);
	}

	bool Handler::OnMouseRightDown      (POINT const &mousePosition)
	{
		if( ClickedAnyHandling(mousePosition) )
		{
			return false;
		}

		return Control::OnMouseRightDown(mousePosition);
	}
//
	bool Handler::OnMouseLeftDblClick   (POINT const &mousePosition)
	{
		if( ClickedAnyHandling(mousePosition) )
		{
			return false;
		}

		return Control::OnMouseLeftDblClick(mousePosition);
	}

	bool Handler::OnMouseMiddleDblClick (POINT const &mousePosition)
	{
		if( ClickedAnyHandling(mousePosition) )
		{
			return false;
		}

		return Control::OnMouseMiddleDblClick(mousePosition);
	}

	bool Handler::OnMouseRightDblClick  (POINT const &mousePosition)
	{
		if( ClickedAnyHandling(mousePosition) )
		{
			return false;
		}

		return Control::OnMouseRightDblClick(mousePosition);
	}
//
		bool Handler::ClickedAnyHandling (POINT const &mousePosition)
		{
			for(int i = 0; i < handlings.size(); i++)
			{
				assert( handlings[i] != NULL );

				if( handlings[i]->ControlHit(mousePosition) || handlings[i]->FrameHit(mousePosition) )
				{
					return true;
				}
			}

			return false;
		}
//
/*****************************************************/

	void Handler::CorrigateHandlingsPositions ()
	{
		for(int i = 0; i < handlings.size(); i++)
		{
			assert( handlings[i] != NULL );

			AdjustHandlingsBoundries( handlings[i] );
			CorrigateHandlingsBorders( handlings[i] );
		}
	}
//
		void Handler::AdjustHandlingsBoundries	    (Control *ctrl)
		{
			ctrl->SetMaxHeight( GetHeight() );
			ctrl->SetMaxWidth( GetWidth() );

			assert( ctrl->GetHeight() <= GetHeight() );
			assert( ctrl->GetWidth()  <= GetWidth() );
		}

		void Handler::CorrigateHandlingsBorders     (Control *ctrl)
		{
			RECT handlingBorder = ctrl->GetBorders();

			CorrigateHorizontally(handlingBorder);
			CorrigateVeritcally(handlingBorder);

			ctrl->SetBorders(handlingBorder);
		}

		void Handler::CorrigateHorizontally		    (RECT &handlingBorder)
		{
			bool AdjustedHorizontally = false;

			while( handlingBorder.left < controlBorder.left )
			{
				handlingBorder.left  += 1;
				handlingBorder.right += 1;

				AdjustedHorizontally = true;
			}

			while( handlingBorder.right > controlBorder.right )
			{
				assert( AdjustedHorizontally == false );

				handlingBorder.left  -= 1;
				handlingBorder.right -= 1;
			}
		}

		void Handler::CorrigateVeritcally		    (RECT &handlingBorder)
		{
			bool AdjustedVertically   = false;

			while( handlingBorder.bottom < controlBorder.bottom )
			{
				handlingBorder.bottom  += 1;
				handlingBorder.top     += 1;

				AdjustedVertically = true;
			}

			while( handlingBorder.top > controlBorder.top )
			{
				assert( AdjustedVertically == false );

				handlingBorder.bottom  -= 1;
				handlingBorder.top     -= 1;
			}
		}
//
/*****************************************************/

	void Handler::Save (std::fstream &file)
	{
		Control::Save(file);

		file << "\t" << "#----------------------------------------Handler_components----------------------------------------#" << "\n\n";

		SaveHandlings(file);	
	}
//
	void Handler::SaveHandlings (std::fstream &file)
	{
		file << "\t" << "#Handlings_indices:" << "\n";

		int controlsCount = controlManager.GetControlCount();
		assert( handlings.size() < controlsCount );


		for(int i = 0; i < handlings.size(); i++)
		for(int j = 0; j < controlsCount;    j++)
		{
			if( controlManager.GetControl(j) == handlings[i] )
			{
				file << "\t" << j << "\n";
				break;
			}
		}

		file << "\t" << "End" << "\n";
		file << "\n";
	}
//
	void Handler::Load (std::fstream &file)
	{
		Control::Load(file);
		LoadHandlings(file);
	}
//
	void Handler::LoadHandlings (std::fstream &file)
	{
		std::string line;
		int handlingsIndex;


		do
		{
			line = ReadNewValue(file, '#');

			if( line != "End" )
			{
				assert( IsNumber(line, true) );
				handlingsIndex = atoi( line.c_str() );

				Control *newHandling = controlManager.GetControl(handlingsIndex);
				assert( newHandling != NULL );

				AddHandling(newHandling);
			}
		}
		while( line != "End" );
	}