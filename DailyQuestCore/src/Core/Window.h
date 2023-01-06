#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Timer.h"
#include "InputManager.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include "Program.h"
#include "Error.h"
#include "Random.h"

class Window : public Program
{
public:
	int Run() override;
	void Dispose();

	void Close() override;

	bool ShouldRestart() override;

	static Window* Current;

protected:
	virtual void OnInit() = 0;
	virtual void OnDispose() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnDraw() = 0;
	virtual void OnImGUIDraw() = 0;

	virtual bool SaveImGUILayout() = 0;

	int m_width = 1024, m_height = 720;
	std::string m_title;
	bool m_shouldRestart = false;

	GLFWwindow* window = nullptr;
private:
	void ResizeCallback(GLFWwindow* window, int width, int height);
	bool UpdateCall(float& deltaTime);
	void DrawCall(float deltaTime);
	void LateUpdateCall(float deltaTime);

	bool disposed = false;
	ImGuiIO* _io = nullptr;
};