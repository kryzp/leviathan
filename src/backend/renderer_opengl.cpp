#if LEV_USE_OPENGL

#include <lev/core/app.h>
#include <lev/core/log.h>
#include <backend/renderer.h>
#include <backend/platform.h>
#include <third_party/glad/glad.h>
#include <iostream>

using namespace Lev;

struct State
{
	void* context;
};

namespace
{
	State g_state;
}

/*********************************************************/
/* TEXTURE STUFF                                         */
/*********************************************************/

class OpenGLTexture : public Texture
{
	u32 m_id;
	TextureData m_data;

public:
	OpenGLTexture(const TextureData& data)
		: Texture()
		, m_id(0)
		, m_data(data)
	{
		glGenTextures(1, &m_id);
	}

	~OpenGLTexture()
	{
		glDeleteTextures(1, &m_id);
	}

	void set(const byte* data) override
	{
		LEV_ASSERT(data != nullptr);

		int fmt = GL_RGBA; // default format

		if (m_data.format == TextureFormat::RGBA) {
			fmt = GL_RGBA;
		} else if (m_data.format == TextureFormat::RGB) {
			fmt = GL_RGB;
		}

		bind();

		// temp //
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// temp //

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_data.width, m_data.height, 0, fmt, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void bind(int i = 0) const override
	{
		GLenum active = GL_TEXTURE0;

		// lmao what
		if      (i == 1)  active = GL_TEXTURE1;
		else if (i == 2)  active = GL_TEXTURE2;
		else if (i == 3)  active = GL_TEXTURE3;
		else if (i == 4)  active = GL_TEXTURE4;
		else if (i == 5)  active = GL_TEXTURE5;
		else if (i == 6)  active = GL_TEXTURE6;
		else if (i == 7)  active = GL_TEXTURE7;
		else if (i == 8)  active = GL_TEXTURE8;
		else if (i == 9)  active = GL_TEXTURE9;
		else if (i == 10) active = GL_TEXTURE10;
		else if (i == 11) active = GL_TEXTURE11;
		else if (i == 12) active = GL_TEXTURE12;
		else if (i == 13) active = GL_TEXTURE13;
		else if (i == 14) active = GL_TEXTURE14;
		else if (i == 15) active = GL_TEXTURE15;
		else if (i == 16) active = GL_TEXTURE16;
		else if (i == 17) active = GL_TEXTURE17;
		else if (i == 18) active = GL_TEXTURE18;
		else if (i == 19) active = GL_TEXTURE19;
		else if (i == 20) active = GL_TEXTURE20;
		else if (i == 21) active = GL_TEXTURE21;
		else if (i == 22) active = GL_TEXTURE22;
		else if (i == 23) active = GL_TEXTURE23;
		else if (i == 24) active = GL_TEXTURE24;
		else if (i == 25) active = GL_TEXTURE25;
		else if (i == 26) active = GL_TEXTURE26;
		else if (i == 27) active = GL_TEXTURE27;
		else if (i == 28) active = GL_TEXTURE28;
		else if (i == 29) active = GL_TEXTURE29;
		else if (i == 30) active = GL_TEXTURE30;

		glActiveTexture(active);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	TextureData data() const override { return m_data; }
	u32 id() const { return m_id; }
};

Ref<Texture> Renderer::create_texture(const TextureData& data)
{
	return create_ref<OpenGLTexture>(data);
}

/*********************************************************/
/* SHADER STUFF                                          */
/*********************************************************/

class OpenGLShader : public Shader
{
	u32 m_id;
	ShaderData m_data;

public:
	OpenGLShader(const ShaderData& data)
		: Shader()
		, m_id(0)
		, m_data(data)
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

	void assign_uniform(const char* name, UniformType type, UniformFlags flags) override
	{
		UniformData uniform;
		uniform.name = name;
		uniform.type = type;
		uniform.flags = flags;

		m_data.uniforms.push_back(uniform);
	}

	ShaderData data() const override { return m_data; }
	u32 id() const { return m_id; }

	void set(const char* name, bool value) const override { glUniform1i(glGetUniformLocation(m_id, name), (int)value); }
	void set(const char* name, int value) const override { glUniform1i(glGetUniformLocation(m_id, name), value); }
	void set(const char* name, float value) const override { glUniform1f(glGetUniformLocation(m_id, name), value); }
	void set(const char* name, const Mat3x2& value) const override { glUniformMatrix3x2fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, value.value_ptr()); }
	void set(const char* name, const Mat4x4& value) const override { glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, value.value_ptr()); }
};

Ref<Shader> Renderer::create_shader(const ShaderData& data)
{
	return create_ref<OpenGLShader>(data);
}

/*********************************************************/
/* MAIN STUFF                                            */
/*********************************************************/

bool Renderer::init()
{
	g_state.context = Platform::gl_context_create();
	Platform::gl_context_make_current(g_state.context);

	if (!Platform::gl_load_glad_loader())
	{
		std::cout << "failed to initialize glad" << std::endl;
		return false;
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;
}

void Renderer::destroy()
{
	Platform::gl_context_destroy(g_state.context);
}

void Renderer::before_render()
{
	clear(0.0f, 0.75f, 0.7f);
}

void Renderer::after_render()
{
}

void Renderer::render(const RenderPass& pass)
{
	// todo: temp

	u32 vao, vbo, ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pass.vertex_count * sizeof(float), pass.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pass.index_count * sizeof(u32), pass.indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Renderer::clear(float r, float g, float b, float a)
{
	glViewport(0, 0, App::draw_width(), App::draw_height());
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

RendererType Renderer::renderer_type()
{
	return RendererType::OpenGL;
}

#endif
