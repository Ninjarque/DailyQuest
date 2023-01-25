#pragma once

#include <memory>
#include <string>

class Name
{
public:
	Name()
	{
		_value = "name(" + std::to_string(_count) + ")";
		_count++;
	}
	Name(std::string value) 
	{
		_value = value; 
		_count++;
	}
	~Name() { }
	static std::shared_ptr<Name> Create(std::string value);

	operator std::string() const { return _value; }
private:
	std::string _value;

	static int _count;
};