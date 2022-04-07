#include <lev/core/app.h>
#include <lev/assets/asset_mgr.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/input/input.h>
#include <lev/entity/entity.h>

using namespace lev;

// todo: breakout level loader

/*
class Player : public Node2D
{
public:
    Player()
        : Node2D()
    {
        transform.origin(12.5f, 0);

        // collider
        {
            m_collider = add<Collider2D>(RectF(25, 5));
        }

        // sprite
        {
            TextureRegion paddle_tex = TextureRegion(AssetMgr::inst()->get<Texture>("breakout"), RectI(0, 0, 25, 5));
            m_sprite = add<Sprite2D>(paddle_tex);
        }
    }

    void update() override
    {
        Node2D::update();

        m_collider->collide_all([&](Collision2D hit) -> void {
        });

        if (Input::inst()->down_key(KEY_LEFT))
            transform.move(-1, 0);

        if (Input::inst()->down_key(KEY_RIGHT))
            transform.move(1, 0);
    }

private:
    Collider2D* m_collider;
    Sprite2D* m_sprite;
};
*/

void initialize_scene()
{
    auto player_entity = EntityMgr::inst()->add_entity();

    /*
    auto player_node = scene->add<Player>();
    {
        player_node->transform.position(320 / 2, 180 - 25);
    }
     */
}

int main()
{
    SpriteBatch batch;

    Config conf;
    {
        conf.on_init = [&]() -> void
        {
            // load assets
            AssetMgr::inst()->load<Texture>("breakout", "D:\\_PROJECTS\\leviathan\\testing\\res\\textures\\breakout_sheet.png");

            // create scene
            initialize_scene();
        };

        conf.on_update = [&]() -> void
        {
        };

        conf.on_render = [&]() -> void
        {
            App::inst()->clear();
            batch.push_matrix(Mat3x2::create_scale(4.0f));

            batch.pop_matrix();
            batch.render();
        };
    }

    App::inst()->start(conf);

    return 0;
}
