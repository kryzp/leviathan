#pragma once

#include <lev/core/util.h>

#include <lev/containers/vector.h>
#include <lev/containers/pair.h>
#include <lev/containers/optional.h>

/*
for (auto& [thing0, thing1] : m_map)
{
	print(thing0);
	thing0 += thing1;
}
 */

namespace lv
{
	template <typename Key, typename Value>
	class HashMap
	{
	public:
		struct Entry
		{
			Pair<Key, Value> data;
			Entry* next;
			Entry* prev;
			bool used;

			Entry()
				: data()
				, next(nullptr)
				, prev(nullptr)
				, used(false)
			{
			}

			Entry(const Pair<Key, Value>& data, Entry* next, Entry* prev)
				: data(data)
				, next(next)
				, prev(prev)
				, used(true)
			{
			}
		};

		struct KeyValueIterator
		{
			KeyValueIterator(Entry* initial_ptr)
				: m_ptr(initial_ptr)
				, m_end(nullptr)
			{
			}

			KeyValueIterator(Entry* initial_ptr, Entry* end_ptr)
				: m_ptr(initial_ptr)
				, m_end(end_ptr)
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
					// go all the way back
					while (m_ptr->prev)
						m_ptr = m_ptr->prev;

					// increment
					m_ptr++;

					if (m_ptr == m_end)
						return *this;

					// increment until used entry is found
					while (!m_ptr->used && m_ptr)
					{
						m_ptr++;

						if (m_ptr == m_end)
							return *this;
					}
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
			Entry* m_ptr;
			Entry* m_end;
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

		Vector<Key> keys() const;
		Vector<Value> values() const;

	private:
		void reallocate();

		int m_size;
		int m_count;
		Entry* m_entrys;
	};

	template <typename Key, typename Value>
	HashMap<Key, Value>::HashMap()
		: m_size(16)
		, m_count(0)
	{
		m_entrys = new Entry[m_size];
		mem::set(m_entrys, 0, sizeof(Entry) * m_size);
	}

	template <typename Key, typename Value>
	HashMap<Key, Value>::HashMap(int initial_capacity)
		: m_size(initial_capacity)
		, m_count(0)
	{
		m_entrys = new Entry[m_size];
		mem::set(m_entrys, 0, sizeof(Entry) * m_size);
	}

	template <typename Key, typename Value>
	HashMap<Key, Value>::~HashMap()
	{
		for (int i = 0; i < m_size; i++)
		{
			Entry* entry = m_entrys[i].next;

			while (entry)
			{
				Entry* next = entry->next;
				delete entry;
				entry = next;
			}
		}

		delete[] m_entrys;

		m_size = 0;
		m_count = 0;
	}

	template <typename Key, typename Value>
	void HashMap<Key, Value>::insert(const Key& key, const Value& value)
	{
		reallocate();

		Entry& existing = m_entrys[index_of(key)];

		if (!existing.used)
		{
			m_entrys[index_of(key)] = Entry(Pair(key, value), nullptr, nullptr);
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
				Entry* entry = existing.next;
				
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

					entry->next = new Entry(Pair(key, value), nullptr, entry);
				}
				else
				{
					existing.next = new Entry(Pair(key, value), nullptr, &existing);
				}
			}
		}
	}

	template <typename Key, typename Value>
	void HashMap<Key, Value>::remove(const Key& key)
	{
		reallocate();

		Entry* entry = &m_entrys[index_of(key)];
		while (entry)
		{
			auto next = entry->next;
			(*entry) = Entry();
			entry = next;
		}

		m_count--;
	}

	template <typename Key, typename Value>
	Value HashMap<Key, Value>::at(const Key& key) const
	{
		Entry* entry = &m_entrys[index_of(key)];

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
		
		Entry* newbuf = new Entry[m_size];
		mem::set(newbuf, 0, sizeof(Entry) * m_size);

        for (int i = 0; i < oldsize; i++)
			new (newbuf + index_of(m_entrys[i].data.first)) Entry(std::move(m_entrys[i]));

		delete[] m_entrys;
		m_entrys = newbuf;
	}

	template <typename Key, typename Value>
	bool HashMap<Key, Value>::contains(const Key& key) const
	{
		Entry* entry = &m_entrys[index_of(key)];

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
		Entry* begin = nullptr;
		Entry* end = nullptr;

		for (int i = 0; i < m_size; i++)
		{
			if (m_entrys[i].used)
			{
				begin = &m_entrys[i];
				break;
			}
		}

		for (int i = m_size; i >= 0; i--)
		{
			if (m_entrys[i].used)
			{
				end = &m_entrys[i+1];
				break;
			}
		}

		return KeyValueIterator(begin, end);
	}

	template <typename Key, typename Value>
	const typename HashMap<Key, Value>::KeyValueIterator HashMap<Key, Value>::begin() const
	{
		Entry* begin = nullptr;
		Entry* end = nullptr;

		for (int i = 0; i < m_size; i++)
		{
			if (m_entrys[i].used)
			{
				begin = &m_entrys[i];
				break;
			}
		}

		for (int i = m_size; i >= 0; i--)
		{
			if (m_entrys[i].used)
			{
				end = &m_entrys[i+1];
				break;
			}
		}

		return KeyValueIterator(begin, end);
	}

	template <typename Key, typename Value>
	typename HashMap<Key, Value>::KeyValueIterator HashMap<Key, Value>::end()
	{
		for (int i = m_size; i >= 0; i--)
		{
			if (m_entrys[i].used)
				return KeyValueIterator(&m_entrys[i+1]);
		}

		return KeyValueIterator(nullptr);
	}

	template <typename Key, typename Value>
	const typename HashMap<Key, Value>::KeyValueIterator HashMap<Key, Value>::end() const
	{
		for (int i = m_size; i >= 0; i--)
		{
			if (m_entrys[i].used)
				return KeyValueIterator(&m_entrys[i+1]);
		}

		return KeyValueIterator(nullptr);
	}

	template <typename Key, typename Value>
	Vector<Key> HashMap<Key, Value>::keys() const
	{
		Vector<Key> result;

		for (int i = 0; i < m_size; i++)
		{
			Entry* entry = &m_entrys[i];

			while (entry && entry->used)
			{
				result.push_back(entry->data.first);
				entry = entry->next;
			}
		}

		return result;
	}

	template <typename Key, typename Value>
	Vector<Value> HashMap<Key, Value>::values() const
	{
		Vector<Value> result;

		for (int i = 0; i < m_size; i++)
		{
			Entry* entry = &m_entrys[i];

			while (entry && entry->used)
			{
				result.push_back(entry->data.second);
				entry = entry->next;
			}
		}

		return result;
	}
};
