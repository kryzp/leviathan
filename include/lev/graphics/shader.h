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
        Vector4
	};

	struct ShaderData
	{
		char vertex_source[512]; // todo: string class?
		char fragment_source[512];

		// stuff like uniforms, source data, etc...
	};

	class Shader
	{
	public:
		Shader(u32 id);
		~Shader();

		void use() const;
		void free() const;
		u32 id() const;

		void set(const char* name, bool value) const;
		void set(const char* name, int value) const;
		void set(const char* name, float value) const;

		static Ref<Shader> create(const char* vertex, const char* fragment);

	private:
		u32 m_id;
	};
}
