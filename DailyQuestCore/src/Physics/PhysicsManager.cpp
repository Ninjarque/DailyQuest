#include "PhysicsManager.h"

std::unique_ptr<b2World> PhysicsManager::_world;
std::unordered_map<Name*, b2Body*> PhysicsManager::_bodies;

void PhysicsManager::Init()
{
	_world = std::make_unique<b2World>(b2Vec2(0.0f, -9.81f));
}

void PhysicsManager::Update(float deltaTime)
{
	_world->Step(deltaTime, 6, 2);
}

void PhysicsManager::LateUpdate(float deltaTime)
{
}
