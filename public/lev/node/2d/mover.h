#pragma once

#include <lev/node/2d/node2d.h>

namespace lev
{
	class Mover : public Node2D
	{
	public:
		Mover(float speed)
			: m_speed(speed)
		{
		}

		void update() override
		{
			position.x += m_speed;
			Log::print("%f %f", position.x, position.y);
		}

	private:
		float m_speed;
	};
}
