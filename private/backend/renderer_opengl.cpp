#if LEV_USE_OPENGL

#include <lev/core/app.h>
#include <lev/containers/hash_map.h>

#include <backend/renderer.h>
#include <backend/system.h>

#include <third_party/glad/glad.h>

#include <string>
#include <stdlib.h>

using namespace lev;

static u64 gen_gl_vertex_attribs()
{
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	return sizeof(Vertex);
}

static u32 get_gl_texture_fmt(u8 fmt)
{
	switch (fmt)
	{
		case TEX_FMT_RED:			return GL_RED;
		case TEX_FMT_RG:			return GL_RG;
		case TEX_FMT_RGB:			return GL_RGB;
		case TEX_FMT_RGBA:			return GL_RGBA;
		case TEX_FMT_DEPTH_STENCIL:	return GL_DEPTH_STENCIL;
	}

	return 0;
}

static u32 get_gl_texture_internal_fmt(u8 fmt)
{
	switch (fmt)
	{
		case TEX_I_FMT_R8: 					return GL_R8;
		case TEX_I_FMT_R8_SNORM: 			return GL_R8_SNORM;
		case TEX_I_FMT_R16: 				return GL_R16;
		case TEX_I_FMT_R16_SNORM: 			return GL_R16_SNORM;
		case TEX_I_FMT_RG8: 				return GL_RG8;
		case TEX_I_FMT_RG8_SNORM: 			return GL_RG8_SNORM;
		case TEX_I_FMT_RG16: 				return GL_RG16;
		case TEX_I_FMT_RG16_SNORM: 			return GL_RG16_SNORM;
		case TEX_I_FMT_R3_G3_B2: 			return GL_R3_G3_B2;
		case TEX_I_FMT_RGB4: 				return GL_RGB4;
		case TEX_I_FMT_RGB5: 				return GL_RGB5;
		case TEX_I_FMT_RGB8: 				return GL_RGB8;
		case TEX_I_FMT_RGB8_SNORM: 			return GL_RGB8_SNORM;
		case TEX_I_FMT_RGB10: 				return GL_RGB10;
		case TEX_I_FMT_RGB12: 				return GL_RGB12;
		case TEX_I_FMT_RGB16_SNORM: 		return GL_RGB16_SNORM;
		case TEX_I_FMT_RGBA2: 				return GL_RGBA2;
		case TEX_I_FMT_RGBA4: 				return GL_RGBA4;
		case TEX_I_FMT_RGB5_A1: 			return GL_RGB5_A1;
		case TEX_I_FMT_RGBA8: 				return GL_RGBA8;
		case TEX_I_FMT_RGBA8_SNORM: 		return GL_RGBA8_SNORM;
		case TEX_I_FMT_RGB10_A2: 			return GL_RGB10_A2;
		case TEX_I_FMT_RGB10_A2UI: 			return GL_RGB10_A2UI;
		case TEX_I_FMT_RGBA12: 				return GL_RGBA12;
		case TEX_I_FMT_RGBA16: 				return GL_RGBA16;
		case TEX_I_FMT_SRGB8: 				return GL_SRGB8;
		case TEX_I_FMT_SRGB8_ALPHA8: 		return GL_SRGB8_ALPHA8;
		case TEX_I_FMT_R16F: 				return GL_R16F;
		case TEX_I_FMT_RG16F: 				return GL_RG16F;
		case TEX_I_FMT_RGB16F: 				return GL_RGB16F;
		case TEX_I_FMT_RGBA16F: 			return GL_RGBA16F;
		case TEX_I_FMT_R32F: 				return GL_R32F;
		case TEX_I_FMT_RG32F: 				return GL_RG32F;
		case TEX_I_FMT_RGB32F: 				return GL_RGB32F;
		case TEX_I_FMT_RGBA32F: 			return GL_RGBA32F;
		case TEX_I_FMT_R11F_G11F_B10F: 		return GL_R11F_G11F_B10F;
		case TEX_I_FMT_RGB9_E5: 			return GL_RGB9_E5;
		case TEX_I_FMT_R8I: 				return GL_R8I;
		case TEX_I_FMT_R8UI: 				return GL_R8UI;
		case TEX_I_FMT_R16I: 				return GL_R16I;
		case TEX_I_FMT_R16UI: 				return GL_R16UI;
		case TEX_I_FMT_R32I: 				return GL_R32I;
		case TEX_I_FMT_R32UI: 				return GL_R32UI;
		case TEX_I_FMT_RG8I: 				return GL_RG8I;
		case TEX_I_FMT_RG8UI: 				return GL_RG8UI;
		case TEX_I_FMT_RG16I: 				return GL_RG16I;
		case TEX_I_FMT_RG16UI: 				return GL_RG16UI;
		case TEX_I_FMT_RG32I: 				return GL_RG32I;
		case TEX_I_FMT_RG32UI: 				return GL_RG32UI;
		case TEX_I_FMT_RGB8I: 				return GL_RGB8I;
		case TEX_I_FMT_RGB8UI: 				return GL_RGB8UI;
		case TEX_I_FMT_RGB16I: 				return GL_RGB16I;
		case TEX_I_FMT_RGB16UI: 			return GL_RGB16UI;
		case TEX_I_FMT_RGB32I: 				return GL_RGB32I;
		case TEX_I_FMT_RGB32UI: 			return GL_RGB32UI;
		case TEX_I_FMT_RGBA8I: 				return GL_RGBA8I;
		case TEX_I_FMT_RGBA8UI: 			return GL_RGBA8UI;
		case TEX_I_FMT_RGBA16I: 			return GL_RGBA16I;
		case TEX_I_FMT_RGBA16UI: 			return GL_RGBA16UI;
		case TEX_I_FMT_RGBA32I: 			return GL_RGBA32I;
		case TEX_I_FMT_RGBA32UI: 			return GL_RGBA32UI;
	}

	return 0;
}

static u32 get_gl_texture_type(u8 type)
{
	switch (type)
	{
		case TEX_TYPE_UNSIGNED_BYTE:	    return GL_UNSIGNED_BYTE;
		case TEX_TYPE_BYTE:                 return GL_BYTE;
		case TEX_TYPE_UNSIGNED_SHORT:       return GL_UNSIGNED_SHORT;
		case TEX_TYPE_UNSIGNED_INT:         return GL_UNSIGNED_INT;
		case TEX_TYPE_UNSIGNED_INT_24_8:    return GL_UNSIGNED_INT_24_8;
		case TEX_TYPE_INT:                  return GL_INT;
		case TEX_TYPE_HALF_FLOAT:           return GL_HALF_FLOAT;
		case TEX_TYPE_FLOAT:			    return GL_FLOAT;
	}

	return 0;
}

static u32 get_gl_blend_equation(u8 func)
{
	switch (func)
	{
		case BLEND_EQUATION_ADD:				return GL_FUNC_ADD;
		case BLEND_EQUATION_SUBTRACT:			return GL_FUNC_SUBTRACT;
		case BLEND_EQUATION_REVERSE_SUBTRACT:	return GL_FUNC_REVERSE_SUBTRACT;
		case BLEND_EQUATION_MIN:				return GL_MIN;
		case BLEND_EQUATION_MAX:				return GL_MAX;
	}

	return 0;
}

static u32 get_gl_blend_func(u8 func)
{
	switch (func)
	{
		case BLEND_FUNC_ZERO:						return GL_ZERO;
		case BLEND_FUNC_ONE:						return GL_ONE;
		case BLEND_FUNC_SRC_COLOUR:					return GL_SRC_COLOR;
		case BLEND_FUNC_SRC1_COLOUR:				return GL_SRC1_COLOR;
		case BLEND_FUNC_ONE_MINUS_SRC_COLOUR:		return GL_ONE_MINUS_SRC_COLOR;
		case BLEND_FUNC_ONE_MINUS_SRC1_COLOUR:		return GL_ONE_MINUS_SRC1_COLOR;
		case BLEND_FUNC_DST_COLOUR:					return GL_DST_COLOR;
		case BLEND_FUNC_ONE_MINUS_DST_COLOUR:		return GL_ONE_MINUS_DST_COLOR;
		case BLEND_FUNC_SRC_ALPHA:					return GL_SRC_ALPHA;
		case BLEND_FUNC_SRC1_ALPHA:					return GL_SRC1_ALPHA;
		case BLEND_FUNC_ONE_MINUS_SRC_ALPHA:		return GL_ONE_MINUS_SRC_ALPHA;
		case BLEND_FUNC_ONE_MINUS_SRC1_ALPHA:		return GL_ONE_MINUS_SRC1_ALPHA;
		case BLEND_FUNC_DST_ALPHA:					return GL_DST_ALPHA;
		case BLEND_FUNC_ONE_MINUS_DST_ALPHA:		return GL_ONE_MINUS_DST_ALPHA;
		case BLEND_FUNC_CONSTANT_COLOUR:			return GL_CONSTANT_COLOR;
		case BLEND_FUNC_ONE_MINUS_CONSTANT_COLOUR:	return GL_ONE_MINUS_CONSTANT_COLOR;
		case BLEND_FUNC_CONSTANT_ALPHA:				return GL_CONSTANT_ALPHA;
		case BLEND_FUNC_ONE_MINUS_CONSTANT_ALPHA:	return GL_ONE_MINUS_CONSTANT_ALPHA;
	}

	return 0;
}

static u32 get_gl_compare_face(u8 face)
{
	switch (face)
	{
		case FACE_FRONT:			return GL_FRONT;
		case FACE_BACK:				return GL_BACK;
		case FACE_FRONT_AND_BACK: 	return GL_FRONT_AND_BACK;
	}

	return 0;
}

static u32 get_gl_compare_func(u8 func)
{
	switch (func)
	{
		case COMPARE_NEVER:		return GL_NEVER;
		case COMPARE_LESS:		return GL_LESS;
		case COMPARE_LEQUAL:	return GL_LEQUAL;
		case COMPARE_GREATER:	return GL_GREATER;
		case COMPARE_GEQUAL:	return GL_GEQUAL;
		case COMPARE_EQUAL:		return GL_EQUAL;
		case COMPARE_NOTEQUAL:	return GL_NOTEQUAL;
		case COMPARE_ALWAYS:	return GL_ALWAYS;
	}

	return 0;
}

static u32 get_gl_compare_fail(u8 fail)
{
	switch (fail)
	{
		case COMPARE_FAIL_KEEP: 		return GL_KEEP;
		case COMPARE_FAIL_ZERO: 		return GL_ZERO;
		case COMPARE_FAIL_REPLACE: 		return GL_REPLACE;
		case COMPARE_FAIL_INCR: 		return GL_INCR;
		case COMPARE_FAIL_INCR_WRAP: 	return GL_INCR_WRAP;
		case COMPARE_FAIL_DECR: 		return GL_DECR;
		case COMPARE_FAIL_DECR_WRAP: 	return GL_DECR_WRAP;
		case COMPARE_FAIL_INVERT: 		return GL_INVERT;
	}

	return 0;
}

/*********************************************************/
/* TEXTURE                                               */
/*********************************************************/

class OpenGLTexture : public Texture
{
	u32 m_id;

	u32 m_width;
	u32 m_height;

	u8 m_format;
	u8 m_internal_format;
	u8 m_type;

	u32 m_gl_format;
	u32 m_gl_internal_format;
	u32 m_gl_type;

public:
	OpenGLTexture(const TextureData& data)
		: Texture()
		, m_id(0)
		, m_width(data.width)
		, m_height(data.height)
		, m_format(data.format)
		, m_internal_format(data.internal_format)
		, m_type(data.type)
		, m_gl_format(get_gl_texture_fmt(m_format))
		, m_gl_internal_format(get_gl_texture_internal_fmt(m_internal_format))
		, m_gl_type(get_gl_texture_type(m_type))
	{
		glGenTextures(1, &m_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, m_gl_internal_format, m_width, m_height, 0, m_gl_format, m_gl_type, nullptr);
		update(TextureSampler::pixel()); // dont remove its needed for compute shaders lol
	}

	~OpenGLTexture() override
	{
		glDeleteTextures(1, &m_id);
	}

	void bind(int idx) const override
	{
		LEV_ASSERT(idx >= 0 && idx <= 31, "Index must be within [0, 31] inclusive");
		glActiveTexture(GL_TEXTURE0 + idx);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void bind_image(int idx) const override
	{
		LEV_ASSERT(idx >= 0 && idx <= 31, "Index must be within [0, 31] inclusive");
		glActiveTexture(GL_TEXTURE0 + idx);
		glBindImageTexture(0, m_id, 0, GL_FALSE, 0, GL_READ_WRITE, m_gl_internal_format);
	}

	void generate(const void* data) override
	{
		LEV_ASSERT(data, "Data must not be nullptr");

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			m_gl_internal_format,
			m_width, m_height,
			0,
			m_gl_format,
			m_gl_type,
			data
		);
	}

	void get_data(float* buf) override
	{
		LEV_ASSERT(buf, "Buffer must not be nullptr");

		glGetTexImage(
			GL_TEXTURE_2D,
			0,
			m_gl_format,
			m_gl_type,
			buf
		);
	}

	void update(const TextureSampler& sampler) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sampler.wrap_x == TEX_WRAP_CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT); // can we just take a moment to appreciate how nicely the spacing lines up here oh my god
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (sampler.wrap_y == TEX_WRAP_CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (sampler.filter == TEX_FILTER_LINEAR) ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (sampler.filter == TEX_FILTER_LINEAR) ? GL_LINEAR : GL_NEAREST);
	}

	void copy_to(const Ref<Texture>& other) override
	{
		OpenGLTexture* cast_other_ptr = (OpenGLTexture*)other.get();

		glCopyImageSubData(
			m_id,				  GL_TEXTURE_2D, 0, 0, 0, 0,
			cast_other_ptr->m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
			m_width, m_height, 1
		);
	}

	u32 width() const override
	{
		return m_width;
	}

	u32 height() const override
	{
		return m_height;
	}

	u8 format() const override
	{
		return m_format;
	}

	u8 internal_format() const override
	{
		return m_internal_format;
	}

	u8 texture_type() const override
	{
		return m_type;
	}

	u32 id() const
	{
		return m_id;
	}
};

// todo: could this be merged with 'Texture'?
// lots of duplicate code

class OpenGLArrayTexture : public ArrayTexture
{
	u32 m_id;

	u32 m_width;
	u32 m_height;
	u32 m_depth;

	u8 m_format;
	u8 m_internal_format;
	u8 m_type;

	u32 m_gl_format;
	u32 m_gl_internal_format;
	u32 m_gl_type;

public:
	OpenGLArrayTexture(const TextureData& data, u32 depth)
		: ArrayTexture()
		, m_id(0)
		, m_width(data.width)
		, m_height(data.height)
		, m_format(data.format)
		, m_internal_format(data.internal_format)
		, m_type(data.type)
		, m_gl_format(get_gl_texture_fmt(m_format))
		, m_gl_internal_format(get_gl_texture_internal_fmt(m_internal_format))
		, m_gl_type(get_gl_texture_type(m_type))
	{
		glGenTextures(1, &m_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

		glTexImage3D(
			GL_TEXTURE_2D_ARRAY,
			0,
			m_gl_internal_format,
			data.width, data.height,
			depth,
			0,
			m_gl_format,
			m_gl_type,
			nullptr
		);

		update(TextureSampler::pixel());
	}

	~OpenGLArrayTexture() override
	{
		glDeleteTextures(1, &m_id);
	}

	void bind(int idx) const override
	{
		glActiveTexture(GL_TEXTURE0 + idx);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
	}

	void generate(const void* data) override
	{
		LEV_ASSERT(data, "Data must not be nullptr");

		glTexImage3D(
			GL_TEXTURE_2D_ARRAY,
			0,
			m_gl_internal_format,
			m_width, m_height,
			m_depth,
			0,
			m_gl_format,
			m_gl_type,
			data
		);
	}

	void update(const TextureSampler& sampler) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sampler.wrap_x == TEX_WRAP_CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT); // can we just take a moment to appreciate how nicely the spacing lines up here oh my god
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (sampler.wrap_y == TEX_WRAP_CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (sampler.filter == TEX_FILTER_LINEAR) ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (sampler.filter == TEX_FILTER_LINEAR) ? GL_LINEAR : GL_NEAREST);
	}

	u32 width() const override
	{
		return m_width;
	}

	u32 height() const override
	{
		return m_height;
	}

	u32 image_count() const override
	{
		return m_depth;
	}

	u8 format() const override
	{
		return m_format;
	}

	u8 internal_format() const override
	{
		return m_internal_format;
	}

	u8 texture_type() const override
	{
		return m_type;
	}

	u32 id() const
	{
		return m_id;
	}
};

/*********************************************************/
/* SHADER                                                */
/*********************************************************/

class OpenGLShaderBuffer : public ShaderBuffer
{
	u32 m_id;
	u64 m_size;

public:
	OpenGLShaderBuffer(u64 size)
		: ShaderBuffer()
		, m_id(0)
		, m_size(size)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_COPY);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	~OpenGLShaderBuffer() override
	{
		glDeleteBuffers(1, &m_id);
	}

	void bind(int idx) const override
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, idx, m_id);
	}

	void set(void* buf) override
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
		void* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		mem::copy(p, buf, m_size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	void release() override
	{
		glDeleteBuffers(1, &m_id);
	}

	u64 size() const override
	{
		return m_size;
	}

	u32 id() const
	{
		return m_id;
	}
};

class OpenGLShader : public Shader
{
	GLuint m_id;
	u8 m_type;
	HashMap<String, GLint> m_gl_uniforms;

	GLint get_uniform_location(const String& name)
	{
		if (!m_gl_uniforms.contains(name))
			m_gl_uniforms.insert(name, glGetUniformLocation(m_id, name.c_str()));
		return m_gl_uniforms.at(name);
	}

public:
	OpenGLShader(const ShaderData& data)
		: Shader()
		, m_id(0)
		, m_type(data.type)
	{
		/*
		 * welcome to hell st. 5th avenue
		 */

		int success;
		char infolog[512];

		GLuint id = glCreateProgram();

		if (data.type & SHADER_TYPE_SINGLE)
		{
			LEV_ERROR("OpenGL backend doesn't support shaders in a single file");
			return;
		}

		auto& compute_source = data.seperated.compute_source;

		if (data.type & SHADER_TYPE_COMPUTE)
		{
			const char* compute_data = compute_source;

			GLuint compute = glCreateShader(GL_COMPUTE_SHADER);
			glShaderSource(compute, 1, &compute_data, nullptr);
			glCompileShader(compute);
			glAttachShader(id, compute);

			glLinkProgram(id);
			
			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(id, 512, nullptr, infolog);
				log::error("failed to link compute shader program: %s", infolog);
			}

			glDeleteShader(compute);
		}
		else
		{
			const char* vertex_data = data.seperated.vertex_source;
			const char* fragment_data = data.seperated.fragment_source;
			const char* geometry_data = data.seperated.geometry_source;

			GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertex_data, nullptr);
			glCompileShader(vertex);

			glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertex, 512, nullptr, infolog);
				log::error("failed to compile vertex shader: %s", infolog);
			}

			GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragment_data, nullptr);
			glCompileShader(fragment);

			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragment, 512, nullptr, infolog);
				log::error("failed to compile fragment shader: %s", infolog);
			}

			GLuint geometry = 0;
			if (data.seperated.has_geometry)
			{
				geometry = glCreateShader(GL_GEOMETRY_SHADER);

				glShaderSource(geometry, 1, &geometry_data, nullptr);
				glCompileShader(geometry);

				glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(geometry, 512, nullptr, infolog);
					log::error("failed to compile geometry shader: %s", infolog);
				}
			}

			glAttachShader(id, vertex);
			glAttachShader(id, fragment);

			if (data.seperated.has_geometry)
				glAttachShader(id, geometry);

			glLinkProgram(id);

			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(id, 512, nullptr, infolog);
				log::error("failed to link rendering shader program: %s", infolog);
			}

			glDeleteShader(vertex);
			glDeleteShader(fragment);

			if (data.seperated.has_geometry)
				glDeleteShader(geometry);
		}

		m_id = id;
	}

	~OpenGLShader() override
	{
		glDeleteProgram(m_id);
	}

	Shader& dispatch_compute(u32 n_groups_x, u32 n_groups_y, u32 n_groups_z) override
	{
		LEV_ASSERT(n_groups_x != 0 && n_groups_y != 0 && n_groups_z != 0, "Inputs must not be 0");
		glDispatchCompute(n_groups_x, n_groups_y, n_groups_z);
		return *this;
	}

	Shader& dispatch_compute(s64 indirect) override
	{
		LEV_ASSERT(indirect != 0, "Input must not be 0");
		glDispatchComputeIndirect(indirect);
		return *this;
	}

	Shader& wait_compute() override
	{
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		return *this;
	}

	Shader& use() override
	{
		glUseProgram(m_id);
		return *this;
	}

	Shader& set_buffer(const Ref<ShaderBuffer>& buf, int binding) override
	{
		buf->bind(binding);
		return *this;
	}

	u8 type() override
	{
		return m_type;
	}

	u32 id() const
	{
		return m_id;
	}

	Shader& set(const char* name, bool value) override
	{
		glUniform1i(get_uniform_location(name), value);
		return *this;
	}

	Shader& set(const char* name, int value) override
	{
		glUniform1i(get_uniform_location(name), value);
		return *this;
	}

	Shader& set(const char* name, float value) override
	{
		glUniform1f(get_uniform_location(name), value);
		return *this;
	}

	Shader& set(const char* name, const Vec2I& value) override
	{
		glUniform2i(get_uniform_location(name), value.x, value.y);
		return *this;
	}

	Shader& set(const char* name, const Vec2F& value) override
	{
		glUniform2f(get_uniform_location(name), value.x, value.y);
		return *this;
	}

	Shader& set(const char* name, const Vec3I& value) override
	{
		glUniform3i(get_uniform_location(name), value.x, value.y, value.z);
		return *this;
	}

	Shader& set(const char* name, const Vec3F& value) override
	{
		glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
		return *this;
	}

	Shader& set(const char* name, const Mat3x2& value) override
	{
		glUniformMatrix3x2fv(get_uniform_location(name), 1, GL_FALSE, value.value_ptr());
		return *this;
	}

	Shader& set(const char* name, const Mat4x4& value) override
	{
		glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, value.value_ptr());
		return *this;
	}
};

/*********************************************************/
/* FRAMEBUFFER                                           */
/*********************************************************/

class OpenGLFramebuffer : public Framebuffer
{
	u32 m_id;
	
	Vector<Ref<Texture>> m_attachments;
	Vector<GLenum> m_gl_attachments;

	int m_width;
	int m_height;

public:
	OpenGLFramebuffer(const FramebufferData& data)
		: Framebuffer()
		, m_id(0)
		, m_width(data.width)
		, m_height(data.height)
	{
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		for (int i = 0; i < data.attachments.size(); i++)
		{
			auto& [texture_data, texture_sampler] = data.attachments[i];

			auto tex = Texture::create(
				m_width, m_height,
				texture_data.format,
				texture_data.internal_format,
				texture_data.type,
				nullptr
			);

			auto gltex = (OpenGLTexture*)tex.get();
			gltex->update(texture_sampler);

			if (texture_data.format == TEX_FMT_DEPTH_STENCIL)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gltex->id(), 0);
				m_gl_attachments.push_back(GL_DEPTH_STENCIL_ATTACHMENT);
			}
			else
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gltex->id(), 0);
				m_gl_attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
			}

			m_attachments.push_back(tex);
		}
	}

	~OpenGLFramebuffer() override
	{
		glDeleteFramebuffers(1, &m_id);
	}

	void bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void clear(const Colour& colour = Colour::empty()) override
	{
		bind();

		glViewport(0, 0, m_width, m_height);

		glClearColor(
			(float)colour.r / 255.0f,
			(float)colour.g / 255.0f,
			(float)colour.b / 255.0f,
			(float)colour.a / 255.0f
		);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	const Vector<Ref<Texture>>& attachments() const override
	{
		return m_attachments;
	}

	const Vector<GLenum>& gl_attachments() const
	{
		return m_gl_attachments;
	}

	int attachment_count() const override
	{
		return m_attachments.size();
	}

	int width() const override
	{
		return m_width;
	}

	int height() const override
	{
		return m_height;
	}

	u32 id() const
	{
		return m_id;
	}
};

/*********************************************************/
/* MESH                                                  */
/*********************************************************/

class OpenGLMesh : public Mesh
{
	u32 m_id;
	
	u32 m_vertex_buffer;
	u64 m_vertex_count;

	u32 m_index_buffer;
	u64 m_index_count;

	u32 m_instance_buffer;
	u64 m_instance_count;

public:
	OpenGLMesh()
		: Mesh()
		, m_id(0)
		, m_vertex_buffer(0)
		, m_vertex_count(0)
		, m_index_buffer(0)
		, m_index_count(0)
		, m_instance_buffer(0)
		, m_instance_count(0)
	{
		glGenVertexArrays(1, &m_id);
		glGenBuffers(1, &m_vertex_buffer);
		glGenBuffers(1, &m_index_buffer);
		glGenBuffers(1, &m_instance_buffer);
	}

	~OpenGLMesh() override
	{
		glDeleteVertexArrays(1, &m_id);
		glDeleteBuffers(1, &m_vertex_buffer);
		glDeleteBuffers(1, &m_index_buffer);
		glDeleteBuffers(1, &m_instance_buffer);
	}

	void vertex_data(const Vertex* vertices, u64 count) override
	{
		m_vertex_count = count;

		glBindVertexArray(m_id);
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
			u64 size = gen_gl_vertex_attribs();

			glBufferData(GL_ARRAY_BUFFER, count * size, vertices, GL_DYNAMIC_DRAW);
		}
		glBindVertexArray(0);
	}

	void index_data(const u32* indices, u64 count) override
	{
		m_index_count = count;

		glBindVertexArray(m_id);
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_DYNAMIC_DRAW);
		}
		glBindVertexArray(0);
	}

	void instance_data(const Vertex* vertices, const void* instances, u64 count) override
	{
		m_instance_count = count;

		glBindVertexArray(m_id);
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_instance_buffer);
			u64 size = gen_gl_vertex_attribs();

			glBufferData(GL_ARRAY_BUFFER, count * size, instances, GL_DYNAMIC_DRAW);
		}
		glBindVertexArray(0);
	}

	u64 vertex_count() const override
	{
		return m_vertex_count;
	}

	u64 index_count() const override
	{
		return m_index_count;
	}

	u64 instance_count() const override
	{
		return m_instance_count;
	}

	u32 id() const
	{
		return m_id;
	}
};

/*********************************************************/
/* MAIN                                                  */
/*********************************************************/

// yes
static const GLenum COLOUR_ATTACHMENT_0 = GL_COLOR_ATTACHMENT0;

// yes pt.2
static const char* UNIFORM_TEXTURE_NAMES[] = {
		"lev_texture_0",
		"lev_texture_1",
		"lev_texture_2",
		"lev_texture_3",
		"lev_texture_4",
		"lev_texture_5",
		"lev_texture_6",
		"lev_texture_7",
		"lev_texture_8",
		"lev_texture_9",
		"lev_texture_10",
		"lev_texture_11",
		"lev_texture_12",
		"lev_texture_13",
		"lev_texture_14",
		"lev_texture_15",
		"lev_texture_16",
		"lev_texture_17",
		"lev_texture_18",
		"lev_texture_19",
		"lev_texture_20",
		"lev_texture_21",
		"lev_texture_22",
		"lev_texture_23",
		"lev_texture_24",
		"lev_texture_25",
		"lev_texture_26",
		"lev_texture_27",
		"lev_texture_28",
		"lev_texture_29",
		"lev_texture_30",
		"lev_texture_31",
};

class OpenGLRenderer : public Renderer
{
	void* m_context = nullptr;

public:
	bool init() override
	{
		m_context = System::inst()->gl_context_create();
		System::inst()->gl_context_make_current(m_context);

		if (!System::inst()->gl_load_glad_loader())
		{
			std::cout << "failed to initialize glad" << std::endl;
			return false;
		}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		return true;
	}

	void destroy() override
	{
		System::inst()->gl_context_destroy(m_context);
	}

	void before_render() override
	{
	}

	void after_render() override
	{
	}

	void render(const RenderPass& pass) override
	{
		auto shader = (OpenGLShader*)pass.material.shader().get();
		auto target = (OpenGLFramebuffer*)pass.target.get();
		auto mesh = (OpenGLMesh*)pass.mesh.get();
		auto& blend = pass.blend;

		if (target)
		{
			target->bind();
			glDrawBuffers(target->gl_attachments().size(), target->gl_attachments().begin());
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDrawBuffers(1, &COLOUR_ATTACHMENT_0);
		}

		shader->use();

		for (int i = 0; i < LEV_MAT_TEXTURES; i++)
		{
			auto texture = (OpenGLTexture*)pass.material.texture(i).get();
			auto sampler = pass.material.sampler(i);

			if (!texture)
				continue;

			texture->bind(i);
			texture->update(sampler);
			shader->set(UNIFORM_TEXTURE_NAMES[i], i);
		}

		// depth stencil
		{
			if (pass.stencil.face != FACE_NONE)
			{
				glEnable(GL_STENCIL_TEST);

				u32 gl_face = get_gl_compare_face(pass.stencil.face);

				glStencilFuncSeparate(
					gl_face,
					get_gl_compare_func(pass.stencil.func),
					pass.stencil.ref,
					pass.stencil.mask
				);

				glStencilOpSeparate(
					gl_face,
					get_gl_compare_fail(pass.stencil.s_fail),
					get_gl_compare_fail(pass.stencil.dp_fail),
					get_gl_compare_fail(pass.stencil.dp_pass)
				);
			}
			else
			{
				glDisable(GL_STENCIL_TEST);
			}

			if (pass.depth != COMPARE_NONE)
			{
				glEnable(GL_DEPTH_TEST);

				glDepthFunc(get_gl_compare_func(pass.depth));
				//glDepthMask(pass.depth_mask_toggle);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}

		if (pass.viewport != RectI::zero())
		{
			glViewport(
				pass.viewport.x, pass.viewport.y,
				pass.viewport.w, pass.viewport.h
			);
		}

		if (pass.scissor != RectI::zero())
		{
			glEnable(GL_SCISSOR_TEST);

			glScissor(
				pass.scissor.x, pass.scissor.y,
				pass.scissor.w, pass.scissor.h
			);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}

		// blending
		{
			glEnable(GL_BLEND);

			glBlendEquationSeparate(
				get_gl_blend_equation(blend.equation_rgb),
				get_gl_blend_equation(blend.equation_alpha)
			);

			glBlendFuncSeparate(
				get_gl_blend_func(blend.func_src_rgb),
				get_gl_blend_func(blend.func_dst_rgb),
				get_gl_blend_func(blend.func_src_alpha),
				get_gl_blend_func(blend.func_dst_alpha)
			);
		}

		glBindVertexArray(mesh->id());

		if (pass.instance_count > 0)
		{
			glDrawElementsInstanced(
				GL_TRIANGLES,
				pass.mesh->index_count(),
				GL_UNSIGNED_INT,
				0,
				pass.instance_count
			);
		}
		else
		{
			glDrawElements(
				GL_TRIANGLES,
				pass.mesh->index_count(),
				GL_UNSIGNED_INT,
				0
			);
		}

		glBindVertexArray(0);
	}

	void clear(const Colour& colour) override
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, App::inst()->draw_width(), App::inst()->draw_height());

		glClearColor(
			(float)colour.r / 255.0f,
			(float)colour.g / 255.0f,
			(float)colour.b / 255.0f,
			(float)colour.a / 255.0f
		);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	Ref<Texture> create_texture(const TextureData& data) override
	{
		return create_ref<OpenGLTexture>(data);
	}

	Ref<ArrayTexture> create_array_texture(const TextureData& data, u32 depth) override
	{
		return create_ref<OpenGLArrayTexture>(data, depth);
	}

	Ref<ShaderBuffer> create_shader_buffer(u64 size) override
	{
		return create_ref<OpenGLShaderBuffer>(size);
	}

	Ref<Shader> create_shader(const ShaderData& data) override
	{
		return create_ref<OpenGLShader>(data);
	}

	Ref<Framebuffer> create_framebuffer(const FramebufferData& data) override
	{
		return create_ref<OpenGLFramebuffer>(data);
	}

	Ref<Mesh> create_mesh() override
	{
		return create_ref<OpenGLMesh>();
	}

	void unbind_texture() override
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void unbind_array_texture() override
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	void unbind_texture_image() override
	{
		glBindImageTexture(0, 0, 0, 0, 0, 0, 0);
	}

	void unbind_shader() override
	{
		glUseProgram(0);
	}

	void unbind_shader_buffer() override
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void unbind_framebuffer() override
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};

Renderer* Renderer::inst()
{
	static OpenGLRenderer* instance = nullptr;
	if (!instance) { instance = new OpenGLRenderer(); }
	return instance;
}

#endif
