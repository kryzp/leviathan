#pragma once

namespace LEV
{
	struct Float2
	{
		float x;
		float y;

		Float2()
			: x(0)
			, y(0)
		{
		}

		Float2(float x)
			: x(x)
			, y(x)
		{
		}

		Float2(float x, float y)
			: x(x)
			, y(y)
		{
		}
	};
}
