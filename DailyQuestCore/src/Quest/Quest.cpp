#include "Quest.h"

Quest::SubQuest::SubQuest() : _targetName(nullptr),
_hasName(nullptr), _hasntName(nullptr),
_longestActionRequierement(nullptr) { }

Quest::SubQuest::~SubQuest()
{
	Dispose();
}

void Quest::SubQuest::Dispose()
{
	_targetName.reset();
	if (HasChilds())
	{
		_hasName->Dispose();
		_hasntName->Dispose();
		delete _hasName; _hasName = nullptr;
		delete _hasntName; _hasntName = nullptr;
	}
	_actionRequierements.clear();
	_actionNames.clear();
	_longestActionRequierement = nullptr;
}

void Quest::SubQuest::Push(std::shared_ptr<Name> actionName, std::shared_ptr<ActionRequierement> actionRequierement)
{
	if (actionRequierement == nullptr) return;
	if (HasChilds())
	{
		if (actionRequierement->Has(_targetName))
			_hasName->Push(actionName, actionRequierement);
		else
			_hasntName->Push(actionName, actionRequierement);
		return;
	}
	_actionRequierements[actionRequierement] = actionRequierement->GetRequierementAverageNameCount();
	_actionNames[actionName] = actionRequierement;
	if (_longestActionRequierement == nullptr || _longestActionRequierement->GetRequierementAverageNameCount())
		_longestActionRequierement = actionRequierement.get();

	Build(ACTION_REQUIEREMENT_COUNT_SPLIT);
}

bool Quest::SubQuest::Erase(std::shared_ptr<Name> actionName)
{
	if (HasChilds())
	{
		bool erased = false;
		erased = _hasName->Erase(actionName);
		if (!erased)
			erased = _hasntName->Erase(actionName);
		if (erased && _hasName->IsEmpty() && _hasntName->IsEmpty())
		{
			Dispose();
		}
		return erased;
	}
	if (_actionNames.count(actionName))
	{
		std::shared_ptr<ActionRequierement> action = _actionNames[actionName];
		if (_actionRequierements.count(action))
		{
			_actionRequierements.erase(action);
			_actionNames.erase(actionName);
			if (_longestActionRequierement == action.get())
			{
				RecalculateLongest();
			}
			return true;
		}
	}
	return false;
}

std::vector<Quest::ActionRequierement*> Quest::SubQuest::MatchingActions(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context,
	int minMatchCount, int maxMatchIterationTries)
{
	bool doingBoth = false;
	int doingBothSinceIterations = 0;
	return MatchingActions(context, minMatchCount, maxMatchIterationTries, doingBoth, doingBothSinceIterations);
}

void Quest::SubQuest::Build(int actionRequierementCountSplit)
{
	if (HasChilds())
	{
		_hasName->Build(actionRequierementCountSplit);
		_hasntName->Build(actionRequierementCountSplit);
		return;
	}
	if (_actionRequierements.size() >= actionRequierementCountSplit)
	{
		_targetName = GetBestName();
		_hasName = new SubQuest();
		_hasntName = new SubQuest();
		for (auto it = _actionNames.begin(); it != _actionNames.end(); )
		{
			std::shared_ptr<ActionRequierement> action = it->second;
			if (action->Has(_targetName))
			{
				action->DisableUpdateForName(_targetName);
				_hasName->Push(it->first, action);
			}
			else
			{
				_hasntName->Push(it->first, action);
			}
			++it;
		}
		_actionRequierements.clear();
		_actionNames.clear();

		_hasName->Build(actionRequierementCountSplit);
		_hasntName->Build(actionRequierementCountSplit);
	}
	for (auto action : _actionNames)
	{
		action.second->ClearDisabledUpdatesForAllNames();
	}
}

//best is the one wich has the least distance to the half of the set
//->the Name that splits the most and that the longest actionRequierement knows
std::shared_ptr<Name> Quest::SubQuest::GetBestName()
{
	std::unordered_map<std::shared_ptr<Name>, int> scores;
	for (auto it = _actionRequierements.begin(); it != _actionRequierements.end(); )
	{
		it->first->UpdateMatchingNames(scores);
		++it;
	}
	std::shared_ptr<Name> best = nullptr;
	int minScore = 10000;
	for (auto score : scores)
	{
		int s = _actionRequierements.size() / 2 - score.second; if (s < 0) s *= -1;
		if (s < minScore && _longestActionRequierement->Has(score.first))
		{
			minScore = s;
			best = score.first;
		}
	}
	return best;
}

void Quest::SubQuest::RecalculateLongest()
{
	_longestActionRequierement = nullptr;
	if (_actionRequierements.size() != 0)
		_longestActionRequierement = _actionRequierements.begin()->first.get();
}

std::vector<Quest::ActionRequierement*> Quest::SubQuest::MatchingActions(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context,
	int minMatchCount, int maxMatchIterationTries, bool& doingBoth, int& doingBothSinceIterations)
{
	std::vector<ActionRequierement*> res;
	if (HasChilds())
	{
		if (context.count(_targetName))
		{
			std::vector<ActionRequierement*> hasRes = _hasName->MatchingActions(context, minMatchCount, maxMatchIterationTries,
				doingBoth, doingBothSinceIterations);
			res.insert(res.begin(), hasRes.begin(), hasRes.end());
		}
		minMatchCount -= res.size();
		if (!context.count(_targetName) && minMatchCount > 0)
		{
			doingBoth = doingBoth || res.size() != 0;
			std::vector<ActionRequierement*> hasntRes = _hasName->MatchingActions(context, minMatchCount, maxMatchIterationTries,
				doingBoth, doingBothSinceIterations);
			res.insert(res.end(), hasntRes.begin(), hasntRes.end());
		}
		return res;
	}
	if (doingBoth) doingBothSinceIterations++;
	if (doingBothSinceIterations > maxMatchIterationTries)
		return res;
	for (auto it = _actionRequierements.begin(); it != _actionRequierements.end(); )
	{
		if (it->first->IsValid(context))
		{
			res.push_back(it->first.get());
		}
		if (it->first->GetRequierementAverageNameCount() < context.size())
			return res;
		++it;
	}
	return res;
}

Quest::Quest(std::shared_ptr<Story> story)
{
	_story = story;
}

Quest::~Quest()
{
	_subQuest.Dispose();
	_story.reset();
}

void Quest::Define(std::shared_ptr<Name> eventName, std::unique_ptr<Action> action, std::unique_ptr<Requirement> requirement)
{
	_subQuest.Push(eventName, std::make_shared<ActionRequierement>(std::move(action), std::move(requirement)));
}

bool Quest::Remove(std::shared_ptr<Name> eventName)
{
	return _subQuest.Erase(eventName);
}

std::unique_ptr<Event> Quest::TriggerEvent(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context)
{
	std::vector<ActionRequierement*> actionRequierements = _subQuest.MatchingActions(context, 1, 0);
	if (actionRequierements.size() > 0)
	{
		if (auto story = _story.lock())
		{
			if (auto storyInformations = story->GetInformations().lock())
			{
				Action* action = actionRequierements[0]->GetAction();
				Event* event = new Event(storyInformations, action, context);
				std::unique_ptr<Event> e(event);// = std::make_unique<Event>(event);
				event = nullptr;
				e->Run();
				return e;
			}
		}
	}
	return nullptr;
}
