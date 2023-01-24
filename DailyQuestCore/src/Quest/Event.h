#pragma once

#include <memory>

#include "Action.h"

class Event
{
public:
	Event(Action* action, std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context) : _action(action), _context(context) { }
	~Event() { _action = nullptr; _context.clear(); }

	void Run() { _action->Run(_context); }
private:
	Action* _action;
	std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> _context;
};

