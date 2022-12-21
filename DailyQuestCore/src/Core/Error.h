#pragma once

#include <iostream>
#include <string>

class Error
{
public:
	static void fatalError(std::string text);
	static void warning(std::string text);
};

