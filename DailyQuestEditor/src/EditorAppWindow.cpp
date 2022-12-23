#include "EditorAppWindow.h"

using namespace std;


void EditorAppWindow::OnInit()
{
	
}

void EditorAppWindow::OnDispose()
{
	cout << "Disposed of editor I see" << endl;
}

void EditorAppWindow::OnUpdate(float deltaTime)
{

}

void EditorAppWindow::OnDraw()
{
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.0f, 0.5f);
	glVertex2f(0.5f, -0.5f);
	glEnd();
}

void EditorAppWindow::OnImGUIDraw()
{
}
