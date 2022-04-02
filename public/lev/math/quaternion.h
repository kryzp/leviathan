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

		// todo
	};
}
