#pragma once

#include "../Editor/CommandManager.h"
#include <string>

class PrintableCommand
{
public:
	virtual std::string ToString() = 0;
};

class AddCommand : public ICommand, PrintableCommand
{
public:
	AddCommand(float value, float* expression) { _value = value; _expression = expression; }
	void Do() override {
		*_expression = *_expression + _value;
	}
	void Undo() override {
		*_expression = *_expression - _value;
	}
	bool TryMerge(ICommand* other) override {
		if (AddCommand* command = dynamic_cast<AddCommand*>(other))
		{
			Undo();
			_value += command->_value;
			Do();
			return true;
		}
		return false;
	}
	std::string ToString() override {
		return "Add (" + std::to_string(_value) + ")";
	}
private:
	float _value;
	float* _expression;
};
class MultCommand : public ICommand, PrintableCommand
{
public:
	MultCommand(float value, float* expression) { _value = value; _expression = expression; _dtExpression = *expression; }
	void Do() override {
		_dtExpression = *_expression;
		*_expression = *_expression * _value;
	}
	void Undo() override {
		*_expression = _dtExpression;
	}
	bool TryMerge(ICommand* other) override {
		if (MultCommand* command = dynamic_cast<MultCommand*>(other))
		{
			Undo();
			_value *= command->_value;
			Do();
			return true;
		}
		return false;
	}
	std::string ToString() override {
		return "Mult (" + std::to_string(_value) + ")";
	}
private:
	float _value;
	float _dtExpression;
	float* _expression;
};