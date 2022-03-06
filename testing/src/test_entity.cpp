
#include <leviathan.h>
#include <iostream>

// yes i know im using too much OOP and i could just use functions instead of singletons just let me have this ok doing ::inst() is fun >:(
// this is how i like to do entity systems nowadays, except if i were to do it properly i'd probably have some better resource management system
// pointers take up like no memory so it doesnt matter at all that theyre in the base class
// atleast compared to the amount of space graphics rendering takes up
// by no means am i actually knowledgable enough to talk about entity systems its just that i find this 'structure' to work nicest

using namespace lev;

struct Physics
{
	Vec2 velocity;
	float angular_velocity;
	phys::Collider collider;
};

struct Sprite
{
	Vec2 offset;
	gfx::TextureRegion texture;
};

class Entity
{
public:
	Entity();
	virtual ~Entity() = default;

	virtual bool is_player() { return false; }

	Transform transform;
	Physics* physics;
	Sprite* sprite;
};

Entity::Entity()
	: transform()
	, physics(nullptr)
	, sprite(nullptr)
{
}

class Player : public Entity
{
public:
	Player()
		: Entity()
	{
		transform.origin(128, 128);

		physics = new Physics();
		physics->velocity = Vec2(0.5f, 0.0f);
		physics->angular_velocity = 0.0f;
		physics->collider = RectF(0, 0, 256, 256);
		physics->collider.parent_transform = &transform;

		sprite = new Sprite();
		sprite->offset = Vec2::ZERO;
		sprite->texture = {
			.texture = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\arrow.png"),
			.source = RectI(0, 0, 256, 256)
		};
	}

	~Player()
	{
		delete physics;
		delete sprite;
	}

	bool is_player() override { return true; }
};

class Block : public Entity
{
public:
	Block()
		: Entity()
	{
		transform.origin(128, 128);

		physics = new Physics();
		physics->velocity = Vec2::ZERO;
		physics->angular_velocity = 0.0f;
		physics->collider = RectF(0, 0, 256, 256);
		physics->collider.parent_transform = &transform;

		sprite = new Sprite();
		sprite->offset = Vec2::ZERO;
		sprite->texture = {
			.texture = gfx::Texture::create("D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\p0.png"),
			.source = RectI(0, 0, 256, 256)
		};
	}

	~Block()
	{
		delete physics;
		delete sprite;
	}
};

class PhysicsSystem
{
	LEV_SINGLETON_CLASS(PhysicsSystem)

public:
	void update(const Vector<Entity*>& entities);
};

void PhysicsSystem::update(const Vector<Entity*>& entities)
{
	for (auto& entity : entities)
	{
		if (!entity->physics)
			continue;

		entity->transform.move(entity->physics->velocity);
		entity->transform.rotate(entity->physics->angular_velocity);
		
		// todo: solve collisions with collider class
	}
}

class RenderSystem
{
	LEV_SINGLETON_CLASS(RenderSystem)

public:
	RenderSystem();

	void render(const Vector<Entity*>& entities);

private:
	gfx::SpriteBatch m_batch;
	Ref<gfx::Shader> m_shader;
};

RenderSystem::RenderSystem()
	: m_batch()
{
	m_shader = gfx::Shader::create(
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\vertex.vert",
		"D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\fragment.frag"
	);

	m_shader->assign_uniform("u_projection", gfx::UniformType::MAT4X4, gfx::UniformFlags::PROJECTION);
	m_shader->assign_uniform("u_texture", gfx::UniformType::SAMPLER2D, gfx::UniformFlags::MAIN_TEXTURE);

	m_batch.push_shader(m_shader);
}

void RenderSystem::render(const Vector<Entity*>& entities)
{
	for (auto& entity : entities)
	{
		if (!entity->sprite)
			continue;

		m_batch.push_matrix(entity->transform
			.offset(entity->sprite->offset)
			.matrix());

		m_batch.render_texture(entity->sprite->texture);

		m_batch.pop_matrix();
	}

	m_batch.render(Mat4x4::create_orthographic(1280.0f, 720.0f, 0.0f, 10000.0f));
}

namespace
{
	Vector<Entity*> g_entities;
}

void init()
{
	auto block = new Block();
	{
		block->transform.position(350, 0);
	}
	g_entities.push_back(block);

	auto player = new Player();
	{
		player->transform.position(-350, 0);
	}
	g_entities.push_back(player);
}

void destroy()
{
	for (auto& entity : g_entities)
		delete entity;
}

void update()
{
	PhysicsSystem::inst().update(g_entities);
}

void render()
{
	RenderSystem::inst().render(g_entities);
}

int main(void)
{
	Log::directory("D:\\_PROJECTS\\leviathan\\testing\\logs\\");

	AppConfig config;
	config.name = "entity demo";
	config.width = 1280;
	config.height = 720;
	config.target_framerate = 60;
	config.resizable = true;
	config.on_init = init;
	config.on_destroy = destroy;
	config.on_update = update;
	config.on_render = render;

	App::start(&config);

	return 0;
}
