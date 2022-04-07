#include <lev/core/app.h>
#include <lev/assets/asset_mgr.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/texture.h>
#include <lev/math/mat4x4.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using namespace lev;

class Camera
{
public:
    Mat4x4 view_matrix() const
    {
        return Mat4x4(
            0.718762, 0.615033, -0.324214, 0,
            -0.393732, 0.744416, 0.539277, 0,
            0.573024, -0.259959, 0.777216, 0,
            0.526967, 1.254234, -2.53215, 1
        );
    }

    Mat4x4 projection_matrix() const
    {
        return Mat4x4::create_projection(90.0f, 10.0f, 0.01f, 100.0f);
    }
};

class Raytracer
{
public:
    void load(Ref<Texture>& target)
    {
        m_target = target;

        m_raytracing_shader = AssetMgr::inst()->load<Shader>("raytracer", ShaderLoadData(
            "D:\\_PROJECTS\\leviathan\\testing\\res\\shaders\\raytrace.comp"
        ));
    }

    void update(const Camera& camera)
    {
        m_target->bind(0);

        u32 groupsx = (u32)Calc::ceil(WINDOW_WIDTH / 8.0f);
        u32 groupsy = (u32)Calc::ceil(WINDOW_HEIGHT / 8.0f);

        m_raytracing_shader->use()
            .set("u_camera_to_world", camera.view_matrix())
            .set("u_camera_inverse_projection", camera.projection_matrix().inverse())
            .dispatch_compute(
                groupsx,
                groupsy,
                1)
            .wait_compute();
    }

    Ref<Texture> target() { return m_target; }

private:
    Ref<Texture> m_target;
    Ref<Shader> m_raytracing_shader;
};

int main()
{
    SpriteBatch batcher;
    Raytracer raytracer;
    Camera camera;

    Config conf;
    {
        conf.width = WINDOW_WIDTH;
        conf.height = WINDOW_HEIGHT;
        conf.target_fps = 60;
        conf.name = "raytrace";

        conf.on_init = [&]()
        {
            auto target = Texture::create(
                WINDOW_WIDTH, WINDOW_HEIGHT,
                TEXTURE_FORMAT_RGBA,
                TEXTURE_TYPE_UNSIGNED_BYTE,
                nullptr
            );

            raytracer.load(target);
        };

        conf.on_update = [&]()
        {
            raytracer.update(camera);
        };

        conf.on_render = [&]()
        {
            App::inst()->clear();
            batcher.push_texture(raytracer.target());
            batcher.render();
        };
    }
    App::inst()->start(conf);

    return 0;
}
