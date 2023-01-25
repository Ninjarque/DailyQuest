#pragma once

#include <vector>
#include <tuple>

#include "entt/entt.hpp"

#include "Entity.h"
//#include "StoryManager.h"

#include "Quest/Quest.h"

//class Entity;
//class StoryManager;

class Story
{
public:
	~Story()
	{

	}

	Entity CreateEntity()
	{
		return Entity(_registry);
	}
	template <typename T, typename... Args>
	Entity CreateEntity(Args&&... args)
	{
		Entity entity = Entity(_registry);
		entity.Add<T, Args>(args);
		return entity;
	}


	std::unique_ptr<Quest> CreateQuest()
	{
		return std::unique_ptr<Quest>();
	}

private:
	Story() { }
	void Update(float deltaTime) { }
	void Draw() { }

	template<typename... Component>
	std::tuple<std::vector<Component>...> GetAll()
	{
		std::tuple<std::vector<Component>...> components;
		auto view = _registry->view<Component...>();
		for (auto entity : view) {
			((std::get<std::vector<Component>>(components).push_back(_registry->get<Component>(entity)), ...));
		}
		return components;
	}
	
	std::shared_ptr<entt::registry> _registry;
	std::weak_ptr<Story> _self;

	friend class Entity;
	friend class StoryManager;
};
