#pragma once

#include <lev/math/vec2.h>
#include <lev/math/mat3x2.h>

#include <functional>

namespace lev
{
    class Transform2D
    {
    public:
        Transform2D();
        Transform2D(const Transform2D& other);

        Transform2D offset(const Vec2F& amount);

        Vec2F origin() const;
        Transform2D& origin(const Vec2F& v);
        Transform2D& origin(float x, float y);

        Vec2F position() const;
        Transform2D& position(const Vec2F& v);
        Transform2D& position(float x, float y);
        Transform2D& move(const Vec2F& v);
        Transform2D& move(float x, float y);

        Vec2F scale() const;
        Transform2D& scale(const Vec2F& v);
        Transform2D& scale(float x, float y);

        float rotation() const;
        Transform2D& rotation(float r);
        Transform2D& rotate(float r);

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
