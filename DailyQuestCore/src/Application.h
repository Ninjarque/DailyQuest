#pragma once

#include <string>
#include <vector>

#include "Core/Program.h"
#include "Error.h"

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

