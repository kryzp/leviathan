#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/math/vec2.h>
#include <lev/math/vec3.h>
#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>
#include <lev/graphics/texture.h>

#include <string>

#define LEV_SHADER_MAX_SIZE LEV_KILOBYTES(5)

namespace lev
{
	enum ShaderType
	{
		SHADER_TYPE_FRAGMENT	= 1 << 0,
		SHADER_TYPE_VERTEX		= 1 << 1,
		SHADER_TYPE_GEOMETRY	= 1 << 2,
		SHADER_TYPE_COMPUTE		= 1 << 3,
		SHADER_TYPE_MAX
	};

	struct ShaderData
	{
		u8 type;
		std::string vertex_source;
		std::string fragment_source;
		std::string geometry_source;
		std::string compute_source;
	};

	class ShaderBuffer
	{
	public:
		ShaderBuffer() = default;
		virtual ~ShaderBuffer() = default;

		static Ref<ShaderBuffer> create(u64 size);

		static void unbind();

		virtual void bind(int idx) = 0;
		virtual void set(void* buf) = 0;
		virtual void release() = 0;
		virtual u64 size() const = 0;
	};

	class Shader
	{
	public:
		static constexpr const char* PROJECTION = "u_projection";
		static constexpr const char* RESOLUTION = "u_resolution";

		Shader();
		virtual ~Shader();

		static Ref<Shader> create(const char* vertex, const char* fragment, const char* geometry = nullptr, bool is_source = false);
		static Ref<Shader> create_compute(const char* compute, bool is_source = false);

		static void unbind();

		virtual u8 type() = 0;
		virtual Shader& use() = 0;

		virtual Shader& dispatch_compute(u32 n_groups_x, u32 n_groups_y, u32 n_groups_z) = 0;
		virtual Shader& dispatch_compute(s64 indirect) = 0;
		virtual Shader& wait_compute() = 0;

		virtual Shader& set_buffer(const Ref<ShaderBuffer>& buf, int binding) = 0;

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
