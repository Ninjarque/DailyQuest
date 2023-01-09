#include "EditorAppWindow.h"

#include <math.h>

using namespace std;

void EditorAppWindow::OnInit()
{
    bool ret = LoadTextureFromFile("C:\\Users\\ninja\\Pictures\\bh3rd\\2022-08-24-19-45-55_0.png", texture, width, height);
    //IM_ASSERT(ret);
    int twidth; int theight;
    LoadTextureFromFile("C:\\Users\\ninja\\Pictures\\pink_soldier_x7.png", modelTexture1, twidth, theight);
    //LoadTextureFromFile("C:\\Users\\ninja\\Pictures\\Screenshots\\Capture d'écran_20221111_173201.png", modelTexture1, twidth, theight);
    //LoadTextureFromFile("C:\\Users\\ninja\\Pictures\\Screenshots\\Capture d'écran_20221126_163516.png", modelTexture1, twidth, theight);
    LoadTextureFromFile("C:\\Users\\ninja\\Pictures\\Screenshots\\Capture d'écran_20221216_184959.png", modelTexture2, twidth, theight);

    Renderer2D::Init(128);
    
    frame.Init(buffer_width, buffer_height);
    shader.Init("res/Shaders/Basic.shader");

    model.Init(
        std::vector<float>{ 
            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f,
            0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f,
            0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f,
            -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f
        },
        std::vector<int>{ 2, 3, 2, 1 },
        std::vector<bool>{ false, false, false, false },
        std::vector<unsigned int>{ 0, 1, 2, 2, 3, 0 });
    
    particleSystem.Init(10000, ParticleSystem::RenderMode::Render2D, shader);
    particleSystem.SetPhysics(ParticlePhysic(
        glm::vec3(0.0f, 3.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec4(-1.5f, -1.0f, -1.0f, -2.0f),
        0.01f, 0.01f, 0.01f
    ));

    InputManager::SetBinding(InputType::Keyboard, "Test", GLFW_KEY_SPACE);
    InputManager::SetBinding(InputType::Mouse, "Spawn", GLFW_MOUSE_BUTTON_1);

    camera = Camera(glm::vec2(0.0f, 0.0f), 1.0f);
}

void EditorAppWindow::OnDispose()
{
	cout << "Disposed of editor I see" << endl;
}

void EditorAppWindow::OnUpdate(float deltaTime)
{
    this->deltaTime = deltaTime;
    time += deltaTime;
    static float particleSpawnTime = 0.0f;
    particleSpawnTime += deltaTime;
    bool spawn = false;
    if (particleSpawnTime > 0.02f)
    {
        spawn = true;
    }
    int w, h;
    Window::Current->GetSize(w, h);
    if (InputManager::IsPressed("Test"))
    {
        Bindings* b = new Bindings();
        b->SetBinding(InputType::Mouse, "None", GLFW_MOUSE_BUTTON_1);
        InputManager::PushBindings(b);
    }
    if (InputManager::IsReleased("None"))
    {
        std::cout << "Hold on" << std::endl;
    }
    if ((InputManager::IsDown("Spawn") || InputManager::IsDown("None")) && spawn)
    {
        double x;
        double y;
        Mouse::GetPosition(x, y);
        particleSpawnTime = 0.0f;
        ParticleProperties fire(
            1.0f,
            glm::vec3((float)x, (float)y, 0.0f),
            glm::vec3(0.0f),
            glm::vec3(100.0f, 200.0f, 100.0f),
            glm::vec4(1.0f, 0.4f, 0.3f, 1.0f),
            0);
        fire.PositionVariation = glm::vec3(0.03f) * (float)h;
        fire.Velocity = glm::vec3(0.0f, -0.3f, 0.0f) * 0.5f * (float)h;
        fire.VelocityVariation = glm::vec3(0.3f, 0.5f, 0.0f) * 0.2f * (float)h;
        fire.ColorChange = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        for (int i = 0; i < 30; i++)
        {
            particleSystem.Emit(fire);
        }
    }

    particleSystem.Update(deltaTime);
}

void EditorAppWindow::OnDraw()
{
    int w, h;
    Window::Current->GetSize(w, h);
    std::vector<GLuint> textures = { modelTexture1, modelTexture2 };
    
    // /*
    shader.Begin();
    
    //Renderer2D::Begin();
    //Renderer2D::SetUniforms(shader);

    //Renderer2D::DrawQuad({ -1.0f, -1.0f }, { 2.0f, 2.0f }, 0.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    //Renderer2D::End();


    particleSystem.Render();

    shader.End();
    
    // */

    //glViewport(0, 0, m_width, m_height);
    shader.Begin();

    //frame.StartFrame(buffer_width, buffer_height);

    Renderer2D::Begin(&camera, &shader);

    if (false)//(time < 100.0f)
    {
        //renderer.DrawQuad(-1.0f, 0.0f, 1.0f, 1.0f, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), modelTexture2);
        //renderer.DrawQuad(0.0f, -1.0f, 1.0f, 1.0f, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), texture);
        //renderer.DrawQuad(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), modelTexture1);
        float xs = 128.0f;
        float ys = 128.0f;
        int t = 0;
        for (float x = 0; x < xs; x++)
        {
            for (float y = 0; y < ys; y++)
            {
                float rx = x / xs * w;
                float ry = y / ys * h;
                float rw = w / xs;
                float rh = h / ys;
                ry += cos(time * 0.05f * (rx - (0.5f * w))) * (h / xs * 4.0f);
                Renderer2D::DrawQuad({ rx, ry }, { rw, rh }, 0.0f, glm::vec4(x / xs, 0.0f, y / ys, 1.0f));//textures[t]);                
                //Renderer2D::DrawQuad({ rx, ry }, { rw, rh }, 0.0f, textures[t]);                
                t++;
                t %= textures.size();
            }
        }
    }
    else
    {
        double x;
        double y;
        Mouse::GetPosition(x, y);
        Renderer2D::DrawQuad({ (float)w / 2.0f, (float)h / 2.0f }, { (float)x- (float)w / 2.0f, (float)y - (float)h / 2.0f }, 0.0f, texture);
    }
    
    Renderer2D::End();
    //renderer.EndRender();

    //textures = frame.EndFrame();
    //glViewport(0, 0, m_width, m_height);

    //renderer.StartRender();
    //renderer.DrawQuad(-1.0f, -1.0f, 2.0f, 2.0f, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), textures[0]);
    //renderer.EndRender();

    shader.End();
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
    // */
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

    int drawCount, quadCount;
    Renderer2D::GetStats(drawCount, quadCount);
    Renderer2D::ResetStats();
    ImGui::Text("Draw count %i, quad count %i", drawCount, quadCount);

    ImGui::Text("Engine Application average %.3f ms/frame (%.1f FPS)",
        deltaTime * 1000.0f, 1.0f / deltaTime);//1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("ImGUI Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
