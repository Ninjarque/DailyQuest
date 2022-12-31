#pragma once

#include <glm/glm.hpp>

#include "image/image.h"
#include "imgui/imgui.h"

#include "Drawing/Renderer2D.h"
#include "Drawing/Frame.h"
#include "Drawing/Shader.h"
#include "Drawing/Model.h"
#include "Drawing/ParticleSystem.h"

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

	int buffer_width = 400;
	int buffer_height = 300;

	Frame frame;
	Shader shader;

	Model model;
	GLuint modelTexture1;
	GLuint modelTexture2;

	ParticleSystem particleSystem;

	float time = 0.0f;
	float deltaTime = 0.0f;

	float color[4] = { 0.5f, 0.3f, 0.5f, 0.4f };
	std::vector<GLuint> textures;


	void OnInit() override;
	void OnDispose() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnImGUIDraw() override;

	bool SaveImGUILayout() override { return true; }
};

