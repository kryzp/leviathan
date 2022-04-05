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
	for (auto& n : m_children)
		delete n;
}

void Node::init()
{
	for (auto& n : m_children)
		n->init();
}

void Node::update()
{
	for (auto& n : m_children)
		n->update();
}

void Node::render(SpriteBatch& b)
{
	for (auto& n : m_children)
		n->render(b);
}

void Node::recieve_signal(const Signal& s)
{
}

void Node::broadcast_signal(const Signal& s)
{
	owner()->broadcast_signal_down(s);
}

void Node::broadcast_signal_down(const Signal& s)
{
	recieve_signal(s);

	for (auto& n : m_children)
		n->broadcast_signal_down(s);
}

void Node::remove(const Node* other)
{
	remove(other->m_id);
}

void Node::remove(u64 id)
{
	for (auto& n : m_children)
	{
		if (n->m_id == id)
		{
			m_children.erase(n->m_id);
			m_free_ids.push_back(n->m_id);
		}
	}
}

bool Node::has(u64 id)
{
	for (auto& n : m_children)
	{
		if (n->m_id == id)
			return true;
	}

	return false;
}

Node* Node::get(u64 id)
{
	for (auto& n : m_children)
	{
		if (n->m_id == id)
			return n;
	}

	return nullptr;
}

const Node* Node::get(u64 id) const
{
	for (auto& n : m_children)
	{
		if (n->m_id == id)
			return n;
	}

	return nullptr;
}

Node* Node::owner()
{
	Node* result = this;

	while (result->m_parent)
		result = result->m_parent;

	return result;
}

Node* Node::parent() { return m_parent; }
const Node* Node::parent() const { return m_parent; }

u64 Node::id() const { return m_id; }
