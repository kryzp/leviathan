// this is an awkward template implementation of matrix but honestly too much work haha.
// ill come back to this but for now im just using manually made Mat3x2 and Mat4x4 matrix implementations
// theyre really all you need 99% of the time.

/*
#pragma once

#include <lev/math/vec2.h>

namespace Lev
{
	template <int Columns, int Rows>
	struct Matrix
	{
		float elements[Columns * Rows];

		Matrix()
		{
			for (int i = 0; i < Columns * Rows; i++)
				elements[i] = 0;
		}

		Matrix(float data[])
		{
			LEVIATHAN_ASSERT(LEV_ARRAY_LENGTH(elements) == Columns*Rows);
			memcpy(elements, data, sizeof(float) * (Columns*Rows));
		}

		float determinant() const
		{
			LEVIATHAN_ASSERT(Rows != Columns);
		}

		Matrix cofactor() const
		{
			Matrix mat = identity();
			Matrix<Columns-1, Rows-1> sub_mat = Matrix<Columns-1, Rows-1>::identity();
			
			return mat;
		}

		Matrix transpose() const
		{
			Matrix mat = identity();

			for(int i = 0; i < Rows; i++)
			{
				for(int j = 0; j < Columns; j++)
				{
					solution[(j*Columns)+i] = matrix1[(i*Columns)+j];
				}
			}

			return solution;
		}

		Matrix inverse() const
		{
			float det = 1.0f / determinant();
			Matrix mat = identity();

			for(int i = 0; i < Rows * Columns; i++) {
				mat.elements[i] *= det; 
			}

			return transpose(cofactor(mat));
		}

		static Matrix identity()
		{
			Matrix mat;

			for (int i = 0; i < Rows; i++)
				mat.elements[(i*Columns)+i] = 1;

			return mat;
		}

		static Matrix create_scale(const Vec2& scale)
		{
			Matrix mat;

			return mat;
		}

		static Matrix create_rotation(float rotation)
		{
			Matrix mat;

			return mat;
		}

		static Matrix create_translation(const Vec2& position)
		{
			Matrix mat;

			return mat;
		}

		static Matrix create_transformation(const Vec2& position, float rotation, const Vec2& scale, const Vec2& origin)
		{
			Matrix mat = identity();

			if (origin.x != 0 || origin.y != 0)
				mat *= create_translation(-origin);

			if (scale.x != 0 || scale.y != 0)
				mat *= create_scale(scale);

			if (rotation != 0)
				mat *= create_rotation(rotation);

			if (position.x != 0 || position.y != 0)
				mat *= create_translation(position);

			return mat;
		}

		Matrix operator - (const Matrix& other)
		{
			Matrix mat;

			for (int i = 0; i < Columns * Rows; i++)
				mat.elements[i] = this->elements[i] - other.elements[i];

			return mat;
		}

		Matrix operator + (const Matrix& other)
		{
			Matrix mat;

			for (int i = 0; i < Columns * Rows; i++)
				mat.elements[i] = this->elements[i] + other.elements[i];

			return mat;
		}

		Matrix operator * (const Matrix& other)
		{
		}
	};

	using Mat2x2 = Matrix<2, 2>;
	using Mat3x2 = Matrix<3, 2>;
	using Mat3x3 = Matrix<3, 3>;
	using Mat4x4 = Matrix<4, 4>;
}
*/