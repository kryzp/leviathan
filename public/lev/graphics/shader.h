#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/containers/string.h>
#include <lev/math/vec2.h>
#include <lev/math/vec3.h>
#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>
#include <lev/graphics/texture.h>

#define LEV_SHADER_MAX_SIZE LEV_KILOBYTES(5)

namespace lev
{
	enum ShaderType
	{
		SHADER_TYPE_NONE,
		SHADER_TYPE_SEPERATED 	= 1 << 0,
		SHADER_TYPE_SINGLE 		= 1 << 1,
		SHADER_TYPE_RENDER 		= 1 << 2,
		SHADER_TYPE_COMPUTE 	= 1 << 3,
		SHADER_TYPE_MAX
	};

	enum UniformType
	{
		SHADER_UNIFORM_NONE,
		SHADER_UNIFORM_BOOL,
		SHADER_UNIFORM_INT,
		SHADER_UNIFORM_FLOAT,
		SHADER_UNIFORM_VEC2I,
		SHADER_UNIFORM_VEC2F,
		SHADER_UNIFORM_VEC3I,
		SHADER_UNIFORM_VEC3F,
		SHADER_UNIFORM_MAT3X2,
		SHADER_UNIFORM_MAT4X4,
		SHADER_UNIFORM_SAMPLER,
		SHADER_UNIFORM_MAX
	};

	struct ShaderData
	{
		u8 type;

		union
		{
			union
			{
				struct
				{
					bool has_geometry;
					char vertex_source[LEV_SHADER_MAX_SIZE];
					char fragment_source[LEV_SHADER_MAX_SIZE];
					char geometry_source[LEV_SHADER_MAX_SIZE];
				};

				char compute_source[LEV_SHADER_MAX_SIZE * 3];
			}
			seperated;

			char single_source[LEV_SHADER_MAX_SIZE * 3];
		};
	};

	class ShaderBuffer
	{
	public:
		ShaderBuffer() = default;
		virtual ~ShaderBuffer() = default;

		static Ref<ShaderBuffer> create(u64 size);

		static void unbind();

		virtual void bind(int idx) const = 0;
		virtual void set(void* buf) = 0;
		virtual void release() = 0;
		virtual u64 size() const = 0;
	};

	class Shader : public NonCopyable, public NonMovable
	{
	public:
		static constexpr const char* PROJECTION = "lev_projection";
		static constexpr const char* RESOLUTION = "lev_resolution";

		Shader() = default;
		virtual ~Shader() = default;

		static Ref<Shader> create_single(const lev::String& file);
		static Ref<Shader> create_seperated(const lev::String& vertex, const lev::String& fragment, const lev::String& geometry = nullptr);
		static Ref<Shader> create_compute_seperated(const lev::String& compute);

		static void unbind();

		virtual u8 type() = 0;
		virtual Shader& use() = 0;

		virtual Shader& dispatch_compute(u32 n_groups_x, u32 n_groups_y, u32 n_groups_z) = 0;
		virtual Shader& dispatch_compute(s64 indirect) = 0;
		virtual Shader& wait_compute() = 0;

		virtual Shader& set_buffer(const Ref<ShaderBuffer>& buf, int binding) = 0;

		virtual Shader& set(const char* name, bool value)						= 0;
		virtual Shader& set(const char* name, int value)						= 0;
		virtual Shader& set(const char* name, float value)						= 0;
		virtual Shader& set(const char* name, const Vec2I& value)				= 0;
		virtual Shader& set(const char* name, const Vec2F& value)				= 0;
		virtual Shader& set(const char* name, const Vec3I& value)				= 0;
		virtual Shader& set(const char* name, const Vec3F& value)				= 0;
		virtual Shader& set(const char* name, const Mat3x2& value)				= 0;
		virtual Shader& set(const char* name, const Mat4x4& value)				= 0;
	};
}
