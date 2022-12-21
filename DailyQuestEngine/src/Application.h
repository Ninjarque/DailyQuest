#pragma once

#include "pch.h"

#include "Core/Program.h"

class Application
{
public:
	static void Attach(Program* program);
	static void Run();
	static void Stop();
protected:
	static std::vector<Program*> programs;
	static bool forcedStop;
};

