#pragma once

#include <lev/core/util.h>

#include <lev/containers/vector.h>
#include <lev/containers/pair.h>
#include <lev/containers/optional.h>

// todo: this needs  a big professor re-work moment

namespace lv
{
	template <typename Key, typename Value>
	class HashMap
	{
		struct Entry
		{
			Pair<Key, Value> data;
			Entry* next;
			bool used;

			Entry()
				: data()
				, next(nullptr)
				, used(false)
			{
			}

			Entry(const Pair<Key, Value>& data, Entry* next)
				: data(data)
				, next(next)
				, used(true)
			{
			}
		};

	public:
		HashMap();
		HashMap(int initial_capacity);
		~HashMap();

		void insert(const Key& key, const Value& value);
		void remove(const Key& key);
		Value at(const Key& key) const;

		bool contains(const Key& key) const;
		int index_of(const Key& key) const;

		Entry* begin();
		const Entry* begin() const;

		Entry* end();
		const Entry* end() const;

		Vector<Pair<Key, Value>> items() const;
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
			m_entrys[index_of(key)] = Entry(Pair(key, value), nullptr);
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

					entry->next = new Entry(Pair(key, value), nullptr);
				}
				else
				{
					existing.next = new Entry(Pair(key, value), nullptr);
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
	typename HashMap<Key, Value>::Entry* HashMap<Key, Value>::begin()
	{
		return m_entrys;
	}

	template <typename Key, typename Value>
	const typename HashMap<Key, Value>::Entry* HashMap<Key, Value>::begin() const
	{
		return m_entrys;
	}

	template <typename Key, typename Value>
	typename HashMap<Key, Value>::Entry* HashMap<Key, Value>::end()
	{
		return m_entrys + m_count;
	}

	template <typename Key, typename Value>
	const typename HashMap<Key, Value>::Entry* HashMap<Key, Value>::end() const
	{
		return m_entrys + m_count;
	}

	template <typename Key, typename Value>
	Vector<Pair<Key, Value>> HashMap<Key, Value>::items() const
	{
		Vector<Pair<Key, Value>> result;

		for (int i = 0; i < m_size; i++)
		{
			Entry* entry = &m_entrys[i];

			while (entry && entry->used)
			{
				result.push_back(entry->data);
				entry = entry->next;
			}
		}

		return result;
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
