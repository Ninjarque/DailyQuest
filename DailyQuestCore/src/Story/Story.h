#pragma once

#include <vector>
#include <tuple>

#include "entt/entt.hpp"

#include "Entity.h"
//#include "StoryManager.h"

#include "Quest/Quest.h"
#include "StoryInformations.h"

//class Entity;
//class StoryManager;
class Quest;

class Story
{
public:
	Story() 
	{
		_informations = std::make_shared<StoryInformations>();
	}
	Story(const Story& story) = default;
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

private:
	void Update(float deltaTime) { }
	void Draw() { }

	std::shared_ptr<StoryInformations> _informations;

	friend class Entity;
	friend class StoryManager;
};
