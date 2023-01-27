#pragma once

#include <vector>
#include <tuple>
#include <iostream>

#include "entt/entt.hpp"

#include "Entity.h"
//#include "StoryManager.h"

#include "Quest/Quest.h"
#include "StoryInformations.h"

#include "Core/TimeStep.h"

//class Entity;
//class StoryManager;
class Quest;

class Story
{
public:
	Story(const Story& story) 
	{
		_informations = story._informations;
	}
	~Story()
	{

	}

	Entity CreateEntity()
	{
		return Entity(_informations, _informations->GetRegistry().lock());
	}
	template <typename T, typename... Args>
	Entity CreateEntity(Args&&... args)
	{
		Entity entity = Entity(_informations, _informations->GetRegistry().lock());
		entity.Add<T, Args>(args);
		return entity;
	}

	std::weak_ptr<StoryInformations> GetInformations() { return _informations; }

	std::unique_ptr<Quest> CreateQuest()
	{
		return std::make_unique<Quest>(std::shared_ptr<Story>(this));
	}

	template<typename... Component>
	void ComputeForEachEntity(void (*f)(Entity entity, Component&... components))
	{
		auto all = _informations->GetAllEntities<Component...>();
		std::apply([&f, this](auto& entities, auto&... argsVectors) {
			for (int i = 0; i < std::min({ entities.size(), argsVectors.size()... }); ++i) {
				Entity e = CreateTempEntity(entities[i], _informations);
				f(e,  * argsVectors[i]...);
			}
			}, all);
	}
private:
	/*
	static void DrawEntity(Entity entity, Location& location, Size& size)
	{
		//std::cout << "Rendering an entity at " << location.X << "," << location.Y << " dude!" << std::endl;
	}
	*/
	static Entity CreateTempEntity(entt::entity entity, std::shared_ptr<StoryInformations> informations)
	{
		return Entity(entity, informations->GetRegistry().lock().get());
	}

	Story() 
	{
		_informations = std::make_shared<StoryInformations>();
	}
	void Update(TimeStep timestep) { }
	void Draw() 
	{
		//ComputeForEachEntity<Location, Size>(DrawEntity);
	}

	std::shared_ptr<StoryInformations> _informations;

	friend class Entity;
	friend class StoryManager;
};