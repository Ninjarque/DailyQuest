#pragma once

#include <unordered_map>
#include <vector>
#include <tuple>
#include <memory>

#include "Story.h"
#include "Quest/Quest.h"

#include "Core/TimeStep.h"

class StoryManager
{
public:
	static void Init()
	{

	}
	static void Update(TimeStep timestep)
	{
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
			}
		}
	}

	static void Draw()
	{
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
			}
		}
	}

	static std::shared_ptr<Story> CreateStory()
	{
		auto story = std::shared_ptr<Story>(new Story());
		_stories.push_back(story);
		return story;
	}

	static std::unique_ptr<Quest> CreateQuest(std::shared_ptr<Story> targetStory)
	{
		return std::make_unique<Quest>(targetStory);
	}

	template<typename... Component>
	static std::vector<std::tuple<std::vector<Component*>...>> GetInAllStories();

	template<typename... Component>
	static void ComputeForEach(void (*f)(Component&... components));

	template<typename... Component>
	static void ComputeForEachEntity(void (*f)(Entity, Component&... components));

private:
	static std::vector<std::weak_ptr<Story>> _stories;

	//friend class Story;
};

template<typename... Component>
static std::vector<std::tuple<std::vector<Component*>...>> StoryManager::GetInAllStories()
{
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
				s = _stories.erase(s);
		}
		else
			s = _stories.erase(s);
	}
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
	for (auto s = _stories.begin(); s != _stories.end(); )
	{
		if (auto story = s->lock())
		{
			story->ComputeForEachEntity(f);
			++s;
		}
		else
			s = _stories.erase(s);
	}
}