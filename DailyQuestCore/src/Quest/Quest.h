#pragma once

#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <algorithm>

#include "Name.h"
#include "Information.h"
#include "Action.h"
#include "Event.h"
#include "Requirement.h"

#include "Story/Story.h"

#define ACTION_REQUIEREMENT_COUNT_SPLIT 1000

class Story;

class Quest
{
private:
	struct NameScore
	{
		std::shared_ptr<Name> Target;
		int Score;

		NameScore(std::shared_ptr<Name> target, int score) : Target(target), Score(score) { }
	};

	class ActionRequierement
	{
	public:
		ActionRequierement(std::unique_ptr<Action> action, std::unique_ptr<Requirement> requirement)
		{
			_action = std::move(action);
			_requirement = std::move(requirement);
		}
		~ActionRequierement() { Dispose(); }
		void Dispose() { _action = nullptr; _requirement = nullptr; }
		bool IsValid(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context)
		{
			return _requirement->IsValid(context);
		}
		bool Has(std::shared_ptr<Name> name)
		{
			return _requirement->Has(name);
		}

		void UpdateMatchingNames(std::unordered_map<std::shared_ptr<Name>, int>& matches)
		{
			_requirement->UpdateMatchingNames(matches);
		}
		void DisableUpdateForName(std::shared_ptr<Name> name)
		{
			_requirement->DisableUpdateForName(name);
		}

		float GetRequierementWeight() const
		{
			return _requirement->GetWeight();
		}
		int GetRequierementAverageNameCount() const
		{
			return _requirement->GetAverageNameCount();
		}
		Action* GetAction() { return _action.get(); }

	private:
		std::unique_ptr<Action> _action;
		std::unique_ptr<Requirement> _requirement;
	};

	struct cmp_actions {
		bool operator() (const std::shared_ptr<ActionRequierement>& a, const std::shared_ptr<ActionRequierement>& b) const {
			return a->GetRequierementWeight() > b->GetRequierementWeight();
		}
	};

	class SubQuest
	{
	public:
		SubQuest();
		~SubQuest();

		void Dispose();

		void Push(std::shared_ptr<Name> actionName, std::shared_ptr<ActionRequierement> actionRequierement);
		bool Erase(std::shared_ptr<Name> actionName);

		std::vector<ActionRequierement*> MatchingActions(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context,
			int minMatchCount, int maxMatchIterationTries);

		bool HasChilds()
		{
			return _hasName && _hasntName;
		}
		bool IsEmpty()
		{
			return !HasChilds() && _actionRequierements.size() == 0;
		}

	private:
		std::shared_ptr<Name> _targetName;
		SubQuest* _hasName;
		SubQuest* _hasntName;

		std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<ActionRequierement>> _actionNames;
		std::map<std::shared_ptr<ActionRequierement>, float, cmp_actions> _actionRequierements;
		ActionRequierement* _longestActionRequierement;
		
		void Build(int actionRequierementCountSplit);

		std::shared_ptr<Name> GetBestName();
		void RecalculateLongest();

		std::vector<ActionRequierement*> MatchingActions(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context,
			int minMatchCount, int maxMatchIterationTries, bool& doingBoth, int& doingBothSinceIterations);
	};

public:
	Quest(std::shared_ptr<Story> story);
	~Quest();

	void Define(std::shared_ptr<Name> eventName, std::unique_ptr<Action> action, std::unique_ptr<Requirement> requirement);
	bool Remove(std::shared_ptr<Name> eventName);
	std::unique_ptr<Event> TriggerEvent(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context);

private:
	SubQuest _subQuest;
	std::weak_ptr<Story> _story;
};

