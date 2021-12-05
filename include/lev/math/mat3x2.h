#pragma once

namespace Lev
{
	struct Vec2;

	struct Mat3x2
	{
		static const Mat3x2 IDENTITY;

		float m11;
		float m12;
		float m21;
		float m22;
		float m31;
		float m32;

		Mat3x2();
		Mat3x2(float m11, float m12, float m21, float m22, float m31, float m32);

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
