#pragma once

#include <memory>
#include <unordered_map>

#include "Name.h"
#include "Information.h"

class Requirement
{
public:
	virtual bool IsValid(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context) = 0;
	virtual bool Has(std::shared_ptr<Name> name) = 0;

	virtual void UpdateMatchingNames(std::unordered_map<std::shared_ptr<Name>, int>& matches) = 0;
	virtual void DisableUpdateForName(std::shared_ptr<Name> name) = 0;
	virtual float GetWeight() = 0;
	virtual int GetAverageNameCount() = 0;
};