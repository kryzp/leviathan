#pragma once

#include <lev/core/util.h>
#include <functional>

namespace lev
{
	template <typename Key, typename Value>
	class HashMap
	{
		struct Entry
		{
			Key key;
			Value value;
			Entry* next;

			Entry()
				: key()
				, value()
				, next(nullptr)
			{
			}

			Entry(const Key& key, const Value& value, Entry* next)
				: key(key)
				, value(value)
				, next(next)
			{
			}
		};

	public:
		HashMap();
		HashMap(int initial_capacity);
		~HashMap();

		void insert(const Key& key, const Value& value);
		Value at(const Key& key);
		void remove(const Key& key);
		bool contains(const Key& key) const;
		int indexof(const Key& key) const;

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
	}

	template <typename Key, typename Value>
	HashMap<Key, Value>::HashMap(int initial_capacity)
		: m_size(initial_capacity)
		, m_count(0)
	{
		m_entrys = new Entry[m_size];
	}

	template <typename Key, typename Value>
	HashMap<Key, Value>::~HashMap()
	{
		if (m_entrys)
			delete[] m_entrys;

		m_size = 0;
		m_count = 0;
	}

	template <typename Key, typename Value>
	void HashMap<Key, Value>::insert(const Key& key, const Value& value)
	{
		reallocate();

		Entry& existing = m_entrys[indexof(key)];

		if (existing.value == Value())
		{
			m_entrys[indexof(key)] = Entry(key, value, nullptr);
			m_count++;
		}
		else
		{
			if (existing.key == key)
			{
				existing.value = value;
				return;
			}
			else
			{
				Entry* entry = existing.next;
				
				if (entry)
				{
					while (entry->next)
						entry = entry->next;

					entry->next = new Entry(key, value, nullptr);
				}
				else
				{
					existing.next = new Entry(key, value, nullptr);
				}

				return;
			}

			Entry* entry = existing.next;
			while (entry)
			{
				if (entry->key == key)
				{
					entry->value = value;
					return;
				}

				entry = entry->next;
			}
		}
	}

	template <typename Key, typename Value>
	void HashMap<Key, Value>::remove(const Key& key)
	{
		reallocate();

		Entry* entry = &m_entrys[indexof(key)];
		while (entry)
		{
			auto next = entry->next;
			entry->value.~Value();
			(*entry) = Entry();
			entry = next;
		}

		m_count--;
	}

	template <typename Key, typename Value>
	Value HashMap<Key, Value>::at(const Key& key)
	{
		Entry* entry = &m_entrys[indexof(key)];

		while (entry)
		{
			if (entry->key == key)
				return entry->value;

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

        for (int i = 0; i < oldsize; i++)
			new (newbuf+indexof(m_entrys[i].key)) Entry(std::move(m_entrys[i]));

		delete[] m_entrys;
		m_entrys = newbuf;
	}

	template <typename Key, typename Value>
	bool HashMap<Key, Value>::contains(const Key& key) const
	{
		Entry* entry = &m_entrys[indexof(key)];

		while (entry)
		{
			if (entry->key == key)
				return true;

			entry = entry->next;
		}

		return false;
	}

	template <typename Key, typename Value>
	int HashMap<Key, Value>::indexof(const Key& key) const
	{
		std::hash<Key> keyhash;
		return keyhash(key) % m_size;
	}
};
