#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Story/Entity.h"

namespace Components
{
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
	struct Color
	{
		float R, G, B, A;
		Color() : R(1.0f), G(1.0f), B(1.0f), A(1.0f) { }
		Color(float r, float g, float b) : R(r), G(g), B(b), A(1.0f) { }
		Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
	};
	struct UVRect
	{
		float X;
		float Y;
		float Width;
		float Height;
		UVRect() : X(0.0f), Y(0.0f), Width(1.0f), Height(1.0f) { }
		UVRect(float x, float y, float width, float height) : X(x), Y(y), Width(width), Height(height) { }
	};
}