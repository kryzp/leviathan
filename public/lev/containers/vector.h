#pragma once

#include <initializer_list>
#include <new>
#include <iostream>

#include <lev/core/util.h>
#include <lev/math/calc.h>

namespace lev
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
        u64 m_count;
        u64 m_size;
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
        : Vector()
    {
        allocate(data.size());
        m_count = data.size();

        for (u64 i = 0; i < m_size; i++)
            new (m_buf + i) T(data.begin()[i]);
    }

    template <typename T>
    Vector<T>::Vector(u64 initial_capacity)
        : Vector()
    {
        allocate(initial_capacity);
        m_count = initial_capacity;

        for (u64 i = 0; i < m_size; i++)
            new (m_buf + i) T();
    }
    
    template <typename T>
    Vector<T>::Vector(const Vector& other)
        : Vector()
    {
        if (other.m_size > 0)
        {
            allocate(other.m_size);
            clear();
            m_count = other.size();

            for (int i = 0; i < other.m_size; i++)
                new (m_buf + i) T(other.m_buf[i]);
        }
    }
    
    template <typename T>
    Vector<T>::Vector(Vector&& other) noexcept
    {
        clear();

        if (m_buf)
            ::operator delete (m_buf, sizeof(T) * m_size);

        this->m_size = std::move(other.m_size);
        this->m_count = std::move(other.m_count);
        this->m_buf = std::move(other.m_buf);

        other.m_size = 0;
        other.m_count = 0;
        other.m_buf = nullptr;
    }
    
    template <typename T>
    Vector<T>& Vector<T>::operator = (const Vector& other)
    {
        if (other.m_size > 0)
        {
            allocate(other.m_size);
            clear();
            m_count = other.size();

            for (int i = 0; i < other.m_size; i++)
                new (m_buf + i) T(other.m_buf[i]);
        }

        return *this;
    }
    
    template <typename T>
    Vector<T>& Vector<T>::operator = (Vector&& other) noexcept
    {
        clear();

        if (m_buf)
            ::operator delete (m_buf, sizeof(T) * m_size);

        this->m_size = std::move(other.m_size);
        this->m_count = std::move(other.m_count);
        this->m_buf = std::move(other.m_buf);

        other.m_size = 0;
        other.m_count = 0;
        other.m_buf = nullptr;

        return *this;
    }

    template <typename T>
    Vector<T>::~Vector()
    {
        clear();

        if (m_buf)
            ::operator delete (m_buf, sizeof(T) * m_size);

        m_buf = nullptr;
        m_size = 0;
        m_count = 0;
    }

    template <typename T>
    void Vector<T>::clear()
    {
        for (int i = 0; i < m_count; i++)
            m_buf[i].~T();

        m_count = 0;
    }

    template <typename T>
    void Vector<T>::allocate(u64 capacity)
    {
        if (capacity > m_size)
        {
            // 8 is just a nice number since vectors this small likely will have lots of rapid push/pop action
            u64 newsize = calc::max(8, m_size);
            
            while (newsize < capacity)
                newsize *= 2;

		    T* new_buf = (T*)::operator new (sizeof(T) * newsize);
			mem::set(new_buf, 0, sizeof(T) * newsize);

            for (int i = 0; i < m_count; i++)
            {
                if (i < m_size)
                    new (new_buf+i) T(std::move(m_buf[i]));

                m_buf[i].~T();
            }

            if (m_buf)
		        ::operator delete (m_buf, sizeof(T) * m_size);

            m_buf = new_buf;
            m_size = newsize;
        }
    }

    template <typename T>
    void Vector<T>::resize(u64 new_count)
    {
        if (new_count < m_count)
            erase(new_count, m_count - new_count);

        else if (new_count > m_count)
            expand(new_count - m_count);
    }

    template <typename T>
    void Vector<T>::erase(u64 index, u64 amount)
    {
        LEV_ASSERT(amount > 0, "Erase amount must be higher than 0");

        for (int i = 0; i < m_count - amount; i++)
            m_buf[i] = std::move(m_buf[i+amount]);

        for (int i = m_count - amount; i < m_count; i++)
            m_buf[i].~T();

        m_count -= amount;
    }

    template <typename T>
    void Vector<T>::expand(u64 amount)
    {
        LEV_ASSERT(amount > 0, "Expand amount must be higher than 0");

        allocate(m_count + amount);

        for (int i = 0; i < amount; i++)
            new (m_buf + m_count + i) T();
    }

    template <typename T>
    void Vector<T>::fill(byte value)
    {
        mem::set(m_buf, value, sizeof(T) * m_size);
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
        return m_buf[m_count - 1];
    }

    template <typename T>
    const T& Vector<T>::back() const
    {
        return m_buf[m_count - 1];
    }

    template <typename T>
    void Vector<T>::push_front(T item)
    {
        resize(m_count + 1);
        mem::move(m_buf + 1, m_buf, sizeof(T) * m_count);
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
        T item = std::move(m_buf[0]);
        m_buf[0].~T();

        for (int i = 0; i < m_count-1; i++)
            m_buf[i] = std::move(m_buf[i+1]);

        m_count--;
        return item;
    }

    template <typename T>
    T Vector<T>::pop_back()
    {
        T item = std::move(m_buf[m_count-1]);
        m_buf[m_count-1].~T();
        m_count--;
        return item;
    }

    template <typename T>
    u64 Vector<T>::size() const
    {
        return m_count;
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
