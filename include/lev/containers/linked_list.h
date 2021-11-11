#pragma once

namespace Lev
{
	template <typename T>
	class LinkedList
	{
	public:
		struct Node
		{
			Node* next = nullptr;
			Node* prev = nullptr;
			T data;
		};

		LinkedList();
		~LinkedList();

		Node* add(T item);
		void remove(Node* node);
		void remove(T item);

		Node* find(T item) const;

		Node* first = nullptr;
		Node* last = nullptr;
	};

	template <typename T>
	LinkedList<T>::LinkedList()
	{
	}

	template <typename T>
	LinkedList<T>::~LinkedList()
	{
		Node* node = first;
		while (node != nullptr)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}

		first = nullptr;
		last = nullptr;
	}

	template <typename T>
	inline typename LinkedList<T>::Node* LinkedList<T>::add(T item)
	{
		Node* node = new Node();
		node->data = item;

		if (last)
		{
			last->next = node;
			node->prev = last;
			node->next = nullptr;
			last = node;
		}
		else
		{
			node->next = nullptr;
			node->prev = nullptr;

			first = node;
			last = node;
		}

		return node;
	}

	template <typename T>
	inline void Lev::LinkedList<T>::remove(Node* node)
	{
		if (!node)
			return;

		if (node->next)
			node->next->prev = node->prev;

		if (node->prev)
			node->prev->next = node->next;

		if (node == first)
			first = node->next;

		if (node == last)
			last = node->prev;

		node->next = nullptr;
		node->prev = nullptr;

		delete node;
	}

	template <typename T>
	inline void Lev::LinkedList<T>::remove(T item)
	{
		remove(find(item));
	}

	template <typename T>
	inline typename LinkedList<T>::Node* Lev::LinkedList<T>::find(T item) const
	{
		for (auto* node = first; node != nullptr; node = node->next)
			if (node->data == item)
				return node;
		
		return nullptr;
	}
}
