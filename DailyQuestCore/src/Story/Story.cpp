#include "Story.h"

Story::Story()
{
}

Story::~Story()
{
	StoryManager::Remove(this);
}

std::unique_ptr<Story> Story::Create()
{
	auto story = std::unique_ptr<Story>();
	StoryManager::Add(story.get());
	return story;
}

Entity Story::CreateEntity()
{
	auto handle = _registry.create();
	return Entity(handle, this);
}

template<typename T, typename ...Args>
inline Entity Story::CreateEntity(Args && ...args)
{
	auto handle = _registry.create();
	Entity entity = Entity(handle, this);
	entity.Add<T, Args>(args);
	return entity;
}

std::unique_ptr<Quest> Story::CreateQuest()
{
	return std::unique_ptr<Quest>();
}

void Story::Update(float deltaTime)
{
}

void Story::Draw()
{
}
