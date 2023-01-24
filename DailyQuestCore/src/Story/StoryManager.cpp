#include "StoryManager.h"

std::unordered_map<Story*, int> StoryManager::_stories;

void StoryManager::Init()
{
}

void StoryManager::Update(float deltaTime)
{
	for (auto& story : _stories)
	{
		story.first->Update(deltaTime);
	}
}

void StoryManager::Draw()
{
	for (auto& story : _stories)
	{
		story.first->Draw();
	}
}

void StoryManager::Add(Story* story)
{
	_stories[story] = _stories.size();
}

void StoryManager::Remove(Story* story)
{
	_stories.erase(story);
}
