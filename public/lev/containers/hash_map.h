#pragma once

#include <lev/core/util.h>
#include <lev/containers/pair.h>

// std::hash
#include <functional>

namespace lv
{
	template <typename TKey, typename TValue>
	class HashMap
	{
	public:
		using KeyValuePair = Pair<TKey, TValue>;

		struct Bucket
		{
			Bucket() : data(), next(nullptr), prev(nullptr) { }
			Bucket(const TKey& k, const TValue& v) : data(k, v), next(nullptr), prev(nullptr) { }
			KeyValuePair data;
			Bucket* next;
			Bucket* prev;
		};

		struct Iterator
		{
			Iterator() : m_bucket(nullptr) { }
			Iterator(Bucket* init) : m_bucket(init) { }
			~Iterator() = default;
			KeyValuePair& operator * () const { return m_bucket->data; }
			KeyValuePair* operator -> () const { return &m_bucket->data; }
			Iterator& operator ++ () { m_bucket = m_bucket->next; return *this; }
			Iterator& operator -- () { m_bucket = m_bucket->prev; return *this; }
			bool operator == (const Iterator& other) { return this->m_bucket == other.m_bucket; }
			bool operator != (const Iterator& other) { return this->m_bucket != other.m_bucket; }
		private:
			Bucket* m_bucket;
		};

		struct ConstIterator
		{
			ConstIterator() : m_bucket(nullptr) { }
			ConstIterator(const Bucket* init) : m_bucket(init) { }
			~ConstIterator() = default;
			KeyValuePair& operator * () const { return m_bucket->data; }
			KeyValuePair* operator -> () const { return &m_bucket->data; }
			ConstIterator& operator ++ () { m_bucket = m_bucket->next; return *this; }
			ConstIterator& operator -- () { m_bucket = m_bucket->prev; return *this; }
			bool operator == (const ConstIterator& other) { return this->m_bucket == other.m_bucket; }
			bool operator != (const ConstIterator& other) { return this->m_bucket != other.m_bucket; }
		private:
			const Bucket* m_bucket;
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

		int bucket_count() const;
		int capacity() const;
		bool is_empty() const;

		Bucket* first();
		const Bucket* first() const;
		Bucket* last();
		const Bucket* last() const;

		Iterator begin();
		ConstIterator begin() const;
		Iterator end();
		ConstIterator end() const;

	private:
		int index_of(const TKey& key);
		void realloc();

		int m_capacity;
		Bucket** m_buckets;
		int m_bucket_count;
	};

	template <typename TKey, typename TValue>
	HashMap<TKey, TValue>::HashMap()
		: m_buckets(nullptr)
		, m_bucket_count(0)
		, m_capacity(0)
	{
	}

	template <typename TKey, typename TValue>
	HashMap<TKey, TValue>::HashMap(int initial_capacity)
		: m_buckets(nullptr)
		, m_bucket_count(0)
		, m_capacity(initial_capacity)
	{
		realloc();
	}

	template <typename TKey, typename TValue>
	HashMap<TKey, TValue>::~HashMap()
	{
		delete[] m_buckets;
		m_capacity = 0;
		m_bucket_count = 0;
	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::insert(const KeyValuePair& pair)
	{
	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::erase(const TKey& key)
	{
		Bucket* b = m_buckets[index_of(key)];

		while (b)
		{
			if (b->data.first == key)
			{
				if (b->next)
					b->next->prev = b->prev;

				if (b->prev)
					b->prev->next = b->next;

				if (b == m_buckets[0])
					m_buckets[0] = b->next;

				if (b == m_buckets[m_capacity-1])
					m_buckets[m_capacity-1] = b->prev;

				::operator delete(b, sizeof(Bucket));
			}

			b = b->next;
		}
	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::clear()
	{
	}

	template <typename TKey, typename TValue>
	TValue& HashMap<TKey, TValue>::get(const TKey& key)
	{
		Bucket* b = m_buckets[index_of(key)];

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
		Bucket* b = m_buckets[index_of(key)];

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
		Bucket* b = m_buckets[index_of(key)];

		while (b)
		{
			if (b->data.first == key)
				return true;

			b = b->next;
		}

		return false;
	}

	template <typename TKey, typename TValue>
	int HashMap<TKey, TValue>::bucket_count() const
	{
		return m_bucket_count;
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
			if (m_buckets[i])
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
	void HashMap<TKey, TValue>::realloc()
	{
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Bucket* HashMap<TKey, TValue>::first()
	{
		return m_buckets;
	}

	template <typename TKey, typename TValue>
	const typename HashMap<TKey, TValue>::Bucket* HashMap<TKey, TValue>::first() const
	{
		return m_buckets;
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Bucket* HashMap<TKey, TValue>::last()
	{
		return m_buckets + m_capacity - 1;
	}

	template <typename TKey, typename TValue>
	const typename HashMap<TKey, TValue>::Bucket* HashMap<TKey, TValue>::last() const
	{
		return m_buckets + m_capacity - 1;
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::begin()
	{
		return Iterator(m_buckets);
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::begin() const
	{
		return ConstIterator(m_buckets);
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Iterator HashMap<TKey, TValue>::end()
	{
		return Iterator(m_buckets + m_capacity);
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::ConstIterator HashMap<TKey, TValue>::end() const
	{
		return ConstIterator(m_buckets + m_capacity);
	}
};

/*
template <typename Key, typename Value>
void HashMap<Key, Value>::insert(const Key& key, const Value& value)
{
	reallocate();

	Bucket& existing = m_buckets[index_of(key)];

	if (!existing.used)
	{
		m_buckets[index_of(key)] = Bucket(Pair(key, value), nullptr, nullptr);
		m_bucket_count++;
	}
	else
	{
		if (existing.data.first == key)
		{
			existing.data.second = value;
		}
		else
		{
			Bucket* entry = existing.next;

			if (entry)
			{
				while (entry)
				{
					if (entry->data.first == key)
					{
						entry->data.second = value;
						return;
					}

					entry = entry->next;
				}

				entry->next = new Bucket(Pair(key, value), nullptr, entry);
			}
			else
			{
				existing.next = new Bucket(Pair(key, value), nullptr, &existing);
			}
		}
	}
}

template <typename Key, typename Value>
void HashMap<Key, Value>::remove(const Key& key)
{
	reallocate();

	Bucket* entry = &m_buckets[index_of(key)];
	while (entry)
	{
		auto next = entry->next;
		(*entry) = Bucket();
		entry = next;
	}

	m_bucket_count--;
}


template <typename Key, typename Value>
void HashMap<Key, Value>::reallocate()
{
	int oldsize = m_capacity;

	if (m_bucket_count >= m_capacity)
		m_capacity += 16;
	else if (m_bucket_count < m_capacity-16)
		m_capacity -= 16;

	if (oldsize == m_capacity)
		return;

	Bucket* newbuf = new Bucket[m_capacity];
	mem::set(newbuf, 0, sizeof(Bucket) * m_capacity);

	for (int i = 0; i < oldsize; i++)
		new (newbuf + index_of(m_buckets[i].data.first)) Bucket(std::move(m_buckets[i]));

	delete[] m_buckets;
	m_buckets = newbuf;
}
 */
