#pragma once

#include <initializer_list>
#include <new>
#include <iostream>

#include <lev/core/util.h>

namespace Lev
{
    // im pretty sure this is much worse than the standard library vector...
    // was still fun to make though!

	template <typename T>
	class Vector
	{
    public:
        Vector();
        Vector(std::initializer_list<T> data);
        Vector(int initial_capacity);

        ~Vector();

        void allocate(int capacity);
        void resize(int new_size);
        void erase(int index, int amount);
        void expand(int amount);

        void push_front(T item);
        void push_back(T item);

        T pop_front();
        T pop_back();

        void clear();
        int size() const;

        T* begin();
        const T* begin() const;
        T* end();
        const T* end() const;
        T* at(int i);
        const T* at(int i) const;

        T* operator [] (int i);
        const T* operator [] (int i) const;

    private:
        T* m_buf = nullptr;
        int m_count = 0;
        int m_size = 0;
	};

    template <typename T>
    Vector<T>::Vector()
		: m_buf(nullptr)
        , m_count(0)
        , m_size(0)
    {
    }
    
    template <typename T>
    Vector<T>::Vector(std::initializer_list<T> data)
    {
        allocate(data.size());
        m_size = m_count = data.size();

        for (int i = 0; i < m_size; i++)
            *(m_buf + i) = *(data.begin() + i);
    }

    template <typename T>
    Vector<T>::Vector(int initial_capacity)
        : m_size(initial_capacity)
        , m_buf(nullptr)
        , m_count()
    {
        allocate(initial_capacity);
    }

    template <typename T>
    Vector<T>::~Vector()
    {
        m_size = 0;
        m_count = 0;
        m_buf = 0;
    }

    template <typename T>
    void Vector<T>::clear()
    {
        m_count = 0;

        for (int i = 0; i < m_count; i++)
            m_buf[i].~T();

        m_buf = nullptr;
    }

    template <typename T>
    int Vector<T>::size() const
    {
        return m_count;
    }

    template <typename T>
    void Vector<T>::allocate(int capacity)
    {
        LEV_ASSERT(capacity > 0);



		T* new_buf = (T*)::operator new (sizeof(T) * capacity);

        for (int i = 0; i < m_count; i++)
        {
            if (i < m_size)
                new (new_buf+i) T(std::move(m_buf[i]));

            m_buf[i].~T();
        }

		::operator delete (m_buf, sizeof(T) * m_size);

        m_buf = new_buf;
        m_size = capacity;
    }

    template <typename T>
    void Vector<T>::resize(int new_size)
    {
        if (new_size < m_size)
            erase(new_size, m_size - new_size);

        else if (new_size > m_size)
            expand(new_size - m_size);
    }

    template <typename T>
    void Vector<T>::erase(int index, int amount)
    {
        LEV_ASSERT((index + amount) <= m_count);

        if (index + amount <= m_count)
            memmove(m_buf + index, m_buf + index + amount, sizeof(T) * amount);

        for (int i = (m_count - amount); i < m_count; i++)
            m_buf[i].~T();

        m_count -= amount;
    }

    template <typename T>
    void Vector<T>::expand(int amount)
    {
        LEV_ASSERT(amount > 0);

        allocate(m_count + amount);

        for (int i = 0; i < m_count; i++)
            new (m_buf + m_count + i) T();
    }

    template <typename T>
    void Vector<T>::push_front(T item)
    {
        resize(m_count + 1);
        memmove(m_buf + 1, m_buf, sizeof(T) * m_count);
        new (m_buf) T(std::move(item));
        m_count++;
    }

    template <typename T>
    void Vector<T>::push_back(T item)
    {
        resize(m_count + 1);
        new (m_buf + m_count) T(std::move(item));
        m_count++;
    }

    template <typename T>
    T Vector<T>::pop_front()
    {
        T item = std::move(*m_buf);
        (*m_buf).~T();
        memmove(m_buf - 1, m_buf, sizeof(T) * m_count);
        resize(m_count - 1);
        return item;
    }

    template <typename T>
    T Vector<T>::pop_back()
    {
        T item = std::move(*(m_buf + m_count - 1));
        (*(m_buf + m_count - 1)).~T();
        resize(m_count - 1);
        return item;
    }
    
    template <typename T>
    T* Vector<T>::begin()
    {
        return m_buf;
    }

    template <typename T>
    const T* Vector<T>::begin() const
    {
        return m_buf;
    }

    template <typename T>
    T* Vector<T>::end()
    {
        return m_buf + m_count;
    }

    template <typename T>
    const T* Vector<T>::end() const
    {
        return m_buf + m_count;
    }

    template <typename T>
    T* Vector<T>::at(int i)
    {
        return m_buf + i;
    }

    template <typename T>
    const T* Vector<T>::at(int i) const
    {
        return m_buf + i;
    }

    template<typename T>
    T* Vector<T>::operator [] (int i)
    {
        return at(i);
    }

    template<typename T>
    const T* Vector<T>::operator [] (int i) const
    {
        return at(i);
    }
}
