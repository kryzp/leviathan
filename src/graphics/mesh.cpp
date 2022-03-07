#include <lev/graphics/mesh.h>
#include <backend/renderer.h>

using namespace lev;
using namespace lev::gfx;

Ref<Mesh> Mesh::create()
{
	return Renderer::create_mesh();
}
