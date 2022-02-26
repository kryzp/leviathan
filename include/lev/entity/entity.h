#pragma once

#include <lev/core/core.h>
#include <lev/containers/string.h>
#include <lev/math/transform.h>
#include <lev/math/calc.h>

/*
Designed to be as simple and unintrusive/malluable as possible
*/

namespace Lev
{
	class SpriteBatch;

	class Entity
	{
	public:
		Entity() = default;
		virtual ~Entity() = default;

		virtual void update() = 0;
		virtual void render(const SpriteBatch& b) = 0;

		virtual String name() = 0;

		template <class T> T* as() { return static_cast<T*>(this); }

		Transform transform;
	};
	
	// e.g component
	class Health
	{
		int m_health;
		int m_maximum;

	public:
		Health()
			: m_health(0)
			, m_maximum(0)
		{
		}

		Health(int initial)
			: m_health(initial)
			, m_maximum(initial)
		{
		}

		void set(int to) { m_health = Calc::clamp(to, 0, m_maximum); }
		void change(int hp, int max) { m_health = hp; m_maximum = max; }

		void take_damage(int amount = 1) { m_health = Calc::max(0, m_health - amount); }
		void max_out() { m_health = m_maximum; }

		int current() const { return m_health; }
		int maximum() const { return m_maximum; }
	};

	// e.g entity
	class Player : public Entity
	{
	public:
		Health hp;

		Player()
			: hp(10)
		{
		}

		~Player() = default;

		void update() override
		{
			hp.take_damage();
		}

		void render(const SpriteBatch& b) override { }

		String name() override { return "Player"; }
	};

	// e.g entity
	class Ball : public Entity
	{
	public:
		Ball() = default;
		~Ball() = default;

		void update() override { }
		void render(const SpriteBatch& b) override { }

		String name() override { return "Ball"; }
	};
}
