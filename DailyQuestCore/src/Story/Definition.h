#pragma once

#include <memory>

#include "Name.h"
#include "Information.h"

class Definition
{
public:
	virtual bool IsValid(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context) = 0;
	virtual bool Contains(std::shared_ptr<Name> name) = 0;

	virtual void UpdateMatchingNames(std::unordered_map<std::shared_ptr<Name>, int>& matches) = 0;
	virtual void DisableUpdateForName(std::shared_ptr<Name> name) = 0;
	virtual float GetWeight() = 0;
	virtual int GetAverageNameCount() = 0;
};