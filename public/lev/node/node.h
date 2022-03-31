#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>

namespace lev
{
	class SpriteBatch;

	class Node
	{
	public:
		Node();
		virtual ~Node();
		
		virtual void init();
		virtual void update();
		virtual void render(SpriteBatch& b);

		template <class T, typename... Args>
		T* add(Args&&... args);
		void remove(u64 id);
		bool has(u64 id);
		Node* get(u64 id);
		const Node* get(u64 id) const;

		Node* parent();
		const Node* parent() const;

		u64 id() const;

	private:
		Node* m_parent;
		Vector<Node*> m_nodes;

		u64 m_id;
		u64 m_id_counter;
		Vector<u64> m_free_ids;
	};

	template <class T, typename... Args>
	T* Node::add(Args&&... args)
	{
		T* node = new T(std::forward<Args>(args)...);
		node->m_parent = this;

		if (m_free_ids.size() > 0)
			node->m_id = m_free_ids.pop_back();
		else
			node->m_id = m_id_counter++;

		node->init();
		m_nodes.push_back(node);
		return node;
	}
}
