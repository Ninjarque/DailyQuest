#pragma once

#include <glm/glm.hpp>

#include "imgui/imgui.h"

#include "Core/InputManager.h"

#include "Core/Window.h"
#include "Core/TimeStep.h"

#include "Drawing/Renderer.h"
#include "Drawing/Renderer2D.h"
#include "Drawing/Frame.h"
#include "Drawing/Model.h"
#include "Drawing/ParticleSystem.h"

#include "Drawing/Image.h"

#include "IO/ContentManager.h"
#include "IO/Serializable.h"

#include "Drawing/Text/FontManager.h"
#include "Drawing/Text/Font.h"

#include "Story/StoryManager.h"

#include "Components/Basic.h"
#include "Components/Texture.h"
#include "Components/Shader.h"
#include "Components/Camera.h"

#include "Physics/Physics2D.h"

class Game : public Window
{
public:
	Game() {
		m_title = "Moashy's game";
		m_width = 512;
		m_height = 512;
	}

protected:
	void OnInit() override;
	void OnDispose() override;
	void OnUpdate(TimeStep timestep) override;
	void OnDraw() override;
	void OnImGUIDraw() override;

	bool SaveImGUILayout() override { return false; }

private:
	Shader* shader;
};

