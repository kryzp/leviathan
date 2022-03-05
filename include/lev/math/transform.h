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

        Vec2 origin() const;
        void origin(const Vec2& v);
        void origin(float x, float y);
        void offset(const Vec2& v);

        Vec2 position_with_origin() const;

        Vec2 position() const;
        void position(const Vec2& v);
        void position(float x, float y);
        void move(const Vec2& v);
        void movex(float x);
        void movey(float y);

        Vec2 scale() const;
        void scale(const Vec2& v);
        void scale(float x, float y);

        float rotation() const;
        void rotation(float r);
        void rotate(float r);

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
