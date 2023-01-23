#include <iostream>

using namespace std;

#include "Core/Window.h"
#include "Application.h"

#include "src/Game.h"

int main()
{
	//Window* editor = new EditorAppWindow();
	Window* window = new Game();
	Application::Attach(window);
	Application::Run();
	//int i;
	//std::cin >> i;
}