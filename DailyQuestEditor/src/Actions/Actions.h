#pragma once

#include "Quest/Action.h"
#include "Story/StoryManager.h"
#include "Story/Story.h"

class AddEntityAction : public Action
{
public:
	AddEntityAction(std::vector<Entity>& container) : _container(container)
	{
	}
	void Run(std::shared_ptr<StoryInformations> storyInformations, 
		std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context)
	{
		std::shared_ptr<Story> story = StoryManager::GetStory(storyInformations);
		_container.push_back(story->CreateEntity());
	}
private:
	std::vector<Entity>& _container;
};