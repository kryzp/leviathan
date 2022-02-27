#pragma once

namespace Lev
{
	struct Vec2;
	struct Mat4x4;

	// special matrix used for 2D transformations
	// mathematically this doesnt actually make sense

	struct Mat3x2
	{
		static const Mat3x2 IDENTITY;

		union
		{
			struct
			{
				float m11;
				float m12;
				float m21;
				float m22;
				float m31;
				float m32;
			};

			float elements[6];
		};

		Mat3x2();
		Mat3x2(float initial);
		Mat3x2(
			float m11, float m12,
			float m21, float m22,
			float m31, float m32
		);

		float* value_ptr();
		const float* value_ptr() const;

		// utility function: converts this 'fake' 2D transformation matrix into something that's *actually* a transformation matrix (usable in rendering)
		Mat4x4 to_mat4x4_transform() const;

		float scaling_factor() const;
		float determinant() const;
		Mat3x2 inverse() const;

		static Mat3x2 create_skew(const Vec2& amount);
		static Mat3x2 create_scale(const Vec2& scale);
		static Mat3x2 create_rotation(float rotation);
		static Mat3x2 create_translation(const Vec2& position);
		static Mat3x2 create_transformation(const Vec2& position, float rotation, const Vec2& scale, const Vec2& origin);

		Mat3x2 operator - (const Mat3x2& other);
		Mat3x2 operator + (const Mat3x2& other);
		Mat3x2 operator * (const Mat3x2& other);

		Mat3x2& operator -= (const Mat3x2& other);
		Mat3x2& operator += (const Mat3x2& other);
		Mat3x2& operator *= (const Mat3x2& other);
	};
};
