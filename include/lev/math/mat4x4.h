#pragma once

namespace Lev
{
	struct Mat4x4
	{
		static const Mat4x4 IDENTITY;

		union
		{
			struct
			{
				float m11;
				float m12;
				float m13;
				float m14;
				float m21;
				float m22;
				float m23;
				float m24;
				float m31;
				float m32;
				float m33;
				float m34;
				float m41;
				float m42;
				float m43;
				float m44;
			};

			float elements[16];
		};

		Mat4x4();
		Mat4x4(float initial);
		Mat4x4(
			float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44
		);

		float* value_ptr();
		const float* value_ptr() const;

		Mat4x4 operator - (const Mat4x4& other);
		Mat4x4 operator + (const Mat4x4& other);
		Mat4x4 operator * (const Mat4x4& other);

		Mat4x4& operator -= (const Mat4x4& other);
		Mat4x4& operator += (const Mat4x4& other);
		Mat4x4& operator *= (const Mat4x4& other);
	};
};
