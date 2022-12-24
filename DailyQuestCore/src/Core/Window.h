#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Timer.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include "Program.h"

class Window : public Program
{
public:
	int Run() override;
	void Dispose();

	void Close() override;

	bool ShouldRestart() override;

protected:
	virtual void OnInit() = 0;
	virtual void OnDispose() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnDraw() = 0;
	virtual void OnImGUIDraw() = 0;

	int m_width = 1024, m_height = 720;
	std::string m_title;
	bool m_shouldRestart = false;

	GLFWwindow* window = nullptr;
private:
	bool disposed = false;
};