#pragma once

#include <lev/core/util.h>

namespace Lev
{
	enum class UniformType
	{
        Float,
        Integer,
        Vector2,
        Vector3,
        Vector4,
        Max
	};

	struct ShaderData
	{
		const char* vertex_source;
		const char* fragment_source;

		// stuff like uniforms, source data, etc...
	};

	class Shader
	{
	public:
		Shader();
		Shader(const char* vertex, const char* fragment);
		~Shader();

		void load(const char* vertex, const char* fragment);
		void use() const;
		void free() const;

		void set(const char* name, bool value) const;
		void set(const char* name, int value) const;
		void set(const char* name, float value) const;

	private:
		u32 m_id;
	};
}
