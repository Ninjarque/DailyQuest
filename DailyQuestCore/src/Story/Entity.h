#pragma once

#include <entt/entt.hpp>

#include "Error.h"

#include "Quest/Name.h"
#include "StoryInformations.h"

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

	template<typename T, typename ...Args>
	T& Set(Args&&... args);

	Entity()
	{
	}
	Entity(const Entity& entity)
	{
		_storyInformations = std::weak_ptr<StoryInformations>(entity._storyInformations);
		_handle = std::shared_ptr<entt::entity>(entity._handle);
		_registry = entity._registry;
	}
	~Entity()
	{
		if (_isTemp)
			return;
		if (_handle.use_count() == 1)
			_registry->destroy(*(_handle.get()));
		//_handle.reset();
		//_storyInformations.reset();
	}

	operator bool() const { return _registry != nullptr && !_storyInformations.expired(); }
private:
	Entity(std::weak_ptr<StoryInformations> storyInformations, std::shared_ptr<entt::registry> registry)
	{
		_storyInformations = std::weak_ptr<StoryInformations>(storyInformations);
		if (auto informations = storyInformations.lock())
		{
			_handle = std::make_shared<entt::entity>(registry->create());
			_registry = registry.get();
			Add<Name>();
		}
	}
	Entity(entt::entity handle, entt::registry* registry)
	{
		_handle = std::make_shared<entt::entity>(handle);
		_registry = registry;
		_isTemp = true;
	}

	std::shared_ptr<entt::entity> _handle;
	entt::registry* _registry = nullptr;
	std::weak_ptr<StoryInformations> _storyInformations;
	bool _isTemp = false;

	friend class Story;
};

template<typename T>
bool Entity::Has()
{
	return _registry->all_of<T>(*(_handle.get()));
}

template<typename T>
T& Entity::Get()
{
	if (!Has<T>()) Error::fatalError("Could not get component because it hasn't been added yet!");

	return _registry->get<T>(*(_handle.get()));
}


template<typename T, typename ...Args>
T& Entity::Add(Args&&... args)
{
	if (Has<T>()) Error::fatalError("Could not add component because it already has been added!");

	return _registry->emplace<T>(*(_handle.get()), std::forward<Args>(args)...);
}

template<typename T, typename ...Args>
T& Entity::Set(Args&&... args)
{
	return _registry->emplace_or_replace<T>(*(_handle.get()), std::forward<Args>(args)...);
}

template<typename T>
bool Entity::Remove()
{
	if (!Has<T>())
	{
		Error::fatalError("Could not remove component because it hasn't been added yet!");
		return false;
	}
	_registry->remove<T>(*(_handle.get()));
	return true;
}
