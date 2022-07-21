#pragma once

#include <initializer_list>
#include <new>
#include <iostream>

#include <lev/core/util.h>
#include <lev/math/calc.h>

namespace lv
{
	template <typename T>
	class Vector
	{
    public:
        Vector();
        
        Vector(std::initializer_list<T> data);
        Vector(u64 initial_capacity);

        Vector(const Vector& other);
        Vector(Vector&& other) noexcept;

        Vector& operator = (const Vector& other);
        Vector& operator = (Vector&& other) noexcept;

        ~Vector();

        void allocate(u64 capacity);
        void resize(u64 new_count);
        void erase(u64 index, u64 amount = 1);
        void expand(u64 amount = 1);
        void fill(byte value);

        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        void push_front(T item);
        void push_back(T item);

        T pop_front();
        T pop_back();

        void clear();
        u64 size() const;

        T* begin();
        const T* begin() const;
        T* end();
        const T* end() const;
        T* at(u64 idx);
        const T* at(u64 idx) const;

        T& operator [] (u64 idx);
        const T& operator [] (u64 idx) const;

    private:
        T* m_buf;
        u64 m_size;
        u64 m_capacity;
	};

    template <typename T>
    Vector<T>::Vector()
		: m_buf(nullptr)
        , m_size(0)
        , m_capacity(0)
    {
    }
    
    template <typename T>
    Vector<T>::Vector(std::initializer_list<T> data)
        : Vector()
    {
        allocate(data.size());
        m_size = data.size();

        for (u64 i = 0; i < m_size; i++)
            new (m_buf + i) T(data.begin()[i]);
    }

    template <typename T>
    Vector<T>::Vector(u64 initial_capacity)
        : Vector()
    {
        allocate(initial_capacity);
        m_size = initial_capacity;

        for (u64 i = 0; i < m_capacity; i++)
            new (m_buf + i) T();
    }
    
    template <typename T>
    Vector<T>::Vector(const Vector& other)
        : Vector()
    {
        if (other.m_capacity > 0)
        {
            allocate(other.m_capacity);
            clear();
            m_size = other.size();

            for (int i = 0; i < other.m_capacity; i++)
                new (m_buf + i) T(other.m_buf[i]);
        }
    }
    
    template <typename T>
    Vector<T>::Vector(Vector&& other) noexcept
    {
        this->m_capacity = std::move(other.m_capacity);
        this->m_size = std::move(other.m_size);
        this->m_buf = std::move(other.m_buf);

        other.m_capacity = 0;
        other.m_size = 0;
        other.m_buf = nullptr;
    }
    
    template <typename T>
    Vector<T>& Vector<T>::operator = (const Vector& other)
    {
        if (other.m_capacity > 0)
        {
            allocate(other.m_capacity);
            clear();
            m_size = other.size();

            for (int i = 0; i < other.m_capacity; i++)
                new (m_buf + i) T(other.m_buf[i]);
        }

        return *this;
    }
    
    template <typename T>
    Vector<T>& Vector<T>::operator = (Vector&& other) noexcept
    {
        clear();

        if (m_buf)
            ::operator delete (m_buf, sizeof(T) * m_capacity);

        this->m_capacity = std::move(other.m_capacity);
        this->m_size = std::move(other.m_size);
        this->m_buf = std::move(other.m_buf);

        other.m_capacity = 0;
        other.m_size = 0;
        other.m_buf = nullptr;

        return *this;
    }

    template <typename T>
    Vector<T>::~Vector()
    {
        clear();

        if (m_buf)
            ::operator delete (m_buf, sizeof(T) * m_capacity);

        m_buf = nullptr;
        m_capacity = 0;
        m_size = 0;
    }

    template <typename T>
    void Vector<T>::clear()
    {
        for (int i = 0; i < m_size; i++)
            m_buf[i].~T();

		mem::set(m_buf, 0, m_capacity * sizeof(T));

        m_size = 0;
    }

    template <typename T>
    void Vector<T>::allocate(u64 capacity)
    {
        if (capacity > m_capacity)
        {
            // 8 is just a nice number since vectors this small likely will have lots of rapid push/pop action
            u64 newsize = calc::max(8, m_capacity);
            
            while (newsize < capacity)
                newsize *= 2;

		    T* new_buf = (T*)::operator new (sizeof(T) * newsize);
			mem::set(new_buf, 0, sizeof(T) * newsize);

            for (int i = 0; i < m_size; i++)
            {
                if (i < m_capacity)
                    new (new_buf+i) T(std::move(m_buf[i]));

                m_buf[i].~T();
            }

            if (m_buf)
		        ::operator delete (m_buf, sizeof(T) * m_capacity);

            m_buf = new_buf;
            m_capacity = newsize;
        }
    }

    template <typename T>
    void Vector<T>::resize(u64 new_count)
    {
        if (new_count < m_size)
            erase(new_count, m_size - new_count);

        else if (new_count > m_size)
            expand(new_count - m_size);
    }

    template <typename T>
    void Vector<T>::erase(u64 index, u64 amount)
    {
		if (amount > 0)
		{
			for (int i = 0; i < m_size - amount; i++)
				m_buf[i] = std::move(m_buf[i + amount]);

			for (int i = m_size - amount; i < m_size; i++)
				m_buf[i].~T();

			m_size -= amount;
		}
    }

    template <typename T>
    void Vector<T>::expand(u64 amount)
    {
        LEV_ASSERT(amount > 0, "Expand amount must be higher than 0");

        allocate(m_size + amount);

        for (int i = 0; i < amount; i++)
            new (m_buf + m_size + i) T();
    }

    template <typename T>
    void Vector<T>::fill(byte value)
    {
        mem::set(m_buf, value, sizeof(T) * m_capacity);
    }

    template <typename T>
    T& Vector<T>::front()
    {
        return m_buf[0];
    }

    template <typename T>
    const T& Vector<T>::front() const
    {
        return m_buf[0];
    }

    template <typename T>
    T& Vector<T>::back()
    {
        return m_buf[m_size - 1];
    }

    template <typename T>
    const T& Vector<T>::back() const
    {
        return m_buf[m_size - 1];
    }

    template <typename T>
    void Vector<T>::push_front(T item)
    {
        resize(m_size + 1);
        mem::move(m_buf + 1, m_buf, sizeof(T) * m_size);
        new (m_buf) T(std::move(item));
        m_size++;
    }

    template <typename T>
    void Vector<T>::push_back(T item)
    {
        resize(m_size + 1);
        new (m_buf + m_size) T(std::move(item));
        m_size++;
    }

    template <typename T>
    T Vector<T>::pop_front()
    {
        T item = std::move(m_buf[0]);
        m_buf[0].~T();

        for (int i = 0; i < m_size-1; i++)
            m_buf[i] = std::move(m_buf[i+1]);

        m_size--;
        return item;
    }

    template <typename T>
    T Vector<T>::pop_back()
    {
        T item = std::move(m_buf[m_size-1]);
        m_buf[m_size-1].~T();
        m_size--;
        return item;
    }

    template <typename T>
    u64 Vector<T>::size() const
    {
        return m_size;
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
        return m_buf + m_size;
    }

    template <typename T>
    const T* Vector<T>::end() const
    {
        return m_buf + m_size;
    }

    template <typename T>
    T* Vector<T>::at(u64 idx)
    {
        return m_buf + idx;
    }

    template <typename T>
    const T* Vector<T>::at(u64 idx) const
    {
        return m_buf + idx;
    }

    template<typename T>
    T& Vector<T>::operator [] (u64 idx)
    {
        return m_buf[idx];
    }

    template<typename T>
    const T& Vector<T>::operator [] (u64 idx) const
    {
        return m_buf[idx];
    }
}
