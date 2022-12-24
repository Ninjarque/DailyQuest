#include "EditorAppWindow.h"
#include <imgui.h>
#include <image.h>

using namespace std;

GLuint texture;
int width;
int height;

void EditorAppWindow::OnInit()
{
    bool ret = LoadTextureFromFile("C:\\Users\\ninja\\Pictures\\bh3rd\\2022-08-24-19-45-55_0.png", texture, width, height);
    IM_ASSERT(ret);
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
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(0.0f, 0.5f);
	glVertex2f(0.5f, -0.5f);
	glEnd();
}

void EditorAppWindow::OnImGUIDraw()
{
    static float f = 0.0f;
    static int counter = 0;
    
    ImGui::Begin("OpenGL Texture Text");
    ImGui::Text("pointer = %p", texture);
    ImGui::Text("size = %d x %d", width, height);
    ImGui::Image((ImTextureID)texture, ImVec2(width, height));
    ImGui::End();
    //commande, recommendation, tests model-livre, tests sur vues
    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
