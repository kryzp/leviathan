#include <lev/graphics/mesh.h>
#include <backend/renderer.h>

using namespace lev;

Ref<Mesh> Mesh::create()
{
	return Renderer::inst()->create_mesh();
}
