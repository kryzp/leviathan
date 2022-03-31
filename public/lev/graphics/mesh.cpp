#include <lev/graphics/mesh.h>
#include <backend/renderer.h>

using namespace lev;

Ref<Mesh> Mesh::create()
{
	return Renderer::create_mesh();
}
