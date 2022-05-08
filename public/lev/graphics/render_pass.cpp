#include <lev/graphics/render_pass.h>
#include <backend/renderer.h>

using namespace lev;

void RenderPass::perform()
{
	bknd::Renderer::inst()->render(*this);
}
