#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/containers/string.h>
#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>

// oh mama oh no 5kb of data!!!!
#define LEV_SHADER_CHAR_SIZE LEV_KILOBYTES(5)

namespace lev::gfx
{
	enum class UniformType
	{
        INTEGER,
        FLOAT,
        VECTOR2,
        VECTOR3,
        VECTOR4,
		MAT3X2,
		MAT4X4,
		SAMPLER2D
	};

	struct UniformData
	{
		String name;
		UniformType type;
	};

	struct ShaderData
	{
		char vertex_source[LEV_SHADER_CHAR_SIZE] = {0};
		char fragment_source[LEV_SHADER_CHAR_SIZE] = {0};
	};

	class Shader
	{
	public:
		static constexpr const char* PROJECTION = "u_projection";
		static constexpr const char* MAIN_TEXTURE = "u_texture";

		Shader();
		virtual ~Shader();

		static Ref<Shader> create(const char* vertex, const char* fragment, bool is_source = false);

		virtual void use() const = 0;

		virtual void set(const char* name, bool value) const = 0;
		virtual void set(const char* name, int value) const = 0;
		virtual void set(const char* name, float value) const = 0;
		virtual void set(const char* name, const Mat3x2& value) const = 0;
		virtual void set(const char* name, const Mat4x4& value) const = 0;
	};
}
