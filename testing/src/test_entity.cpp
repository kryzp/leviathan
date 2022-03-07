
#include <leviathan.h>
#include <iostream>

// yes i know im using too much OOP and i could just use functions instead of singletons just let me have this ok doing ::inst() is fun >:(
// by no means is this *actually* a good entity system, it's just thrown together to demonstrate what it might look like
// ive made lots of shortcuts and horrible horribleness so do not take this for good code

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

	virtual void update() { }

	bool operator == (const Entity& other) const;
	bool operator != (const Entity& other) const;

	u32 id;
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

bool Entity::operator == (const Entity& other) const { return this->id == other.id; }
bool Entity::operator != (const Entity& other) const { return !(*this == other); }

class Player : public Entity
{
public:
	Player()
		: Entity()
	{
		transform.origin(128, 128);

		physics = new Physics();
		physics->velocity = Vec2::ZERO;
		physics->angular_velocity = 0.0f;
		physics->collider = RectF(0, 0, 256, 256);
		physics->collider.parent = &transform;

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

	void update() override
	{
		if (Input::down(Key::W)) { physics->velocity = Vec2( 0.0f, -1.0f); } // up
		if (Input::down(Key::S)) { physics->velocity = Vec2( 0.0f,  1.0f); } // down
		if (Input::down(Key::A)) { physics->velocity = Vec2(-1.0f,  0.0f); } // left
		if (Input::down(Key::D)) { physics->velocity = Vec2( 1.0f,  0.0f); } // right
	}
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
		physics->collider.parent = &transform;

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
	for (auto& entity1 : entities)
	{
		if (!entity1->physics)
			continue;

		entity1->transform.move(entity1->physics->velocity);
		entity1->transform.rotate(entity1->physics->angular_velocity);
		
		for (auto& entity2 : entities)
		{
			if (entity1 == entity2)
				continue;

			auto& c1 = entity1->physics->collider;
			auto& c2 = entity2->physics->collider;

			Vec2 pushout = Vec2::ZERO;
			bool overlaps = c1.overlaps(c2, &pushout);

			if (overlaps)
				entity1->transform.move(-pushout);
		}
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
};

RenderSystem::RenderSystem()
{
	m_batch.initialize();
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
		block->id = 1;
		block->transform.position(350, 0);
	}
	g_entities.push_back(block);

	auto player = new Player();
	{
		block->id = 2;
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
	for (auto& entity : g_entities)
		entity->update();

	PhysicsSystem::inst().update(g_entities);
}

void render()
{
	App::clear(Colour(0, 190, 170));

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
