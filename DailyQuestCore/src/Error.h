#pragma once

#include <iostream>
#include <string>

#include "Application.h"

class Error
{
public:
	static void fatalError(std::string text);
	static void warning(std::string text);
};

