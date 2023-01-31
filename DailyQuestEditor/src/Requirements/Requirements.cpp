#include "Requirements.h"

CompositeRequierement::CompositeRequierement(std::vector<std::shared_ptr<Requirement>> requirements)
{
	_requirements = requirements;
}

CompositeRequierement::~CompositeRequierement()
{
	for (auto r : _requirements)
		r.reset();
	_requirements.clear();
}

bool CompositeRequierement::IsValid(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context)
{
	for (auto r : _requirements)
	{
		if (!r->IsValid(context))
			return false;
	}
	return true;
}

bool CompositeRequierement::Has(std::shared_ptr<Name> name)
{
	for (auto r : _requirements)
	{
		if (r->Has(name))
			return true;
	}
	return false;
}

void CompositeRequierement::UpdateMatchingNames(std::unordered_map<std::shared_ptr<Name>, int>& matches)
{
	for (auto r : _requirements)
	{
		r->UpdateMatchingNames(matches);
	}
}

void CompositeRequierement::DisableUpdateForName(std::shared_ptr<Name> name)
{
	for (auto r : _requirements)
	{
		r->DisableUpdateForName(name);
	}
}

void CompositeRequierement::ClearDisabledUpdatesForAllNames()
{
	for (auto r : _requirements)
	{
		r->ClearDisabledUpdatesForAllNames();
	}
}

float CompositeRequierement::GetWeight()
{
	float weights = 0.0f;
	for (auto r : _requirements)
	{
		weights += r->GetWeight();
	}
	return weights;
}

int CompositeRequierement::GetAverageNameCount()
{
	float averageNameCount = 0.0f;
	for (auto r : _requirements)
	{
		averageNameCount += r->GetAverageNameCount();
	}
	return averageNameCount;
}

HasNameRequirement::HasNameRequirement(std::shared_ptr<Name> target, float weight)
{
	_target = target;
	_weight = weight;
	_disabledName = false;
}

HasNameRequirement::~HasNameRequirement()
{
	_target.reset();
}

bool HasNameRequirement::IsValid(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context)
{
	if (context.count(_target))
		return true;
	return false;
}

bool HasNameRequirement::Has(std::shared_ptr<Name> name)
{
	return (_target == name);
}

void HasNameRequirement::UpdateMatchingNames(std::unordered_map<std::shared_ptr<Name>, int>& matches)
{
	if (_disabledName) return;
	if (!matches.count(_target))
		matches[_target] = 0;
	matches[_target] = 1 + matches[_target];
}

void HasNameRequirement::DisableUpdateForName(std::shared_ptr<Name> name)
{
	if (_target == name)
		_disabledName = true;
}

void HasNameRequirement::ClearDisabledUpdatesForAllNames()
{
	_disabledName = false;
}

float HasNameRequirement::GetWeight()
{
	return _weight;
}

int HasNameRequirement::GetAverageNameCount()
{
	return 1;
}
