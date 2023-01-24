#pragma once

#include <memory>
#include <unordered_map>

#include "Name.h"
#include "Information.h"

class Action
{
public:
	virtual void Run(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context) = 0;
};