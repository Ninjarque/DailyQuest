#pragma once

#include "Quest/Requirement.h"

#include <vector>
#include <memory>

class CompositeRequierement : public Requirement
{
public:
	CompositeRequierement(std::vector<std::shared_ptr<Requirement>> requirements);
	~CompositeRequierement();

	bool IsValid(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context) override;
	bool Has(std::shared_ptr<Name> name)  override;

	void UpdateMatchingNames(std::unordered_map<std::shared_ptr<Name>, int>& matches)  override;
	void DisableUpdateForName(std::shared_ptr<Name> name)  override;
	void ClearDisabledUpdatesForAllNames() override;
	float GetWeight()  override;
	int GetAverageNameCount()  override;

private:
	std::vector<std::shared_ptr<Requirement>> _requirements;
};

class HasNameRequirement : public Requirement
{
public:
	HasNameRequirement(std::shared_ptr<Name> target, float weight = 1.0f);
	~HasNameRequirement();

	bool IsValid(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context) override;
	bool Has(std::shared_ptr<Name> name)  override;

	void UpdateMatchingNames(std::unordered_map<std::shared_ptr<Name>, int>& matches)  override;
	void DisableUpdateForName(std::shared_ptr<Name> name)  override;
	void ClearDisabledUpdatesForAllNames() override;
	float GetWeight()  override;
	int GetAverageNameCount()  override;
private:
	std::shared_ptr<Name> _target;
	float _weight;
	bool _disabledName;
};