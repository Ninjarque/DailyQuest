#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

#include "Vertex2D.h"
#include "Camera.h"
#include "Shader.h"

struct RendererData
{
	GLuint quadVAO;
	GLuint quadVBO;
	GLuint quadIBO;

	GLuint defaultTexture = 0;
	unsigned int defaultTextureSlot = 0;

	unsigned int indexCount = 0;

	Vertex2D* quadBuffer;
	Vertex2D* quadBufferPtr;

	std::unordered_map<GLuint, int> textureSlotsMap;
	unsigned int textureSlotIndex = 0;
};

struct RendererStats
{
	int drawCount;
	int quadCount;
};

class Renderer2D
{
public:

	static void Init(int maxQuadCount, int maxTextureCount = 0);
	static void Dispose();

	static void Begin(Shader* shader);
	static void Begin(Shader* shader, bool enableDepth);
	static void Begin(Camera* camera, Shader* shader);
	static void Begin(Camera* camera, Shader* shader, bool enableDepth);
	static void End();

	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		glm::vec4 color);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		GLuint textureID);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		glm::vec4 color, GLuint textureID);

	static void GetStats(int& drawCount, int& quadCount);
	static void ResetStats();
private:
	static int MaxQuadCount;
	static int MaxVertexCount;
	static int MaxIndexCount;
	static int MaxTextureCount;

	static Camera* _currentCamera;
	static Camera* _defaultCamera;
	static Shader* _currentShader;

	static void SetUniforms(Shader* shader);

	static RendererData data;
	static RendererStats stats;

	static void End(bool flush);
	static void Flush();
};

