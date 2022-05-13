
/*
 * FABRIK solver
 */

#include <lev/core/app.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/math/vec2.h>
#include <lev/math/circle.h>
#include <lev/input/input.h>
#include <lev/containers/linked_list.h>

struct Joint
{
	lv::Vec2F position;
	float length;
};

class Skeleton
{
public:
	static constexpr int ITERATIONS = 25;

	void add_joint(const lv::Vec2F& position)
	{
		Joint joint;
		joint.position = position;

		if (m_joints.last)
			m_joints.last->data.length = (position - m_joints.last->data.position).length();

		m_joints.add(joint);
	}

	void solve_fabrik(const lv::Vec2F& target)
	{
		lv::Vec2F initial_position = m_joints.first->data.position;

		auto* starting_point = m_joints.last->prev;
		bool backwards = true;

		for (int i = 0; i < ITERATIONS; i++)
		{
			if (backwards)
				m_joints.last->data.position = target;
			else
				m_joints.first->data.position = initial_position;

			for (auto* node = starting_point; node; node = (backwards) ? node->prev : node->next)
			{
				const auto* next_n = node->next;

				if (!next_n)
					break;

				const Joint& prev = next_n->data;
				Joint& joint = node->data;

				auto angle = (joint.position - prev.position).angle();
				joint.position = prev.position + lv::Vec2F::from_angle(angle, joint.length);
			}

			backwards = !backwards;
			starting_point = (backwards) ? m_joints.last->prev : m_joints.first;
		}
	}

	const lv::LinkedList<Joint>& joints() const { return m_joints; }

private:
	lv::LinkedList<Joint> m_joints;
};

Skeleton g_skelly;
lv::SpriteBatch g_batch;

static void init()
{
	for (int i = 0; i < 50; i++)
		g_skelly.add_joint(lv::Vec2F(20.0f + i * 40.0f, 60.0f));
}

static void update()
{
	g_skelly.solve_fabrik(lv::Input::inst()->mouse_position());
}

static void render()
{
	lv::App::clear(0x101013FF);
	{
		for (auto* node = g_skelly.joints().first; node; node = node->next)
		{
			const auto& joint = node->data;

			if (node->next)
				g_batch.push_line(lv::Line(joint.position, node->next->data.position), 2.0f, lv::Colour::blue());

			g_batch.push_circle(lv::Circle(joint.position, 5.0f), 10U, lv::Colour::cyan());
		}
	}
	g_batch.render();
}

int main(int argc, char** argv)
{
	lv::Config conf;
	{
		conf.name = "yes";
		conf.width = 1280;
		conf.height = 720;
		conf.resizable = false;
		conf.target_fps = 60;
		conf.vsync = true;
		conf.max_updates = 5;
		conf.cursor_visible = true;
		conf.on_init = init;
		conf.on_update = update;
		conf.on_render = render;
	}
	lv::App::inst()->start(conf);

	return 0;
}
