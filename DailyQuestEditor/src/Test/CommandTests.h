#pragma once

#include "../Editor/CommandManager.h"

class AddCommand : public ICommand
{
public:
	AddCommand(float value, float* expression) { _value = value; _expression = expression; }
	void Do() override {
		*_expression = *_expression + _value;
	}
	void Undo() override {
		*_expression = *_expression - _value;
	}
private:
	float _value;
	float* _expression;
};
class MultCommand : public ICommand
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
private:
	float _value;
	float _dtExpression;
	float* _expression;
};