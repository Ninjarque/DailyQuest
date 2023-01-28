#pragma once

#include <box2d/box2d.h>
#include "Story/Entity.h"

namespace Components
{
	struct Body
	{
		Name ID;
		bool IsDynamic;
		Body(Name id, bool isDynamic) : ID(id), IsDynamic(isDynamic) { }
	};
	struct Shape
	{
		b2Fixture* Fixture;
		Shape(b2Fixture* fixture) : Fixture(fixture) { }
	};
}