#include "StoryManager.h"

std::vector<std::weak_ptr<Story>> StoryManager::_stories;
std::unordered_map<std::shared_ptr<StoryInformations>, std::weak_ptr<Story>> StoryManager::_storiesFromInformations;
