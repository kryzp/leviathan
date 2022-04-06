#pragma once

namespace lev
{
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

			struct
			{
				float w;
				float x;
				float y;
				float z;
			};

			float elements[4];
		};

		Quaternion();
		Quaternion(float x);
		Quaternion(float s, float i, float j, float k);

		static const Quaternion& zero();
		static const Quaternion& one();
	};
}
