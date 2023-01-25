#pragma once

#include <memory>

#include "Action.h"
#include "Story/StoryInformations.h"

class Event
{
public:
	~Event() { _action = nullptr; _context.clear(); _storyInformations.reset(); }

	void Run() 
	{
		if (auto storyInformations = _storyInformations.lock())
			_action->Run(storyInformations, _context);
	}
private:
	Event(std::shared_ptr<StoryInformations> storyInformations, Action* action,
		std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context) 
		: _action(action), _context(context), _storyInformations(storyInformations) { }

	Action* _action;
	std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> _context;
	std::weak_ptr<StoryInformations> _storyInformations;

	friend class Quest;
};

