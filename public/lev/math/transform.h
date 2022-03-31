#pragma once

#include <lev/math/vec2.h>
#include <lev/math/mat3x2.h>

#include <functional>

namespace lev
{
    class Transform
    {
    public:
        Transform();
        Transform(const Transform& other);

        Transform offset(const Vec2F& amount);

        Vec2F origin() const;
        Transform& origin(const Vec2F& v);
        Transform& origin(float x, float y);

        Vec2F position() const;
        Transform& position(const Vec2F& v);
        Transform& position(float x, float y);
        Transform& move(const Vec2F& v);
        Transform& move(float x, float y);

        Vec2F scale() const;
        Transform& scale(const Vec2F& v);
        Transform& scale(float x, float y);

        float rotation() const;
        Transform& rotation(float r);
        Transform& rotate(float r);

        float rotation_rad() const;
        float rotation_deg() const;

        Mat3x2 matrix();

		std::function<void(void)> on_transformed;

    private:
        Mat3x2 m_matrix;
        Vec2F m_origin;
        Vec2F m_position;
        Vec2F m_scale;
        float m_rotation;
        bool m_dirty;
    };
}
