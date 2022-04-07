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

    class EntityHandle
    {
    public:
        EntityHandle();
        EntityHandle(u16 id, u16 index);
        EntityHandle(const Entity& entity);

        inline Entity* operator -> ();
        inline const Entity* operator -> () const;

        Entity* get();
        const Entity* get() const;

        inline bool operator == (const EntityHandle& other) const;
        inline bool operator != (const EntityHandle& other) const;

    private:
        u16 m_id;
        u16 m_index;
    };

    class Entity
    {
    public:
        u16 id();
        u16 index();

        template <class T, typename... Args>
        ComponentHandle<T> add(Args&&... args);

        template <class T>
        ComponentHandle<T> get();

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
        ComponentHandle(const Component* component);

        u16 id() const;
        u16 index() const;

        T* operator -> ();
        const T* operator -> () const;

        T* get();
        const T* get() const;

        inline bool operator == (const ComponentHandle& other) const;
        inline bool operator != (const ComponentHandle& other) const;

    private:
        u16 m_id;
        u16 m_index;
    };

	class ComponentRegistry
	{
		LEV_SINGLETON_CLASS(ComponentRegistry)

	public:
		template <class Asset, typename... LoadData>
		u16 retrieve_type_id()
		{
			static u16 id = m_counter++;
			return id;
		}

	private:
		u16 m_counter = 0;
	};

    class EntityMgr
    {
        LEV_SINGLETON_CLASS(EntityMgr)

    public:
        bool init();
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
}
