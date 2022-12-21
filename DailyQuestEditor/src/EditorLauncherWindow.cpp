#include "EditorLauncherWindow.h"

using namespace std;

#include "EditorAppWindow.h"

void EditorLauncherWindow::OnInit()
{
}

void EditorLauncherWindow::OnDispose()
{
	cout << "Disposed of launcher so sad..." << endl;
}

void EditorLauncherWindow::OnUpdate(float deltaTime)
{
	delay -= deltaTime;
	cout << delay << endl;
	if (delay <= 0.0f)
	{
		cout << "We are through!" << endl;
		Close();
		Window* editor = new EditorAppWindow();
		Application::Attach(editor);
	}
}

void EditorLauncherWindow::OnDraw()
{
}

void EditorLauncherWindow::OnImGUIDraw()
{
}