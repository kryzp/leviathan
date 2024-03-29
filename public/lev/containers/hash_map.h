#pragma once

#include <lev/core/util.h>
#include <lev/containers/pair.h>

// std::hash
#include <functional>

namespace lev
{
	template <typename TKey, typename TValue>
	class HashMap
	{
	public:
		static constexpr int MIN_CAPACITY = 16;

		using KeyValuePair = Pair<TKey, TValue>;

		struct Element
		{
			Element() : data(), next(nullptr), prev(nullptr) { }
			Element(const KeyValuePair& p) : data(p), next(nullptr), prev(nullptr) { }
			Element(Element&& other) noexcept : data(std::move(other.data)), next(std::move(other.next)), prev(std::move(other.prev)) { };
			KeyValuePair data;
			Element* next;
			Element* prev;
		};

		struct Iterator
		{
			Iterator() : m_bucket(nullptr) { }
			Iterator(Element* init) : m_bucket(init) { }
			~Iterator() = default;
			KeyValuePair& operator * () const { return m_bucket->data; }
			KeyValuePair* operator -> () const { return &m_bucket->data; }
			Iterator& operator ++ () { if (m_bucket) { m_bucket = m_bucket->next; } return *this; }
			Iterator& operator -- () { if (m_bucket) { m_bucket = m_bucket->prev; } return *this; }
			bool operator == (const Iterator& other) const { return this->m_bucket == other.m_bucket; }
			bool operator != (const Iterator& other) const { return this->m_bucket != other.m_bucket; }
		private:
			Element* m_bucket;
		};

		struct ConstIterator
		{
			ConstIterator() : m_elem(nullptr) { }
			ConstIterator(const Element* init) : m_elem(init) { }
			~ConstIterator() = default;
			const KeyValuePair& operator * () const { return m_elem->data; }
			const KeyValuePair* operator -> () const { return &m_elem->data; }
			ConstIterator& operator ++ () { if (m_elem) { m_elem = m_elem->next; } return *this; }
			ConstIterator& operator -- () { if (m_elem) { m_elem = m_elem->prev; } return *this; }
			bool operator == (const ConstIterator& other) const { return this->m_elem == other.m_elem; }
			bool operator != (const ConstIterator& other) const { return this->m_elem != other.m_elem; }
		private:
			const Element* m_elem;
		};

		HashMap();
		HashMap(int initial_capacity);
		~HashMap();

		void insert(const KeyValuePair& pair);
		void erase(const TKey& key);
		void clear();

		TValue& get(const TKey& key);
		const TValue& get(const TKey& key) const;

		bool contains(const TKey& key);

		int element_count() const;
		int capacity() const;
		bool is_empty() const;

		Element* first();
		const Element* first() const;
		Element* last();
		const Element* last() const;

		Iterator begin();
		ConstIterator begin() const;
		Iterator end();
		ConstIterator end() const;

		TValue& operator [] (const TKey& idx);
		const TValue& operator [] (const TKey& idx) const;

	private:
		int index_of(const TKey& key);
		void realloc();
		void realign_ptrs();

		Element** m_elements;
		int m_element_count;
		int m_capacity;
	};

	template <typename TKey, typename TValue>
	HashMap<TKey, TValue>::HashMap()
		: m_elements(nullptr)
		, m_element_count(0)
		, m_capacity(0)
	{
		realloc();
	}

	template <typename TKey, typename TValue>
	HashMap<TKey, TValue>::HashMap(int initial_capacity)
		: m_elements(nullptr)
		, m_element_count(0)
		, m_capacity(initial_capacity)
	{
		realloc();
	}

	template <typename TKey, typename TValue>
	HashMap<TKey, TValue>::~HashMap()
	{
		delete[] m_elements;
		m_capacity = 0;
		m_element_count = 0;
	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::insert(const KeyValuePair& pair)
	{
		auto idx = index_of(pair.first);

		Element* b = m_elements[idx];

		if (b)
		{
			while (b->next)
				b = b->next;

			b->next = new Element(pair);
			b->next->prev = b;
		}
		else
		{
			m_elements[idx] = new Element(pair);
		}

		realign_ptrs();

		m_element_count++;
	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::erase(const TKey& key)
	{
		Element* b = m_elements[index_of(key)];

		while (b)
		{
			if (b->data.first == key)
			{
				if (b == m_elements[index_of(key)])
					m_elements[index_of(key)] = b->next;

				if (b->next)
					b->next->prev = b->prev;

				if (b->prev)
					b->prev->next = b->next;

				::operator delete(b, sizeof(Element));

				m_element_count--;

				realign_ptrs();

				return;
			}

			b = b->next;
		}

	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::clear()
	{
		m_element_count = 0;
	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::realloc()
	{
		int old_size = m_capacity;

		if (m_element_count >= m_capacity)
			m_capacity += MIN_CAPACITY;
		else if (m_element_count < m_capacity-MIN_CAPACITY)
			m_capacity -= MIN_CAPACITY;

		if (old_size == m_capacity)
			return;

		Element** new_buf = new Element*[m_capacity];
		mem::set(new_buf, 0, sizeof(Element*) * m_capacity);

		for (int i = 0; i < old_size; i++)
		{
			if (m_elements[i])
			{
				int idx = index_of(m_elements[i]->data.first);
				new_buf[idx] = new Element(std::move(*m_elements[i]));
			}
		}

		delete[] m_elements;
		m_elements = new_buf;

		realign_ptrs();
	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::realign_ptrs()
	{
		// big o who?

		auto f = first();
		auto l = last();

		for (int i = 0; i < m_capacity; i++)
		{
			Element* bkt = m_elements[i];

			if (!bkt)
				continue;

			if (bkt != l)
			{
				for (int j = 0; j < m_capacity - 1; j++)
				{
					int check_idx = (i + j + 1) % m_capacity;

					if (m_elements[check_idx])
					{
						bkt->next = m_elements[check_idx];
						break;
					}
				}
			}

			if (bkt != f)
			{
				for (int j = 0; j < m_capacity - 1; j++)
				{
					int check_idx = m_capacity - 1 - j + i;

					if (check_idx < 0)
						check_idx += m_capacity;
					else if (check_idx >= m_capacity)
						check_idx -= m_capacity;

					if (m_elements[check_idx])
					{
						bkt->prev = m_elements[check_idx];
						break;
					}
				}
			}
		}
	}

	template <typename TKey, typename TValue>
	TValue& HashMap<TKey, TValue>::get(const TKey& key)
	{
		Element* b = m_elements[index_of(key)];

		while (b)
		{
			if (b->data.first == key)
				return b->data.second;

			b = b->next;
		}

		LEV_ERROR("Could not find bucket matching key");
	}

	template <typename TKey, typename TValue>
	const TValue& HashMap<TKey, TValue>::get(const TKey& key) const
	{
		Element* b = m_elements[index_of(key)];

		while (b)
		{
			if (b->data.first == key)
				return b->data.second;

			b = b->next;
		}

		LEV_ERROR("Could not find bucket matching key");
	}

	template <typename TKey, typename TValue>
	bool HashMap<TKey, TValue>::contains(const TKey& key)
	{
		Element* b = m_elements[index_of(key)];

		while (b)
		{
			if (b->data.first == key)
				return true;

			b = b->next;
		}

		return false;
	}

	template <typename TKey, typename TValue>
	int HashMap<TKey, TValue>::element_count() const
	{
		return m_element_count;
	}

	template <typename TKey, typename TValue>
	int HashMap<TKey, TValue>::capacity() const
	{
		return m_capacity;
	}

	template <typename TKey, typename TValue>
	bool HashMap<TKey, TValue>::is_empty() const
	{
		for (int i = 0; i < m_capacity; i++)
		{
			if (m_elements[i])
				return false;
		}

		return true;
	}

	template <typename TKey, typename TValue>
	int HashMap<TKey, TValue>::index_of(const TKey& key)
	{
		return std::hash<TKey>{}(key) % m_capacity;
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Element* HashMap<TKey, TValue>::first()
	{
		for (int i = 0; i < m_capacity; i++)
		{
			if (m_elements[i])
				return m_elements[i];
		}

		return nullptr;
	}

	template <typename TKey, typename TValue>
	const typename HashMap<TKey, TValue>::Element* HashMap<TKey, TValue>::first() const
	{
		for (int i = 0; i < m_capacity; i++)
		{
			if (m_elements[i])
				return m_elements[i];
		}

		return nullptr;
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Element* HashMap<TKey, TValue>::last()
	{
		for (int i = m_capacity - 1; i >= 0; i--)
		{
			if (m_elements[i])
				return m_elements[i];
		}

		return nullptr;
	}

	template <typename TKey, typename TValue>
	const typename HashMap<TKey, TValue>::Element* HashMap<TKey, TValue>::last() const
	{
		for (int i = m_capacity; i >= 0; i--)
		{
			if (m_elements[i])
				return m_elements[i];
		}

		return nullptr;
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::begin()
	{
		return Iterator(first());
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::begin() const
	{
		return ConstIterator(first());
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::end()
	{
		return Iterator(nullptr);
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::end() const
	{
		return ConstIterator(nullptr);
	}

	template <typename TKey, typename TValue>
	TValue& HashMap<TKey, TValue>::operator [] (const TKey& idx)
	{
		return get(idx);
	}

	template <typename TKey, typename TValue>
	const TValue& HashMap<TKey, TValue>::operator [] (const TKey& idx) const
	{
		return get(idx);
	}
};
