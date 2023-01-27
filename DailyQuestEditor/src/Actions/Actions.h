#pragma once

#include "Quest/Action.h"

class AddEntityAction : public Action
{
	void Run(std::shared_ptr<StoryInformations> storyInformations, 
		std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context)
	{

	}
};