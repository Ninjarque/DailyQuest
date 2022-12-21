#include "Window.h"


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

    OnInit();

    Timer::start(-1);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Poll for and process events */
        glfwPollEvents();

        float deltaTime = Timer::end(-1, TIME_TYPE::MILLISECONDES) / 1000.0f;
        Timer::start(-1);

        OnUpdate(deltaTime);
        if (disposed)
            break;

        OnDraw();
        OnImGUIDraw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    Dispose();

    return 0;
}

void Window::Close()
{
    glfwDestroyWindow(window);
    window = nullptr;
    Dispose();
}

bool Window::ShouldRestart() { return m_shouldRestart; }