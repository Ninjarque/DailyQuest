#pragma once

#include "entt/entt.hpp"

#include "Entity.h"
#include "StoryManager.h"

#include "Quest/Quest.h"

class Entity;
class StoryManager;

class Story
{
public:
	Story();
	~Story();
	static std::unique_ptr<Story> Create();

	Entity CreateEntity();
	template <typename T, typename... Args>
	Entity CreateEntity(Args&&... args);


	std::unique_ptr<Quest> CreateQuest();


	void Update(float deltaTime);
	void Draw();

private:

	entt::registry _registry;
	std::weak_ptr<Story> _self;

	friend class Entity;
};
