#pragma once

struct Location
{
	float X;
	float Y;
	Location() : X(0.0f), Y(0.0f) { }
	Location(float x, float y) : X(x), Y(y) { }
};
struct Size
{
	float X;
	float Y;
	Size() : X(0.0f), Y(0.0f) { }
	Size(float x, float y) : X(x), Y(y) { }
};
struct Angle
{
	float Value;
	Angle() : Value(0.0f) { }
	Angle(float value) : Value(value) { }
};