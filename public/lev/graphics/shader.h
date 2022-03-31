#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/math/vec2.h>
#include <lev/math/vec3.h>
#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>
#include <lev/graphics/texture.h>

#define LEV_SHADER_CHAR_SIZE LEV_KILOBYTES(5)

namespace lev
{
	struct ShaderData
	{
		char vertex_source[LEV_SHADER_CHAR_SIZE] = {0};
		char fragment_source[LEV_SHADER_CHAR_SIZE] = {0};
	};

	class Shader
	{
	public:
		static constexpr const char* PROJECTION = "u_projection";
		static constexpr const char* RESOLUTION = "u_resolution";

		Shader();
		virtual ~Shader();

		static Ref<Shader> create(const char* vertex, const char* fragment, bool is_source = false);

		virtual Shader& use() = 0;

		virtual Shader& set(const char* name, bool value)						= 0;
		virtual Shader& set(const char* name, bool* values, int count)			= 0;
		virtual Shader& set(const char* name, int value)						= 0;
		virtual Shader& set(const char* name, int* values, int count)			= 0;
		virtual Shader& set(const char* name, float value)						= 0;
		virtual Shader& set(const char* name, float* values, int count)			= 0;
		virtual Shader& set(const char* name, const Vec2I& value)				= 0;
		virtual Shader& set(const char* name, const Vec2I* values, int count)	= 0;
		virtual Shader& set(const char* name, const Vec2F& value)				= 0;
		virtual Shader& set(const char* name, const Vec2F* values, int count)	= 0;
		virtual Shader& set(const char* name, const Vec3I& value)				= 0;
		virtual Shader& set(const char* name, const Vec3I* values, int count)	= 0;
		virtual Shader& set(const char* name, const Vec3F& value)				= 0;
		virtual Shader& set(const char* name, const Vec3F* values, int count)	= 0;
		virtual Shader& set(const char* name, const Mat3x2& value)				= 0;
		virtual Shader& set(const char* name, const Mat4x4& value)				= 0;
	};
}
