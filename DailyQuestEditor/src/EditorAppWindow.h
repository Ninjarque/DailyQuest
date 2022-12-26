#pragma once

#include "image/image.h"
#include "imgui/imgui.h"

#include "Drawing/Renderer.h"
#include "Drawing/Shader.h"

#include "Core/Window.h"

class EditorAppWindow : public Window
{
public:
	EditorAppWindow() {
		m_title = "Editor";
		m_width = 1024;
		m_height = 720;
	}

protected:

	GLuint texture;
	int width;
	int height;

	int buffer_width = 60;
	int buffer_height = 40;
	Renderer renderer;
	Shader shader;

	float color[4] = { 0.5f, 0.3f, 0.5f, 0.4f };
	std::vector<GLuint> textures;


	void OnInit() override;
	void OnDispose() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnImGUIDraw() override;

	bool SaveImGUILayout() override { return true; }
};

