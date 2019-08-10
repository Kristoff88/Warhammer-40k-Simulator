	#include "GlobalHeaders.h"

	#include "Label.h"


	#define DEFAULT_MAX_CHARACTERS (255)


	Label::Label()
	{
		controlType = "Label";
		SetDefaultLabelData();
	}
//
		void Label::SetDefaultLabelData()
		{
			textColor = Color(0,0,0,1);
			maxCharacterCount = DEFAULT_MAX_CHARACTERS;

			rowList.push_back("");
			scrollDown = 0;
					
			ClearAlignmentFlags();
			LeftAlignment = true;		
			Multiline = true;
		}
//
	Label::Label(Label const &label)
	{
		this->Control::operator=(label);
		CopyLabelData(label);
	}
//
		void Label::CopyLabelData(Label const &label)
		{
			CopyLabelTextData(label);
			CopyLabelBooleanFlags(label);	
		}
//
		void Label::CopyLabelTextData(Label const &label)
		{
			controlText   = label.controlText;
			textColor     = label.textColor;

			maxCharacterCount = label.maxCharacterCount;

			currentFont = label.currentFont;

			rowList	   = label.rowList;
			scrollDown = label.scrollDown;
		}

		void Label::CopyLabelBooleanFlags(Label const &label)
		{
			LeftAlignment   = label.LeftAlignment;
			CenterAlignment = label.CenterAlignment;
			RightAlignment  = label.RightAlignment;

			Multiline = label.Multiline;
		}
//
	Label::~Label()
	{}
//
	Label& Label::operator= (Control const &ctrl)
	{
		Control::operator =(ctrl);
		return *this;
	}
	Label& Label::operator= (Label const &label)
	{
		Control::operator=(label);
		CopyLabelData(label);
		return *this;
	}
//
/******************************************************
		Properties managment.
******************************************************/

	void Label::SetText         (std::string const &text)
	{
		controlText = text;

		if( currentFont.fontID )
		{
			rowList = ProcessText(controlText);
		}
	}

//
		std::vector <std::string> Label::ProcessText (std::string const &text)
		{
			SliceOffSuperflousCharacters();

			if( Multiline )
			{
				return ProcessTextToRows(text);
			}
			else
				return ProcessTextToLine(text);
		}

//
		std::vector <std::string> Label::ProcessTextToLine (std::string const &text)
		{
			std::vector <std::string> line;
			std::string slicedText = text;
			float controlWidth = GetWidth();

			#define CHAR_WIDTH   (currentFont.width*1.2)


			while( slicedText.size()*CHAR_WIDTH > controlWidth )
			{
				slicedText.erase( slicedText.begin() );
			}

			JustifyRow(slicedText);
			line.push_back(slicedText);


			#undef CHAR_WIDTH

			return line;
		}

		std::vector <std::string> Label::ProcessTextToRows (std::string const &text)
		{
			std::vector <std::string> rows;

			std::vector <std::string> wordList = ProcessTextToWords(controlText);
			rows = ProcessWordsToRows(wordList);

			return rows;
		}

//
			std::vector <std::string> Label::ProcessTextToWords      (std::string &text)
			{
				std::vector <std::string> wordList;
				std::string word,
					        singleCharacter;


				for(int i = 0; i < text.size(); i++)
				{
					if( IsWhiteCharacter(text[i]) )
					{
						if( !word.empty() )
						{
							wordList.push_back(word);
							word.clear();
						}

						PushBackSingleCharacter(wordList, text[i]);
					}
					else if( IsEndOfLineMarker(text[i]) )
					{
						if( !word.empty() )
						{
							wordList.push_back(word);
							word.clear();
						}

						PushBackSingleCharacter(wordList, text[i]);
					}
					else
					{
						word.push_back(text[i]);
					}
				}

				// something left unadded
				if( !word.empty() )
				{
					wordList.push_back(word);
				}

				return wordList;
			}
			std::vector <std::string> Label::PushBackSingleCharacter (std::vector <std::string> & wordList, char const &character)
			{
				std::string singleCharacter;

				singleCharacter.push_back(character);
				wordList.push_back(singleCharacter);

				return wordList;
			}
//
			std::vector <std::string> Label::ProcessWordsToRows      (std::vector <std::string> &wordList)
			{
				std::vector <std::string> rowList;

				float controlWidth  = controlBorder.right - controlBorder.left   - (2*frameThickness),
					  controlHeight = controlBorder.top   - controlBorder.bottom - (2*frameThickness);

				int rowWidth   = 0,
					textHeight = 0;

				std::string newRow("");



				#define PUSH_ROW     JustifyRow(newRow);               \
									 rowList.push_back(newRow);        \
									 newRow.clear();                   \
									 rowWidth = 0;                     \
									 textHeight += currentFont.height;               
						

				#define POP_ROW      rowList.erase( rowList.end()-1 ); \
				                     textHeight -= currentFont.height;
	
				#define WORD_WIDTH   (wordList[i].size()*currentFont.width*1.2)
				#define CHAR_WIDTH   (currentFont.width*1.2)




				for(int i = 0; i < wordList.size(); i++)
				{
					if( IsEndOfLineMarker(wordList[i][0]) )
					{
						int EndOfLineMarkerIsSingleCharacterWord = 1;
						assert( wordList[i].size() == EndOfLineMarkerIsSingleCharacterWord );

						PUSH_ROW
					}
					else 
					{
						if( (rowWidth + WORD_WIDTH) <= controlWidth )
						{
							newRow   += wordList[i];
							rowWidth += WORD_WIDTH;
						}
						else
						{
							if( !newRow.empty() )
							{
								PUSH_ROW
							}

							std::vector <std::string> wordSections;

							wordSections = SplitWordBetweenRows(wordList[i], CHAR_WIDTH, controlWidth);

							for(int j = 0; j < wordSections.size()-1; j++)
							{
								newRow = wordSections[j];
								PUSH_ROW;
							}

							wordList[i] = wordSections.back();

							newRow   += wordList[i];
							rowWidth += WORD_WIDTH;
						}
					}


					// there is something left, unadded...
					if( i == wordList.size()-1 )//&& !newRow.empty() )
					{
						PUSH_ROW
					}

					minControlHeight;

					if( textHeight >= controlHeight )
					{
						// slicing off the superflous text
						while( textHeight > controlHeight )
						{
							POP_ROW
						}
						break;
					}
				}


				#undef PUSH_ROW 
				#undef POP_ROW
				#undef WORD_WIDTH
				#undef CHAR_WIDTH


				// there's always one, empty row!
				if( rowList.empty() )
				{
					rowList.push_back("");
				}


				return rowList;
			}
//
			std::vector <std::string> Label::SplitWordBetweenRows (std::string const &word, float const &charWidth, float const &controlWidth)
			{
				std::vector <std::string> wordSections;

				std::string section("");
				float sectionWidth = 0;

				// Single character can't fit in this control!
				if( charWidth > controlWidth )
				{
					wordSections.push_back( section );
					return wordSections;
				}


				for(int i = 0; i < word.size(); i++)
				{
					if( sectionWidth + charWidth < controlWidth )
					{
						section.push_back( word[i] );
						sectionWidth += charWidth;
					}
					else
					{
						wordSections.push_back ( section );
						section.clear();
						sectionWidth = 0;

						section.push_back( word[i] );
						sectionWidth += charWidth;
					}
				}


				// Add possible leftovers...
				if( !section.empty() )
				{
					wordSections.push_back( section );
				}


				return wordSections;
			}
//
			void Label::JustifyRow		   (std::string &row)
			{
				if( currentFont.fontID == 0 )
					return;


				float controlWidth  = controlBorder.right - controlBorder.left - (2*frameThickness);

				int rowSpaceLeft = (controlWidth/currentFont.width) - row.size();

				assert( rowSpaceLeft >= 0 );
				assert( CheckAlignmentFlagsCorrectness() );


				if( LeftAlignment )
				{
					;
				}
				else if( CenterAlignment )
				{
					bool ToTheEnd = true;

					for(int i = 0; i < rowSpaceLeft; i++)
					{
						if( ToTheEnd )
						{
							row.insert( row.end(), ' ');
						}
						else
						{
							row.insert( row.begin(), ' ');
						}

						ToTheEnd = !ToTheEnd;
					}
				}
				else if( RightAlignment )
				{
					for(int i = 0; i < rowSpaceLeft; i++)
					{
						row.insert( row.begin(), ' ');
					}
				}
			}

			bool Label::IsWhiteCharacter   (char const &Char)
			{
				if( Char == ' ' )
				{
					return true;
				}
				else
					return false;
			}

			bool Label::IsEndOfLineMarker  (char const &Char)
			{
				if( Char == '\n' )
				{
					return true;
				}
				else
					return false;
			}
//
	void Label::AdjustWidthToCaption ()
	{
		if( !controlText.empty() )
		{
			std::string caption = FilterOutEndOfLineMarks();

			controlBorder.right = controlBorder.left;
			rowList = ProcessText(controlText);

			while( rowList.empty() || rowList[0] != caption )
			{
				controlBorder.right += 1;
				rowList = ProcessText(controlText);
			}
		}
	}
//
	std::string Label::FilterOutEndOfLineMarks ()
	{
		std::string filteredText;

		for(int i = 0; i < controlText.size(); i++)
		{
			if( controlText[i] != '\n' )
			{
				filteredText.push_back( controlText[i] );
			}
		}

		return filteredText;
	}
//
	void Label::SetTextColor    (Color const &color)
	{
		textColor = color;
	}

//
	void Label::SetMaxCharacterCount (unsigned int const &charCount)
	{
		maxCharacterCount = charCount;
		SliceOffSuperflousCharacters();
	}
//
		void Label::SliceOffSuperflousCharacters ()
		{
			while( controlText.size() > maxCharacterCount )
			{
				controlText.erase( controlText.end()-1 );
			}
		}
//
	void Label::SetFont (Font &font, HDC const &hdc) throw( std::string )
	{
		HFONT fontHandler = NULL;

		fontHandler = CreateFont(font.height, font.width, 
			                     0, 0, font.weight, 
							     font.Italic, font.Underline, font.Strikeout,
							     ANSI_CHARSET, 
							     OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
							     ANTIALIASED_QUALITY, FF_DONTCARE | FIXED_PITCH,
							     font.fontName); 

		CheckFontCorrectnes(fontHandler, font);
		GenerateNewFont(hdc, fontHandler, font);

		rowList = ProcessText(controlText);
	}
//
		void Label::CheckFontCorrectnes      (HFONT const &fontHandler, Font const &font) throw( std::string )
		{
			if( !fontHandler || !font.fontName )
			{
				InformAboutIncorrectFont(font);
			}
		}

		void Label::InformAboutIncorrectFont (Font const &font)                           throw( std::string )
		{
			std::string error;

			char heightAsChar [MAX_PATH];
			char widthAsChar  [MAX_PATH];
			char weightAsChar [MAX_PATH];


			_itoa_s(font.height, heightAsChar, MAX_PATH, 10);
			_itoa_s(font.width,  widthAsChar, MAX_PATH, 10);

			
			switch( font.weight )
			{
				case FW_DONTCARE:
					strcpy( weightAsChar, "FW_DONTCARE" );
				break;

				case FW_THIN:
					strcpy( weightAsChar, "FW_THIN" );
				break;

				case FW_EXTRALIGHT:
					strcpy( weightAsChar, "FW_EXTRALIGHT" );
				break;

				case FW_LIGHT:
					strcpy( weightAsChar, "FW_LIGHT" );
				break;

				case FW_NORMAL:
					strcpy( weightAsChar, "FW_NORMAL" );
				break;

				case FW_MEDIUM:
					strcpy( weightAsChar, "FW_MEDIUM" );
				break;

				case FW_SEMIBOLD:
					strcpy( weightAsChar, "FW_SEMIBOLD" );
				break;

				case FW_BOLD:
					strcpy( weightAsChar, "FW_BOLD" );
				break;

				case FW_EXTRABOLD:
					strcpy( weightAsChar, "FW_EXTRABOLD" );
				break;

				case FW_HEAVY:
					strcpy( weightAsChar, "FW_HEAVY" );
				break;
			}



			error  = "Font couldn't be created properly.\n\n name: \"";

			if( font.fontName )
			{
				error += font.fontName;
			}
			else
				error += "NULL";


			error += "\",\n\n height: ";
			error += heightAsChar;

			error += "\",\n\n width: ";
			error += widthAsChar;

			error += ",\n weight: ";
			error += weightAsChar;

			error += ",\n\n Italic: ";
			if( font.Italic )
				error += "TRUE";
			else
				error += "FALSE";

			error += ",\n Underline: ";
			if( font.Underline )
				error += "TRUE";
			else
				error += "FALSE";

			error += ",\n Strikeout: ";
			if( font.Strikeout )
				error += "TRUE";
			else
				error += "FALSE";


			throw error;
		}

//
		void Label::GenerateNewFont (HDC const &hdc, HFONT const &fontHandler, Font &font)
		{
			font.fontID = glGenLists(96);
	    
			SelectObject(hdc, fontHandler);
			wglUseFontBitmaps(hdc, 32, 96, font.fontID);

			currentFont = font;

			DeleteObject(fontHandler);
		}
//
	void Label::SetLeftAlignment   ()
	{
		ClearAlignmentFlags();
		LeftAlignment = true;

		rowList = ProcessText(controlText);
	}

	void Label::SetCenterAlignment ()
	{
		ClearAlignmentFlags();
		CenterAlignment = true;

		rowList = ProcessText(controlText);
	}

	void Label::SetRightAlignment  ()
	{
		ClearAlignmentFlags();
		RightAlignment = true;

		rowList = ProcessText(controlText);
	}

//
	    bool Label::CheckAlignmentFlagsCorrectness ()
		{
			return (LeftAlignment + CenterAlignment + RightAlignment) == 1;
		}

		void Label::ClearAlignmentFlags            ()
		{
			LeftAlignment = CenterAlignment = RightAlignment = false;
		}
//
	void Label::SetMultiline (bool const &isMultiline)
	{
		Multiline = isMultiline;
		rowList = ProcessText(controlText);
	}
	bool Label::IsMultiline ()
	{
		return Multiline;
	}
//
	std::string  Label::GetText		         ()
	{
		return controlText;
	}


	std::string  Label::GetRow               (unsigned int const &index)
	{
		if( index >= rowList.size() )
		{
			return rowList.back();
		}
		else
		{
			return rowList[index];
		}
	}

	int          Label::GetRowCount          ()
	{
		return rowList.size();
	}
	Color		 Label::GetTextColor         ()
	{
		return textColor;
	}

	unsigned int Label::GetMaxCharacterCount ()
	{
		return maxCharacterCount;
	}

	Font         Label::GetCurrentFont       ()
	{
		return currentFont;
	}
//
/******************************************************
		Standard function redefinitions.
******************************************************/

	void Label::SetBorders (RECT const &Border)
	{
		Control::SetBorders(Border);
		rowList = ProcessText(controlText);
	}
//
	void Label::Resize (POINT const &mousePosition)
	{
		Control::Resize(mousePosition);
		rowList = ProcessText(controlText);
	}

	void Label::Scale  (float const &scaleMultiplier)
	{
		Control::Scale(scaleMultiplier);
		rowList = ProcessText(controlText);
	}
//
/******************************************************
		Rendering.
******************************************************/

	void Label::Render    (Color const &GUIColor, bool const &TransparentGUI)
	{
		Control::Render(GUIColor, TransparentGUI);
		PrintText(GUIColor, TransparentGUI);	
	}
	void Label::PrintText (Color const &GUIColor, bool const &TransparentGUI)
	{
		if ( currentFont.fontID == 0 ) 
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

	    
		float shiftDelta = currentFont.height,
			  shift = shiftDelta;

		glPushAttrib(GL_LIST_BIT);

			glListBase(currentFont.fontID-32);

			for(int i = scrollDown; i < rowList.size()+scrollDown; i++)
			{
				glRasterPos2f( controlBorder.left + frameThickness, 
					           controlBorder.top  - frameThickness - shift);

				static char rowText[MAX_PATH];

				if( i < rowList.size() )
				{
					strcpy( rowText, rowList[i].c_str() );
				}
				else
				{
					strcpy( rowText, "" );
				}

				glCallLists(strlen(rowText), GL_UNSIGNED_BYTE, rowText);

				shift += shiftDelta;
			}

		glPopAttrib();


		glPopMatrix();
	}
//
/*****************************************************/

	void Label::Save (std::fstream &file)
	{
		Control::Save(file);

		file << "\t" << "#----------------------------------------Label_components----------------------------------------#\n\n";

		SaveLabelText(file);
		SaveLabelFont(file);
		SaveLabelBooleans(file);
	}
//
		void Label::SaveLabelText          (std::fstream &file)
		{
			file << "\t" << "#Control_text:\n";

			if( controlText.empty() )
			{
				file << "\t" << "None" << "\n\n";
			}
			else
				file << "\t" << controlText << "\n\n";

			file << "\t" << "#Text_color:\n"
				 << "\t" << "#Red:"   << "\t" << textColor.GetRed()   << "\n"
				 << "\t" << "#Green:" << "\t" << textColor.GetGreen() << "\n"
				 << "\t" << "#Blue:"  << "\t" << textColor.GetBlue()  << "\n"
				 << "\t" << "#Alpha:" << "\t" << textColor.GetAlpha() << "\n\n"

				 << "\t" << "#Scroll_down:" << "\t" << scrollDown << "\n\n";
		}

		void Label::SaveLabelFont          (std::fstream &file)
		{
			file << "\t" << "#Current_font:\n";

			if( currentFont.fontID )
			{
				file << "\t" << "#Height:"    << "\t" << currentFont.height    << "\n"
					 << "\t" << "#Width:"     << "\t" << currentFont.width     << "\n"
					 << "\t" << "#Weight:"    << "\t" << currentFont.weight    << "\n"

					 << "\t" << "#Italic:"    << "\t" << currentFont.Italic    << "\n"
					 << "\t" << "#Underline:" << "\t" << currentFont.Underline << "\n"
					 << "\t" << "#Strikeout:" << "\t" << currentFont.Strikeout << "\n"

					 << "\t" << "#Name:"      << "\t" << currentFont.fontName << "\n\n";
			}
			else
			{
				file << "\t" << "None" << "\n\n";
			}
		}

		void Label::SaveLabelBooleans      (std::fstream &file)
		{
			file << "\t" << "#Left_alignment:"   << "\t" << LeftAlignment   << "\n"
				 << "\t" << "#Center_alignment:" << "\t" << CenterAlignment << "\n"
				 << "\t" << "#Right_alignment:"  << "\t" << RightAlignment  << "\n\n";
		}
//
	void Label::Load (std::fstream &file)
	{
		Control::Load(file);

		LoadLabelText(file);
		LoadLabelFont(file);
		LoadLabelBooleans(file);
	}
//
		void Label::LoadLabelText          (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');	

			if( line != "None" )
			{
				controlText = line;
			}
				

			line = ReadNewValue(file, '#');		textColor.SetRed   ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		textColor.SetGreen ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		textColor.SetBlue  ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		textColor.SetAlpha ( atof(line.c_str()) );

			line = ReadNewValue(file, '#');		scrollDown = atoi( line.c_str() );
		}

		void Label::LoadLabelFont          (std::fstream &file)
		{
			std::string line;
			Font loadedFont;

			line = ReadNewValue(file, '#');		

			if( line != "None" )
			{
				loadedFont.height = atoi( line.c_str() );

				line = ReadNewValue(file, '#');		loadedFont.width  = atoi( line.c_str() );
				line = ReadNewValue(file, '#');		loadedFont.weight = atoi( line.c_str() );

				line = ReadNewValue(file, '#');		loadedFont.Italic    = atoi( line.c_str() );
				line = ReadNewValue(file, '#');		loadedFont.Underline = atoi( line.c_str() );
				line = ReadNewValue(file, '#');		loadedFont.Strikeout = atoi( line.c_str() );

				line = ReadNewValue(file, '#');		loadedFont.fontName = const_cast<char*>(line.c_str());


				SetFont(loadedFont, GetDC(NULL));
			}
		}

		void Label::LoadLabelBooleans      (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		LeftAlignment   = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		CenterAlignment = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		RightAlignment  = atoi( line.c_str() );

			assert( CheckAlignmentFlagsCorrectness() );
		}
