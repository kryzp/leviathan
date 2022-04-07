#pragma once

#include <lev/core/util.h>
#include <lev/containers/vector.h>
#include <lev/graphics/sprite_batch.h>

#define LEV_MAX_COMPONENT_TYPES 256

#define LEV_COMPONENT_DECL(_classname) \
public: \
    class Meta \
    { \
    private: \
        static int m_id; \
    public: \
        static inline int type_id() { return m_id; } \
    }; \

#define LEV_COMPONENT_IMPL(_classname) \
int _classname::Meta::m_id = ComponentRegistry::retrieve_type_id();

namespace lev
{
    class Entity;
    class EntityHandle;
    class EntityMgr;
    class Component;
    template <class T> class ComponentHandle;
    class ComponentRegistry;

    class ComponentRegistry
    {
    LEV_SINGLETON_CLASS(ComponentRegistry);

    public:
        template <class T>
        u16 retrieve_type_id()
        {
            static u16 id = m_counter++;
            return id;
        }

    private:
        u16 m_counter = 0;
    };

    class EntityHandle
    {
    public:
        EntityHandle();
        EntityHandle(u16 id, u16 index);
        EntityHandle(const Entity& entity);

        inline Entity* operator -> ();
        Entity* get();

        inline bool operator == (const EntityHandle& other) const;
        inline bool operator != (const EntityHandle& other) const;

    private:
        u16 m_id;
        u16 m_index;
    };

    class Entity
    {
    public:
        u16 id() const;
        u16 index() const;

        template <class T, typename... Args>
        ComponentHandle<T> add(Args&&... args);

        template <class T>
        ComponentHandle<T> get();

        template <class T>
        bool has();

        template <class T>
        void remove();

        bool enabled = true;

        Entity* next = nullptr;
        Entity* prev = nullptr;

    private:
        struct ComponentData // what to call this???
        {
            u16 id;
            u16 index;
            u8 type;

            ComponentData(u16 id, u16 index, u8 type)
                : id(id)
                , index(index)
                , type(type)
            {
            }
        };

        Vector<ComponentData> m_components;
        
        u16 m_id;
        u16 m_index;
    };

    class Component
    {
    public:
        virtual void init();
        virtual void destroyed();
        virtual void update();
        virtual void render(SpriteBatch& b);

        u16 id() const;
        u16 index() const;

        EntityHandle entity();

        bool enabled = true;

        Component* next = nullptr;
        Component* prev = nullptr;

    private:
        u16 m_id;
        u16 m_index;
        EntityHandle m_entity;
    };

    template <class T>
    class ComponentHandle
    {
    public:
        ComponentHandle(u16 id, u16 index);
        ComponentHandle(const Component& component);

        u16 id() const;
        u16 index() const;

        T* operator -> ();
        T* get();

        inline bool operator == (const ComponentHandle& other) const;
        inline bool operator != (const ComponentHandle& other) const;

    private:
        u16 m_id;
        u16 m_index;
    };

    class EntityMgr
    {
    LEV_SINGLETON_CLASS(EntityMgr);

    public:
        bool init();
        void destroy();

        void update();
        void render(SpriteBatch& b);

        EntityHandle add_entity();
        void remove_entity(const EntityHandle& handle);
        Entity* retrieve_entity(const EntityHandle& handle);

        template <class T, typename... Args>
        ComponentHandle<T>* add_component(const EntityHandle& handle, Args&&... args);

        template <class T>
        T* retrieve_component(const ComponentHandle<T>& handle);

        template <class T>
        void remove_component(const ComponentHandle<T>& handle);

    private:
        // todo
    };

    template <class T, typename... Args>
    ComponentHandle<T> Entity::add(Args&&... args)
    {
        EntityMgr::inst()->add_component<T>(EntityHandle(*this), args...);
    }

    template <class T>
    ComponentHandle<T> Entity::get()
    {
        for (int i = 0; i < m_components.size(); i++)
        {
            const auto& c = m_components[i];

            if (c.type == T::Meta::type_id())
                return ComponentHandle<T>(c.id, c.index);
        }

        return ComponentHandle<T>();
    }

    template <class T>
    bool Entity::has()
    {
        for (int i = 0; i < m_components.size(); i++)
        {
            const auto& c = m_components[i];

            if (c.type == T::Meta::type_id())
                return true;
        }

        return false;
    }

    template <class T>
    void Entity::remove()
    {
        for (int i = 0; i < m_components.size(); i++)
        {
            const auto& c = m_components[i];

            if (c.type == T::Meta::type_id())
            {
                EntityMgr::inst()->remove_component(ComponentHandle<T>(c.id, c.index));
                m_components.erase(i);
                return;
            }
        }
    }

    template <class T>
    ComponentHandle<T>::ComponentHandle(u16 id, u16 index)
        : m_id(id)
        , m_index(index)
    {
    }

    template <class T>
    ComponentHandle<T>::ComponentHandle(const Component& component)
        : m_id(component.id())
        , m_index(component.index())
    {
    }

    template <class T>
    u16 ComponentHandle<T>::id() const
    {
        return m_id;
    }

    template <class T>
    u16 ComponentHandle<T>::index() const
    {
        return m_index;
    }

    template <class T>
    T* ComponentHandle<T>::operator -> ()
    {
        return get();
    }

    template <class T>
    T* ComponentHandle<T>::get()
    {
        return EntityMgr::inst()->retrieve_component(*this);
    }

    template <class T>
    inline bool ComponentHandle<T>::operator == (const ComponentHandle<T>& other) const
    {
        return (
            (this->m_id == other.m_id) &&
            (this->m_id == other.m_id)
        );
    }

    template <class T>
    inline bool ComponentHandle<T>::operator != (const ComponentHandle<T>& other) const
    {
        return !(*this == other);
    }

    template <class T, typename... Args>
    ComponentHandle<T>* EntityMgr::add_component(const EntityHandle& handle, Args&&... args)
    {
    }

    template <class T>
    T* EntityMgr::retrieve_component(const ComponentHandle<T>& handle)
    {
    }

    template <class T>
    void EntityMgr::remove_component(const ComponentHandle<T>& handle)
    {
    }
}
