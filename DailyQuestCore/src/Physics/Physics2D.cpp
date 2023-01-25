#include "Physics2D.h"

std::unique_ptr<b2World> Physics2D::_world;
std::unordered_map<Name*, b2Body*> Physics2D::_bodies;
std::unordered_map<Name*, b2Fixture*> Physics2D::_fixtures;

void Physics2D::Init()
{
	_world = std::make_unique<b2World>(b2Vec2(0.0f, -9.81f));
}

void Physics2D::Update(float deltaTime)
{
	StoryManager::ComputeForEach<
		Body, 
		Shape, 
		Location,
		Angle>(SetBodyPosition);

	_world->Step(deltaTime, 6, 2);

	StoryManager::ComputeForEach<
		Body,
		Shape,
		Location,
		Angle>(UpdateFromBodyTransforms);
}

void Physics2D::LateUpdate(float deltaTime)
{
}

Body& Physics2D::CreateBody(Entity& entity)
{
	b2BodyDef def;
	Location location = Location(0.0f, 0.0f);
	if (entity.Has<Location>())
		location = entity.Get<Location>();
	else
	{
		entity.Add<Location>(location.X, location.Y);
	}
	def.type = b2_dynamicBody;
	def.position.Set(location.X, location.Y);

	b2Body* b2body = _world->CreateBody(&def);
	_bodies[&entity.Get<Name>()] = b2body;
	return entity.Add<Body>(&entity.Get<Name>());
}
Shape& Physics2D::CreateBoxShape(Entity& entity)
{
	b2PolygonShape shape;
	Size size = Size(50.0f, 50.0f);
	if (entity.Has<Size>())
		size = entity.Get<Size>();
	else
	{
		entity.Add<Size>(size.X, size.Y);
	}
	shape.SetAsBox(size.X/2.0f, size.Y/2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	auto name = entity.Get<Name>();
	if (!_bodies.count(&name)) CreateBody(entity);
	_fixtures[&name] = _bodies[&name]->CreateFixture(&fixtureDef);
	return entity.Add<Shape>(_fixtures[&name]);
}


void Physics2D::SetBodyPosition(Body body, Shape shape, 
	Location location, Angle angle)
{
	//if (auto name = body.ID.lock())
	//{
	auto b2body = _bodies[body.ID];

	b2body->SetTransform(b2Vec2(location.X, location.Y), angle.Value);
	//}
}
void Physics2D::UpdateFromBodyTransforms(Body body, Shape shape,
	Location location, Angle angle)
{
	//if (auto name = body.ID.lock())
	//{
		auto b2body = _bodies[body.ID];

	(&location)->X = b2body->GetPosition().x;
	(&location)->Y = b2body->GetPosition().y;
	(&angle)->Value = b2body->GetAngle();
	//}
}