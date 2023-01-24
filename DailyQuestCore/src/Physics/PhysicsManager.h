#pragma once

#include <box2d/box2d.h>

#include <memory>
#include <unordered_map>

#include "Story/Name.h"

class PhysicsManager
{
public:
	static void Init();

	static void Update(float deltaTime);
	static void LateUpdate(float deltaTime);

	/*
	Maybe make physicsManager create unique_ptr for other components to play
	*/
	/*
	Add physicsobjects here maybe much ?
	Or tryna make the entity component system now so it doens't get messy early
	*/

private:
	static std::unique_ptr<b2World> _world;
	static std::unordered_map<std::shared_ptr<Name>, b2Body*> _bodies;
};

