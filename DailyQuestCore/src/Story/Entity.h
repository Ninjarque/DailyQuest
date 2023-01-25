#pragma once

#include <entt/entt.hpp>

#include "Error.h"

#include "Quest/Name.h"

class Entity
{
public:
	template <typename T>
	bool Has();

	template <typename T>
	T& Get();

	template <typename T, typename... Args>
	T& Add(Args&&... args);

	template <typename T>
	bool Remove();

	Entity()
	{
	}
	~Entity()
	{
		_registry->destroy(_handle);
	}
private:
	Entity(std::shared_ptr<entt::registry> registry)
	{
		_handle = registry->create();
		_registry = registry.get();
		Add<std::shared_ptr<Name>>();
	}

	entt::entity _handle{0};
	entt::registry* _registry = nullptr;

	friend class Story;
};

template<typename T>
bool Entity::Has()
{
	return _registry->all_of<T>(_handle);
}

template<typename T>
T& Entity::Get()
{
	if (!Has<T>()) Error::fatalError("Could not get component because it hasn't been added yet!");

	return _registry->get<T>(_handle);
}


template<typename T, typename ...Args>
T& Entity::Add(Args&&... args)
{
	//if (Has<T>()) Error::fatalError("Could not add component because it already has been added!");

	return _registry->get_or_emplace<T>(_handle, std::forward<Args>(args)...);
}

template<typename T>
bool Entity::Remove()
{
	if (!Has<T>())
	{
		Error::fatalError("Could not remove component because it hasn't been added yet!");
		return false;
	}
	_registry->remove<T>(_handle);
	return true;
}
