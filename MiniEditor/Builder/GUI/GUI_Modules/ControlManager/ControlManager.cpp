	#include "GlobalHeaders.h"

	#include "ControlManager.h"

	#include "..//..//Utilities//LineReader//LineReader.h"



	#define CONTROL (1)

	#define LABEL  (2)
	#define EDIT   (3)
	#define BUTTON (4)

	#define CHECKER_BOX  (5)
	#define PROGRESS_BAR (6)
	#define TAB_BAR      (7)

	#define SLIDER        (8)
	#define MARKED_SLIDER (9)
	#define SCROLL_BAR	  (10)

	#define LIST_BOX	  (11)
	#define COMBO_BOX	  (12)

	#define HANDLER		  (13)


/******************************************************
		Control managment.
******************************************************/

	Control* ControlManager::GetControl    (std::string const &name) throw( std::string )
	{
		int index;

		try
		{
			index = FindControlIndexByName(name);
		}
		catch( std::string error )
		{
			error += "\n\nOperation: Trying to get pointer to a control.";
			throw error;
		}

		return controlArray[index];
	}

	Control* ControlManager::GetControl    (int const &index)        throw( std::string )
	{
		if( index >= 0 && index < controlArray.size() )
		{
			assert( controlArray[index] != NULL );
			return controlArray[index];
		}
		else
		{
			std::string error;
			
			char indexAsString[MAX_PATH];
			_itoa_s( index, indexAsString, MAX_PATH, 10 );

			error  = "Index (";
			error += indexAsString;
			error += ") sent to GetControl(index), is inadequate (Smaller than zero or greater than size of control registry).";

			throw error;
		}
	}

//
	void     ControlManager::AddControl    (Control *newCtrl)        throw( std::string )
	{
		if( newCtrl != NULL )
		{
			CheckForControlCollision(newCtrl);

			newCtrl->OnCreate();
			controlArray.push_back(newCtrl);

			SortControls();
		}
		else
		{
			std::string error = "Pointer to a control, that is attempted to be added, is a NULL.";
			throw error;
		}
	}
		void ControlManager::CheckForControlCollision(Control *ctrl) throw( std::string )
		{
			bool ControlCollision = false;

			for(int i = 0; i < controlArray.size(); i++)
			{
				assert( controlArray[i] != NULL );

				if( ctrl == controlArray[i] )
				{
					ControlCollision = true;
					break;
				}
			}

			if( ControlCollision )
			{
				std::string error;

				error  = "Control, by the name of \"";
				error += ctrl->GetName();
				error += "\" is already placed in GUI.";

				throw error;
			}
		}
//
	void     ControlManager::RemoveControl (Control *(&ctrl))        throw( std::string )
	{
		if( ctrl != NULL )
		{
			int index;

			try
			{
				index = FindControlIndexByPointer(ctrl);
			}
			catch( std::string error )
			{
				error += "\n\nOperation: Removing control from registry, by using pointer.";
				throw error;
			}

			PerformControlRemovement(index);
			ctrl = NULL;
		}
		else
		{
			std::string error = "Pointer to a control, that is attempted to be removed, is a NULL.";
			throw error;
		}
	}

	void     ControlManager::RemoveControl (std::string const &name) throw( std::string )
	{
		int index;

		try
		{
			index = FindControlIndexByName(name);
		}
		catch( std::string error )
		{
			error += "\n\nOperation: Removing control from control registry, by using name string.";
			throw error;
		}

		PerformControlRemovement(index);
	}

	void     ControlManager::RemoveControl (int const &index)		 throw( std::string )
	{	
		if( index >= 0 && index < controlArray.size() )
		{
			PerformControlRemovement(index);
		}
		else
		{
			std::string error;
			
			char indexAsString[MAX_PATH];
			_itoa_s( index, indexAsString, MAX_PATH, 10 );

			error  = "Index (";
			error += indexAsString;
			error += ") sent to RemoveControl(index), is inadequate (Smaller than zero, greater than size of control registry or non-existent index).";

			throw error;
		}
	}

//
		void ControlManager::PerformControlRemovement (int index)
		{
			assert( controlArray[index] != NULL );

			if( controlArray[index] == currentFocus )
			{
				currentFocus->OnFocusLost();
				currentFocus = NULL;
			}

			controlArray[index]->OnDestroy();
			delete controlArray[index];

			controlArray.erase( controlArray.begin() + index );

			SortControls();
		}
//
	void ControlManager::RemoveAllControls () throw( std::string )
	{
		for(int i = controlArray.size()-1; i >= 0 ; i--)
		{
			assert( controlArray[i] != NULL );

			if( controlArray[i] == currentFocus )
			{
				currentFocus->OnFocusLost();
				currentFocus = NULL;
			}

			RemoveControl(i);
		}
	}
//
	int ControlManager::GetControlCount ()
	{
		return controlArray.size();
	}
//
//
	void ControlManager::SetToTheFront (Control *pointer) throw( std::string )
	{
		int index = -1;

		for(int i = 0; i < controlArray.size(); i++)
		{
			assert( controlArray[i] != NULL );

			if( pointer == controlArray[i] )
			{
				index = i;
				break;
			}
		}

		if( index != -1 && index <= controlArray.size()-1 )
		{
			if( index != controlArray.size()-1 )
			{
				Control *helper = controlArray[index];

				controlArray.erase( controlArray.begin()+index );
				controlArray.push_back( helper );
			}
		}
		else
		{
			std::string error;

			if( pointer == NULL )
			{
				error = "NULL pointer sent to SentToTheFront(...) function.";
			}
			else
			{
				error  = "Control by the name \"";
				error += pointer->GetName();
				error += "\" hasn't been found in controlArray registry.";

				error += "\n\nOperation: SetToTheFront(...)";
			}

			throw error;
		}
	}

	void ControlManager::SetToTheBack  (Control *pointer) throw( std::string )
	{
		int index = -1;

		for(int i = 0; i < controlArray.size(); i++)
		{
			assert( controlArray[i] != NULL );

			if( pointer == controlArray[i] )
			{
				index = i;
				break;
			}
		}

		if( index != -1 && index >= 0 )
		{
			if( index != 0 )
			{
				Control *helper = controlArray[index];

				controlArray.erase( controlArray.begin()+index );
				controlArray.insert( controlArray.begin(), helper );
			}
		}
		else
		{
			std::string error;

			if( pointer == NULL )
			{
				error = "NULL pointer sent to SetToTheBack(...) function.";
			}
			else
			{
				error  = "Control by the name \"";
				error += pointer->GetName();
				error += "\" hasn't been found in controlArray registry.";

				error += "\n\nOperation: SetToTheBack(...)";
			}

			throw error;
		}
	}
//
//
	void ControlManager::SetVeritcalSegregation   ()
	{
		VerticalSegregation = true;
		SortControls();
	}

	void ControlManager::SetHorizontalSegregation ()
	{
		VerticalSegregation = false;
		SortControls();
	}
//
	void ControlManager::Tab        ()
	{
		if( currentFocus )
		{
			assert( !controlArray.empty() );

			bool FoundCurrentFocusWhileTabbing = false;

			for(int i = 0; i < controlArray.size(); i++)
			{
				if( controlArray[i] == currentFocus )
				{
					if( i < controlArray.size()-1 )
					{
						SetNewFocus(i+1);
					}
					else
					{
						SetNewFocus(0);
					}

					FoundCurrentFocusWhileTabbing = true;
					break;
				}
			}

			assert( FoundCurrentFocusWhileTabbing );
		}
	}

	void ControlManager::ReverseTab ()
	{
		if( currentFocus )
		{
			assert( !controlArray.empty() );

			bool FoundCurrentFocusWhileReverseTabbing = false;

			for(int i = 0; i < controlArray.size(); i++)
			{
				if( controlArray[i] == currentFocus )
				{
					if( i == 0 )
					{
						SetNewFocus(controlArray.size()-1);
					}
					else
					{
						SetNewFocus(i-1);
					}

					FoundCurrentFocusWhileReverseTabbing = true;
					break;
				}
			}

			assert( FoundCurrentFocusWhileReverseTabbing );
		}
	}
//
/******************************************************
		Focus managment.
******************************************************/

	void     ControlManager::SetCurrentFocus (Control *ctrl)           throw( std::string )
	{
		if( ctrl != NULL )
		{
			int index;

			try
			{
				index = FindControlIndexByPointer(ctrl);
			}
			catch( std::string error )
			{
				error += "\n\nOperation: Trying to focus a control, by using pointer.";
				throw error;
			}

			SetNewFocus(index);
		}
		else
		{
			std::string error = "Pointer to a control, that is attempted to be focused, is a NULL.";
			throw error;
		}
	}

	void     ControlManager::SetCurrentFocus (std::string const &name) throw( std::string )
	{
		int index;

		try
		{
			index = FindControlIndexByName(name);
		}
		catch( std::string error )
		{
			error += "\n\nOperation: Trying to focus a control, by using name string.";
			throw error;
		}

		SetNewFocus(index);
	}

	void     ControlManager::SetCurrentFocus (int const &index)        throw( std::string )
	{
		if( index >= 0 && index < controlArray.size() )
		{
			assert( controlArray[index] != NULL );
			SetNewFocus(index);
		}
		else
		{
			std::string error;
			
			char indexAsString[MAX_PATH];
			_itoa_s( index, indexAsString, MAX_PATH, 10 );

			error  = "Index (";
			error += indexAsString;
			error += ") sent to SetCurrentFocus(index), is inadequate (Smaller than zero, greater than size of control registry or non-existent index).";

			throw error;
		}
	}

//
	Control* ControlManager::GetCurrentFocus ()
	{
		return currentFocus;
	}
//
	void ControlManager::SetNewFocus     (int index)
	{
		if( currentFocus != controlArray[index] )
		{
			if( currentFocus != NULL )
			{
				currentFocus->OnFocusLost();
			}

			if( controlArray[index]->IsFocusable() )
			{
				currentFocus = controlArray[index];
				currentFocus->OnFocusReceived();
			}
			else
				currentFocus = NULL;
		}
	}

	bool ControlManager::ScanForNewFocus (POINT &mousePosition)
	{
		bool GotNewFocus = false;

		if( currentFocus != NULL )
		{
			currentFocus->OnFocusLost();
			currentFocus = NULL;
		}

		// Scanning from the back, because we want to focus the ones on the "surface".
		// (Controls are rendered from 0 to controlArray.size()-1)
		for(int i = controlArray.size()-1; i >=0 ; i--)
		{
			assert( controlArray[i] != NULL );

			if( controlArray[i]->OnMouseLeftDown(mousePosition) )
			{
				SetNewFocus(i);
				GotNewFocus = true;
				break;
			}
		}

		return GotNewFocus;
	}
//
/******************************************************
		Functions used both by control
		and focus managment.
******************************************************/

	int	ControlManager::FindControlIndexByPointer (Control *ctrl)           throw( std::string )
	{
		std:: string error;

		for(int i = 0; i < controlArray.size(); i++)
		{
			assert( controlArray[i] != NULL );

			if( ctrl == controlArray[i] )
			{
				return i;
			}
		}
	
		error  = "Control by the name \"";
		error += ctrl->GetName();
		error += "\" and of type \"";
		error += ctrl->GetType();
		error += "\" hasn't been found in control registry.";

		throw error;
	}

	int	ControlManager::FindControlIndexByName    (std::string const &name) throw( std::string )
	{
		std::string error;

		for(int i = 0; i < controlArray.size(); i++)
		{
			assert( controlArray[i] != NULL );

			if( controlArray[i]->GetName() == name )
			{
				return i;
			}				
		}

		error  = "Control by the name \"";
		error += name;
		error += "\" hasn't been found in control registry.";

		throw error;
	}
//
/*****************************************************/

	void ControlManager::SortControls ()
	{
		if( VerticalSegregation )
		{
			std::sort( controlArray.begin(), controlArray.end(), &ControlManager::SegregateVertically );
		}
		else
		{
			std::sort( controlArray.begin(), controlArray.end(), &ControlManager::SegregateHorizontally );
		}

		std::sort( controlArray.begin(), controlArray.end(), &ControlManager::SortHandlersToTheFront );
	}
//
/*****************************************************/

	void ControlManager::Save (std::fstream &file)
	{
		file << "#Control_count:" << "\t" << controlArray.size() << "\n\n";

		file << "##################################################################################################################################################\n\n";

		// Handlers need to be saved last (when loading, there have to be rest of the controls loaded).
		std::sort( controlArray.begin(), controlArray.end(), &ControlManager::SortHandlersToTheBack );

		for(int i = 0; i < controlArray.size(); i++)
		{
			controlArray[i]->Save(file);
			file << "##################################################################################################################################################\n\n";
		}

		std::sort( controlArray.begin(), controlArray.end(), &ControlManager::SortHandlersToTheFront );
	}

	void ControlManager::Load (std::fstream &file) throw( std::string )
	{
		RemoveAllControls();// <- might throw error

		int controlCount = LoadControlCount(file);
		LoadControls(file, controlCount);
	}
//
		int  ControlManager::LoadControlCount     (std::fstream &file)
		{
			std::string line = ReadNewValue(file, '#');

			return atoi(line.c_str());
		}

		void ControlManager::LoadControls         (std::fstream &file, int const &controlCount)
		{
			int controlType;

			for(int i = 0; i < controlCount; i++)
			{
				controlType = LoadControlType(file);

				assert( controlType != 0 );

				assert( LoadSpecificControl(file, controlType) );
			}
		}

		int  ControlManager::LoadControlType      (std::fstream &file)
		{
			std::string line = ReadNewValue(file, '#');


			if( line == "Control" )
			{
				return CONTROL;
			}


			else if( line == "Label" )
			{
				return LABEL;
			}
			else if( line == "Edit" )
			{
				return EDIT;
			}
			else if( line == "Button" )
			{
				return BUTTON;
			}


			else if( line == "CheckerBox" )
			{
				return CHECKER_BOX;
			}
			else if( line == "ProgressBar" )
			{
				return PROGRESS_BAR;
			}
			else if( line == "TabBar" )
			{
				return TAB_BAR;
			}


			else if( line == "Slider" )
			{
				return SLIDER;
			}
			else if( line == "MarkedSlider" )
			{
				return MARKED_SLIDER;
			}
			else if( line == "ScrollBar" )
			{
				return SCROLL_BAR;
			}


			else if( line == "ListBox" )
			{
				return LIST_BOX;
			}
			else if( line == "ComboBox" )
			{
				return COMBO_BOX;
			}


			else if( line == "Handler" )
			{
				return HANDLER;
			}

			return 0;
		}

		bool ControlManager::LoadSpecificControl  (std::fstream &file, int const &controlType)
		{
			bool FoundSpecificControl = true;

			Control *newCtrl;

			switch( controlType )
			{
				case CONTROL:	    newCtrl = new Control;	     break;

				case LABEL:		    newCtrl = new Label;	     break;
				case EDIT:		    newCtrl = new Edit;		     break;
				case BUTTON:	    newCtrl = new Button;	     break;

				case CHECKER_BOX:	newCtrl = new CheckerBox;	 break;
				case PROGRESS_BAR:	newCtrl = new ProgressBar;	 break;
				case TAB_BAR:       newCtrl = new TabBar;        break;

				case SLIDER:		newCtrl = new Slider;		 break;
				case MARKED_SLIDER: newCtrl = new MarkedSlider;  break;
				case SCROLL_BAR:    newCtrl = new ScrollBar;     break;

				case LIST_BOX:      newCtrl = new ListBox;       break;
				case COMBO_BOX:     newCtrl = new ComboBox;      break;

				case HANDLER:		newCtrl = new Handler;		 break;

				default:
					FoundSpecificControl = false;
				break;
			}

			if( FoundSpecificControl )
			{
				newCtrl->Load(file);
				AdjustControlTextures(newCtrl);
				controlArray.push_back(newCtrl);
				SortControls();
			}

			return FoundSpecificControl;
		}
//
		void ControlManager::AdjustControlTextures (Control *loadedControl)
		{
			std::vector <textureObj> textureArray      = Data.GetTextures();
			std::vector <textureObj> loadedTextures    = Data.GetSavedTextures();

			std::vector <unsigned int> controlTextures = loadedControl->GetTextures();
			std::string currentTexture;


			for(int i = 0; i < controlTextures.size(); i++)
			{
				currentTexture = "";
			
				for(int j = 0; j < loadedTextures.size(); j++)
				{
					if( controlTextures[i] == loadedTextures[j].textureID )
					{
						currentTexture = loadedTextures[j].filename;
					}
				}

				if( !currentTexture.empty() )
				{
					controlTextures[i] = 0;
					
					for(int j = 0; j < textureArray.size(); j++)
					{
						if( currentTexture == textureArray[j].filename )
						{
							controlTextures[i] = textureArray[j].textureID;
							break;
						}
					}
				}
				else
					controlTextures[i] = 0;
			}


			loadedControl->SetTextures(controlTextures);
		}
//
/******************************************************
		Hidden consructor and destructor.
******************************************************/

	ControlManager::ControlManager  (): Data( GUI_DataManager::Singleton() ),
										currentFocus(NULL)
	{
		VerticalSegregation = true;
	}

	ControlManager::~ControlManager ()
	{
		for(int i = 0; i < controlArray.size(); i++)
		{
			delete controlArray[i];
		}

		controlArray.clear();
	}
//
//

	#undef CONTROL

	#undef LABEL
	#undef EDIT  
	#undef BUTTON

	#undef CHECKER_BOX 
	#undef PROGRESS_BAR
	#undef TAB_BAR  

	#undef SLIDER   
	#undef MARKED_SLIDER
	#undef SCROLL_BAR

	#undef LIST_BOX
	#undef COMBO_BOX

	#undef HANDLER	