#include <lev/graphics/render_pass.h>
#include <backend/renderer.h>

using namespace lv;

void RenderPass::perform() const
{
	bknd::Renderer::inst()->render(*this);
}
