#pragma once

struct Color
{
	float R;
	float G;
	float B;
	float A;
	Color() : Color(1.0f, 1.0f, 1.0f) { }
	Color(float r, float g, float b) : Color(r, g, b, 1.0f) { }
	Color(float r, float g, float b, float a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}
};