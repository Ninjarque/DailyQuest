#pragma once
#include "pch.h"

#include "Core/Window.h"

class EditorLauncherWindow : public Window
{
public:
	EditorLauncherWindow() {
		m_title = "Editor launcher biatches !!!";
		m_width = 512;
		m_height = 512;
	}

protected:
	void OnInit() override;
	void OnDispose() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnImGUIDraw() override;

private:
	float delay = 1.0f;
};
