#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <iostream>
#include <fstream>

#include "Data.h"

class ContentManager
{
public:
	static void Init(std::string root);

private:
	static std::string Root;
};

