#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/node/signal.h>

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

		virtual void recieve_signal(const Signal& s);
		virtual void broadcast_signal(const Signal& s);
		virtual void broadcast_signal_down(const Signal& s);

		template <class T, typename... Args>
		T* add(Args&&... args);

		void remove(const Node* other);
		void remove(u64 id);
		void clear();
		
		bool has(u64 id);
		
		Node* get(u64 id);
		const Node* get(u64 id) const;

		Node* owner();
		Node* parent();
		const Node* parent() const;

		bool enabled() const;
		void enabled(bool v);

		const Vector<Node*>& children() const;

		u64 id() const;

	private:
		Node* m_parent;
		Vector<Node*> m_children;

		bool m_enabled;

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
		m_children.push_back(node);
		return node;
	}
}
