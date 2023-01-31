#pragma once

#include <unordered_map>
#include <vector>
#include <tuple>
#include <memory>

#include "Story.h"
//#include "Quest/Quest.h"

#include "Core/TimeStep.h"

class StoryManager
{
public:
	static void Init()
	{

	}
	static void Update(TimeStep timestep)
	{
		bool needsStoryUpdate = false;
		for (auto s = _stories.begin(); s != _stories.end(); )
		{
			if (auto story = s->lock())
			{
				story->Update(timestep);
				++s;
			}
			else
			{
				//s = std::remove(_stories.begin(), _stories.end(), s);
				s = _stories.erase(s);
				needsStoryUpdate = true;
			}
		}
		if (needsStoryUpdate) UpdateStories();
	}

	static void Draw()
	{
		bool needsStoryUpdate = false;
		for (auto s = _stories.begin(); s != _stories.end(); )
		{
			if (auto story = s->lock())
			{
				story->Draw();
				++s;
			}
			else
			{
				//s = std::remove(_stories.begin(), _stories.end(), s);
				s = _stories.erase(s);
				needsStoryUpdate = true;
			}
		}
		if (needsStoryUpdate) UpdateStories();
	}

	static std::shared_ptr<Story> CreateStory()
	{
		auto story = std::shared_ptr<Story>(new Story());
		_stories.push_back(story);
		_storiesFromInformations[story->GetInformations().lock()] = story;
		return story;
	}
	static std::shared_ptr<Story> GetStory(std::shared_ptr<StoryInformations> storyInformations)
	{
		if (_storiesFromInformations.count(storyInformations))
			return _storiesFromInformations[storyInformations].lock();
		return nullptr;
	}

	template<typename... Component>
	static std::vector<std::tuple<std::vector<Component*>...>> GetInAllStories();

	template<typename... Component>
	static void ComputeForEach(void (*f)(Component&... components));

	template<typename... Component>
	static void ComputeForEachEntity(void (*f)(Entity, Component&... components));

private:
	static std::vector<std::weak_ptr<Story>> _stories;
	static std::unordered_map<std::shared_ptr<StoryInformations>, std::weak_ptr<Story>> _storiesFromInformations;

	static void UpdateStories()
	{
		_storiesFromInformations.clear();
		for (auto s = _stories.begin(); s != _stories.end(); )
		{
			if (auto story = s->lock())
			{
				_storiesFromInformations[story->GetInformations().lock()] = story;
				++s;
			}
		}
	}

	//friend class Story;
};


template<typename... Component>
static std::vector<std::tuple<std::vector<Component*>...>> StoryManager::GetInAllStories()
{
	bool needsStoryUpdate = false;
	std::vector<std::tuple<std::vector<Component*>...>> listOfComponents;
	for (auto s = _stories.begin(); s != _stories.end(); )
	{
		if (auto story = s->lock())
		{
			if (auto informations = story->GetInformations().lock())
			{
				listOfComponents.push_back(informations->GetAll<Component...>());
				++s;
			}
			else
			{
				s = _stories.erase(s);
				needsStoryUpdate = true;
			}
		}
		else
		{
			s = _stories.erase(s);
			needsStoryUpdate = true;
		}
	}
	if (needsStoryUpdate) UpdateStories();
	return listOfComponents;
}

template<typename... Component>
static void StoryManager::ComputeForEach(void (*f)(Component&... components))
{
	auto all = GetInAllStories<Component...>();
	for (auto& tuple : all)
	{
		std::apply([&f](auto&... argsVectors) {
			for (int i = 0; i < std::min({ argsVectors.size()... }); ++i) {
				f(*argsVectors[i]...);
			}
			}, tuple);
	}
}

template<typename... Component>
static void StoryManager::ComputeForEachEntity(void (*f)(Entity, Component&... components))
{
	bool needsStoryUpdate = false;
	for (auto s = _stories.begin(); s != _stories.end(); )
	{
		if (auto story = s->lock())
		{
			story->ComputeForEachEntity(f);
			++s;
		}
		else
		{
			s = _stories.erase(s);
			needsStoryUpdate = true;
		}
	}
	if (needsStoryUpdate) UpdateStories();
}