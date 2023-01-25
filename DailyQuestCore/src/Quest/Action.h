#pragma once
#include <memory>
#include <unordered_map>

#include "Name.h"
#include "Information.h"
#include "Story/StoryInformations.h"

class Story;

class Action
{
public:
    virtual void Run(std::shared_ptr<StoryInformations> storyInformations, std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context) = 0;
};