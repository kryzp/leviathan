#if LEV_USE_OPENGL

#include <lev/core/app.h>
#include <backend/renderer.h>
#include <backend/system.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <third_party/glad/glad.h>

using namespace lev;

namespace
{
	void* g_context;

	int get_gl_texture_fmt(TextureFormat fmt)
	{
		switch (fmt)
		{
			case TEXTURE_FORMAT_RED:			return GL_RED;
			case TEXTURE_FORMAT_RG:				return GL_RG;
			case TEXTURE_FORMAT_RGB:			return GL_RGB;
			case TEXTURE_FORMAT_RGBA:			return GL_RGBA;
			case TEXTURE_FORMAT_DEPTH_STENCIL:	return GL_DEPTH_STENCIL;
		}
	}

	int get_gl_texture_internal_fmt(TextureFormat fmt)
	{
		switch (fmt)
		{
			case TEXTURE_FORMAT_RED:			return GL_R32F;
			case TEXTURE_FORMAT_RG:				return GL_RG32F;
			case TEXTURE_FORMAT_RGB:			return GL_RGB32F;
			case TEXTURE_FORMAT_RGBA:			return GL_RGBA32F;
			case TEXTURE_FORMAT_DEPTH_STENCIL:	return GL_DEPTH24_STENCIL8;
		}
	}

	int get_gl_texture_type(TextureType type)
	{
		switch (type)
		{
			case TEXTURE_TYPE_UNSIGNED_BYTE:	return GL_UNSIGNED_BYTE;
			case TEXTURE_TYPE_FLOAT:			return GL_FLOAT;
			case TEXTURE_TYPE_INT_24_8:			return GL_UNSIGNED_INT_24_8;
		}
	}

	int get_gl_blend_equation(BlendEquation func)
	{
		switch (func)
		{
			case BLEND_EQUATION_ADD:				return GL_FUNC_ADD;
			case BLEND_EQUATION_SUBTRACT:			return GL_FUNC_SUBTRACT;
			case BLEND_EQUATION_REVERSE_SUBTRACT:	return GL_FUNC_REVERSE_SUBTRACT;
			case BLEND_EQUATION_MIN:				return GL_MIN;
			case BLEND_EQUATION_MAX:				return GL_MAX;
		}
	}

	int get_gl_blend_func(BlendFunc factor)
	{
		switch (factor)
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
	}
}

/*********************************************************/
/* TEXTURE                                               */
/*********************************************************/

class OpenGLTexture : public Texture
{
	u32 m_id;
	
	int m_width;
	int m_height;

	TextureFormat m_format;
	TextureType m_type;
	
	int m_gl_format;
	int m_gl_internal_format;
	int m_gl_type;

public:
	OpenGLTexture(const TextureData& data)
		: Texture()
		, m_id(0)
		, m_width(data.width)
		, m_height(data.height)
		, m_format(data.format)
		, m_type(data.type)
	{
		m_gl_format = get_gl_texture_fmt(m_format);
		m_gl_internal_format = get_gl_texture_internal_fmt(m_format);
		m_gl_type = get_gl_texture_type(m_type);

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, m_gl_internal_format, m_width, m_height, 0, m_gl_format, m_gl_type, nullptr);
		update(TextureSampler::pixel()); // removing this breaks compute shader imageSize for whatever reason (i guess the wrap has something todo with it?)
	}

	~OpenGLTexture()
	{
		glDeleteTextures(1, &m_id);
	}

	void bind(int i) const override
	{
		LEV_ASSERT(i >= 0 && i < 32, "Index must be within 0 -> 31");
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void bind_image(int i) const override
	{
		LEV_ASSERT(i >= 0 && i < 32, "Index must be within 0 -> 31");
		glActiveTexture(GL_TEXTURE0 + i);
		glBindImageTexture(0, m_id, 0, GL_FALSE, 0, GL_READ_WRITE, m_gl_internal_format);
	}

	void generate(const void* data) override
	{
		LEV_ASSERT(data, "Data must not be nullptr");
		glTexImage2D(GL_TEXTURE_2D, 0, m_gl_internal_format, m_width, m_height, 0, m_gl_format, m_gl_type, data);
	}

	void get_data(float* buf) override
	{
		LEV_ASSERT(buf, "Buffer must not be nullptr");
		glGetTexImage(GL_TEXTURE_2D, 0, m_gl_format, m_gl_type, buf);
	}

	void update(const TextureSampler& sampler)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sampler.wrap_x == TEXTURE_WRAP_CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT); // can we just take a moment to appreciate how nicely the spacing lines up here oh my god
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (sampler.wrap_y == TEXTURE_WRAP_CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (sampler.filter == TEXTURE_FILTER_LINEAR) ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (sampler.filter == TEXTURE_FILTER_LINEAR) ? GL_LINEAR : GL_NEAREST);
	}

	void copy_to(Ref<Texture>& other) override
	{
		OpenGLTexture* other_ptr = (OpenGLTexture*)other.get();

		glCopyImageSubData(
			m_id,				GL_TEXTURE_2D, 0, 0, 0, 0,
			other_ptr->m_id,	GL_TEXTURE_2D, 0, 0, 0, 0,
			m_width, m_height, 1
		);
	}

	int width() const override { return m_width; }
	int height() const override { return m_height; }
	TextureFormat format() const override { return m_format; }
	u32 id() const { return m_id; }
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
		: m_id(0)
		, m_size(size)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_COPY);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	~OpenGLShaderBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void bind(int idx) override
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, idx, m_id);
	}

	void set(void* buf) override
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
		void* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		MemUtil::copy(p, buf, m_size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	u64 size() const override { return m_size; }
	u32 id() const { return m_id; }
};

class OpenGLShader : public Shader
{
	u32 m_id;
	u8 m_type;

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

		u32 id = glCreateProgram();

		if (data.type == SHADER_TYPE_COMPUTE)
		{
			const char* computedata = data.compute_source.c_str();

			u32 compute = glCreateShader(GL_COMPUTE_SHADER);
			glShaderSource(compute, 1, &computedata, NULL);
			glCompileShader(compute);
			glAttachShader(id, compute);

			glLinkProgram(id);
			
			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(id, 512, NULL, infolog);
				Log::error("failed to link compute shader program: %s", infolog);
			}

			glDeleteShader(compute);
		}
		else
		{
			const char* vertexdata = data.vertex_source.c_str();
			const char* fragmentdata = data.fragment_source.c_str();
			const char* geometrydata = data.geometry_source.c_str();

			u32 vertex = glCreateShader(GL_VERTEX_SHADER);
			if (data.type & SHADER_TYPE_VERTEX)
			{
				glShaderSource(vertex, 1, &vertexdata, NULL);
				glCompileShader(vertex);

				glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(vertex, 512, NULL, infolog);
					Log::error("failed to compile vertex shader: %s", infolog);
				}
			}

			u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
			if (data.type & SHADER_TYPE_FRAGMENT)
			{
				glShaderSource(fragment, 1, &fragmentdata, NULL);
				glCompileShader(fragment);

				glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(fragment, 512, NULL, infolog);
					Log::error("failed to compile fragment shader: %s", infolog);
				}
			}

			u32 geometry = 0;
			if (data.type & SHADER_TYPE_GEOMETRY)
			{
				geometry = glCreateShader(GL_GEOMETRY_SHADER);

				glShaderSource(geometry, 1, &geometrydata, NULL);
				glCompileShader(geometry);

				glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(geometry, 512, NULL, infolog);
					Log::error("failed to compile geometry shader: %s", infolog);
				}
			}

			glAttachShader(id, vertex);
			glAttachShader(id, fragment);

			if (data.type & SHADER_TYPE_GEOMETRY)
				glAttachShader(id, geometry);

			glLinkProgram(id);

			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(id, 512, NULL, infolog);
				Log::error("failed to link rendering shader program: %s", infolog);
			}

			glDeleteShader(vertex);
			glDeleteShader(fragment);

			if (data.type & SHADER_TYPE_GEOMETRY)
				glDeleteShader(geometry);
		}

		m_id = id;
	}

	~OpenGLShader()
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

	u8 type() override { return m_type; }
	u32 id() const { return m_id; }

	Shader& set(const char* name, bool value)						override { glUniform1i			(glGetUniformLocation(m_id, name), value							); return *this; }
	Shader& set(const char* name, bool* values, int count)			override { glUniform1iv			(glGetUniformLocation(m_id, name), count, (GLint*)values			); return *this; }
	Shader& set(const char* name, int value)						override { glUniform1i			(glGetUniformLocation(m_id, name), value							); return *this; }
	Shader& set(const char* name, int* values, int count)			override { glUniform1iv			(glGetUniformLocation(m_id, name), count, (GLint*)values			); return *this; }
	Shader& set(const char* name, float value)						override { glUniform1f			(glGetUniformLocation(m_id, name), value							); return *this; }
	Shader& set(const char* name, float* values, int count)			override { glUniform1fv			(glGetUniformLocation(m_id, name), count, values					); return *this; }
	Shader& set(const char* name, const Vec2I& value)				override { glUniform2i			(glGetUniformLocation(m_id, name), value.x, value.y					); return *this; }
	Shader& set(const char* name, const Vec2I* values, int count)	override { glUniform2iv			(glGetUniformLocation(m_id, name), count, (GLint*)values			); return *this; }
	Shader& set(const char* name, const Vec2F& value)				override { glUniform2f			(glGetUniformLocation(m_id, name), value.x, value.y					); return *this; }
	Shader& set(const char* name, const Vec2F* values, int count)	override { glUniform2fv			(glGetUniformLocation(m_id, name), count, (GLfloat*)values			); return *this; }
	Shader& set(const char* name, const Vec3I& value)				override { glUniform3i			(glGetUniformLocation(m_id, name), value.x, value.y, value.z		); return *this; }
	Shader& set(const char* name, const Vec3I* values, int count)	override { glUniform3iv			(glGetUniformLocation(m_id, name), count, (GLint*)values			); return *this; }
	Shader& set(const char* name, const Vec3F& value)				override { glUniform3f			(glGetUniformLocation(m_id, name), value.x, value.y, value.z		); return *this; }
	Shader& set(const char* name, const Vec3F* values, int count)	override { glUniform3fv			(glGetUniformLocation(m_id, name), count, (GLfloat*)values			); return *this; }
	Shader& set(const char* name, const Mat3x2& value)				override { glUniformMatrix3x2fv	(glGetUniformLocation(m_id, name), 1, GL_FALSE, value.value_ptr()	); return *this; }
	Shader& set(const char* name, const Mat4x4& value)				override { glUniformMatrix4fv	(glGetUniformLocation(m_id, name), 1, GL_FALSE, value.value_ptr()	); return *this; }
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
		: m_id(0)
		, m_width(data.width)
		, m_height(data.height)
	{
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		for (int i = 0; i < data.attachment_count; i++)
		{
			auto tex = Texture::create(m_width, m_height, data.attachments[i].format, data.attachments[i].type,  nullptr);
			auto gltex = (OpenGLTexture*)tex.get();

			if (data.attachments[i].format == TEXTURE_FORMAT_DEPTH_STENCIL)
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

	~OpenGLFramebuffer()
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
			colour.r,
			colour.g,
			colour.b,
			colour.a
		);

		glClear(GL_COLOR_BUFFER_BIT);
	}

	virtual const Vector<Ref<Texture>>& attachments() const override { return m_attachments; }
	const Vector<GLenum>& gl_attachments() const { return m_gl_attachments; }
	virtual int attachment_count() const override { return m_attachments.size(); }
	virtual int width() const override { return m_width; }
	virtual int height() const override { return m_height; }
	u32 id() const { return m_id; }
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

public:
	OpenGLMesh()
		: m_id(0)
		, m_vertex_buffer(0)
		, m_vertex_count(0)
		, m_index_buffer(0)
		, m_index_count(0)
	{
		glGenVertexArrays(1, &m_id);
		glGenBuffers(1, &m_vertex_buffer);
		glGenBuffers(1, &m_index_buffer);
	}

	~OpenGLMesh()
	{
		glDeleteVertexArrays(1, &m_id);
		glDeleteBuffers(1, &m_vertex_buffer);
		glDeleteBuffers(1, &m_index_buffer);
	}

	void vertex_data(const Vertex* vertices, u64 count) override
	{
		m_vertex_count = count;

		glBindVertexArray(m_id);
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), vertices, GL_DYNAMIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 * sizeof(float)));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(float)));
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));
			glEnableVertexAttribArray(3);
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

	u64 vertex_count() const override { return m_vertex_count; }
	u64 index_count() const override { return m_index_count; }
	u32 id() const { return m_id; }
};

/*********************************************************/
/* MAIN                                                  */
/*********************************************************/

bool Renderer::init()
{
	g_context = System::gl_context_create();
	System::gl_context_make_current(g_context);

	if (!System::gl_load_glad_loader())
	{
		std::cout << "failed to initialize glad" << std::endl;
		return false;
	}

	glEnable(GL_BLEND);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;
}

void Renderer::destroy()
{
	System::gl_context_destroy(g_context);
}

void Renderer::before_render()
{
}

void Renderer::after_render()
{
}

static const GLenum COLOUR_ATTACHMENT_0 = GL_COLOR_ATTACHMENT0;

static const char* UNIFORM_TEXTURE_NAMES[] = {
	"u_texture_0",
	"u_texture_1",
	"u_texture_2",
	"u_texture_3",
	"u_texture_4",
	"u_texture_5",
	"u_texture_6",
	"u_texture_7",
	"u_texture_8",
	"u_texture_9",
	"u_texture_10",
	"u_texture_11",
	"u_texture_12",
	"u_texture_13",
	"u_texture_14",
	"u_texture_15",
	"u_texture_16",
	"u_texture_17",
	"u_texture_18",
	"u_texture_19",
	"u_texture_20",
	"u_texture_21",
	"u_texture_22",
	"u_texture_23",
	"u_texture_24",
	"u_texture_25",
	"u_texture_26",
	"u_texture_27",
	"u_texture_28",
	"u_texture_29",
	"u_texture_30",
	"u_texture_31",
};

void Renderer::render(const RenderPass& pass)
{
	auto shader = (OpenGLShader*)pass.material.shader.get();
	auto target = (OpenGLFramebuffer*)pass.target.get();
	auto mesh = (OpenGLMesh*)pass.mesh.get();
	auto& blend = pass.blend;
	
	if (target)
	{
		target->bind();
		glDrawBuffers(target->gl_attachments().size(), target->gl_attachments().begin());
		glViewport(0, 0, target->width(), target->height());
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffers(1, &COLOUR_ATTACHMENT_0);
		glViewport(0, 0, App::draw_width(), App::draw_height());
	}

	shader->use();

	for (int i = 0; i < LEV_MAT_TEXTURES; i++)
	{
		auto texture = (OpenGLTexture*)pass.material.textures[i].get();
		auto sampler = pass.material.samplers[i];

		if (texture)
		{
			texture->bind(i);
			texture->update(sampler);
			shader->set(UNIFORM_TEXTURE_NAMES[i], i);
		}
	}

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

	glBindVertexArray(mesh->id());
	glDrawElements(GL_TRIANGLES, pass.mesh->index_count(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::clear(const Colour& colour)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, App::draw_width(), App::draw_height());
	glClearColor(
		colour.r,
		colour.g,
		colour.b,
		colour.a
	);
	glClear(GL_COLOR_BUFFER_BIT);
}

/*********************************************************/
/* MISC                                                  */
/*********************************************************/

Ref<Texture> Renderer::create_texture(const TextureData& data)
{
	return create_ref<OpenGLTexture>(data);
}

Ref<ShaderBuffer> Renderer::create_shader_buffer(u64 size)
{
	return create_ref<OpenGLShaderBuffer>(size);
}

Ref<Shader> Renderer::create_shader(const ShaderData& data)
{
	return create_ref<OpenGLShader>(data);
}

Ref<Framebuffer> Renderer::create_framebuffer(const FramebufferData& data)
{
	return create_ref<OpenGLFramebuffer>(data);
}

Ref<Mesh> Renderer::create_mesh()
{
	return create_ref<OpenGLMesh>();
}

void Renderer::unbind_texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::unbind_texture_image()
{
	// yes
	glBindImageTexture(0, 0, 0, 0, 0, 0, 0);
}

void Renderer::unbind_shader()
{
	glUseProgram(0);
}

void Renderer::unbind_shader_buffer()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Renderer::unbind_framebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif
