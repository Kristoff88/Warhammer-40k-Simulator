#include "GlobalHeaders.h"

#include "Vector3.h"


	Vector3::Vector3(): x(0), y(0), z(0) 
	{}

	Vector3::Vector3(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
//
/******************************************************
		Dodatkowe funkcje
******************************************************/

	bool   Vector3::Equals	    (Vector3 const &arg)
	{
		return x == arg.x && y == arg.y && z == arg.z;
	}

	bool   Vector3::NotEquals   (Vector3 const &arg)
	{
		return !Equals(arg);
	}
//
	Vector3& Vector3::Add       (Vector3 const &arg)
	{
		x += arg.x;
		y += arg.y;
		z += arg.z;

		return *this;
	}

	Vector3& Vector3::Substract (Vector3 const &arg)
	{
		x -= arg.x;
		y -= arg.y;
		z -= arg.z;

		return *this;
	}
//
	Vector3& Vector3::Multiply  (Vector3 const &arg)
	{
		x *= arg.x;
		y *= arg.y;
		z *= arg.z;

		return *this;
	}

	Vector3& Vector3::Divide    (Vector3 const &arg)
	{
		x /= arg.x;
		y /= arg.y;
		z /= arg.z;

		return *this;
	}
//
/******************************************************
		Pomocnicze operatory
******************************************************/

	bool   Vector3::operator== (Vector3 const &arg)
	{
		return Equals(arg);
	}

	bool   Vector3::operator!= (Vector3 const &arg)
	{
		return NotEquals(arg);
	}
//
	Vector3  Vector3::operator+  (Vector3 const &arg) const
	{
		Vector3 result = *this;

		return result.Add(arg);;
	}

	Vector3& Vector3::operator+= (Vector3 const &arg)
	{
		Add(arg);
		return *this;
	}
//
	Vector3  Vector3::operator-  (Vector3 const &arg) const
	{
		Vector3 result = *this;

		return result.Substract(arg);
	}

	Vector3& Vector3::operator-= (Vector3 const &arg)
	{
		Substract(arg);
		return *this;
	}
//
	Vector3  Vector3::operator*  (Vector3 const &arg) const
	{
		Vector3 result = *this;

		return result.Multiply(arg);
	}

	Vector3& Vector3::operator*= (Vector3 const &arg)
	{
		Multiply(arg);
		return *this;
	}
//
	Vector3  Vector3::operator/  (Vector3 const &arg) const
	{
		Vector3 result = *this;

		return result.Divide(arg);
	}

	Vector3& Vector3::operator/= (Vector3 const &arg)
	{
		Divide(arg);
		return *this;
	}
//
	Vector3& Vector3::operator=  (Vector3 const &arg)
	{
		x = arg.x;
		y = arg.y;
		z = arg.z;

		return *this;
	}

	float  Vector3::operator[] (int index)        throw( std::string )
	{
		switch( index )
		{
			case 0:		return x;
			case 1:		return y;
			case 2:		return z;

			default:
			{
				std::string error;

				char wrongIndex[MAX_PATH];
				_itoa_s(index, wrongIndex, MAX_PATH, 10);
				
				error += "Vector3::operator[]. Incorrect index: ";
				error += wrongIndex;

				throw error;
			}
			break;
		}
	}
//
/******************************************************
			Konwertery
******************************************************/

	Vector3::operator double* ()
	{
		double conversion[4] = {x, y, z};
		return conversion;
	}

	Vector3::operator float*  ()
	{
		float conversion[4] = {x, y, z};
		return conversion;
	}

	Vector3::operator int*    ()
	{
		int conversion[4] = {x, y, z};
		return conversion;
	}