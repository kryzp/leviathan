#pragma once

namespace lv
{
	template <typename T> struct Vec3;
	using Vec3F = Vec3<float>;

	struct Quaternion
	{
		union
		{
			struct
			{
				float s;
				float i;
				float j;
				float k;
			};

			float data[4];
		};

		Quaternion();
		Quaternion(float x);
		Quaternion(float s, float i, float j, float k);

		static Quaternion from_euler(float pitch, float yaw, float roll);
		static Vec3F to_euler(const Quaternion& quat);

		float* value_ptr();
		const float* value_ptr() const;

		static const Quaternion& zero();
		static const Quaternion& one();
	};
}
