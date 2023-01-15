#pragma once

#include <glm/glm.hpp>

#include "imgui/imgui.h"

#include "Core/InputManager.h"

#include "Core/Window.h"

#include "Drawing/Renderer2D.h"
#include "Drawing/Frame.h"
#include "Drawing/Shader.h"
#include "Drawing/Model.h"
#include "Drawing/ParticleSystem.h"

#include "Drawing/Image.h"

#include "IO/ContentManager.h"
#include "IO/Serializable.h"

#include "Node.h"

class EditorAppWindow : public Window
{
public:
	EditorAppWindow() {
		m_title = "Editor";
		m_width = 1024;
		m_height = 720;
	}

protected:

	Texture* texture = nullptr;

	int buffer_width = 400;
	int buffer_height = 300;

	Frame frame;
	Shader shader;
	Camera camera;

	Model model;
	Texture* modelTexture1 = nullptr;
	Texture* modelTexture2 = nullptr;

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

