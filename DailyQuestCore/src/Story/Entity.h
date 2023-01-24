#pragma once

#include <entt/entt.hpp>

#include "Story.h"
#include "Error.h"

class Story;

class Entity
{
public:
	template <typename T>
	bool Has();

	template <typename T, typename... Args>
	T& Add(Args&&... args);

	template <typename T>
	T& Get();

	template <typename T>
	bool Remove();

	Entity();
private:
	Entity(entt::entity handle, Story* story);
	Entity(const Entity& entity) = default;
	~Entity();

	entt::entity _handle{0};
	Story* _story = nullptr;

	friend class Story;
};