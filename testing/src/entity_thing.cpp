
/*
 * Trying out a way I saw to create entities
 * Kinda like it
 */

#include <lev/core/app.h>
#include <lev/assets/asset_mgr.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/texture.h>
#include <lev/containers/vector.h>
#include <lev/math/vec2.h>

using namespace lv;

struct Transform { Vec2F position;       };
struct Physics   { Vec2F velocity;       };
struct Graphics  { Ref<Texture> texture; };

class Entity
{
public:
	struct Resources
	{
		Physics* physics = nullptr;
		Graphics* graphics = nullptr;
	};

	Entity() = default;

	virtual ~Entity()
	{
		delete p_res.physics;
		delete p_res.graphics;
	}

	virtual void update() { }
	virtual void render(lv::SpriteBatch& b) { }

	Resources& resources() { return p_res; }
	const Resources& resources() const { return p_res; }

	Transform& transform() { return p_transform; }
	const Transform& transform() const { return p_transform; }

protected:
	Resources p_res;
	Transform p_transform;
};

class PhysicsMgr
{
public:
	static PhysicsMgr* inst()
	{
		static PhysicsMgr* instance = nullptr;
		if (!instance) { instance = new PhysicsMgr(); }
		return instance;
	}

	void update(const Vector<Entity*>& entities)
	{
		for (auto& entity : entities)
		{
			auto& transform = entity->transform();
			const auto& physics = entity->resources().physics;

			if (physics)
			{
				transform.position += physics->velocity;
			}
		}
	}
};

class RenderMgr
{
public:
	static RenderMgr* inst()
	{
		static RenderMgr* instance = nullptr;
		if (!instance) { instance = new RenderMgr(); }
		return instance;
	}

	void update(SpriteBatch& b, const Vector<Entity*>& entities)
	{
		for (auto& entity : entities)
		{
			const auto& transform = entity->transform();
			const auto& graphics = entity->resources().graphics;

			if (graphics)
			{
				b.push_matrix(Mat3x2::create_translation(transform.position));
				b.push_texture(graphics->texture);
				b.pop_matrix();
			}
		}

		b.render();
	}
};

class World
{
public:
	World() = default;

	~World()
	{
		for (auto& entity : m_entities)
			delete entity;
	}

	Entity* add_entity()
	{
		Entity* result = new Entity();
		m_entities.push_back(result);
		return result;
	}

	void update()
	{
		PhysicsMgr::inst()->update(m_entities);
	}

	void render(SpriteBatch& b)
	{
		RenderMgr::inst()->update(b, m_entities);
	}

private:
	Vector<Entity*> m_entities;
};

SpriteBatch g_batch;
World g_world;
AssetMgr g_assets;

static void init()
{
	auto entity = g_world.add_entity();
	entity->transform().position = Vec2F::zero();

	auto* p = new Physics();
	{
		p->velocity = Vec2F(1.0f, 2.0f);
	}
	entity->resources().physics = p;

	auto* g = new Graphics();
	{
		g->texture = g_assets.load<Texture>("res\\textures\\p0.png");
	}
	entity->resources().graphics = g;
}

static void lv_exit()
{
	log::print("exiting...");
}

static void update()
{
	g_world.update();
}

static void render()
{
	App::clear(0x101013FF);
	g_world.render(g_batch);
}

int main(int argc, char** argv)
{
	Config conf;
	{
		conf.name = "entit y";
		conf.width = 1280;
		conf.height = 720;
		conf.resizable = false;
		conf.target_fps = 60;
		conf.vsync = true;
		conf.max_updates = 5;
		conf.cursor_visible = true;
		conf.on_init = init;
		conf.on_exit = lv_exit;
		conf.on_update = update;
		conf.on_render = render;
	}
	App::inst()->start(conf);

	return 0;
}
