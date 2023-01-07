#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Core/Window.h"

class Camera
{
public:
	Camera()
	{
		this->position = glm::vec2(0.0f, 0.0f);
		this->scale = 1.0f;
		matrix = glm::mat4(1.0f);
		needsRecalculation = true;
	}
	Camera(glm::vec2 position, float scale) 
	{ 
		this->position = position;
		this->scale = scale; 
		matrix = glm::mat4(1.0f); 
		needsRecalculation = true; 
	}
	~Camera() {

	}

	void Move(glm::vec2 amount)
	{
		position += amount;
		needsRecalculation = true;
	}
	void Rescale(float amount)
	{
		scale = scale * (1 + amount);
		needsRecalculation = true;
	}

	glm::mat4 GetTransforms()
	{
		if (needsRecalculation || Window::Current->NeedsViewportRecalculations())
		{
			matrix = CalculateTransforms();
			needsRecalculation = false;
		}
		return matrix;
	}

protected:
	glm::vec2 position;
	float scale;

	glm::mat4 matrix;
	bool needsRecalculation;

	glm::mat4 CalculateTransforms()
	{
		int width, height;
		Window::Current->GetSize(width, height);
		glm::mat4 res = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1000.0f)
			;//* glm::scale(glm::mat4(1.0f), glm::vec3(scale));

		return res;
	}
};

