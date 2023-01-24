#include "Entity.h"

Entity::Entity()
{
}

Entity::Entity(entt::entity handle, Story* story)
{
	_handle = handle;
	_story = story;
	Add<Name>();
}

Entity::~Entity()
{
	_story->_registry.destroy(_handle);
}


template<typename T>
inline bool Entity::Has()
{
	return _story->_registry.all_of<T>(_handle);
}

template<typename T>
T& Entity::Get()
{
	if (!Has<T>()) Error::fatalError("Could not get component because it hasn't been added yet!");
	return _story->_registry.get<T>(_handle);
}

template<typename T, typename ...Args>
T& Entity::Add(Args&&... args)
{
	if (Has<T>()) Error::fatalError("Could not add component because it already has been added!");
	return _story->_registry.emplace<T>(_handle, std::forward<Args>(args)...);
}

template<typename T>
bool Entity::Remove()
{
	if (!Has<T>())
	{
		Error::fatalError("Could not remove component because it hasn't been added yet!");
		return false;
	}
	_story->_registry.remove<T>(_handle);
	return true;
}
