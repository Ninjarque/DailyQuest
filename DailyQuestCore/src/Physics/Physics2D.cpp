#include "Physics2D.h"

std::unique_ptr<b2World> Physics2D::_world;
std::unordered_map<Name, b2Body*> Physics2D::_bodies;
std::unordered_map<Name, b2Fixture*> Physics2D::_fixtures;

void Physics2D::Init()
{
	_world = std::make_unique<b2World>(b2Vec2(0.0f, 9.81f));
}

void Physics2D::Update(TimeStep timestep)
{
	StoryManager::ComputeForEachEntity<
		Body, 
		Shape, 
		Location,
		Angle>(SetBodyPosition);

	_world->Step(timestep, 6, 2);

	StoryManager::ComputeForEachEntity<
		Body,
		Shape,
		Location,
		Angle>(UpdateFromBodyTransforms);
}

void Physics2D::LateUpdate(TimeStep timestep)
{
}

Body& Physics2D::CreateBody(Entity& entity, bool isStatic)
{
	b2BodyDef def;
	Location location = Location(0.0f, 0.0f);
	Angle angle = Angle(0.0f);
	if (entity.Has<Location>())
		location = entity.Get<Location>();
	else
		entity.Add<Location>(location.X, location.Y);
	if (entity.Has<Angle>())
		angle = entity.Get<Angle>();
	else
		entity.Add<Angle>(angle.Value);
	def.type = isStatic ? b2_staticBody : b2_dynamicBody;
	def.position.Set(location.X * WORLD_RATIO, location.Y * WORLD_RATIO);
	def.angle = angle.Value;

	b2Body* b2body = _world->CreateBody(&def);
	auto name = entity.Get<Name>();
	_bodies[name] = b2body;
	return entity.Add<Body>(name);
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
	shape.SetAsBox(size.X/2.0f * WORLD_RATIO, size.Y/2.0f * WORLD_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f / WORLD_RATIO;
	fixtureDef.friction = 0.3f;
	auto name = entity.Get<Name>();
	if (!_bodies.count(name)) CreateBody(entity);
	_fixtures[name] = _bodies[name]->CreateFixture(&fixtureDef);
	return entity.Add<Shape>(_fixtures[name]);
}


void Physics2D::SetBodyPosition(Entity entity, Body& body, Shape& shape,
	Location& location, Angle& angle)
{
	auto b2body = _bodies[body.ID];

	b2body->SetTransform(b2Vec2(location.X * WORLD_RATIO, location.Y * WORLD_RATIO), angle.Value);
}
void Physics2D::UpdateFromBodyTransforms(Entity entity, Body& body, Shape& shape,
	Location& location, Angle& angle)
{
	auto b2body = _bodies[body.ID];
	Location& l = entity.Get<Location>();

	l.X = b2body->GetPosition().x / WORLD_RATIO;
	l.Y = b2body->GetPosition().y / WORLD_RATIO;
	angle.Value = b2body->GetAngle();
}