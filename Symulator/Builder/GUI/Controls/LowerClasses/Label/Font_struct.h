#pragma once


	class Label;
	class Edit;

	class Font
	{
		friend Label;
		friend Edit;

		public:

			int width, 
				height;

			int weight;

			bool Italic,
				 Underline,
				 Strikeout;

			char *fontName;


			Font(): width(12), height(24), weight(FW_NORMAL),
				    Italic(false), Underline(false), Strikeout(false),
					fontName("Courier"), fontID(0)
			{}
			~Font()
			{}

			unsigned int GetFontID ()
			{  return fontID;  }

		private:

			unsigned int fontID;
	};
