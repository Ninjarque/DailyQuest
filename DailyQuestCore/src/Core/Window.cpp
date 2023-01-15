#include "Window.h"

Window* Window::Current;

void Window::Dispose()
{
    if (disposed)
        return;
    disposed = true;
    if (window != nullptr)
        Close();
    OnDispose();
}

int Window::Run()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::string error = (const char*)glewGetErrorString(err);
        Error::fatalError("Error: " + error);
    }
    std::string glewVersion = (const char*)glewGetString(GLEW_VERSION);
    Error::warning("Status: Using GLEW " + glewVersion);

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    InputManager::Init(window);
    Window::Current = this;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    _io = &ImGui::GetIO(); (void)_io;
    _io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    _io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    _io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;

    Random::Init();

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            Window::Current->ResizeCallback(window, width, height);
        });
    glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focus)
        {
            Window::Current->FocusCallback(window, focus);
        });

    ContentManager::Init();

    OnInit();

    Timer::start(-1);

    if (!SaveImGUILayout())
        ImGui::GetIO().IniFilename = NULL;

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    float deltaTime = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (disposed)
            break;
        if (UpdateCall(deltaTime))
        {
            DrawCall(deltaTime);
            LateUpdateCall(deltaTime);
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    Dispose();

    return 0;
}

void Window::GetSize(int& width, int& height)
{
    width = m_width;
    height = m_height;
}

void Window::SetFreezeOnLostFocus(bool freeze)
{
    freezes = freeze;
}

bool Window::NeedsViewportRecalculations()
{
    return _needsRecalculations;
}

void Window::Close()
{
    glfwDestroyWindow(window);
    window = nullptr;
    Dispose();
}

bool Window::ShouldRestart() { return m_shouldRestart; }

void Window::ResizeCallback(GLFWwindow* window, int width, int height)
{
    if (disposed)
        return;
    _needsRecalculations = true;
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    m_width = display_w;
    m_height = display_h;
    float deltaTime;
    if (UpdateCall(deltaTime))
    {
        DrawCall(deltaTime);
        LateUpdateCall(deltaTime);
    }
}

void Window::FocusCallback(GLFWwindow* window, int focus)
{
    if (focus)
    {
        Window::Current = this;
        _needsRecalculations = true;
        focused = true;
        Timer::end(-1, TIME_TYPE::MILLISECONDES);
    }
    else if (freezes)
    {
        focused = false;
        while (!focused)
        {
            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    else
    {
        focused = false;
    }
}

bool Window::UpdateCall(float& deltaTime)
{
    deltaTime = Timer::end(-1, TIME_TYPE::MILLISECONDES) / 1000.0f;
    Timer::start(-1);

    InputManager::Update(deltaTime);

    /* Poll for and process events */
    glfwPollEvents();

    OnUpdate(deltaTime);
    if (disposed)
        return false;
    return true;
}

void Window::DrawCall(float deltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    OnImGUIDraw();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    OnDraw();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    // */
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}

void Window::LateUpdateCall(float deltaTime)
{
    InputManager::LateUpdate(deltaTime);

    _needsRecalculations = false;
}
