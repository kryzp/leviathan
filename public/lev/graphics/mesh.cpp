#include <lev/graphics/mesh.h>
#include <backend/renderer.h>

using namespace lv;

Ref<Mesh> Mesh::create()
{
	return bknd::Renderer::inst()->create_mesh();
}
