#pragma once

#include <lev/core/util.h>
#include <lev/containers/pair.h>

// std::hash
#include <functional>

// note to self:
// each bucket stores a 'next' and 'prev'.
// last element of a bucket stores a next pointer to the next bucket for fast iteration

namespace lev
{
	template <typename TKey, typename TValue>
	class HashMap
	{
	public:
		static constexpr int MIN_CAPACITY = 16;

		using KeyValuePair = Pair<TKey, TValue>;

		struct Bucket
		{
			Bucket() : data(), next(nullptr), prev(nullptr) { }
			Bucket(const KeyValuePair& p) : data(p), next(nullptr), prev(nullptr) { }
			Bucket(Bucket&& other) noexcept : data(std::move(other.data)), next(std::move(other.next)), prev(std::move(other.prev)) { };
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
			Iterator& operator ++ () { if (m_bucket) { m_bucket = m_bucket->next; } return *this; }
			Iterator& operator -- () { if (m_bucket) { m_bucket = m_bucket->prev; } return *this; }
			bool operator == (const Iterator& other) const { return this->m_bucket == other.m_bucket; }
			bool operator != (const Iterator& other) const { return this->m_bucket != other.m_bucket; }
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
			ConstIterator& operator ++ () { if (m_bucket) { m_bucket = m_bucket->next; } return *this; }
			ConstIterator& operator -- () { if (m_bucket) { m_bucket = m_bucket->prev; } return *this; }
			bool operator == (const ConstIterator& other) const { return this->m_bucket == other.m_bucket; }
			bool operator != (const ConstIterator& other) const { return this->m_bucket != other.m_bucket; }
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

		TValue& operator [] (const TKey& idx);
		const TValue& operator [] (const TKey& idx) const;

	private:
		int index_of(const TKey& key);
		void realloc();
		void realign_ptrs();

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
		realloc();
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
		auto idx = index_of(pair.first);

		Bucket* b = m_buckets[idx];

		if (b)
		{
			while (b->next)
				b = b->next;

			b->next = new Bucket(pair);
			b->next->prev = b;
		}
		else
		{
			m_buckets[idx] = new Bucket(pair);
		}

		realign_ptrs();

		m_bucket_count++;
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
				m_buckets[index_of(key)] = nullptr;

				m_bucket_count--;

				realign_ptrs();

				return;
			}

			b = b->next;
		}

	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::clear()
	{
		m_bucket_count = 0;
	}

	template <typename TKey, typename TValue>
	void HashMap<TKey, TValue>::realloc()
	{
		int old_size = m_capacity;

		if (m_bucket_count >= m_capacity)
			m_capacity += MIN_CAPACITY;
		else if (m_bucket_count < m_capacity-MIN_CAPACITY)
			m_capacity -= MIN_CAPACITY;

		if (old_size == m_capacity)
			return;

		Bucket** new_buf = new Bucket*[m_capacity];
		mem::set(new_buf, 0, sizeof(Bucket*) * m_capacity);

		for (int i = 0; i < old_size; i++)
		{
			if (m_buckets[i])
			{
				int idx = index_of(m_buckets[i]->data.first);
				new_buf[idx] = new Bucket(std::move(*m_buckets[i]));
			}
		}

		delete[] m_buckets;
		m_buckets = new_buf;

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
			Bucket* bkt = m_buckets[i];

			if (!bkt)
				continue;

			if (bkt != l)
			{
				for (int j = 0; j < m_capacity - 1; j++)
				{
					int check_idx = (i + j + 1) % m_capacity;

					if (m_buckets[check_idx])
					{
						bkt->next = m_buckets[check_idx];
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

					if (m_buckets[check_idx])
					{
						bkt->prev = m_buckets[check_idx];
						break;
					}
				}
			}
		}
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
	typename HashMap<TKey, TValue>::Bucket* HashMap<TKey, TValue>::first()
	{
		for (int i = 0; i < m_capacity; i++)
		{
			if (m_buckets[i])
				return m_buckets[i];
		}

		return nullptr;
	}

	template <typename TKey, typename TValue>
	const typename HashMap<TKey, TValue>::Bucket* HashMap<TKey, TValue>::first() const
	{
		for (int i = 0; i < m_capacity; i++)
		{
			if (m_buckets[i])
				return m_buckets[i];
		}

		return nullptr;
	}

	template <typename TKey, typename TValue>
	typename HashMap<TKey, TValue>::Bucket* HashMap<TKey, TValue>::last()
	{
		for (int i = m_capacity - 1; i >= 0; i--)
		{
			if (m_buckets[i])
				return m_buckets[i];
		}

		return nullptr;
	}

	template <typename TKey, typename TValue>
	const typename HashMap<TKey, TValue>::Bucket* HashMap<TKey, TValue>::last() const
	{
		for (int i = m_capacity; i >= 0; i--)
		{
			if (m_buckets[i])
				return m_buckets[i];
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
