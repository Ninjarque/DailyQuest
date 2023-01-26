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

#include "Editor/CommandManager.h"
#include "Test/CommandTests.h"

#include "Drawing/Text/FontManager.h"
#include "Drawing/Text/Font.h"

#include "Node.h"

#include "Story/StoryManager.h"
#include "Story/Components.h"

#include "Physics/Physics2D.h"

class EditorAppWindow : public Window
{
public:
	EditorAppWindow() {
		m_title = "Editor";
		m_width = 1024;
		m_height = 720;
	}

protected:

	void CommandManagerPrint()
	{
		std::cout << "Command stack size " << CommandManager::GetCommandCount() << std::endl;
		std::cout << "Command stack index " << CommandManager::GetCommandStackIndex() << std::endl;
		std::vector<ICommand*> commands = CommandManager::GetCommands();
		if (commands.size() != CommandManager::GetCommandCount()) std::cout << "WUUUUT" << std::endl;
		for (int i = 0; i < commands.size(); i++)
		{
			AddCommand* add = static_cast<AddCommand*>(commands[i]);
			if (add != nullptr)
			{
				std::cout << "Command [" << add->ToString() << "]" << std::endl;
			}
			MultCommand* mult = static_cast<MultCommand*>(commands[i]);
			if (mult != nullptr)
			{
				std::cout << "Command [" << mult->ToString() << "]" << std::endl;
			}
		}
	}

	Texture* texture = nullptr;

	int buffer_width = 400;
	int buffer_height = 300;

	Frame frame;
	Shader shader;
	Shader textShader;
	Camera camera;

	Model model;
	Texture* modelTexture1 = nullptr;

	ParticleSystem particleSystem;

	Font* font;

	std::shared_ptr<Story> story;
	Entity square;
	std::vector<Entity> testEntities;

	float time = 0.0f;
	float deltaTime = 0.0f;

	float color[4] = { 0.5f, 0.3f, 0.5f, 0.4f };
	std::vector<GLuint> textures;

	float TestExpression;

	void OnInit() override;
	void OnDispose() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnImGUIDraw() override;

	bool SaveImGUILayout() override { return true; }
};

