#include "EditorAppWindow.h"

using namespace std;

void EditorAppWindow::OnInit()
{
    bool ret = LoadTextureFromFile("C:\\Users\\ninja\\Pictures\\bh3rd\\2022-08-24-19-45-55_0.png", texture, width, height);
    IM_ASSERT(ret);

    frame.Init(buffer_width, buffer_height);
    shader.Init("res/Shaders/Basic.shader");

    model.Init(
        std::vector<float>{ 
            -0.5f, -0.5f, 1.0f, 0.2f, 0.2f,
            0.5f, -0.5f, 0.2f, 1.0f, 0.2f,
            0.5f, 0.5f, 0.2f, 0.2f, 1.0f,
            -0.5f, 0.5f, 0.8f, 0.8f, 0.2f
        },
        std::vector<int>{ 2, 3 },
        std::vector<bool>{ false, false },
        std::vector<unsigned int>{ 0, 1, 2, 2, 3, 0 });
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
    frame.StartFrame(buffer_width, buffer_height);
    shader.Begin();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int modelCount;
    ModelDrawMode modelDrawMode;
    model.StartModel(&modelCount, &modelDrawMode);
    
    if (modelDrawMode == ModelDrawMode::Arrays)
        glDrawArrays(GL_TRIANGLES, 0, modelCount);
    else
        glDrawElements(GL_TRIANGLES, modelCount, GL_UNSIGNED_INT, NULL);

    /*
	glBegin(GL_TRIANGLES);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(0.0f, 0.5f);
	glVertex2f(0.5f, -0.5f);
	glEnd();
    */
    
    model.EndModel();

    shader.End();
    textures = frame.EndFrame();
}

void EditorAppWindow::OnImGUIDraw()
{
    static float f = 0.0f;
    static int counter = 0;
    
    for (auto texture : textures)
    {
        ImGui::Begin("OpenGL Renderer texture");
        ImGui::Text("pointer = %p", texture);
        ImGui::Text("size = %d x %d", buffer_width, buffer_height);
        ImGui::Image((ImTextureID)texture, ImVec2(m_width, m_height));
        ImGui::End();
    }
    /*
    ImGui::Begin("OpenGL Texture Text");
    ImGui::Text("pointer = %p", texture);
    ImGui::Text("size = %d x %d", width, height);
    ImGui::Image((ImTextureID)texture, ImVec2(width, height));
    ImGui::End();
    */
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
