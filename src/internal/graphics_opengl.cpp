#if LEV_USE_OPENGL

#include <internal/graphics.h>
#include <third_party/glad/include/glad/glad.h>

using namespace Lev;

namespace
{
}

/*********************************************************/
// GRAPHICS STUFF
/*********************************************************/

bool Graphics::init()
{
	return true;
}

void Graphics::shutdown()
{
}

void Graphics::render()
{
}

void Graphics::before_render()
{
}

void Graphics::after_render()
{
}

Ref<Texture> Graphics::create_texture()
{
	return create_ref<Texture>();
}

Ref<Shader> Graphics::create_shader(const ShaderData& data)
{
	return create_ref<Shader>();
}

RendererType Graphics::renderer_type()
{
	return RendererType::OpenGL;
}

/*********************************************************/
// TEXTURE STUFF
/*********************************************************/

Texture::Texture()
{
}

Texture::Texture(const Ref<Image>& image)
{
	load(image);
}

Texture::Texture(const char* path)
{
	load(path);
}

Texture::~Texture()
{
	free();
}

void Texture::load(const Ref<Image>& image)
{
	if (!image->data())
		return;

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		image->width(),
		image->height(),
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image->data()
	);
}

void Texture::load(const char* path)
{
	load(create_ref<Image>(path));
}

void Texture::bind(int i) const
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

void Texture::free() const
{
	glDeleteTextures(1, &m_id);
}

/*********************************************************/
// SHADER STUFF
/*********************************************************/

Shader::Shader()
{
}

Shader::Shader(const char* vertex, const char* fragment)
{
	load(vertex, fragment);
}

Shader::~Shader()
{
	free();
}

void Shader::load(const char* vertex, const char* fragment)
{
	// todo
}

void Shader::use() const
{
	glUseProgram(m_id);
}

void Shader::free() const
{
	glDeleteProgram(m_id);
}

void Shader::set(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name), (int)value);
}

void Shader::set(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::set( const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name), value);
}

#endif
