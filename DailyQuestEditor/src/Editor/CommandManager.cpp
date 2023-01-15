#include "CommandManager.h"

CommandQueue* CommandManager::_queueBegin;
CommandQueue* CommandManager::_commands;
int CommandManager::_queueLength;
int CommandManager::_maxQueueLength;

void CommandManager::Init(int maxCommandQueueLength)
{
	_commands = new CommandQueue();
	_queueBegin = _commands;
	_queueLength = 0;
	_maxQueueLength = maxCommandQueueLength;
}

bool CommandManager::CanDo()
{
	CommandQueue* next = _commands->GetNext();
	if (next == nullptr)
		return false;
	return next->GetCommand() != nullptr;
}

bool CommandManager::CanUndo()
{
	ICommand* command = _commands->GetCommand();
	if (command == nullptr)
		return false;
	return command->IsDone();
}

void CommandManager::Do()
{
	if (CanDo())
	{
		_commands = _commands->GetNext();
		_commands->GetCommand()->Execute();
		_queueLength++;
	}
}

void CommandManager::Undo()
{
	if (CanUndo())
	{
		_commands->GetCommand()->Revert();
		_commands = _commands->GetPrevious();
		_queueLength--;
	}
}

void CommandManager::Push(ICommand* command)
{
	_commands->Push(command);
	Do();

	while (_queueLength > _maxQueueLength && _maxQueueLength > 0)
	{
		CommandQueue* next = _queueBegin->GetNext();
		_queueBegin->Dispose();
		delete _queueBegin;
		_queueBegin = next;
		_queueLength--;
	}
}
