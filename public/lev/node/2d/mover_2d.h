#pragma once

#include <lev/node/2d/node_2d.h>
#include <lev/node/2d/collider_2d.h>
#include <lev/core/util.h>

namespace lev
{
	class Mover2D : public Node2D
	{
	public:
		Mover2D()
			: m_velocity(Vec2F::zero())
			, m_collider(nullptr)
		{
		}

		void update() override
		{
			Node2D::update();

			transform.move(m_velocity * Time::delta);
		}

		void set_velocity(const Vec2F& vel)
		{
			m_velocity = vel;
		}

		void set_collider(const Collider2D* col)
		{
			m_collider = col;
		}

	private:
		Vec2F m_velocity;
		const Collider2D* m_collider;
	};
}
