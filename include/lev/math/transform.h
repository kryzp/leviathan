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

        Transform offset(const Vec2& amount);

        Vec2 origin() const;
        Transform& origin(const Vec2& v);
        Transform& origin(float x, float y);

        Vec2 position() const;
        Transform& position(const Vec2& v);
        Transform& position(float x, float y);
        Transform& move(const Vec2& v);
        Transform& move(float x, float y);

        Vec2 scale() const;
        Transform& scale(const Vec2& v);
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
        Vec2 m_origin;
        Vec2 m_position;
        Vec2 m_scale;
        float m_rotation;
        bool m_dirty;
    };
}
