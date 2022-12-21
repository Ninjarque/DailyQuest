#pragma once
#include "pch.h"

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
	void OnInit() override;
	void OnDispose() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnImGUIDraw() override;
};

