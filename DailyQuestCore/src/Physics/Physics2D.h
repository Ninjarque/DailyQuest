#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>

#include "Core/TimeStep.h"

#include "Components/Basic.h"
#include "Components/Physics.h"
#include "Story/StoryManager.h"
#include "Story/Entity.h"

#include "Quest/Name.h"

#define WORLD_RATIO 0.01f

using namespace Components;

class Physics2D
{
public:
	static void Init();

	static void Update(TimeStep timestep);
	static void LateUpdate(TimeStep timestep);

	static Body& CreateBody(Entity& entity, bool isStatic = false);
	static Shape& CreateBoxShape(Entity& entity);

	static void MakeBox(Entity& entity, glm::vec2 location, glm::vec2 size, float angle, bool isStatic = false);

private:
	static std::unique_ptr<b2World> _world;
	static std::unordered_map<Name, b2Body*> _bodies;
	static std::unordered_map<Name, b2Fixture*> _fixtures;

	static void SetBodyPosition(Entity entity, Body& body, Shape& shape,
		Location& location, Angle& angle);
	static void UpdateFromBodyTransforms(Entity entity, Body& body, Shape& shape,
		Location& location, Angle& angle);
};

