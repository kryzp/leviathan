#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/containers/string.h>
#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>

namespace Lev
{
	// informs the backend on if it should use the uniforms in a specific way
	// e.g: flag what uniform should be set by the sprite batch when rendering a texture
	// e.g: flag what unifrom should be used as the projection matrix
	// not sure if this is the best way to go about doing things buuuutttttt...

	enum class UniformFlags
	{
		None,
		Projection,
		MainTexture
	};

	enum class UniformType
	{
        Integer,
        Float,
        Vector2,
        Vector3,
        Vector4,
		Mat3x2,
		Mat4x4,
		Sampler2D
	};

	struct UniformData
	{
		String name;
		UniformType type;
		UniformFlags flags;
	};

	struct ShaderData
	{
		Str<512> vertex_source;
		Str<512> fragment_source;
		Vector<UniformData> uniforms;
	};

	class Shader
	{
	public:
		Shader();
		virtual ~Shader();

		virtual void use() const = 0;
		virtual const ShaderData& data() const = 0;

		virtual void set(const char* name, bool value) const = 0;
		virtual void set(const char* name, int value) const = 0;
		virtual void set(const char* name, float value) const = 0;
		virtual void set(const char* name, const Mat3x2& value) const = 0;
		virtual void set(const char* name, const Mat4x4& value) const = 0;

		virtual void assign_uniform(const char* name, UniformType type, UniformFlags flags) = 0;
		
		const UniformData& get_uniform_data(UniformFlags flags) const;

		static Ref<Shader> create(const char* vertex, const char* fragment);
	};
}
