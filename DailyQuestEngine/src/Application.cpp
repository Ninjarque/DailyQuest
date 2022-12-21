#include "Application.h"

std::vector<Program*> Application::programs;
bool Application::forcedStop = false;

void Application::Attach(Program* program)
{
	for (auto p : programs)
	{
		if (p == program)
		{
			Error::fatalError("Can't attach an already attached window to application!");
			return;
		}
	}
	programs.push_back(program);
}

void Application::Run()
{
	while (programs.size())
	{
		Program* program = programs[0];
		int code = program->Run();
		if (forcedStop)
		{
			Error::warning("Application has been stopped abruptly!");
			return;
		}
		if (code || !program->ShouldRestart())
		{
			program->Close();
			programs.erase(programs.begin());
			delete program;
			if (code)
			{
				Error::warning("Error (" + std::to_string(code) + ") occured while running a window in application...");
			}
		}
	}
}

void Application::Stop()
{
	for (auto w : programs)
	{
		w->Close();
		delete w;
	}
	programs.clear();
	forcedStop = true;
}