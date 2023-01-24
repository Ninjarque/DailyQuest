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
#include "Definition.h"

class Definition;

class Story
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
		ActionRequierement(std::unique_ptr<Action> action, std::unique_ptr<Definition> definition)
		{
			_action = std::move(action);
			_definition = std::move(definition);
		}
		~ActionRequierement() { Dispose(); }
		void Dispose() { _action = nullptr; _definition = nullptr; }
		bool IsValid(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context)
		{
			return _definition->IsValid(context);
		}
		bool Contains(std::shared_ptr<Name> name) 
		{
			return _definition->Contains(name);
		}

		void UpdateMatchingNames(std::unordered_map<std::shared_ptr<Name>, int>& matches)
		{
			_definition->UpdateMatchingNames(matches);
		}
		void DisableUpdateForName(std::shared_ptr<Name> name)
		{
			_definition->DisableUpdateForName(name);
		}

		float GetRequierementWeight() const
		{
			return _definition->GetWeight();
		}
		int GetRequierementAverageNameCount() const
		{
			return _definition->GetAverageNameCount();
		}
		Action* GetAction() { return _action.get(); }

	private:
		std::unique_ptr<Action> _action;
		std::unique_ptr<Definition> _definition;
	};

	struct cmp_actions {
		bool operator() (const ActionRequierement*& a, const ActionRequierement*& b) const {
			return a->GetRequierementWeight() > b->GetRequierementWeight();
		}
	};

	class SubStory
	{
	public:
		SubStory();
		~SubStory();

		void Dispose();

		void Push(std::shared_ptr<ActionRequierement> actionRequierement);
		bool Erase(std::shared_ptr<ActionRequierement> actionRequierement);

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
		SubStory* _hasName;
		SubStory* _hasntName;

		std::map<std::shared_ptr<ActionRequierement>, float, cmp_actions> _actionRequierements;
		ActionRequierement* _longestActionRequierement;
		
		void Build(int actionRequierementCountSplit);

		std::shared_ptr<Name> GetBestName();
		void RecalculateLongest();

		std::vector<ActionRequierement*> MatchingActions(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context,
			int minMatchCount, int maxMatchIterationTries, bool& doingBoth, int& doingBothSinceIterations);
	};

public:
	Story();
	~Story();

	void Define(std::unique_ptr<Action> action, std::unique_ptr<Definition> definition);
	std::unique_ptr<Event> TriggerEvent(std::unordered_map<std::shared_ptr<Name>, std::shared_ptr<Information>> context);

private:
	SubStory _subStory;
};

