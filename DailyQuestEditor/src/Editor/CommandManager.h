#pragma once

#include <vector>

class ICommand
{
public:
	virtual void Do() = 0;
	virtual void Undo() = 0;
};

class CommandQueue
{
public:

private:
	CommandQueue* _previous = nullptr;
	CommandQueue* _next = nullptr;

	ICommand* _command = nullptr;
};

class CommandManager
{
public:

private:
	CommandQueue _commands;
};

