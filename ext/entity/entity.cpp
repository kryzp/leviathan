#include <lev/entity/entity.h>

using namespace lv;

void Component::init()
{
}

void Component::destroyed()
{
}

void Component::update()
{
}

void Component::render(SpriteBatch& b)
{
}

u16 Component::id() const
{
    return m_id;
}

u16 Component::index() const
{
    return m_index;
}

EntityHandle Component::entity()
{
    return m_entity;
}

///////////////////////////////////////////////////////////////////////////

EntityHandle::EntityHandle()
    : m_id(0)
    , m_index(0)
{
}

EntityHandle::EntityHandle(u16 id, u16 index)
    : m_id(id)
    , m_index(index)
{
}

EntityHandle::EntityHandle(const Entity& entity)
    : m_id(entity.id())
    , m_index(entity.index())
{
}

inline Entity* EntityHandle::operator -> ()
{
    return get();
}

Entity* EntityHandle::get()
{
    return EntityMgr::inst()->retrieve_entity(*this);
}

inline bool EntityHandle::operator == (const EntityHandle& other) const
{
    return (
        (this->m_id == other.m_id) &&
        (this->m_index == other.m_index)
    );
}

inline bool EntityHandle::operator != (const EntityHandle& other) const
{
    return !(*this == other);
}

///////////////////////////////////////////////////////////////////////////

u16 Entity::id() const
{
    return m_id;
}

u16 Entity::index() const
{
    return m_index;
}

///////////////////////////////////////////////////////////////////////////

bool EntityMgr::init()
{
    return true;
}

void EntityMgr::destroy()
{
}

void EntityMgr::update()
{
}

void EntityMgr::render(SpriteBatch& b)
{
}

EntityHandle EntityMgr::add_entity()
{
}

void EntityMgr::remove_entity(const EntityHandle& handle)
{
}

Entity* EntityMgr::retrieve_entity(const EntityHandle& handle)
{
}
