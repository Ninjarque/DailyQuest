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

	void DisposeNext()
	{
		if (_next != nullptr) _next->DisposeNext();
		if (_command != nullptr) delete _command;

		_next = nullptr;
		_command = nullptr;
	}

	void Push(ICommand* command)
	{
		if (_command != nullptr) _next->Push(command);
		else
		{
			_command = command;
			_next = new CommandQueue();
			_next->_previous = this;
		}
	}
	void SetCommand(ICommand* command)
	{
		_command = command;
		if (_next != nullptr)
		{
			_next->DisposeNext();
			delete _next;
			_next = nullptr;
		}
	}

private:
	CommandQueue* _previous = nullptr;
	CommandQueue* _next = nullptr;

	ICommand* _command = nullptr;
};

class CommandManager
{
public:
	bool CanDo();
	bool CanUndo();

	void Do();
	void Undo();
private:
	CommandQueue* _head;
	CommandQueue* _commands;
};

