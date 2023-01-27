#pragma once

#include <box2d/box2d.h>
#include "Story/Entity.h"

namespace Components
{
	struct Body
	{
		Name ID;
		Body(Name id) : ID(id) { }
	};
	struct Shape
	{
		b2Fixture* Fixture;
		Shape(b2Fixture* fixture) : Fixture(fixture) { }
	};
}