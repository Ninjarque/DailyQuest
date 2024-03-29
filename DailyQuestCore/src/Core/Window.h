#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Timer.h"
#include "TimeStep.h"
#include "InputManager.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include "Program.h"
#include "Error.h"
#include "Random.h"

#include "IO/ContentManager.h"

#include "Physics/Physics2D.h"

#include "Story/StoryManager.h"


class Window : public Program
{
public:
	int Run() override;
	void Dispose();

	void Close() override;

	bool ShouldRestart() override;

	static Window* Current;

	void GetSize(int& width, int& height);
	int GetWidth();
	int GetHeight();

	void SetFreezeOnLostFocus(bool freeze);

	bool NeedsViewportRecalculations();

protected:
	virtual void OnInit() = 0;
	virtual void OnDispose() = 0;
	virtual void OnUpdate(TimeStep timestep) = 0;
	virtual void OnDraw() = 0;
	virtual void OnImGUIDraw() = 0;

	virtual bool SaveImGUILayout() = 0;

	int m_width = 1024, m_height = 720;
	std::string m_title;
	bool m_shouldRestart = false;

	GLFWwindow* window = nullptr;
private:
	void ResizeCallback(GLFWwindow* window, int width, int height);
	void FocusCallback(GLFWwindow* window, int focus);

	bool UpdateCall(float& deltaTime);
	void DrawCall(float deltaTime);
	void LateUpdateCall(float deltaTime);

	bool disposed = false;
	bool focused = true;
	bool freezes = false;
	ImGuiIO* _io = nullptr;

	bool _needsRecalculations = false;

	unsigned long _frameID = 0;
};