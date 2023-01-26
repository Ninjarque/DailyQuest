#pragma once

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
	void OnUpdate(TimeStep timestep) override;
	void OnDraw() override;
	void OnImGUIDraw() override;

	bool SaveImGUILayout() override { return false; }

private:
	float delay = 1.0f;
};
