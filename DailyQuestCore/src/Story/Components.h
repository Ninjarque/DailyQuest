#pragma once

#include "Entity.h"

struct Location
{
	float X;
	float Y;
	Location() : X(0.0f), Y(0.0f) { }
	Location(float x, float y) : X(x), Y(y) { }
};
struct Size
{
	float X;
	float Y;
	Size() : X(0.0f), Y(0.0f) { }
	Size(float x, float y) : X(x), Y(y) { }
};
struct Angle
{
	float Value;
	Angle() : Value(0.0f) { }
	Angle(float value) : Value(value) { }
};

struct Visibility
{
	bool Visible;
	Visibility() : Visible(true) { }
	Visibility(bool visible) : Visible(visible) { }
};
struct Depth
{
	float Value;
	Depth() : Value(0.0f) { }
	Depth(float value) : Value(value) { }
};
// /*
class Childrens
{
public:
	template<typename... Args>
	Childrens(Entity current, Args... childs) : _current(current) 
	{
		std::vector<Entity> childEntities = { childs... };
		for (int i = 0; i < childEntities.size(); i++)
			Add(childEntities[i]);
	}
	~Childrens() { _childs.clear(); }
	void Add(Entity child) 
	{ 
		if (!child.Has<Parent>())
		{
			child.Add<Parent>( _current);
		}
		else
			child.Set<Parent>( _current);
		_childs.push_back(child); 
	}
	bool Remove(Entity child) 
	{
		for (auto it = _childs.begin(); it != _childs.end(); )
		{
			auto entity = *it;
			if (!entity || entity == child)
			{
				it = _childs.erase(it);
			}
			else
				++it;
		}
	}
	std::vector<Entity> GetChildrens()
	{
		std::vector<Entity> res;
		for (auto it = _childs.begin(); it != _childs.end(); )
		{
			auto entity = *it;
			if (!entity)
			{
				it = _childs.erase(it);
			}
			else
			{
				res.push_back(entity);
				++it;
			}
		}
		return res;
	}

private:
	Entity _current;
	std::vector<Entity> _childs;
	friend class Parent;
};
class Parent
{
public:
	Parent(Entity current, Entity parent) 
		: _current(current), _parent(parent) { }
	~Parent() { }

	Entity GetParent() { return _parent; }
private:
	Entity _current;
	Entity _parent;

	friend class Childrens;
};
// */