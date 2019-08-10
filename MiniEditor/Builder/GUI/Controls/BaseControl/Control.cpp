	#include "GlobalHeaders.h"

	#include "Control.h"



// Initialization of static name registry.

	std::vector <std::string> Control::nameRegistry;

// Default starting values for base control.

	#define DEFAULT_UNNAMED      ("Unnamed")
	/* If changed -> set new unnamed string in Handler/AddHandling(...) */

	#define DEFAULT_TOPRIGHT     (100)
	#define DEFAULT_BOTTOMLEFT   (0)
 
	#define DEFAULT_MINSIZE      (0)
	#define DEFAULT_MAXSIZE      (1000)

	#define DEFAULT_RED			 (1.f)
	#define DEFAULT_GREEN		 (1.f)
	#define DEFAULT_BLUE		 (1.f)
	#define DEFAULT_ALPHA		 (1.f)

	#define DEFAULT_FRAME		 (7.5f)

	#define DEFAULT_TRANSPARENT	 (false)
	#define DEFAULT_FOCUSABLE	 (true)
	#define DEFAULT_OBSTACLE	 (false)
	#define DEFAULT_STATIC		 (false)
	#define DEFAULT_SCALABLE	 (true)
	#define DEFAULT_RESIZABLE	 (true)
	#define DEFAULT_READINGINPUT (true)



	Control::Control  ():  controlType("Control")
	{
		SetDefaultControlData();
		ClearResizingFlags();
		SetDefaultBooleanFlags();
	}
//
		void Control::SetDefaultControlData  ()
		{
			controlName = DEFAULT_UNNAMED;

			controlBorder.bottom = controlBorder.left  = DEFAULT_BOTTOMLEFT;
			controlBorder.top    = controlBorder.right = DEFAULT_TOPRIGHT;

			minControlWidth = minControlHeight = DEFAULT_MINSIZE;
			maxControlWidth = maxControlHeight = DEFAULT_MAXSIZE;

			controlColor.SetColor(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE, DEFAULT_ALPHA);
		
			currentTexture = 0;

			frameThickness = DEFAULT_FRAME;
		}

		void Control::SetDefaultBooleanFlags ()
		{
			Focused      = false;

			Transparent  = DEFAULT_TRANSPARENT;
			Focusable    = DEFAULT_FOCUSABLE;
			Obstacle     = DEFAULT_OBSTACLE;
			Static       = DEFAULT_STATIC;
			Scalable     = DEFAULT_SCALABLE;
			Resizable    = DEFAULT_RESIZABLE;
			ReadingInput = DEFAULT_READINGINPUT;
			Rendering    = true;
		}

//
	Control::Control  (Control const &ctrl):  controlType("Control")
	{
		controlName = CreateNameCopy(ctrl.controlName);
		nameRegistry.push_back(controlName);

		CopyControlData(ctrl);

		ClearResizingFlags();

		CopyBooleanFlags(ctrl);

		EventManager = ctrl.EventManager;
	}

	Control& Control::operator= (Control const &ctrl)
	{
		controlName = CreateNameCopy(ctrl.controlName);
		nameRegistry.push_back(controlName);

		CopyControlData(ctrl);

		ClearResizingFlags();

		CopyBooleanFlags(ctrl);

		EventManager = ctrl.EventManager;

		return *this;
	}
//
		std::string Control::CreateNameCopy   (std::string name)
		{
			std::string nameCopy;
			int copyEnumerant = 0;

			bool NameCollision;

			do
			{
				nameCopy = name + "_Copy";

				char enumerantAsChar[MAX_PATH];
				_itoa_s( copyEnumerant, enumerantAsChar, MAX_PATH, 10 );

				nameCopy += enumerantAsChar;

				NameCollision = false;

				try
				{
					CheckForNameCollision(nameCopy);
				}
				catch( std::string error )
				{
					NameCollision = true;
					copyEnumerant++;
				}
			}
			while( NameCollision );


			return nameCopy;
		}

		void        Control::CopyControlData  (Control const &ctrl)
		{
			controlBorder.left   = ctrl.controlBorder.left;
			controlBorder.top    = ctrl.controlBorder.top;
			controlBorder.right  = ctrl.controlBorder.right;
			controlBorder.bottom = ctrl.controlBorder.bottom;

			controlColor = ctrl.controlColor;

			textures       = ctrl.textures;
			currentTexture = ctrl.currentTexture;

			frameThickness   = ctrl.frameThickness;
			minControlWidth  = ctrl.minControlWidth;
			maxControlWidth  = ctrl.maxControlWidth;
			minControlHeight = ctrl.minControlHeight;
			maxControlHeight = ctrl.maxControlHeight;
		}

		void        Control::CopyBooleanFlags (Control const &ctrl)
		{
			Focused      = ctrl.Focused;

			Transparent  = ctrl.Transparent;
			Focusable    = ctrl.Focusable;
			Obstacle	 = ctrl.Obstacle;
			Static       = ctrl.Static;
			Scalable     = ctrl.Scalable;
			Resizable    = ctrl.Resizable;
			Focused      = ctrl.Focused;
			ReadingInput = ctrl.ReadingInput;
			Rendering    = ctrl.Rendering;

			assert( Focusable ? true : Focused != true );
		}

//
	Control::~Control ()
	{
		if( controlName != DEFAULT_UNNAMED )
		{
			RemoveNameFromRegistry(controlName);
		}
	}


//
/******************************************************
		Basic managment functions.
******************************************************/

	/*		Mutators		*/
//
	void Control::SetName           (std::string const &newName)  throw( std::string )
	{
		if( newName == DEFAULT_UNNAMED )
		{
			std::string error = "You can't set an \"Unnamed\" string as control name.";
			throw error;
		}

		if( controlName != DEFAULT_UNNAMED )
		{
			RemoveNameFromRegistry(controlName);
		}
		
		CheckForNameCollision(newName);
		
		controlName = newName;
		nameRegistry.push_back(newName);
	}

//
		void Control::RemoveNameFromRegistry (std::string name)
		{
			int index = 0;

			for(int i = 0; i < nameRegistry.size(); i++)
			{
				if( name == nameRegistry[i] )
				{
					index = i;
					break;
				}
			}

			nameRegistry.erase( nameRegistry.begin() + index );
		}

		void Control::CheckForNameCollision  (std::string name)  throw( std::string )
		{
			bool NameCollision = false;

			for(int i = 0; i < nameRegistry.size(); i++)
			{
				if( name == nameRegistry[i] )
				{
					NameCollision = true;
					break;
				}
			}

			if( NameCollision )
			{
				std::string error;

				error  = "There is already a control with name \"";
				error += name;
				error += "\".";

				throw error;
			}
		}
//
	void Control::SetBorders        (RECT const &Border)
	{
		controlBorder.left   = (Border.left < Border.right)  ? Border.left : Border.right;
		controlBorder.top    = (Border.top  > Border.bottom) ? Border.top  : Border.bottom;
		controlBorder.right  = (Border.left > Border.right)  ? Border.left : Border.right;
		controlBorder.bottom = (Border.top  < Border.bottom) ? Border.top  : Border.bottom;

		ControlSizeCorrection();
	}


	void Control::SetColor          (Color const &color)
	{
		controlColor = color;
	}

//
	void Control::SetTextures       (std::vector <unsigned int> const &Textures)
	{
		textures = Textures;

		if( currentTexture > textures.size() )
			currentTexture = 0;
	}

	void Control::SetCurrentTexture (int const &TextureState)
	{
	// In an event of giving incorrect TextureState ID, the control will be untextured.

		if( TextureState < 0 || TextureState > textures.size() )
			currentTexture = 0;
		else
			currentTexture = TextureState;
	}

//
	void Control::SetFrameThickness (float const &width)
	{
		if( !( width > (controlBorder.right - controlBorder.left) ||
			   width > (controlBorder.top   - controlBorder.bottom) ) )
		{
			frameThickness = width;

			if( width < 0 )
			{
				frameThickness *= -1;
			}
		}
	}
//
	void Control::SetMinWidth       (float const &width)
	{
		float newWidth = width;

		if( newWidth <= 0 ) 
		{
			newWidth = 1;
		}

		if( newWidth > maxControlWidth )
		{
			maxControlWidth = newWidth;
		}

		minControlWidth = newWidth;
		ControlSizeCorrection();
	}

	void Control::SetMaxWidth       (float const &width)  
	{
		float newWidth = width;

		if( newWidth <= 0 ) 
		{
			newWidth = 1;
		}

		if( newWidth < minControlWidth )
		{
			minControlWidth = newWidth;
		}

		maxControlWidth = newWidth;
		ControlSizeCorrection();
	}

	void Control::SetMinHeight      (float const &height)
	{
		float newHeight = height;

		if( newHeight <= 0 ) 
		{
			newHeight = 1;
		}

		if( newHeight > maxControlHeight )
		{
			maxControlHeight = newHeight;
		}

		minControlHeight = newHeight;
		ControlSizeCorrection();
	}

	void Control::SetMaxHeight      (float const &height)
	{
		float newHeight = height;

		if( newHeight <= 0 ) 
		{
			newHeight = 1;
		}

		if( newHeight < minControlHeight )
		{
			minControlHeight = newHeight;
		}

		maxControlHeight = newHeight;
		ControlSizeCorrection();
	}

//
	void Control::SetTransparency   (bool const &IsTransparent)
	{
		Transparent = IsTransparent;
	}

	void Control::SetFocusable      (bool const &IsFocusable)
	{
		Focusable = IsFocusable;
	}

	void Control::SetObstacle       (bool const &IsObstacle)
	{
		Obstacle = IsObstacle;
	}
	void Control::SetStatic		    (bool const &IsStatic)
	{
		Static = IsStatic;
	}

	void Control::SetScalable	    (bool const &IsScalable)
	{	
		Scalable = IsScalable;
	}
	void Control::SetResizability   (bool const &Resizability)
	{
		Resizable = Resizability;
	}

	void Control::SetReadingInput   (bool const &inputReadiness)
	{
		ReadingInput = inputReadiness;
	}


	void Control::SetRendering      (bool const &IsRendering)
	{
		Rendering = IsRendering;
	}
//
	/*		Akccessors		*/
//
	std::string                 Control::GetName           ()
	{
		return controlName;
	}

	std::string                 Control::GetType           ()
	{
		return controlType;
	}
//
	RECT						Control::GetBorders        ()
	{
		return controlBorder;
	}

	Color				        Control::GetColor          ()
	{
		return controlColor;
	}

//
	std::vector <unsigned int>  Control::GetTextures       ()
	{
		return textures;
	}

	int                         Control::GetCurrentTexture ()
	{
		return currentTexture;
	}

//
	float                       Control::GetFrameThickness ()
	{
		return frameThickness;
	}
//
	float                       Control::GetMinWidth       ()
	{
		return minControlWidth;
	}

	float                       Control::GetMaxWidth       ()
	{
		return maxControlWidth;
	}

	float						Control::GetWidth		   ()
	{
		return controlBorder.right - controlBorder.left;
	}
//
	float                       Control::GetMinHeight      ()
	{
		return minControlHeight;
	}

	float                       Control::GetMaxHeight      ()
	{
		return maxControlHeight;
	}

	float						Control::GetHeight		   ()
	{
		return controlBorder.top - controlBorder.bottom;
	}
//
	POINT						Control::GetPosition	   ()
	{
		POINT position;

		position.x = controlBorder.left   + GetWidth() /2.f;
		position.y = controlBorder.bottom + GetHeight()/2.f;

		return position;
	}

	POINT						Control::GetPositionDelta  (POINT const &anotherPosition)
	{
		POINT positionDelta;

		positionDelta.x = GetPosition().x - anotherPosition.x;
		positionDelta.y = GetPosition().y - anotherPosition.y;

		return positionDelta;
	}

	POINT						Control::GetPositionDelta  (Control *ctrl)
	{
		return GetPositionDelta( ctrl->GetPosition() );
	}
//
	/*		Predicators		*/
//
	bool Control::IsTransparent      ()
	{
		return Transparent;
	}

	bool Control::IsFocused          ()
	{
		return Focused;
	}

	bool Control::IsFocusable        ()
	{
		return Focusable;
	}

	bool Control::IsObstacle         ()
	{
		return Obstacle;
	}

	bool Control::IsStatic           ()
	{
		return Static;
	}

	bool Control::IsScalable         ()
	{
		return Scalable;
	}
	bool Control::IsCurrentlyResized ()
	{ 
		bool result = Resizable && 
			 
			  (ResizingBy_TopLeft    || ResizingBy_Top    || ResizingBy_TopRight ||
			   ResizingBy_Left       ||					     ResizingBy_Right    ||
			   ResizingBy_BottomLeft || ResizingBy_Bottom || ResizingBy_BottomRight);
		return result;

	}

	bool Control::IsResizable        ()
	{
		return Resizable;
	}

	bool Control::IsReadingInput     ()
	{
		return ReadingInput;
	}

	bool Control::IsRendering	     ()
	{
		return Rendering;
	}
//
/*****************************************************/

	std::string Control::InsertAdditionalErrorInfo ()
	{
		std::string additionalErrorInfo("");

		additionalErrorInfo += InsertControlName();
		additionalErrorInfo += InsertControlType();

		return additionalErrorInfo;
	}
//
		std::string Control::InsertControlName ()
		{
			std::string additionalErrorInfo("");

			additionalErrorInfo += "Control name:";
			additionalErrorInfo += "\t";
			additionalErrorInfo += controlName;
			additionalErrorInfo += "\n\n";

			return additionalErrorInfo;
		}

		std::string Control::InsertControlType ()
		{
			std::string additionalErrorInfo("");

			additionalErrorInfo += "Control type:";
			additionalErrorInfo += "\t";
			additionalErrorInfo += controlType;
			additionalErrorInfo += "\n\n";

			return additionalErrorInfo;
		}
//
/*****************************************************/

	void Control::ControlSizeCorrection ()
	{
		WidthCorrection();
		HeightCorrection();
	}
//
	void Control::WidthCorrection  ()
	{
		if( controlBorder.left == controlBorder.right )
		{
			controlBorder.right = controlBorder.left + 1;
		}

		if( WidthIsTooBig() )
		{
			controlBorder.right = controlBorder.left + maxControlWidth;
		}

		if( WidthIsTooSmall() )
		{
			controlBorder.right = controlBorder.left + minControlWidth;
		}
	}

	bool Control::WidthIsTooBig    ()
	{
		return GetWidth() > maxControlWidth;
	}

	bool Control::WidthIsTooSmall  ()
	{
		return GetWidth() < minControlWidth;
	}

//
	void Control::HeightCorrection ()
	{
		if( controlBorder.bottom == controlBorder.top )
		{
			controlBorder.top = controlBorder.bottom + 1;
		}

		if( HeightIsTooBig() )
		{
			controlBorder.top = controlBorder.bottom + maxControlHeight;
		}

		if( HeightIsTooSmall() )
		{
			controlBorder.top = controlBorder.bottom + minControlHeight;
		}
	}
	bool Control::HeightIsTooBig   ()
	{
		return GetHeight() > maxControlHeight;
	}

	bool Control::HeightIsTooSmall ()
	{
		return GetHeight() < minControlHeight;
	}

//
/******************************************************
		Functions that determine, whether control
		was hit by other control.
******************************************************/

	void Control::ManagePossibleBorderCollision (Control *ctrlAfterMovement, Control *ctrlBeforeMovement)
	{
		if( Obstacle || ctrlAfterMovement->Obstacle )
		{
			if( CheckForBorderCollision(ctrlAfterMovement) )
			{
				AdjustCollidingBorders(ctrlAfterMovement, ctrlBeforeMovement);

				OnCollision();
				ctrlAfterMovement->OnCollision();
			}
		}
	}
//
		bool Control::CheckForBorderCollision (Control *ctrlAfterMovement)
		{
			if( VerticalCollision(ctrlAfterMovement) || HorizontalCollision(ctrlAfterMovement) )
			{
				return true;
			}
			else
				return false;
		}
		void Control::AdjustCollidingBorders  (Control *ctrlAfterMovement, Control *ctrlBeforeMovement)
		{
			if( VerticalCollision(ctrlAfterMovement) )
			{
				if( ctrlAfterMovement->IsCurrentlyResized() )
				{
					ctrlAfterMovement->SetBorders( ctrlBeforeMovement->GetBorders() ); 
				}
				else
				{
					AdjustVertically(ctrlAfterMovement, ctrlBeforeMovement);
				}
			}

			if( HorizontalCollision(ctrlAfterMovement) )
			{
				if( ctrlAfterMovement->IsCurrentlyResized() )
				{
					ctrlAfterMovement->SetBorders( ctrlBeforeMovement->GetBorders() );
				}
				else
				{
					AdjustHorizontally(ctrlAfterMovement, ctrlBeforeMovement);
				}
			}
		}
//
		void Control::AdjustVertically   (Control *ctrlAfterMovement, Control *ctrlBeforeMovement)
		{
			if( CollisionWithTop(ctrlAfterMovement) &&
				HorizontalAlignment(ctrlBeforeMovement) )
			{

				AdjustByTop(ctrlAfterMovement);
			}

			if( CollisionWithBottom(ctrlAfterMovement) &&
				HorizontalAlignment(ctrlBeforeMovement) )
			{

				AdjustByBottom(ctrlAfterMovement);
			}
		}
			void Control::AdjustByTop    (Control *ctrl)
			{
				POINT adjustment;

				adjustment.x = 0;
				adjustment.y = controlBorder.top - ctrl->controlBorder.bottom + 1;

				ctrl->Move(adjustment);
			}

			void Control::AdjustByBottom (Control *ctrl)
			{
				POINT adjustment;

				adjustment.x = 0;
				adjustment.y = controlBorder.bottom - ctrl->controlBorder.top - 1;

				ctrl->Move(adjustment);
			}


		void Control::AdjustHorizontally (Control *ctrlAfterMovement, Control *ctrlBeforeMovement)
		{
			if( CollisionWithLeft(ctrlAfterMovement) &&
				VerticalAlignment(ctrlBeforeMovement) )
			{
				AdjustByLeft(ctrlAfterMovement);
			}

			if( CollisionWithRight(ctrlAfterMovement) &&
				VerticalAlignment(ctrlBeforeMovement) )
			{
				AdjustByRight(ctrlAfterMovement);
			}
		}
			void Control::AdjustByLeft   (Control *ctrl)
			{
				POINT adjustment;

				adjustment.x = controlBorder.left - ctrl->controlBorder.right - 1;
				adjustment.y = 0;

				ctrl->Move(adjustment);
			}

			void Control::AdjustByRight  (Control *ctrl)
			{
				POINT adjustment;

				adjustment.x = controlBorder.right - ctrl->controlBorder.left + 1;
				adjustment.y = 0;

				ctrl->Move(adjustment);
			}

//
		bool Control::VerticalCollision       (Control *ctrl)
		{
			if( ( CollisionWithTop(ctrl) || CollisionWithBottom(ctrl) ) 
				&& HorizontalAlignment(ctrl) )
			{
				return true;
			}
			else
				return false;
		}

			bool Control::CollisionWithTop    (Control *ctrl)
			{
				return ( controlBorder.top >= ctrl->controlBorder.bottom &&
						 controlBorder.top <= ctrl->controlBorder.top );
			}
			bool Control::CollisionWithBottom (Control *ctrl)
			{
				return ( controlBorder.bottom >= ctrl->controlBorder.bottom &&
						 controlBorder.bottom <= ctrl->controlBorder.top );
			}

			bool Control::HorizontalAlignment (Control *ctrl)
			{
				return ( ctrl->controlBorder.left >= controlBorder.left &&
					     ctrl->controlBorder.left <= controlBorder.right )
						 ||
					   ( ctrl->controlBorder.right >= controlBorder.left &&
					     ctrl->controlBorder.right <= controlBorder.right )
						 ||
					   ( ctrl->controlBorder.left  <= controlBorder.left &&
					     ctrl->controlBorder.right >= controlBorder.right );
			}

//
		bool Control::HorizontalCollision     (Control *ctrl)
		{
			if( ( CollisionWithLeft(ctrl) || CollisionWithRight(ctrl) )
				&& VerticalAlignment(ctrl) )
			{
				return true;
			}
			else
				return false;
		}

			bool Control::CollisionWithLeft   (Control *ctrl)
			{
				return ( controlBorder.left <= ctrl->controlBorder.right &&
						 controlBorder.left >= ctrl->controlBorder.left );
			}

			bool Control::CollisionWithRight  (Control *ctrl)
			{
				return ( controlBorder.right <= ctrl->controlBorder.right &&
						 controlBorder.right >= ctrl->controlBorder.left );
			}

			bool Control::VerticalAlignment   (Control *ctrl)
			{
				return ( ctrl->controlBorder.bottom >= controlBorder.bottom &&
					     ctrl->controlBorder.bottom <= controlBorder.top )
						 ||
					   ( ctrl->controlBorder.top >= controlBorder.bottom &&
					     ctrl->controlBorder.top <= controlBorder.top )
						 ||
					   ( ctrl->controlBorder.bottom  <= controlBorder.bottom &&
					     ctrl->controlBorder.top     >= controlBorder.top );
			}

//
/******************************************************
		Functions that determine, whether control
		or it's frame has been hit.
******************************************************/

	bool Control::ControlHit (POINT const &mousePosition)
	{
		if( mousePosition.x > controlBorder.left   + frameThickness  &&  
			mousePosition.x < controlBorder.right  - frameThickness  && 

		    mousePosition.y > controlBorder.bottom + frameThickness  &&  
			mousePosition.y < controlBorder.top    - frameThickness )
		{
			return true;
		}
		else
			return false;
	}
	bool Control::FrameHit   (POINT const &mousePosition)
	{
		if( FrameHitOn_TopLeft (mousePosition) || FrameHitOn_BottomLeft (mousePosition) ||
			FrameHitOn_TopRight(mousePosition) || FrameHitOn_BottomRight(mousePosition) ||
			
			FrameHitOn_Left (mousePosition) || FrameHitOn_Top   (mousePosition) ||
			FrameHitOn_Right(mousePosition) || FrameHitOn_Bottom(mousePosition) )
		{
			CheckForMultipleResizingFlags();

			return true;
		}
		else
			return false;
	}
//
		bool Control::FrameHitOn_TopLeft     (POINT const &mousePosition)
		{
			if( ( mousePosition.y <= controlBorder.top  && mousePosition.y >= controlBorder.top  - frameThickness ) 
														&&
				( mousePosition.x >= controlBorder.left && mousePosition.x <= controlBorder.left + frameThickness ) )
			{

				return ResizingBy_TopLeft = true;
			}
			else
				return false;
		}

		bool Control::FrameHitOn_BottomLeft  (POINT const &mousePosition)
		{
			if( ( mousePosition.y >= controlBorder.bottom && mousePosition.y <= controlBorder.bottom + frameThickness )
														  &&
				( mousePosition.x >= controlBorder.left   && mousePosition.x <= controlBorder.left   + frameThickness ) )
			{

				return ResizingBy_BottomLeft = true;
			}
			else
				return false;
		}

		bool Control::FrameHitOn_TopRight    (POINT const &mousePosition)
		{
			if( ( mousePosition.y <= controlBorder.top   && mousePosition.y >= controlBorder.top   - frameThickness )
														 &&
				( mousePosition.x <= controlBorder.right && mousePosition.x >= controlBorder.right - frameThickness ) )
			{

				return ResizingBy_TopRight = true;
			}
			else
				return false;
		}

		bool Control::FrameHitOn_BottomRight (POINT const &mousePosition)
		{
			if( ( mousePosition.y >= controlBorder.bottom  && mousePosition.y <= controlBorder.bottom + frameThickness )
														   &&
				( mousePosition.x <= controlBorder.right   && mousePosition.x >= controlBorder.right  - frameThickness ) )
			{

				return ResizingBy_BottomRight = true;
			}
			else
				return false;
		}

		bool Control::FrameHitOn_Left        (POINT const &mousePosition)
		{
			if( ( mousePosition.x >= controlBorder.left && 
				  mousePosition.x <= controlBorder.left + frameThickness )
				  &&
				( mousePosition.y >= controlBorder.bottom + frameThickness && 
				  mousePosition.y <= controlBorder.top    - frameThickness ) )
			{

				return ResizingBy_Left = true;
			}
			else
				return false;
		}

		bool Control::FrameHitOn_Top         (POINT const &mousePosition)
		{
			if( ( mousePosition.y <= controlBorder.top  && 
				  mousePosition.y >= controlBorder.top  - frameThickness )
				&&
		        ( mousePosition.x >= controlBorder.left  + frameThickness && 
				  mousePosition.x <= controlBorder.right - frameThickness ) )
			{
	
				return ResizingBy_Top = true;
			}
			else
				return false;
		}

		bool Control::FrameHitOn_Right       (POINT const &mousePosition)
		{
			if( ( mousePosition.x <= controlBorder.right  && 
				  mousePosition.x >= controlBorder.right - frameThickness )
				 &&
				( mousePosition.y >= controlBorder.bottom + frameThickness && 
				  mousePosition.y <= controlBorder.top    - frameThickness ) )
			{
				
				return ResizingBy_Right = true;
			}
			else
				return false;
		}

		bool Control::FrameHitOn_Bottom      (POINT const &mousePosition)
		{
			if( ( mousePosition.y >= controlBorder.bottom && 
				  mousePosition.y <= controlBorder.bottom + frameThickness )
				  &&
		        ( mousePosition.x >= controlBorder.left  + frameThickness && 
				  mousePosition.x <= controlBorder.right - frameThickness ) )	
			{

				return ResizingBy_Bottom = true;
			}
			else
				return false;
		}

//
		void Control::CheckForMultipleResizingFlags ()
		{
			assert( ResizingBy_TopLeft    + ResizingBy_Top    + ResizingBy_TopRight  + 
					ResizingBy_Left       +						ResizingBy_Right     + 
					ResizingBy_BottomLeft + ResizingBy_Bottom + ResizingBy_BottomRight <= 1 );
		}
		void Control::ClearResizingFlags            ()
		{
			ResizingBy_TopLeft    = ResizingBy_Top    = ResizingBy_TopRight    = 
			ResizingBy_Left       =					    ResizingBy_Right       =
			ResizingBy_BottomLeft = ResizingBy_Bottom = ResizingBy_BottomRight = false;
		}

//
/******************************************************
		Standard utility functions.
		(possible redefinition along the heritage)
******************************************************/

	void Control::FocusReceived ()
	{
		Focused = true;
	}

	void Control::FocusLost     ()
	{
		Focused = false;
	}
//
	void Control::Move          (POINT const &mousePositionDelta)
	{
		controlBorder.left   += mousePositionDelta.x;
		controlBorder.right  += mousePositionDelta.x;

		controlBorder.top    += mousePositionDelta.y;
		controlBorder.bottom += mousePositionDelta.y;
	}

	void Control::Resize        (POINT const &mousePosition)
	{
		if( ResizingBy_TopLeft )
		{
			ResizeBy_TopLeft(mousePosition);
		}

		else if( ResizingBy_Top )
		{
			ResizeBy_Top(mousePosition);
		}

		else if( ResizingBy_TopRight )
		{
			ResizeBy_TopRight(mousePosition);
		}

		else if( ResizingBy_Left )
		{
			ResizeBy_Left(mousePosition);
		}

		else if( ResizingBy_Right )
		{
			ResizeBy_Right(mousePosition);
		}

		else if( ResizingBy_BottomLeft )
		{
			ResizeBy_BottomLeft(mousePosition);
		}

		else if( ResizingBy_Bottom )
		{
			ResizeBy_Bottom(mousePosition);
		}

		else if( ResizingBy_BottomRight )
		{
			ResizeBy_BottomRight(mousePosition);
		}
	}
//
		void Control::ResizeBy_TopLeft     (POINT const &mousePosition)
		{
			RECT oldBorder, newBorder;
			oldBorder = newBorder = controlBorder;


			newBorder.left = mousePosition.x;
			newBorder.top  = mousePosition.y;

			if( WidthIsTooBig()  || WidthIsTooSmall() ||
			    HeightIsTooBig() || HeightIsTooSmall() )
			{
				SetBorders(oldBorder);
			}
			else
				SetBorders(newBorder);
		}

		void Control::ResizeBy_Top         (POINT const &mousePosition)
		{
			RECT oldBorder, newBorder;
			oldBorder = newBorder = controlBorder;


			newBorder.top = mousePosition.y;

			if( HeightIsTooBig() || HeightIsTooSmall() )
			{
				SetBorders(oldBorder);
			}
			else
				SetBorders(newBorder);
		}

		void Control::ResizeBy_TopRight    (POINT const &mousePosition)
		{
			RECT oldBorder, newBorder;
			oldBorder = newBorder = controlBorder;


			newBorder.right = mousePosition.x;
			newBorder.top   = mousePosition.y;

			if( WidthIsTooBig()  || WidthIsTooSmall() ||
				HeightIsTooBig() || HeightIsTooSmall() )
			{
				SetBorders(oldBorder);
			}
			else
				SetBorders(newBorder);
		}

		void Control::ResizeBy_Left        (POINT const &mousePosition)
		{
			RECT oldBorder, newBorder;
			oldBorder = newBorder = controlBorder;


			newBorder.left = mousePosition.x;

			if( WidthIsTooBig()  || WidthIsTooSmall() )
			{
				SetBorders(oldBorder);
			}
			else
				SetBorders(newBorder);
		}

		void Control::ResizeBy_Right       (POINT const &mousePosition)
		{
			RECT oldBorder, newBorder;
			oldBorder = newBorder = controlBorder;


			newBorder.right = mousePosition.x;

			if( WidthIsTooBig()  || WidthIsTooSmall() )
			{
				SetBorders(oldBorder);
			}
			else
				SetBorders(newBorder);
		}

		void Control::ResizeBy_BottomLeft  (POINT const &mousePosition)
		{
			RECT oldBorder, newBorder;
			oldBorder = newBorder = controlBorder;


			newBorder.left   = mousePosition.x;
			newBorder.bottom = mousePosition.y;

			if( WidthIsTooBig()  || WidthIsTooSmall() ||
				HeightIsTooBig() || HeightIsTooSmall() )
			{
				SetBorders(oldBorder);
			}
			else
				SetBorders(newBorder);
		}

		void Control::ResizeBy_Bottom      (POINT const &mousePosition)
		{
			RECT oldBorder, newBorder;
			oldBorder = newBorder = controlBorder;


			newBorder.bottom = mousePosition.y;

			if( HeightIsTooBig() || HeightIsTooSmall() )
			{
				SetBorders(oldBorder);
			}
			else
				SetBorders(newBorder);
		}

		void Control::ResizeBy_BottomRight (POINT const &mousePosition)
		{
			RECT oldBorder, newBorder;
			oldBorder = newBorder = controlBorder;


			newBorder.right  = mousePosition.x;
			newBorder.bottom = mousePosition.y;

			if( WidthIsTooBig()  || WidthIsTooSmall() ||
				HeightIsTooBig() || HeightIsTooSmall() )
			{
				SetBorders(oldBorder);
			}
			else
				SetBorders(newBorder);
		}

//
	void Control::Scale         (float const &scaleMultiplier)     
	{
		RECT oldBorder = controlBorder,
			 newBorder = controlBorder;

		float scaledHeight = GetHeight() * scaleMultiplier,
			  scaledWidth  = GetWidth()  * scaleMultiplier;


		newBorder.left  += GetWidth() - scaledWidth;
		newBorder.right -= GetWidth() - scaledWidth;

		newBorder.top    -= GetHeight() - scaledHeight;
		newBorder.bottom += GetHeight() - scaledHeight;


		if( WidthIsTooBig() || WidthIsTooSmall() ||
			HeightIsTooBig() || HeightIsTooSmall() )
		{
			SetBorders(oldBorder);
		}
		else
			SetBorders(newBorder);
	}

//
	void Control::Render        (Color const &GUIColor, bool const &TransparentGUI)
	{
		glPushMatrix();

		
		if( TransparentGUI && Transparent )
		{
			glEnable(GL_BLEND);
		}
		else
			glDisable(GL_BLEND);

		//glColor4fv( GUIColor * controlColor );

		Color mixedColor = GUIColor * controlColor;

		glColor4f(mixedColor.GetRed(), mixedColor.GetGreen(), mixedColor.GetBlue(), mixedColor.GetAlpha());


		if( currentTexture )
		{
			glBindTexture(GL_TEXTURE_2D, textures[ currentTexture-1 ]);
		}
		else
			glBindTexture(GL_TEXTURE_2D, 0);



		glBegin(GL_QUADS);

			glTexCoord2i(0, 0);
			glVertex2f(controlBorder.left,  controlBorder.bottom);

			glTexCoord2i(1, 0);
			glVertex2f(controlBorder.right, controlBorder.bottom);

			glTexCoord2i(1, 1);
			glVertex2f(controlBorder.right, controlBorder.top);

			glTexCoord2i(0, 1);
			glVertex2f(controlBorder.left,  controlBorder.top);

		glEnd();


		glBindTexture(GL_TEXTURE_2D, 0);

	
		glPopMatrix();
	}
//
/******************************************************
		Event managment functions.
		( call standard and specific functions )
******************************************************/

	void Control::OnCreate			    ()
	{
		Create();
		EventManager.OnCreate(this);
	}

	void Control::OnDestroy			    ()
	{
		Destroy();
		EventManager.OnDestroy(this);
	}

//
	void Control::OnKeyDown             (int const &keyCode)
	{
		if( Rendering && ReadingInput )
		{
			KeyDown(keyCode);
			EventManager.OnKeyDown(this, keyCode);
		}
	}

	void Control::OnKeyPressed             (int const &keyCode)
	{
		if( Rendering && ReadingInput )
		{
			KeyPressed(keyCode);
			EventManager.OnKeyPressed(this, keyCode);
		}
	}

	void Control::OnKeyUp               (int const &keyCode)
	{
		if( Rendering && ReadingInput )
		{
			KeyUp(keyCode);
			EventManager.OnKeyUp(this, keyCode);
		}
	}

//
	bool Control::OnMouseLeftDown       (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput )
		{
			if( ControlHit(mousePosition) )
			{
				MouseLeftDown(mousePosition);
				EventManager.OnMouseLeftDown(this, mousePosition);
				return true;
			}

			if( !IsCurrentlyResized() )
			{
				return FrameHit(mousePosition);
			}
			else
				return true;

			return false;
		}
		else
			return false;
	}

	bool Control::OnMouseMiddleDown     (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput && ControlHit(mousePosition) )
		{
			MouseMiddleDown(mousePosition);
			EventManager.OnMouseMiddleDown(this, mousePosition);			
			return true;
		}
		else
			return false;
	}

	bool Control::OnMouseRightDown      (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput && ControlHit(mousePosition)  )
		{
			MouseRightDown(mousePosition);
			EventManager.OnMouseRightDown(this, mousePosition);
			return true;
		}
		else
			return false;
	}

//
	void Control::OnMouseLeftPressed    (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput && ControlHit(mousePosition) )
		{
			MouseLeftPressed(mousePosition);
			EventManager.OnMouseLeftPressed(this, mousePosition);
		}
	}

	void Control::OnMouseMiddlePressed  (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput && ControlHit(mousePosition) )
		{
			MouseMiddlePressed(mousePosition);
			EventManager.OnMouseMiddlePressed(this, mousePosition);
		}
	}

	void Control::OnMouseRightPressed   (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput && ControlHit(mousePosition) )
		{
			MouseRightPressed(mousePosition);
			EventManager.OnMouseRightPressed(this, mousePosition);
		}
	}

//
	void Control::OnMouseLeftUp         (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput )
		{
			MouseLeftUp(mousePosition);
			EventManager.OnMouseLeftUp(this, mousePosition);
		}

		ClearResizingFlags();
	}

	void Control::OnMouseMiddleUp       (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput )
		{
			MouseMiddleUp(mousePosition);
			EventManager.OnMouseMiddleUp(this, mousePosition);
		}
	}

	void Control::OnMouseRightUp        (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput )
		{
			MouseRightUp(mousePosition);
			EventManager.OnMouseRightUp(this, mousePosition);
		}
	}

//
	bool Control::OnMouseLeftDblClick   (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput && ControlHit(mousePosition) )
		{
			MouseLeftDblClick(mousePosition);
			EventManager.OnMouseLeftDblClick(this, mousePosition);
			return true;
		}
		else
			return false;
	}

	bool Control::OnMouseMiddleDblClick (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput && ControlHit(mousePosition) )
		{
			MouseMiddleDblClick(mousePosition);
			EventManager.OnMouseMiddleDblClick(this, mousePosition);
			return true;
		}
		else
			return false;
	}

	bool Control::OnMouseRightDblClick  (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput && ControlHit(mousePosition) )
		{
			MouseRightDblClick(mousePosition);
			EventManager.OnMouseRightDblClick(this, mousePosition);
			return true;
		}
		else
			return false;
	}

//
	void Control::OnMouseMoved          (POINT const &mousePosition)
	{
		if( ReadingInput )
		{
			MouseMoved(mousePosition);
			EventManager.OnMouseMoved(this, mousePosition);
		}
	}

	void Control::OnMouseDragged        (POINT const &mousePosition)
	{
		if( Rendering && ReadingInput )
		{
			MouseDragged(mousePosition);
			EventManager.OnMouseDragged(this, mousePosition);
		}
	}

//
	void Control::OnScroll              (int const &scrollInfo)
	{
		if( Rendering && ReadingInput )
		{
			Scroll(scrollInfo);
			EventManager.OnScroll(this, scrollInfo);
		}
	}

//
	void Control::OnFocusReceived       ()
	{
		if( Focusable )
		{
			FocusReceived();
			EventManager.OnFocusReceived(this);
		}
	}

	void Control::OnFocusLost           ()
	{
		if( Focused )
		{
			FocusLost();
			EventManager.OnFocusLost(this);
		}
	}

//
	void Control::OnCollision		    ()
	{
		Collision();
		EventManager.OnCollision(this);
	}
//
	void Control::OnMoving              (POINT const &mousePositionDelta)
	{
		if( !Static )
		{
			Move(mousePositionDelta);
			EventManager.OnMoving(this, mousePositionDelta);
		}
	}

	void Control::OnResizing            (POINT const &mousePosition)
	{
		if( Resizable )
		{
			Resize(mousePosition);
			EventManager.OnResizing(this, mousePosition);
		}
	}

	void Control::OnScaling             (float const &scaleMultiplier)
	{
		if( Scalable )
		{
			Scale(scaleMultiplier);
			EventManager.OnScaling(this, scaleMultiplier);
		}
	}
//
	void Control::OnRendering           (Color const &GUIColor, bool const &TransparentGUI)
	{
		if( Rendering )
		{
			Render(GUIColor, TransparentGUI);
			EventManager.OnRendering(this, GUIColor, TransparentGUI);
		}
	}
//
//
/*****************************************************/

	void Control::Save (std::fstream &file)
	{
		file << "\t" << "#Control_type:" << "\t" << controlType << "\n"
			 << "\t" << "#Control_name:" << "\t" << controlName << "\n\n";
		
		SaveControlBorder(file);
		SaveControlColor(file);
		SaveControlTextures(file);
		SaveControlBooleans(file);
	}
//
		void Control::SaveControlBorder   (std::fstream &file)
		{
			file << "\t" << "#Control_border:\n"
				 << "\t" << "#Left:"   << "\t" << controlBorder.left   << "\n"
				 << "\t" << "#Top:"    << "\t" << controlBorder.top    << "\n"
				 << "\t" << "#Right:"  << "\t" << controlBorder.right  << "\n"
				 << "\t" << "#Bottom:" << "\t" << controlBorder.bottom << "\n\n"

				 << "\t" << "#Frame_thickness:" << "\t" << frameThickness << "\n\n"

				 << "\t" << "#MinWidth:"  << "\t" << minControlWidth  << "\n"
				 << "\t" << "#MaxWidth:"  << "\t" << maxControlWidth  << "\n"
				 << "\t" << "#MinHeight:" << "\t" << minControlHeight << "\n"
				 << "\t" << "#MaxHeight:" << "\t" << maxControlHeight << "\n\n";
		}

		void Control::SaveControlColor    (std::fstream &file)
		{
			file << "\t" << "#ControlColor:\n"
				 << "\t" << "#Red:"   << "\t" << controlColor.GetRed()   << "\n"
				 << "\t" << "#Green:" << "\t" << controlColor.GetGreen() << "\n"
				 << "\t" << "#Blue:"  << "\t" << controlColor.GetBlue()  << "\n"
				 << "\t" << "#Alpha:" << "\t" << controlColor.GetAlpha() << "\n\n";
		}

		void Control::SaveControlTextures (std::fstream &file)
		{
			file << "\t" << "#TextureID_Vector_Size:" << "\t" << textures.size() << "\n"

				 << "\t" << "#TextureID_Vector:";

			for(int i = 0; i < textures.size(); i++)
			{
				file << "\t" << textures[i]; 
			}

			file << "\n"
				 << "\t" << "#Current_texture:" << "\t" << currentTexture << "\n\n";
		}
		void Control::SaveControlBooleans (std::fstream &file)
		{
			file << "\t" << "#Transparent:"  << "\t" << Transparent  << "\n"
				 << "\t" << "#Focusable:"    << "\t" << Focusable    << "\n"
				 << "\t" << "#Obstacle:"     << "\t" << Obstacle     << "\n"
				 << "\t" << "#Static:"       << "\t" << Static       << "\n"
				 << "\t" << "#Scalable:"     << "\t" << Scalable     << "\n"
				 << "\t" << "#Resizable:"    << "\t" << Resizable    << "\n"
				 << "\t" << "#ReadingInput:" << "\t" << ReadingInput << "\n"
				 << "\t" << "#Rendering:"    << "\t" << Rendering    << "\n\n";
		}
//
	void Control::Load (std::fstream &file)
	{
		std::string line = ReadNewValue(file, '#');

		try
		{
			SetName(line);
		}
		catch(std::string error)
		{
			if( line != DEFAULT_UNNAMED )
			{
				SetName( CreateNameCopy(line) );
			}
		}

		LoadControlBorder(file);
		LoadControlColor(file);
		LoadControlTextures(file);
		LoadControlBooleans(file);
	}
//
		void Control::LoadControlBorder   (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		controlBorder.left   = atof(line.c_str());
			line = ReadNewValue(file, '#');		controlBorder.top    = atof(line.c_str());
			line = ReadNewValue(file, '#');		controlBorder.right  = atof(line.c_str());
			line = ReadNewValue(file, '#');		controlBorder.bottom = atof(line.c_str());

			line = ReadNewValue(file, '#');		frameThickness = atof(line.c_str());

			line = ReadNewValue(file, '#');		minControlWidth  = atof(line.c_str());
			line = ReadNewValue(file, '#');		maxControlWidth  = atof(line.c_str());
			line = ReadNewValue(file, '#');		minControlHeight = atof(line.c_str());
			line = ReadNewValue(file, '#');		maxControlHeight = atof(line.c_str());
		}

		void Control::LoadControlColor    (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		controlColor.SetRed   ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		controlColor.SetGreen ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		controlColor.SetBlue  ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		controlColor.SetAlpha ( atof(line.c_str()) );
		}

		void Control::LoadControlTextures (std::fstream &file)
		{
			std::string line;
			int textureCount;

			line = ReadNewValue(file, '#');		
			textureCount = atoi(line.c_str());


			textures.clear();
			for(int i = 0; i < textureCount; i++)
			{
				line = ReadNewValue(file, '#');		
				textures.push_back( atof(line.c_str()) );
			}

			line = ReadNewValue(file, '#');		
			currentTexture = atoi(line.c_str());
		}

		void Control::LoadControlBooleans (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		Transparent  = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		Focusable    = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		Obstacle     = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		Static       = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		Scalable     = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		Resizable    = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		ReadingInput = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		Rendering    = atoi( line.c_str() );
		}

