#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "Story.h"

class Story;

class StoryManager
{
public:
	static void Init();
	static void Update(float deltaTime);
	static void Draw();

private:
	static void Add(Story* story);
	static void Remove(Story* story);

	static std::unordered_map<Story*, int> _stories;

	friend class Story;
};

