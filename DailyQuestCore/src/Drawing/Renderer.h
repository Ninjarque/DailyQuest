#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

#include "Vertex2D.h"

using namespace glm;

enum class VertexType {
	Quad,
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init(int maxBufferElementCount, int maxBufferTextureCount);

	void StartRender();

	void DrawQuad(float x, float y, float width, float height, float depth, 
		vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f), GLuint texture = 0);

	void EndRender();

private:
	int maxBufferElementCount;
	int maxBufferTextureCount;
	std::unordered_map<VertexType, std::vector<GLuint>> vaosList;
	std::unordered_map<VertexType, std::vector<GLuint>> vbosList;
	std::unordered_map<VertexType, std::vector<GLuint>> ibosList;

	std::unordered_map<VertexType, int> currentArrayElementCounts;
	std::unordered_map<VertexType, std::vector<std::vector<float>>> currentArrayElementData;
	std::unordered_map<VertexType, std::vector<std::vector<unsigned int>>> currentArrayElementIndices;
	std::unordered_map<VertexType, std::vector<std::unordered_map<GLuint, int>>> currentArrayElementTextures;

	int GetVertexesCount(VertexType type);
	int GetSizeofVertexes(VertexType type);
	int GetSizeofIBO(VertexType type);

	void CreateVertexBuffer(GLuint& vao, GLuint& vbo, GLuint& ibo,
		VertexType type, int arrayByteSize, bool useIBO);
};

