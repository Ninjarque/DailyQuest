#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

#include "Vertex2D.h"

#include "Components/Basic.h"
#include "Components/Drawing.h"
#include "Components/Texture.h"
#include "Components/Camera.h"
#include "Components/Shader.h"

#include "Story/Story.h"

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

	static void Begin(Camera* camera, Shader* shader, Viewport* viewport);
	static void Begin(Camera* camera, Shader* shader, Viewport* viewport, bool enableDepth);
	static void End();

	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		glm::vec4 color, glm::vec2 origin, float angle);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		GLuint textureID, glm::vec2 origin, float angle);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		GLuint textureID, glm::vec2 uv_position, glm::vec2 uv_size, glm::vec2 origin, float angle);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		glm::vec4 color, GLuint textureID, glm::vec2 origin, float angle);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		glm::vec4 color, GLuint textureID, glm::vec2 uv_position, glm::vec2 uv_size, glm::vec2 origin, float angle);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		Texture* texture, glm::vec2 origin, float angle);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		Texture* texture, glm::vec2 uv_position, glm::vec2 uv_size, glm::vec2 origin, float angle);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		glm::vec4 color, Texture* texture, glm::vec2 origin, float angle);
	static void DrawQuad(glm::vec2 position, glm::vec2 size, float depth,
		glm::vec4 color, Texture* texture, glm::vec2 uv_position, glm::vec2 uv_size, glm::vec2 origin, float angle);

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
	static Viewport* _currentViewport;

	static void SetUniforms(Shader* shader);

	static RendererData data;
	static RendererStats stats;

	static void End(bool flush);
	static void Flush();
};

