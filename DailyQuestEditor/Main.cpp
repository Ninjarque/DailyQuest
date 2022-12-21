#include <iostream>

using namespace std;

#include "Core/Window.h"
#include "Application.h"

#include "src/EditorAppWindow.h"
#include "src/EditorLauncherWindow.h"

int main()
{
	//Window* editor = new EditorAppWindow();
	Window* window = new EditorAppWindow();
	Application::Attach(window);
	Application::Run();
	int i;
	std::cin >> i;
}