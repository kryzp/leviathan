#pragma once

#include <lev/containers/linked_list.h>

namespace Lev
{
<<<<<<< HEAD
	template <typename Key, typename Value>
	class HashMap
	{
	};
=======
	// currently its just a scuffed linked list but in the future i wanna just make this a hash map instead :/

	template <typename Key, typename Value>
	class Map
	{
		struct Entry
		{
			Key key;
			Value value;

			Entry* next;
			Entry* prev;

			Entry(const Key& key, const Value& value)
				: key(key)
				, value(value)
				, next(nullptr)
				, prev(nullptr)
			{
			}
		};

	public:
		Map();
		~Map();

		void add(const Key& key, const Value& value);
		void remove(const Key& key);

		Value get(const Key& key) const;
		int size() const;

	private:
		Entry* get_entry(const Key& key);
		const Entry* get_entry(const Key& key) const;

		Entry* m_first;
		Entry* m_last;

		int m_size;

	};

	template <typename Key, typename Value>
	Map<Key, Value>::Map()
		: m_first(nullptr)
		, m_last(nullptr)
		, m_size(0)
	{
	}

	template <typename Key, typename Value>
	Map<Key, Value>::~Map()
	{
		Entry* entry = m_first;
		while (entry)
		{
			Entry* next = entry->next;
			delete entry;
			entry = next;
		}
		entry = nullptr;

		m_first = nullptr;
		m_last = nullptr;
	}

	template <typename Key, typename Value>
	void Map<Key, Value>::add(const Key& key, const Value& value)
	{
		Entry* entry = new Entry(key, value);

		if (m_last)
		{
			m_last->next = entry;
			entry->prev = m_last;
			m_last = entry;
		}
		else
		{
			m_first = entry;
			m_last = entry;
		}

		m_size++;
	}

	template <typename Key, typename Value>
	void Map<Key, Value>::remove(const Key& key)
	{
		Entry* entry = get_entry(key);

		if (!entry)
			return;

		if (entry->next)
			entry->next->prev = entry->prev;

		if (entry->prev)
			entry->prev->next = entry->next;

		if (entry == m_first)
			m_first = entry->next;

		if (entry == m_last)
			m_last = entry->prev;

		entry->next = nullptr;
		entry->prev = nullptr;
		delete entry;

		m_size--;
	}

	template <typename Key, typename Value>
	Value Map<Key, Value>::get(const Key& key) const
	{
		const Entry* entry = get_entry(key);

		if (entry)
			return entry->value;
		
		// wha tf u doin
		return 0;
	}

	template <typename Key, typename Value>
	typename Map<Key, Value>::Entry* Map<Key, Value>::get_entry(const Key& key)
	{
		for (Entry* it = m_first; it != nullptr; it = it->next)
		{
			if (it->key == key)
				return it;
		}

		return nullptr;
	}

	template <typename Key, typename Value>
	const typename Map<Key, Value>::Entry* Map<Key, Value>::get_entry(const Key& key) const
	{
		for (Entry* it = m_first; it != nullptr; it = it->next)
		{
			if (it->key == key)
				return it;
		}

		return nullptr;
	}

	template <typename Key, typename Value>
	int Map<Key, Value>::size() const
	{
		return m_size;
	}
>>>>>>> e5268c6ecd106d46bc506f7496d77d5c1f83ddfb
};
