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
	std::string ToString() override {
		return "Mult (" + std::to_string(_value) + ")";
	}
private:
	float _value;
	float _dtExpression;
	float* _expression;
};