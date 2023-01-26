#pragma once

#include <memory>
#include <string>
#include <iostream>

class Name
{
public:
	Name()
	{
		_value = "name(" + std::to_string(_count) + ")";
		_count++;
	}
	~Name() { }
	Name(std::string value) 
	{
		_value = value; 
		_count++;
	}
	static std::shared_ptr<Name> Create(std::string value);

	operator std::string() const { return _value; }
	bool operator==(const Name& other) const {
		return _value == other._value;
	}

	friend std::ostream& operator<<(std::ostream& os, const Name& name) {
		os << name._value;
		return os;
	}
private:
	std::string _value;

	static int _count;
};

namespace std {
	template <>
	struct hash<Name>
	{
		size_t operator()(const Name& name) const
		{
			return hash<std::string>()((std::string)name);
		}
	};
}