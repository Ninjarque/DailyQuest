#pragma once

#include <vector>

class ICommand
{
public:
	void Execute() { Do(); _done = true; }
	void Revert() { Undo(); _done = false; }

	bool IsDone() { return _done; }
protected:
	virtual void Do() = 0;
	virtual void Undo() = 0;
private:
	bool _done = false;
};

class CommandQueue
{
public:

	CommandQueue()
	{
		_command = nullptr;
		_next = nullptr;
		_previous = nullptr;
	}

	void Dispose()
	{
		if (_previous != nullptr)
		{
			_previous->Dispose();
			delete _previous;
		}
		if (_next != nullptr)
		{
			_next->_previous = nullptr;
			if (_command == nullptr && _next->_command != nullptr)
			{
				delete _next->_command;
				_next->_command = nullptr;
			}
		}
		if (_command != nullptr) delete _command;
		_command = nullptr;
		_next = nullptr;
		_previous = nullptr;
	}
	void DisposeNext()
	{
		if (_next != nullptr)
		{
			_next->DisposeNext();
			delete _next;
		}
		if (_command != nullptr) delete _command;

		_next = nullptr;
		_command = nullptr;
	}

	void Push(ICommand* command)
	{
		if (_next == nullptr)
		{
			_next = new CommandQueue();
		}
		else
			_next->DisposeNext();
		_next->_command = command;
		_next->_previous = this;
	}
	CommandQueue* GetNext() { return _next; }
	CommandQueue* GetPrevious() { return _previous; }
	ICommand* GetCommand() { return _command; }

private:
	CommandQueue* _previous = nullptr;
	CommandQueue* _next = nullptr;

	ICommand* _command = nullptr;
};

class CommandManager
{
public:
	static void Init(int maxCommandQueueLength);

	static bool CanDo();
	static bool CanUndo();

	static void Do();
	static void Undo();

	static void Push(ICommand* command);

	static std::vector<ICommand*> GetCommands();
	static int GetCommandStackIndex();
	static int GetCommandCount();
private:
	static CommandQueue* _queueBegin;
	static CommandQueue* _commands;
	static int _queueLength;
	static int _maxQueueLength;
};

