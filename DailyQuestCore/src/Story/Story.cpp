#include "Story.h"

Story::SubStory::SubStory() : _targetName(nullptr),
_hasName(nullptr), _hasntName(nullptr),
_longestActionRequierement(nullptr) { }

Story::SubStory::~SubStory()
{
	Dispose();
}

void Story::SubStory::Dispose()
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
	_longestActionRequierement = nullptr;
}

void Story::SubStory::Push(std::shared_ptr<ActionRequierement> actionRequierement)
{
	if (actionRequierement == nullptr) return;
	if (HasChilds())
	{
		if (actionRequierement->Contains(_targetName))
			_hasName->Push(std::move(actionRequierement));
		else
			_hasntName->Push(std::move(actionRequierement));
		return;
	}
	_actionRequierements[actionRequierement] = actionRequierement->GetRequierementAverageNameCount();
	if (_longestActionRequierement == nullptr || _longestActionRequierement->GetRequierementAverageNameCount())
		_longestActionRequierement = actionRequierement.get();
}

bool Story::SubStory::Erase(std::shared_ptr<ActionRequierement> actionRequierement)
{
	if (HasChilds())
	{
		bool erased = false;
		if (actionRequierement->Contains(_targetName))
		{
			erased = _hasName->Erase(std::move(actionRequierement));
			if (erased && _hasName->IsEmpty() && _hasntName->IsEmpty())
			{
				Dispose();
			}
			return erased;
		}
		else
		{
			erased = _hasntName->Erase(std::move(actionRequierement));
			if (erased && _hasName->IsEmpty() && _hasntName->IsEmpty())
			{
				Dispose();
			}
			return erased;
		}
	}
	if (_actionRequierements.count(actionRequierement))
	{
		if (_longestActionRequierement == actionRequierement.get())
		{
			RecalculateLongest();
		}
		_actionRequierements.erase(actionRequierement);
	}
}

std::vector<Story::ActionRequierement*> Story::SubStory::MatchingActions(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context,
	int minMatchCount, int maxMatchIterationTries)
{
	bool doingBoth = false;
	int doingBothSinceIterations = 0;
	return MatchingActions(context, minMatchCount, maxMatchIterationTries, doingBoth, doingBothSinceIterations);
}

void Story::SubStory::Build(int actionRequierementCountSplit)
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
		_hasName = new SubStory();
		_hasntName = new SubStory();
		for (auto it = _actionRequierements.begin(); it != _actionRequierements.end(); )
		{
			if (it->first->Contains(_targetName))
			{
				it->first->DisableUpdateForName(_targetName);
				_hasName->Push(it->first);
			}
			else
			{
				_hasntName->Push(it->first);
			}
			++it;
		}
		_actionRequierements.clear();

		_hasName->Build(actionRequierementCountSplit);
		_hasntName->Build(actionRequierementCountSplit);
	}
}

//best is the one wich has the least distance to the half of the set
//->the Name that splits the most and that the longest actionRequierement knows
std::shared_ptr<Name> Story::SubStory::GetBestName()
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
		if (s < minScore && _longestActionRequierement->Contains(score.first))
		{
			minScore = s;
			best = score.first;
		}
	}
	return best;
}

void Story::SubStory::RecalculateLongest()
{
	_longestActionRequierement = nullptr;
	if (_actionRequierements.size() != 0)
		_longestActionRequierement = _actionRequierements.begin()->first.get();
}

std::vector<Story::ActionRequierement*> Story::SubStory::MatchingActions(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context,
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

Story::Story()
{
}

Story::~Story()
{
	_subStory.Dispose();
}

void Story::Define(std::unique_ptr<Action> action, std::unique_ptr<Definition> definition)
{
	_subStory.Push(std::make_shared<ActionRequierement>(action, definition));
}

std::unique_ptr<Event> Story::TriggerEvent(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context)
{
	std::vector<ActionRequierement*> actionRequierements = _subStory.MatchingActions(context, 1, 0);
	if (actionRequierements.size() > 0)
	{
		Action* action = actionRequierements[0]->GetAction();
		std::unique_ptr<Event> e = std::make_unique<Event>(action, context);
		e->Run();
		return e;
	}
	return nullptr;
}
