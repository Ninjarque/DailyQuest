#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera
{
public:
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
		if (needsRecalculation)
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
		glm::mat4 res = glm::mat4(1.0f);

		return res;
	}
};

