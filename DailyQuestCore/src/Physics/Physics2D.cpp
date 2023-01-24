#include "Physics2D.h"

std::unique_ptr<b2World> Physics2D::_world;
std::unordered_map<std::shared_ptr<Name>, b2Body*> Physics2D::_bodies;

void Physics2D::Init()
{
	_world = std::make_unique<b2World>(b2Vec2(0.0f, -9.81f));
}

void Physics2D::Update(float deltaTime)
{
	_world->Step(deltaTime, 6, 2);
}

void Physics2D::LateUpdate(float deltaTime)
{
}
