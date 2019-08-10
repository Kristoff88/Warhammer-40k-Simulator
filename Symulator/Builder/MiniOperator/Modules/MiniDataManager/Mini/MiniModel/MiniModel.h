#pragma once

	#include "MiniModel_Data//MiniModel_Data.h"


	/*
		TODO: Dopracowac osie kierunkowe, wychodzace z modelu
	*/


	class MiniModel: public MiniModel_Data
	{
		public:

			MiniModel  ();
			~MiniModel ();

	/****************************************************************/

			void Render ();
				private:
				inline void DrawModel	   ();
				inline void DrawSizeAxises ();
			public:

	/****************************************************************/

			void    SetPosition   (Vector3 const &newPosition);
			void    ShiftPosition (Vector3 const &delta);
			Vector3 GetPosition   ();

			Vector3 GetSize   ();
			float   GetRadius ();

			void    SetRotation (Vector3 const &newRotation);
			void    LookAt		(Vector3 const &position);
			Vector3 GetRotation ();

			void    SetScale (Vector3 const &newScale);
			Vector3 GetScale ();

			void  SetColor (Color &newColor);
			Color GetColor ();

			void SetAxisDrawing  (bool const &newValue);
			bool IsDrawingAxises ();

	/****************************************************************/

			float GetDistance (Vector3 const &position);

			bool PositionWithinModelProximity (Vector3 const &position);

	/****************************************************************/

			void Save (std::fstream &file);
				private:
				inline void SavePosition (std::fstream &file);
				inline void SaveRotation (std::fstream &file);
				inline void SaveScale    (std::fstream &file);
				inline void SaveColor    (std::fstream &file);
			public:

			void Load (std::fstream &file);
				private:
				inline void LoadPosition (std::fstream &file);
				inline void LoadRotation (std::fstream &file);
				inline void LoadScale    (std::fstream &file);
				inline void LoadColor    (std::fstream &file);
			public:


		protected:

			Vector3 position, 
				    rotation, 
					scale;

			Color color;

			bool AxisAreDrawn;
	};