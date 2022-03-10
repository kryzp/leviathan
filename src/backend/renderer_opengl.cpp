#if LEV_USE_OPENGL

#include <lev/core/app.h>
#include <backend/renderer.h>
#include <backend/system.h>

#include <third_party/glad/glad.h>

using namespace lev;

namespace
{
	void* g_context;

	int get_gl_texture_fmt(gfx::TextureFormat fmt)
	{
		switch (fmt)
		{
			case gfx::TextureFormat::RGB:			return GL_RGB;
			case gfx::TextureFormat::RGBA:			return GL_RGBA;
			case gfx::TextureFormat::DEPTH_STENCIL: return GL_DEPTH_STENCIL;
		}

		LEV_ASSERT(false);
	}

	int get_gl_texture_internal_fmt(gfx::TextureFormat fmt)
	{
		switch (fmt)
		{
			case gfx::TextureFormat::RGB:			return GL_RGB;
			case gfx::TextureFormat::RGBA:			return GL_RGBA;
			case gfx::TextureFormat::DEPTH_STENCIL:	return GL_DEPTH24_STENCIL8;
		}

		LEV_ASSERT(false);
	}

	int get_gl_texture_type(gfx::TextureFormat fmt)
	{
		switch (fmt)
		{
			case gfx::TextureFormat::RGB:			return GL_UNSIGNED_BYTE;
			case gfx::TextureFormat::RGBA:			return GL_UNSIGNED_BYTE;
			case gfx::TextureFormat::DEPTH_STENCIL:	return GL_UNSIGNED_INT_24_8;
		}

		LEV_ASSERT(false);
	}

	int get_gl_blend_func(gfx::BlendFunction func)
	{
		switch (func)
		{
			case gfx::BlendFunction::ADD:				return GL_FUNC_ADD;
			case gfx::BlendFunction::SUBTRACT:			return GL_FUNC_SUBTRACT;
			case gfx::BlendFunction::REVERSE_SUBTRACT:	return GL_FUNC_REVERSE_SUBTRACT;
			case gfx::BlendFunction::MIN:				return GL_MIN;
			case gfx::BlendFunction::MAX:				return GL_MAX;
		}
		
		LEV_ASSERT(false);
	}

	int get_gl_blend_factor(gfx::BlendFactor factor)
	{
		switch (factor)
		{
			case gfx::BlendFactor::ZERO:						return GL_ZERO;
			case gfx::BlendFactor::ONE:							return GL_ONE;
			case gfx::BlendFactor::SRC_COLOUR:					return GL_SRC_COLOR;
			case gfx::BlendFactor::SRC1_COLOUR:					return GL_SRC1_COLOR;
			case gfx::BlendFactor::ONE_MINUS_SRC_COLOUR:		return GL_ONE_MINUS_SRC_COLOR;
			case gfx::BlendFactor::ONE_MINUS_SRC1_COLOUR:		return GL_ONE_MINUS_SRC1_COLOR;
			case gfx::BlendFactor::DST_COLOUR:					return GL_DST_COLOR;
			case gfx::BlendFactor::ONE_MINUS_DST_COLOUR:		return GL_ONE_MINUS_DST_COLOR;
			case gfx::BlendFactor::SRC_ALPHA:					return GL_SRC_ALPHA;
			case gfx::BlendFactor::SRC1_ALPHA:					return GL_SRC1_ALPHA;
			case gfx::BlendFactor::ONE_MINUS_SRC_ALPHA:			return GL_ONE_MINUS_SRC_ALPHA;
			case gfx::BlendFactor::ONE_MINUS_SRC1_ALPHA:		return GL_ONE_MINUS_SRC1_ALPHA;
			case gfx::BlendFactor::DST_ALPHA:					return GL_DST_ALPHA;
			case gfx::BlendFactor::ONE_MINUS_DST_ALPHA:			return GL_ONE_MINUS_DST_ALPHA;
			case gfx::BlendFactor::CONSTANT_COLOUR:				return GL_CONSTANT_COLOR;
			case gfx::BlendFactor::ONE_MINUS_CONSTANT_COLOUR:	return GL_ONE_MINUS_CONSTANT_COLOR;
			case gfx::BlendFactor::CONSTANT_ALPHA:				return GL_CONSTANT_ALPHA;
			case gfx::BlendFactor::ONE_MINUS_CONSTANT_ALPHA:	return GL_ONE_MINUS_CONSTANT_ALPHA;
		}

		LEV_ASSERT(false);
	}
}

/*********************************************************/
/* TEXTURE STUFF                                         */
/*********************************************************/

class OpenGLTexture : public gfx::Texture
{
	u32 m_id;
	int m_width;
	int m_height;
	gfx::TextureFormat m_format;
	int m_gl_format;
	int m_gl_internal_format;
	int m_gl_type;

public:
	OpenGLTexture(const gfx::TextureData& data)
		: Texture()
		, m_id(0)
		, m_width(data.width)
		, m_height(data.height)
		, m_format(data.format)
	{
		m_gl_format = get_gl_texture_fmt(m_format);
		m_gl_internal_format = get_gl_texture_internal_fmt(m_format);
		m_gl_type = get_gl_texture_type(m_format);

		glGenTextures(1, &m_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, m_gl_internal_format, m_width, m_height, 0, m_gl_format, m_gl_type, nullptr);
	}

	~OpenGLTexture()
	{
		glDeleteTextures(1, &m_id);
	}

	void bind(int i = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void generate(const byte* data) override
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, m_gl_internal_format, m_width, m_height, 0, m_gl_format, m_gl_type, data);
	}

	void update(const gfx::TextureSampler& sampler)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (sampler.wrap_x == gfx::TextureWrap::CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT); // can we just take a moment to appreciate how nicely the spacing lines up here oh my god
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (sampler.wrap_y == gfx::TextureWrap::CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (sampler.filter == gfx::TextureFilter::LINEAR) ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (sampler.filter == gfx::TextureFilter::LINEAR) ? GL_LINEAR : GL_NEAREST);
	}

	int width() const override { return m_width; }
	int height() const override { return m_height; }
	gfx::TextureFormat format() const override { return m_format; }
	u32 id() const { return m_id; }
};

Ref<gfx::Texture> Renderer::create_texture(const gfx::TextureData& data)
{
	return create_ref<OpenGLTexture>(data);
}

/*********************************************************/
/* SHADER STUFF                                          */
/*********************************************************/

class OpenGLShader : public gfx::Shader
{
	u32 m_id;

public:
	OpenGLShader(const gfx::ShaderData& data)
		: Shader()
		, m_id(0)
	{
		int success;
		char infolog[512];

		const char* vertexdata = data.vertex_source;
		const char* fragmentdata = data.fragment_source;

		u32 vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexdata, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infolog);
			Log::error("failed to compile vertex shader: %s", infolog);
		}

		u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentdata, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infolog);
			Log::error("failed to compile fragment shader: %s", infolog);
		}

		u32 id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, infolog);
			Log::error("failed to link program: %s", infolog);
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		m_id = id;
	}

	~OpenGLShader()
	{
		glDeleteProgram(m_id);
	}

	void use() const override
	{
		glUseProgram(m_id);
	}

	u32 id() const { return m_id; }

	void set(const char* name, bool value) const override { glUniform1i(glGetUniformLocation(m_id, name), (int)value); }
	void set(const char* name, int value) const override { glUniform1i(glGetUniformLocation(m_id, name), value); }
	void set(const char* name, float value) const override { glUniform1f(glGetUniformLocation(m_id, name), value); }
	void set(const char* name, const Mat3x2& value) const override { glUniformMatrix3x2fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, value.value_ptr()); }
	void set(const char* name, const Mat4x4& value) const override { glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, value.value_ptr()); }
};

Ref<gfx::Shader> Renderer::create_shader(const gfx::ShaderData& data)
{
	return create_ref<OpenGLShader>(data);
}

/*********************************************************/
/* FRAMEBUFFER STUFF                                     */
/*********************************************************/

class OpenGLFramebuffer : public gfx::Framebuffer
{
	u32 m_id;
	Vector<Ref<gfx::Texture>> m_attachments;
	int m_attachment_count;
	int m_width;
	int m_height;

public:
	OpenGLFramebuffer(const gfx::FramebufferData& data)
		: m_id(0)
		, m_width(data.width)
		, m_height(data.height)
	{
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		for (int i = 0; i < data.attachment_count; i++)
		{
			auto tex = gfx::Texture::create(m_width, m_height, data.attachments[i]);
			auto gltex = (OpenGLTexture*)tex.get();

			if (data.attachments[i] == gfx::TextureFormat::DEPTH_STENCIL)
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gltex->id(), 0);
			else
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gltex->id(), 0);

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

	void clear(const Colour& colour = Colour::BLACK) override
	{
		bind();

		glClearColor(
			(float)colour.r / 255.0f,
			(float)colour.g / 255.0f,
			(float)colour.b / 255.0f,
			(float)colour.a / 255.0f
		);

		glClear(GL_COLOR_BUFFER_BIT);
	}

	virtual const Vector<Ref<gfx::Texture>>& attachments() const override { return m_attachments; }
	virtual int attachment_count() const override { return m_attachment_count; }
	virtual int width() const override { return m_width; }
	virtual int height() const override { return m_height; }
	u32 id() const { return m_id; }
};

Ref<gfx::Framebuffer> Renderer::create_framebuffer(const gfx::FramebufferData& data)
{
	return create_ref<OpenGLFramebuffer>(data);
}

/*********************************************************/
/* MESH STUFF                                            */
/*********************************************************/

class OpenGLMesh : public gfx::Mesh
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

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4 * sizeof(float)));
			glEnableVertexAttribArray(2);
		}
		glBindVertexArray(0);
	}

	void index_data(const u32* indices, u64 count) override
	{
		m_index_count = count;

		glBindVertexArray(m_id);
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u32), indices, GL_DYNAMIC_DRAW);
		}
		glBindVertexArray(0);
	}

	u64 vertex_count() const override { return m_vertex_count; }
	u64 index_count() const override { return m_index_count; }
	u32 id() const { return m_id; }
};

Ref<gfx::Mesh> Renderer::create_mesh()
{
	return create_ref<OpenGLMesh>();
}

/*********************************************************/
/* MAIN STUFF                                            */
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

void Renderer::render(const RenderPass& pass)
{
	auto shader = (OpenGLShader*)pass.material->shader.get();
	auto texture = (OpenGLTexture*)pass.material->texture.get();
	auto target = (OpenGLFramebuffer*)pass.target.get();
	auto& sampler = pass.material->sampler;
	auto mesh = (OpenGLMesh*)pass.mesh.get();
	auto& blend = pass.blend;
	
	if (target)
		target->bind();
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader->use();

	if (texture)
	{
		texture->bind();
		texture->update(sampler);

		shader->set(gfx::Shader::MAIN_TEXTURE, 0);
	}

	glBlendEquationSeparate(
		get_gl_blend_func(blend.func_rgb),
		get_gl_blend_func(blend.func_alpha)
	);

	glBlendFuncSeparate(
		get_gl_blend_factor(blend.factor_src_rgb),
		get_gl_blend_factor(blend.factor_dst_rgb),
		get_gl_blend_factor(blend.factor_src_alpha),
		get_gl_blend_factor(blend.factor_dst_alpha)
	);

	glBindVertexArray(mesh->id());
	glDrawElements(GL_TRIANGLES, pass.mesh->index_count(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::clear(float r, float g, float b, float a)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, App::draw_width(), App::draw_height());
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clear(const Colour& colour)
{
	clear(colour.r / 255.0f, colour.g / 255.0f, colour.b / 255.0f, colour.a / 255.0f);
}

#endif
