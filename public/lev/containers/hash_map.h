#pragma once

#include <lev/core/util.h>
#include <lev/containers/pair.h>

/*
 * WARNING
 * THIS PROBABLY DOESNT WORK
 * MOST LIKELY
 * DEFINATELY
 *
 * ILL FIX (COMPLETELY REWORK) THIS WHEN I FEEL LIKE IT
 */

namespace lv
{
	template <typename Key, typename Value>
	class HashMap
	{
	public:
		struct Bucket
		{
			Pair<Key, Value> data;
			bool used;

			Bucket* next;
			Bucket* prev;

			Bucket()
				: data()
				, used(false)
				, next(nullptr)
				, prev(nullptr)
			{
			}

			Bucket(const Pair<Key, Value>& data, Bucket* next, Bucket* prev)
				: data(data)
				, used(true)
				, next(next)
				, prev(prev)
			{
			}
		};

		struct KeyValueIterator
		{
			KeyValueIterator(Bucket* initial_ptr)
				: m_ptr(initial_ptr)
			{
			}

			~KeyValueIterator() = default;

			Pair<Key, Value>& operator * () const
			{
				return m_ptr->data;
			}

			Pair<Key, Value>* operator -> ()
			{
				return &m_ptr->data;
			}

			KeyValueIterator& operator ++ ()
			{
				auto* next = m_ptr->next;

				if (next)
				{
					// iterate over bucket
					m_ptr = m_ptr->next;
				}
				else
				{
					// go all the way back to list of buckets
					while (m_ptr->prev)
						m_ptr = m_ptr->prev;

					// increment
					m_ptr++;
				}

				return *this;
			}

			KeyValueIterator operator ++ (int)
			{
				KeyValueIterator tmp = *this;
				++(*this);
				return tmp;
			}

			friend bool operator == (const KeyValueIterator& a, const KeyValueIterator& b) { return a.m_ptr == b.m_ptr; };
			friend bool operator != (const KeyValueIterator& a, const KeyValueIterator& b) { return a.m_ptr != b.m_ptr; };

		private:
			Bucket* m_ptr;
		};

		HashMap();
		HashMap(int initial_capacity);
		~HashMap();

		void insert(const Key& key, const Value& value);
		void remove(const Key& key);
		Value at(const Key& key) const;

		bool contains(const Key& key) const;
		int index_of(const Key& key) const;

		KeyValueIterator begin();
		const KeyValueIterator begin() const;

		KeyValueIterator end();
		const KeyValueIterator end() const;

	private:
		void reallocate();

		int m_size;
		int m_count;

		Bucket* m_buckets;
	};

	template <typename Key, typename Value>
	HashMap<Key, Value>::HashMap()
		: m_size(16)
		, m_count(0)
	{
		m_buckets = new Bucket[m_size];
		mem::set(m_buckets, 0, sizeof(Bucket) * m_size);
	}

	template <typename Key, typename Value>
	HashMap<Key, Value>::HashMap(int initial_capacity)
		: m_size(initial_capacity)
		, m_count(0)
	{
		m_buckets = new Bucket[m_size];
		mem::set(m_buckets, 0, sizeof(Bucket) * m_size);
	}

	template <typename Key, typename Value>
	HashMap<Key, Value>::~HashMap()
	{
//		for (int i = 0; i < m_size; i++)
//		{
//			Bucket* entry = m_buckets[i].next;
//
//			while (entry)
//			{
//				Bucket* next = entry->next;
//				delete entry;
//				entry = next;
//			}
//		}

		delete[] m_buckets;

		m_size = 0;
		m_count = 0;
	}

	template <typename Key, typename Value>
	void HashMap<Key, Value>::insert(const Key& key, const Value& value)
	{
		reallocate();

		Bucket& existing = m_buckets[index_of(key)];

		if (!existing.used)
		{
			m_buckets[index_of(key)] = Bucket(Pair(key, value), nullptr, nullptr);
			m_count++;
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

		m_count--;
	}

	template <typename Key, typename Value>
	Value HashMap<Key, Value>::at(const Key& key) const
	{
		Bucket* entry = &m_buckets[index_of(key)];

		while (entry)
		{
			if (entry->data.first == key)
				return entry->data.second;

			entry = entry->next;
		}

		return Value();
	}

	template <typename Key, typename Value>
	void HashMap<Key, Value>::reallocate()
	{
		int oldsize = m_size;

		if (m_count >= m_size)
			m_size += 16;
		else if (m_count < m_size-16)
			m_size -= 16;

		if (oldsize == m_size)
			return;
		
		Bucket* newbuf = new Bucket[m_size];
		mem::set(newbuf, 0, sizeof(Bucket) * m_size);

        for (int i = 0; i < oldsize; i++)
			new (newbuf + index_of(m_buckets[i].data.first)) Bucket(std::move(m_buckets[i]));

		delete[] m_buckets;
		m_buckets = newbuf;
	}

	template <typename Key, typename Value>
	bool HashMap<Key, Value>::contains(const Key& key) const
	{
		Bucket* entry = &m_buckets[index_of(key)];

		while (entry)
		{
			if (entry->data.first == key)
				return true;

			entry = entry->next;
		}

		return false;
	}

	template <typename Key, typename Value>
	int HashMap<Key, Value>::index_of(const Key& k) const
	{
		std::hash<Key> H;
		return H(k) % m_size;
	}

	template <typename Key, typename Value>
	typename HashMap<Key, Value>::KeyValueIterator HashMap<Key, Value>::begin()
	{
		Bucket* begin = nullptr;

		for (int i = 0; i < m_size; i++)
		{
			if (m_buckets[i].used)
				return KeyValueIterator(&m_buckets[i]);
		}

		return KeyValueIterator(nullptr);
	}

	template <typename Key, typename Value>
	const typename HashMap<Key, Value>::KeyValueIterator HashMap<Key, Value>::begin() const
	{
		Bucket* begin = nullptr;

		for (int i = 0; i < m_size; i++)
		{
			if (m_buckets[i].used)
				return KeyValueIterator(&m_buckets[i]);
		}

		return KeyValueIterator(nullptr);
	}

	template <typename Key, typename Value>
	typename HashMap<Key, Value>::KeyValueIterator HashMap<Key, Value>::end()
	{
		for (int i = m_size; i >= 0; i--)
		{
			if (m_buckets[i].used)
				return KeyValueIterator(&m_buckets[i+1]);
		}

		return KeyValueIterator(nullptr);
	}

	template <typename Key, typename Value>
	const typename HashMap<Key, Value>::KeyValueIterator HashMap<Key, Value>::end() const
	{
		for (int i = m_size; i >= 0; i--)
		{
			if (m_buckets[i].used)
				return KeyValueIterator(&m_buckets[i+1]);
		}

		return KeyValueIterator(nullptr);
	}

//	template <typename Key, typename Value>
//	Vector<Key> HashMap<Key, Value>::keys() const
//	{
//		Vector<Key> result;
//
//		for (int i = 0; i < m_size; i++)
//		{
//			Bucket* entry = &m_buckets[i];
//
//			while (entry && entry->used)
//			{
//				result.push_back(entry->data.first);
//				entry = entry->next;
//			}
//		}
//
//		return result;
//	}
//
//	template <typename Key, typename Value>
//	Vector<Value> HashMap<Key, Value>::values() const
//	{
//		Vector<Value> result;
//
//		for (int i = 0; i < m_size; i++)
//		{
//			Bucket* entry = &m_buckets[i];
//
//			while (entry && entry->used)
//			{
//				result.push_back(entry->data.second);
//				entry = entry->next;
//			}
//		}
//
//		return result;
//	}
};
