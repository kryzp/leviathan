#include <lev/node/node.h>
#include <lev/graphics/sprite_batch.h>

using namespace lev;

Node::Node()
	: m_parent(nullptr)
	, m_id(0)
	, m_id_counter(0)
{
}

Node::~Node()
{
	for (auto& n : m_nodes)
		delete n;
}

void Node::init()
{
	for (auto& n : m_nodes)
		n->init();
}

void Node::update()
{
	for (auto& n : m_nodes)
		n->update();
}

void Node::render(SpriteBatch& b)
{
	for (auto& n : m_nodes)
		n->render(b);
}

void Node::remove(u64 id)
{
	for (auto& n : m_nodes)
	{
		if (n->m_id == id)
		{
			m_nodes.erase(n->m_id);
			m_free_ids.push_back(n->m_id);
		}
	}
}

bool Node::has(u64 id)
{
	for (auto& n : m_nodes)
	{
		if (n->m_id == id)
			return true;
	}

	return false;
}

Node* Node::get(u64 id)
{
	for (auto& n : m_nodes)
	{
		if (n->m_id == id)
			return n;
	}

	return nullptr;
}

const Node* Node::get(u64 id) const
{
	for (auto& n : m_nodes)
	{
		if (n->m_id == id)
			return n;
	}

	return nullptr;
}

Node* Node::parent() { return m_parent; }
const Node* Node::parent() const { return m_parent; }
u64 Node::id() const { return m_id; }
