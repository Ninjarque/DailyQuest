#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>

#include "Story/Components.h"
#include "Story/StoryManager.h"
#include "Story/Entity.h"

#include "Quest/Name.h"

struct Body
{
	Name* ID;
	Body(Name* id) : ID(id) { }
};
struct Shape
{
	b2Fixture* Fixture;
	Shape(b2Fixture* fixture) : Fixture(fixture) { }
};

class Physics2D
{
public:
	static void Init();

	static void Update(float deltaTime);
	static void LateUpdate(float deltaTime);

	static Body& CreateBody(Entity& entity);
	static Shape& CreateBoxShape(Entity& entity);

private:
	static std::unique_ptr<b2World> _world;
	static std::unordered_map<Name*, b2Body*> _bodies;
	static std::unordered_map<Name*, b2Fixture*> _fixtures;

	static void SetBodyPosition(Body body, Shape shape,
		Location location, Angle angle);
	static void UpdateFromBodyTransforms(Body body, Shape shape,
		Location location, Angle angle);
};

