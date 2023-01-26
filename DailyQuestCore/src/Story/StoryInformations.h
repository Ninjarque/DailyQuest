#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include <entt/entt.hpp>

class StoryInformations
{
public:
	StoryInformations()
	{
		_registry = std::make_shared<entt::registry>();
	}
	~StoryInformations()
	{
		_registry.reset();
	}
	template<typename... Component>
	std::tuple<std::vector<Component*>...> GetAll()
	{
		std::tuple<std::vector<Component*>...> components;
		auto view = _registry->view<Component...>();
		for (auto entity : view) {
			((std::get<std::vector<Component*>>(components).push_back(&_registry->get<Component>(entity)), ...));
		}
		return components;
	}
	template<typename... Component>
	std::tuple<std::vector<entt::entity>, std::vector<Component*>...> GetAllEntities()
	{
		std::tuple<std::vector<entt::entity>, std::vector<Component*>...> components;
		auto view = _registry->view<Component...>();

		std::get<0>(components) = std::vector<entt::entity>();
		for (auto entity : view) {
			std::get<0>(components).push_back(entity);
			((std::get<std::vector<Component*>>(components).push_back(&_registry->get<Component>(entity)), ...));
		}
		return components;
	}
private:
	std::shared_ptr<entt::registry> _registry;

	std::weak_ptr<entt::registry> GetRegistry()
	{
		return _registry;
	}

	friend class Story;
};