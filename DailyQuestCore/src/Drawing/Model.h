#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

enum ModelDrawMode {
	Arrays,
	Elements
};

class Model
{
public:
	Model() {
		buffer = 0; ibo = 0; vertexes = 0; ibos = 0;
	}
	~Model() { Dispose(); }

	void Init(std::vector<float> vertexData,
		std::vector<int> componentDimensions,
		std::vector<bool> normalize);
	void Init(std::vector<float> vertexData,
		std::vector<int> componentDimensions,
		std::vector<bool> normalize,
		std::vector<unsigned int> indices);
	void Dispose();

	void StartModel(int* elementCount, ModelDrawMode* mode);
	void EndModel();
private:
	GLuint buffer;
	GLuint ibo;
	int vertexes;
	int ibos;
};

