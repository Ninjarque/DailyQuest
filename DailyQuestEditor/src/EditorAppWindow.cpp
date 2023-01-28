#include "EditorAppWindow.h"

#include <math.h>

using namespace std;

void EditorAppWindow::OnInit()
{
    texture = Image::Load("res/Images/img1.png");
    //IM_ASSERT(ret);
    modelTexture1 = Image::Load("res/Images/soldier.png");
    
    Renderer2D::Init(128);
    frame.Init(buffer_width, buffer_height);

    Renderer::Init("res/Shaders/Basic.shader");

    story = StoryManager::CreateStory();
    
    shader = new Shader("res/Shaders/Basic.shader", Shader::ShaderType::Default);
    textShader = new Shader("res/Shaders/TextShader.shader", Shader::ShaderType::Default);

    particleSystem.Init(10000, ParticleSystem::RenderMode::Render2D);
    particleSystem.SetPhysics(ParticlePhysic(
        glm::vec3(0.0f, 3.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec4(-1.5f, -1.0f, -1.0f, -2.0f),
        0.01f, 0.01f, 0.01f
    ));

    InputManager::SetBinding(InputType::Keyboard, "A", GLFW_KEY_Q);
    InputManager::SetBinding(InputType::Keyboard, "Z", GLFW_KEY_W);
    InputManager::SetBinding(InputType::Mouse, "Spawn", GLFW_MOUSE_BUTTON_1);

    cameraEntity = story->CreateEntity();
    cameraEntity.Add<Camera>(glm::vec2(0.0f, 0.0f), 1.0f);

    NodeList* lst = new NodeList(
        std::vector<Node*>
        {
            new Node("Root"),
            new Node("Cube", new Node("CubeChild1"), new Node("CubeChild2")),
            new Node("Sphere")
        }
    );

    Node* n = new Node("Root",
        new Node("Sub1",
            new Node("SubSub1"),
            new Node("SubSub2")
            ),
        new Node("Sub2")
        );

    /*
    std::cout << "Node list :\n" << std::endl << lst->ToString() << std::endl;
    ContentManager::Save("C:\\Users\\ninja\\Desktop\\NODE.nd", lst);
    Data d;
    d["Node1"];
    Node* n2 = ContentManager::Load<Node>("C:\\Users\\ninja\\Desktop\\NODE.nd", &d);
    std::cout << "Node : \n" << std::endl << n2->ToString() << std::endl;
    */
    CommandManager::Init(3);

    TestExpression = 0.0f;
    std::cout << "Test expression current " << TestExpression << std::endl;
    CommandManager::Push(new AddCommand(1.0f, &TestExpression));
    std::cout << "Test expression current " << TestExpression << std::endl;
    CommandManager::Push(new AddCommand(2.0f, &TestExpression));
    std::cout << "Test expression current " << TestExpression << std::endl;
    CommandManager::Push(new AddCommand(4.0f, &TestExpression));
    std::cout << "Test expression current " << TestExpression << std::endl;
    CommandManagerPrint();
    std::cout << "Test expression current " << TestExpression << std::endl;
    CommandManager::Undo();
    CommandManagerPrint();
    std::cout << "Test expression current " << TestExpression << std::endl;
    CommandManager::Do();
    CommandManagerPrint();
    std::cout << "Test expression current " << TestExpression << std::endl;

    //font = FontManager::Create("res/Fonts/Pokemon.ttf", FontDetails::Better);
    //font = FontManager::Create("res/Fonts/Xiomara.ttf", FontDetails::Better);
    font = FontManager::Create("res/Fonts/Consola.ttf", FontDetails::Better);

    square = story->CreateEntity();
    struct Test { std::string Name; Test(std::string name) : Name(name) { } };
    auto name = square.Get<Name>();
    std::cout << "Entity name : " << name << std::endl;
    int w, h;
    Window::Current->GetSize(w, h);
    square.Add<Location>((float)w/2.0f, (float)h-50.0f);
    square.Add<Size>((float)w/2.0f, 100.0f);
    Physics2D::CreateBody(square, true);
    Physics2D::CreateBoxShape(square);
    squareChild = story->CreateEntity();
    square.Add<Childrens>(squareChild);
}

void EditorAppWindow::OnDispose()
{
	cout << "Disposed of editor I see" << endl;
}

void EditorAppWindow::OnUpdate(TimeStep timestep)
{
    this->deltaTime = timestep;
    time += timestep;
    static float particleSpawnTime = 0.0f;
    particleSpawnTime += timestep;
    bool spawn = false;
    if (particleSpawnTime > 0.02f)
    {
        spawn = true;
    }
    int w, h;
    Window::Current->GetSize(w, h);
    if (InputManager::IsPressed("A"))
    {
        Bindings* b = new Bindings();
        b->SetBinding(InputType::Mouse, "None", GLFW_MOUSE_BUTTON_1);
        InputManager::PushBindings(b);
    }
    if (InputManager::IsPressed("Z"))
    {
        InputManager::PopBindings();
    }
    if (InputManager::IsReleased("None"))
    {
        std::cout << "Hold on" << std::endl;
    }
    square.Set<Location>((float)w / 2.0f, (float)h - 50.0f);
    square.Set<Size>((float)w/2.0f, 100.0f);
    if ((InputManager::IsDown("Spawn") || InputManager::IsDown("None")) && spawn)
    {
        float ox = (Random::Float() - 0.5f) * w / 6.0f;
        float oy = (Random::Float() - 0.5f) * h / 6.0f;
        double x;
        double y;
        Mouse::GetPosition(x, y);
        Entity e = story->CreateEntity();
        e.Add<Location>((float)x + ox, (float)y + oy);
        e.Add<Size>(100.0f, 100.0f);
        Physics2D::CreateBody(e);
        Physics2D::CreateBoxShape(e);
        testEntities.push_back(e);

        /*
        particleSpawnTime = 0.0f;
        ParticleProperties fire(
            1.0f,
            glm::vec3((float)x, (float)y, 0.0f),
            glm::vec3(0.0f),
            glm::vec3(100.0f, 200.0f, 100.0f),
            glm::vec4(1.0f, 0.4f, 0.3f, 1.0f),
            0);
        fire.PositionVariation = glm::vec3(0.03f) * (float)h;
        fire.Torque = glm::vec3(glm::pi<float>() * 2.0f);
        fire.Velocity = glm::vec3(0.0f, -0.3f, 0.0f) * 0.5f * (float)h;
        fire.VelocityVariation = glm::vec3(0.3f, 0.5f, 0.0f) * 0.2f * (float)h;
        fire.ColorChange = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        for (int i = 0; i < 30; i++)
        {
            particleSystem.Emit(fire);
        }
        */
    }

    particleSystem.Update(timestep);
}

void EditorAppWindow::OnDraw()
{
    int w, h;
    Window::Current->GetSize(w, h);
    std::vector<Texture*> textures = { modelTexture1, texture };

    // /*
    shader->Begin();
    
    //Renderer2D::Begin();
    //Renderer2D::SetUniforms(shader);

    //Renderer2D::DrawQuad({ -1.0f, -1.0f }, { 2.0f, 2.0f }, 0.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    //Renderer2D::End();

    particleSystem.Render(cameraEntity.Get<Camera>(), *shader);


    shader->End();
    

    font->Render(&cameraEntity.Get<Camera>(), textShader, ("Cubes : ") + std::to_string(testEntities.size()),
        glm::vec2(0.0f, 0.0f), glm::vec2(w, h/2.0f), 50.0f, glm::vec4(1.0f,1.0,0.0,1.0f));
    font->Render(&cameraEntity.Get<Camera>(), textShader, U"ça beigne là tranquille Gaëlle ? こんにちは ou bien ?",
        glm::vec2(0.0f, h/2.0f), glm::vec2(w, h/2.0f), 200.0f, glm::vec4(1.0f, 1.0, 0.0, 1.0f), -0.2f, glm::vec2(-0.01f, 0.01f), glm::vec4(1.0f,0.0f,0.0f,1.0f));
    //font->Render(&textShader, U"t",
    //    glm::vec2(0.0f, h / 2.0f), glm::vec2(w, h / 2.0f), 3.0f, glm::vec4(1.0f, 1.0, 0.0, 1.0f));

    // */

    //glViewport(0, 0, m_width, m_height);
    shader->Begin();

    //frame.StartFrame(buffer_width, buffer_height);

    Renderer2D::Begin(&cameraEntity.Get<Camera>(), shader);

    /*
    auto loc = square.Get<Location>();
    auto size = square.Get<Size>();
    auto angle = square.Get<Angle>();
    Renderer2D::DrawQuad(glm::vec2(loc.X - size.X / 2.0f, loc.Y - size.Y / 2.0f), glm::vec2(size.X, size.Y),
        0.0f, texture, glm::vec2(loc.X, loc.Y), angle.Value);

    for (auto entity : testEntities)
    {
        auto loc = entity.Get<Location>();
        auto size = entity.Get<Size>();
        auto angle = entity.Get<Angle>();
        Renderer2D::DrawQuad(glm::vec2(loc.X - size.X / 2.0f, loc.Y - size.Y / 2.0f), glm::vec2(size.X, size.Y),
            0.0f, nullptr, glm::vec2(loc.X, loc.Y), angle.Value);
    }
    */
    Renderer::Draw(*story.get());

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
                Renderer2D::DrawQuad({ rx, ry }, { rw, rh }, 0.0f, glm::vec4(x / xs, 0.0f, y / ys, 1.0f), glm::vec2(0.0f), 0.0f);//textures[t]);                
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
        //Renderer2D::DrawQuad({ (float)w / 2.0f, (float)h / 2.0f }, { (float)x- (float)w / 2.0f, (float)y - (float)h / 2.0f }, 0.0f, texture);
    }
    
    Renderer2D::End();
    //renderer.EndRender();

    //textures = frame.EndFrame();
    //glViewport(0, 0, m_width, m_height);

    //renderer.StartRender();
    //renderer.DrawQuad(-1.0f, -1.0f, 2.0f, 2.0f, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), textures[0]);
    //renderer.EndRender();

    shader->End();
}

void EditorAppWindow::OnImGUIDraw()
{
    return;
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
