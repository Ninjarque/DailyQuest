#include "EditorAppWindow.h"

#include <math.h>

using namespace std;

void EditorAppWindow::OnInit()
{
    bool ret = LoadTextureFromFile("C:\\Users\\ninja\\Pictures\\bh3rd\\2022-08-24-19-45-55_0.png", texture, width, height);
    IM_ASSERT(ret);
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

    if (particleSpawnTime > 0.02f)
    {
        ParticleProperties prop(
            1.0f,
            glm::vec3(0.0f, -0.7f, 0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.03f, 0.04f, 1.0f) * 0.2f,
            glm::vec4(1.0f, sin(time) * sin(time), cos(time) * cos(time), 1.0f),
            0);
        prop.PositionVariation = glm::vec3(0.03f) * cos(time) * cos(time);
        prop.Velocity = glm::vec3(0.0f, -0.3f, 0.0f) * cos(time) * cos(time);
        prop.VelocityVariation = glm::vec3(0.3f, 0.5f, 0.0f) * 0.2f * cos(time) * cos(time);
        prop.ColorChange = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        ParticleProperties fire(
            1.0f,
            glm::vec3(0.0f, -0.7f, 0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.03f, 0.13f, 1.0f) * 2.8f,
            glm::vec4(1.0f, 0.4f, 0.3f, 1.0f),
            modelTexture1);
        fire.PositionVariation = glm::vec3(0.03f) * 1.2f;
        fire.Velocity = glm::vec3(0.0f, -0.3f, 0.0f) * 1.5f;
        fire.VelocityVariation = glm::vec3(0.3f, 0.5f, 0.0f) * 0.2f * 1.5f;
        fire.ColorChange = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        for (int i = 0; i < 30; i++)
        {
            particleSystem.Emit(fire);
        }
        particleSpawnTime = 0.0f;
    }

    particleSystem.Update(deltaTime);
}

void EditorAppWindow::OnDraw()
{
    std::vector<GLuint> textures = { modelTexture1, modelTexture2 };
    
    // /*
    shader.Begin();
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Renderer2D::Begin();
    //Renderer2D::SetUniforms(shader);

    //Renderer2D::DrawQuad({ -1.0f, -1.0f }, { 2.0f, 2.0f }, 0.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    //Renderer2D::End();


    particleSystem.Render();

    shader.End();
    
    return;
    // */

    //glViewport(0, 0, m_width, m_height);
    shader.Begin();

    //frame.StartFrame(buffer_width, buffer_height);

    Renderer2D::Begin();
    Renderer2D::SetUniforms(shader);    

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (time < 100.0f)
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
                float rx = -1.0f + x / xs * 2.0f;
                float ry = -1.0f + y / ys * 2.0f;
                float rw = 2.0f / xs;
                float rh = 2.0f / ys;
                ry += cos(time * 10.0f * rx) * (2.0f / xs);
                Renderer2D::DrawQuad({ rx, ry }, { rw, rh }, 0.0f, glm::vec4(x / xs, 0.0f, y / ys, 1.0f));//textures[t]);                
                //Renderer2D::DrawQuad({ rx, ry }, { rw, rh }, 0.0f, textures[t]);                
                t++;
                t %= textures.size();
            }
        }
    }
    else if (time < 8.0f)
    {
        
    }
    else
    {
        Renderer2D::DrawQuad({ -1.0f, -1.0f }, { 2.0f, 2.0f }, 0.0f, texture);
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
