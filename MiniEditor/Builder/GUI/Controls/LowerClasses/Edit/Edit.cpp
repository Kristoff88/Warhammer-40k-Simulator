	#include "GlobalHeaders.h"

	#include "Edit.h"


	#define DEFAULT_BLIPTIME_FREQUENCY (750)



	Edit::Edit()
	{
		controlType = "Edit";
		SetDefaultEditData();
	}
//
		void Edit::SetDefaultEditData()
		{
			if( !caretBlipTimer.Init() )
			{
				MessageBox(NULL, "HiResTimer initialization failure.", "(Edit constructor)", MB_OK);
			}

			timeElapsed = 0;
			caretBlipFrequency = DEFAULT_BLIPTIME_FREQUENCY;

			caretIndex = 0;
			caretPosition.x = caretPosition.y = 0;

			ClearFilterFlags();

			Overwrite = false;
			Edition = true;
			RenderCaret = true;
		}
//
	Edit::Edit(Edit const &edit)
	{
		this->Label::operator=(edit);
		CopyEditData(edit);
	}
//
		void Edit::CopyEditData(Edit const &edit)
		{
			caretBlipFrequency = edit.caretBlipFrequency;

			caretIndex    = edit.caretIndex;
			caretPosition = edit.caretPosition;

			NumbersOnly  = edit.NumbersOnly;
			LettersOnly  = edit.LettersOnly;

			Overwrite = edit.Overwrite;
			Edition   = edit.Edition;
		}
//
	Edit::~Edit()
	{}
//
	Edit& Edit::operator= (Control const &ctrl)
	{
		Control::operator=(ctrl);
		return *this;
	}

	Edit& Edit::operator= (Label const &label)
	{
		Label::operator=(label);
		return *this;
	}

	Edit& Edit::operator= (Edit const &edit)
	{
		this->Label::operator=(edit);
		CopyEditData(edit);
		return *this;
	}
//
/******************************************************
		Text managment.
******************************************************/

	void Edit::SetText (std::string const &text)
	{
		Label::SetText(text);

		caretIndex = text.size();
		CaretIndexToPosition();
	}
//
	void Edit::AddCharacter    (char const &Char)
	{
		if( controlText.size() < maxCharacterCount )
		{
			std::string text;
			text.push_back(Char);

			if( InputTextCorrect(text) )
			{
				if( Overwrite )
				{
					controlText.erase( caretIndex, 1 );
				}

				controlText.insert( caretIndex, &Char, 1 );
				rowList = ProcessText(controlText);

				caretIndex++;
				CaretIndexToPosition();
			}
		}
	}

	void Edit::RemoveCharacter ()
	{
		if( !controlText.empty() )
		{
			if( caretIndex != 0 )
			{
				controlText.erase( caretIndex-1, 1 );
				rowList = ProcessText(controlText);

				caretIndex--;
				CaretIndexToPosition();
			}
		}
	}
//
	bool Edit::InputTextCorrect(std::string const &text)
	{
		bool TextCorrectness = true;

		assert( NumbersOnly + LettersOnly <= 1 );

		if( NumbersOnly && FoundLetters(text) )
		{
			TextCorrectness = false;
		}
		if( LettersOnly && FoundNumbers(text) )
		{
			TextCorrectness = false;
		}

		return TextCorrectness;
	}

//
		bool Edit::FoundLetters (std::string const &text)
		{
			bool FoundLetter = false;

			for(int i = 0; i < text.size(); i++)
			{
				if( text[i] >= 65 && text[i] <= 90
					              ||
				    text[i] >= 97 && text[i] <= 122 )
				{
					FoundLetter = true;
					break;
				}
			}

			return FoundLetter;
		}

		bool Edit::FoundNumbers (std::string const &text)
		{
			bool FoundNumber = false;

			for(int i = 0; i < text.size(); i++)
			{
				if( text[i] >= 48 && text[i] <= 57 )
				{
					FoundNumber = true;
					break;
				}
			}

			return FoundNumber;
		}
//
	void Edit::ScrollDown ()
	{
		scrollDown++;
		CaretIndexToPosition();
	}

	void Edit::ScrollUp   ()
	{
		if( scrollDown > 0 )
		{
			scrollDown--;
			CaretIndexToPosition();
		}
	}
//
/******************************************************
		Caret managment.
******************************************************/

	void Edit::MoveCaret_Left  ()
	{
		if( caretIndex > 0 )
		{
			caretIndex--;
			CaretIndexToPosition();
		}
	}

	void Edit::MoveCaret_Right ()
	{
		if( caretIndex < controlText.size() )
		{
			caretIndex++;
			CaretIndexToPosition();
		}
	}
//
	void Edit::MoveCaret_Up    ()
	{
		if( caretPosition.y > 0 )
		{
			int distance;
			
			if( caretPosition.x >= rowList[caretPosition.y-1].size() )
			{
				distance = caretPosition.x + 1;
			}
			else
				distance = rowList[caretPosition.y-1].size() + 1;

			caretIndex -= distance;
			CaretIndexToPosition();
		}
	}

	void Edit::MoveCaret_Down  ()
	{
		if( caretPosition.y < rowList.size()-1 )
		{
			int distance;

			if( caretPosition.x >= rowList[caretPosition.y+1].size() )
			{
				distance = rowList[caretPosition.y+1].size() + ( rowList[caretPosition.y].size() - caretPosition.x ) + 1;
			}
			else
				distance = rowList[caretPosition.y].size() + 1;


			caretIndex += distance;
			CaretIndexToPosition();
		}
	}
//
	void Edit::CaretIndexToPosition ()
	{
		caretPosition.x = caretPosition.y = -1;
		int index = 0;

		for(int r = 0; r < rowList.size(); r++)
		{
			if( DesiredIndexIsNotInThisRow(index, rowList[r]) )
			{
				if( RowHasEndofLineMarker(index, rowList[r]) )
				{
					index++;
				}

				// Add whole row.
				index += rowList[r].size();

				// If caret is on the left edge of a row.
				if( index == caretIndex )
				{
					caretPosition.x = 0;
					caretPosition.y = r+1;
					break;
				}
			}
			else
			{
				for(int i = 0; i < rowList[r].size(); i++)
				{
					index++;

					if( index == caretIndex )
					{
						caretPosition.x = i+1;
						caretPosition.y = r;
						break;
					}
				}
			}
		}


		caretPosition.y -= scrollDown;

		if( caretPosition.y < 0 )
		{
			caretPosition.x = caretPosition.y = -1;
		}
	}
//
	bool Edit::DesiredIndexIsNotInThisRow (int index, std::string row)
	{
		return (index + row.size()+1) <= caretIndex;
	}

	bool Edit::RowHasEndofLineMarker      (int index, std::string row)
	{
		return controlText[index + row.size()] == '\n';
	}
//
/******************************************************
		Properties managment.
******************************************************/

	void Edit::SetCaretBlipFrequency (unsigned int const &frequency)
	{
		caretBlipFrequency = frequency;
	}
//
	void Edit::SetCaretIndex    (unsigned int const &index)
	{
		if( index > controlText.size() )
		{
			caretIndex = controlText.size();
		}
		else
			caretIndex = index;

		CaretIndexToPosition();
	}

	void Edit::SetCaretPosition (POINT const &position)
	{
		caretPosition = position;
		CaretCorrection();
	}
//
		void Edit::CaretCorrection      ()
		{
			if( rowList.empty() )
			{
				caretPosition.x = caretPosition.y = 0;
				return;
			}


			if( caretPosition.y < 0 )
			{
				caretPosition.y = 0;
			}

			if( caretPosition.x < 0 )
			{
				if( caretPosition.y == 0 )
				{
					caretPosition.x = 0;
				}
				else
				{
					caretPosition.y--;
					caretPosition.x = rowList[caretPosition.y].size();
				}
			}
			

			if( caretPosition.y >= rowList.size() )
			{
				caretPosition.y = rowList.size()-1;
			}

			if( caretPosition.x > rowList[caretPosition.y].size() )
			{
				if( caretPosition.y == rowList.size()-1 )
				{
					caretPosition.x = rowList[caretPosition.y].size();
				}
				else
				{
					caretPosition.y++;
					caretPosition.x = 0;
				}
			}
		}
 //
	void Edit::SetNumbersOnly  (bool const &IsNumbersOnly)
	{
		ClearFilterFlags();
		
		NumbersOnly = IsNumbersOnly;
	}

	void Edit::SetLettersOnly  (bool const &IsLettersOnly)
	{
		ClearFilterFlags();

		LettersOnly = IsLettersOnly;
	}
//
		void Edit::ClearFilterFlags ()
		{
			NumbersOnly = LettersOnly = false;
		}
//
	void Edit::SetOverwrite    (bool const &IsOverwriting)
	{
		Overwrite = IsOverwriting;
	}
	void Edit::SetEdition      (bool const &IsEdited)
	{
		Edition = IsEdited;
	}
//
//
	int Edit::GetCaretBlipFrequency ()
	{
		return caretBlipFrequency;
	}
//
	int   Edit::GetCaretIndex    ()
	{
		return caretIndex;
	}

	POINT Edit::GetCaretPosition ()
	{
		return caretPosition;
	}
//
//
	bool Edit::IsNumbersOnly  ()
	{
		return NumbersOnly;
	}

	bool Edit::IsLettersOnly  ()
	{
		return LettersOnly;
	}

	bool Edit::IsOverwriting  ()
	{
		return Overwrite;
	}
	bool Edit::IsEdited       ()
	{
		return Edition;
	}


//
/******************************************************
		Standard function redefinitions.
******************************************************/

	void Edit::Resize (POINT const &mousePosition)
	{
		Label::Resize(mousePosition);
		CaretIndexToPosition();
	}

	void Edit::Scale  (float const &scaleMultiplier)
	{
		Label::Scale(scaleMultiplier);
		CaretIndexToPosition();
	}
//
/******************************************************
		Rendering.
******************************************************/

	void Edit::Render     (Color const &GUIColor, bool const &TransparentGUI)
	{
		Label::Render(GUIColor, TransparentGUI);


		if( Focused && Edition )
		{
			timeElapsed += caretBlipTimer.GetElapsedSeconds();

			if( timeElapsed >= caretBlipFrequency/1000.f )
			{
				RenderCaret = !RenderCaret;
				timeElapsed = 0;
			}
			
			if( RenderCaret )
			{
				PrintCaret(GUIColor, TransparentGUI);
			}
		}
	}

	void Edit::PrintCaret (Color const &GUIColor, bool const &TransparentGUI)
	{
		if( caretPosition.y == -1 )
			return;


		glPushMatrix();


		if( TransparentGUI )
		{
			glEnable(GL_BLEND);
		}
		else
			glDisable(GL_BLEND);

		//glColor4fv(GUIColor * textColor);

		Color mixedColor = GUIColor * textColor;

		glColor4f(mixedColor.GetRed(), mixedColor.GetGreen(), mixedColor.GetBlue(), mixedColor.GetAlpha());
		


		float horizontalShift = (caretPosition.x*currentFont.width),
			  veritcalShift = (caretPosition.y+1)*currentFont.height;

		if( !Overwrite )
		{
			horizontalShift -=  currentFont.width/2.f;
		}


		char caretChar[MAX_PATH];

		if( Overwrite )
		{
			strcpy( caretChar, "_" );
		}
		else
		{
			strcpy( caretChar, "|" );
		}

		glPushAttrib(GL_LIST_BIT);

			glListBase(currentFont.fontID-32);

			glRasterPos2f( controlBorder.left + frameThickness + horizontalShift, 
						   controlBorder.top  - frameThickness - veritcalShift);

			
			glCallLists(strlen(caretChar), GL_UNSIGNED_BYTE, caretChar);

		glPopAttrib();


		glPopMatrix();
	}
//
/*****************************************************/

	void Edit::Save (std::fstream &file)
	{
		Label::Save(file);

		file << "\t" << "#----------------------------------------Edit_components----------------------------------------#\n\n";

		SaveEditCaret(file);
		SaveEditBooleans(file);
	}
//
		void Edit::SaveEditCaret    (std::fstream &file)
		{
			file << "\t" << "#Caret_blip_frequency:" << "\t" << caretBlipFrequency << "\n"
				 << "\t" << "#Caret_index:"          << "\t" << caretIndex         << "\n"

				 << "\t" << "#Caret_position:"
				 << "\t" << caretPosition.x          << "\t" << caretPosition.y    << "\n\n";
		}

		void Edit::SaveEditBooleans (std::fstream &file)
		{
			file << "\t" << "#Numbers_only:" << "\t" << NumbersOnly << "\n"
				 << "\t" << "#Letters_only:" << "\t" << LettersOnly << "\n"
				 << "\t" << "#Overwrite:"    << "\t" << Overwrite   << "\n"
				 << "\t" << "#Edition:"      << "\t" << Edition     << "\n\n";
		}
//
	void Edit::Load (std::fstream &file)
	{
		Label::Load(file);

		LoadEditCaret(file);
		LoadEditBooleans(file);
	}
//
		void Edit::LoadEditCaret    (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		caretBlipFrequency = atof( line.c_str() );
			line = ReadNewValue(file, '#');		caretIndex         = atoi( line.c_str() );

			line = ReadNewValue(file, '#');		caretPosition.x    = atof( line.c_str() );
			line = ReadNewValue(file, '#');		caretPosition.y    = atof( line.c_str() );
		}

		void Edit::LoadEditBooleans (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		NumbersOnly = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		LettersOnly = atoi( line.c_str() );

			assert( NumbersOnly + LettersOnly <= 1 );

			line = ReadNewValue(file, '#');		Overwrite = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		Edition   = atoi( line.c_str() );
		}
