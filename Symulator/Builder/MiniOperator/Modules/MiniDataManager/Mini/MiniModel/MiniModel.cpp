	#include "GlobalHeaders.h"

	#include "MiniModel.h"


	extern  PFNGLGENBUFFERSARBPROC	         glGenBuffersARB;           // VBO Name Generation Procedure
	extern  PFNGLBINDBUFFERARBPROC			 glBindBufferARB;           // VBO Bind Procedure
	extern  PFNGLBUFFERDATAARBPROC			 glBufferDataARB;           // VBO Data Loading Procedure
	extern  PFNGLBUFFERSUBDATAARBPROC		 glBufferSubDataARB;        // VBO Sub Data Loading Procedure
	extern  PFNGLDELETEBUFFERSARBPROC		 glDeleteBuffersARB;        // VBO Deletion Procedure
	extern  PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB; // return various parameters of VBO
	extern  PFNGLMAPBUFFERARBPROC			 glMapBufferARB;            // map VBO procedure
	extern  PFNGLUNMAPBUFFERARBPROC			 glUnmapBufferARB;      



	MiniModel::MiniModel  ()
	{
		scale = Vector3(1,1,1);
		AxisAreDrawn = false;
	}
	MiniModel::~MiniModel ()
	{}
//
/****************************************************************/

	void MiniModel::Render ()
	{
		glPushMatrix();

			glTranslatef(position.x, position.y, position.z);
			glScalef(scale.x, scale.y, scale.z);

			glRotatef(rotation.x, 1.f, 0.f, 0.f);
			glRotatef(rotation.y, 0.f, 1.f, 0.f);
			glRotatef(rotation.z, 0.f, 0.f, 1.f);


			DrawModel();

			if( AxisAreDrawn )
			{
				DrawSizeAxises();
			}
		
		glPopMatrix();
	}
//
		void MiniModel::DrawModel	   ()
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDv);
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDvt);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDvn);
			glNormalPointer(GL_FLOAT, 0, 0);

			//glColor4fv(color);

			glColor4f(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());

			for(int i = 0; i < meshVector.size(); i++)
			{
				glBindTexture(GL_TEXTURE_2D, meshVector[i].textureID);
				glDrawArrays(GL_TRIANGLES, meshVector[i].firstIndex, meshVector[i].indexCount);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void MiniModel::DrawSizeAxises ()
		{
			glLineWidth(5.f);

			glBegin(GL_LINES);

				glColor4f(1,0,0,1);
				glVertex3f(-size.x, 0, -size.z);
				glVertex3f( size.x, 0, -size.z);

				glColor4f(0,1,0,1);
				glVertex3f(-size.x,          0, -size.z);;
				glVertex3f(-size.x, size.y*2.f, -size.z);

				glColor4f(0,0,1,1);
				glVertex3f(-size.x, 0, -size.z);
				glVertex3f(-size.x, 0,  size.z);

			glEnd();
		}
//
/****************************************************************/

	void    MiniModel::SetPosition   (Vector3 const &newPosition)
	{
		position = newPosition;
	}

	void    MiniModel::ShiftPosition (Vector3 const &delta)
	{
		position += delta;
	}

	Vector3 MiniModel::GetPosition   ()
	{
		return position;
	}
//
	Vector3 MiniModel::GetSize   ()
	{
		return size;
	}

	float   MiniModel::GetRadius ()
	{
		return (size.x >= size.z) ? size.x : size.z;
	}
//
	void    MiniModel::SetRotation (Vector3 const &newRotation)
	{
		rotation = newRotation;
	}

	void    MiniModel::LookAt	   (Vector3 const &position)
	{
		rotation.y = atan2( this->position.x-position.x, this->position.z-position.z ) * 57.2957795;
		rotation.y -= 180.f;
	}

	Vector3 MiniModel::GetRotation ()
	{
		return rotation;
	}
//
	void    MiniModel::SetScale (Vector3 const &newScale)
	{
		size /= scale;
		scale = newScale;
		size *= scale;
	}

	Vector3 MiniModel::GetScale ()
	{
		return scale;
	}
//
	void  MiniModel::SetColor (Color &newColor)
	{
		color = newColor;
	}

	Color MiniModel::GetColor ()
	{
		return color;
	}
//
	void MiniModel::SetAxisDrawing  (bool const &newValue)
	{
		AxisAreDrawn = newValue;
	}

	bool MiniModel::IsDrawingAxises ()
	{
		return AxisAreDrawn;
	}
//
/****************************************************************/

	float MiniModel::GetDistance (Vector3 const &position)
	{
		float distance;

		Vector3 delta = this->position - position;

		distance = sqrt( (delta.x*delta.x) + (delta.z*delta.z) );

		if( distance < 0 )
		{
			distance *= -1;
		}

		return distance;
	}
//
	bool MiniModel::PositionWithinModelProximity (Vector3 const &position)
	{
		Vector3 proximity;

		proximity = this->position - position;

		if( proximity.x < 0 )
			proximity.x *= -1.f;

		if( proximity.y < 0 )
			proximity.y *= -1.f;

		if( proximity.z < 0 )
			proximity.z *= -1.f;

		float radius = GetRadius();

		if( proximity.x <= radius &&
			proximity.y <= size.y &&
			proximity.z <= radius )
		{
			return true;
		}
		else
			return false;
	}
//
/****************************************************************/

	void MiniModel::Save (std::fstream &file)
	{
		MiniModel_Data::Save(file);

		SavePosition(file);
		SaveRotation(file);
		SaveScale(file);
		SaveColor(file);
	}
//
		void MiniModel::SavePosition (std::fstream &file)
		{
			file << "#Position:" << "\n"
				 << "#X:" << "\t" << position.x << "\t" 
				 << "#Y:" << "\t" << position.y << "\t" 
				 << "#Z:" << "\t" << position.z << "\n\n";
		}

		void MiniModel::SaveRotation (std::fstream &file)
		{
			file << "#Rotation:" << "\n"
				 << "#X:" << "\t" << rotation.x << "\t" 
				 << "#Y:" << "\t" << rotation.y << "\t" 
				 << "#Z:" << "\t" << rotation.z << "\n\n";
		}

		void MiniModel::SaveScale    (std::fstream &file)
		{
			file << "#Scale:" << "\n"
				 << "#X:" << "\t" << scale.x << "\t" 
				 << "#Y:" << "\t" << scale.y << "\t" 
				 << "#Z:" << "\t" << scale.z << "\n\n";
		}

		void MiniModel::SaveColor    (std::fstream &file)
		{
			file << "#Color:" << "\n"
				 << "#R:" << "\t" << color.GetRed()   << "\t" 
				 << "#G:" << "\t" << color.GetGreen() << "\t"
				 << "#B:" << "\t" << color.GetBlue()  << "\t"
				 << "#A:" << "\t" << color.GetAlpha() << "\n\n";
		}
//
	void MiniModel::Load (std::fstream &file)
	{
		MiniModel_Data::Load(file);

		LoadPosition(file);
		LoadRotation(file);
		LoadScale(file);
		LoadColor(file);
	}
//
		void MiniModel::LoadPosition (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		position.x = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		position.y = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		position.z = atoi( line.c_str() );
		}

		void MiniModel::LoadRotation (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		rotation.x = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		rotation.y = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		rotation.z = atoi( line.c_str() );
		}

		void MiniModel::LoadScale    (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		scale.x = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		scale.y = atoi( line.c_str() );
			line = ReadNewValue(file, '#');		scale.z = atoi( line.c_str() );
		}

		void MiniModel::LoadColor    (std::fstream &file)
		{
			std::string line;

			line = ReadNewValue(file, '#');		color.SetRed   ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		color.SetGreen ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		color.SetBlue  ( atof(line.c_str()) );
			line = ReadNewValue(file, '#');		color.SetAlpha ( atof(line.c_str()) );
		}