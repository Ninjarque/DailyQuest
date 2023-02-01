#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

#include "Vertex2D.h"
#include "Renderer2D.h"

#include "Story/Story.h"

#include "Components/Basic.h"
#include "Components/Drawing.h"
#include "Components/Texture.h"
#include "Components/Camera.h"
#include "Components/Shader.h"

using namespace glm;
using namespace Components;

class Renderer
{
public:
	static void Draw(Story& story);

	static void Init(const std::string shaderPath);
private:
	enum EntityType
	{
		Quad,
		Text
	};

	static void ExtractShader(Entity entity, Shader& shader);
	static void ExtractCamera(Entity entity, Camera& camera);

	static void Render2D(Entity entity, Location& location, Size& size);

	static std::unordered_map<Shader::ShaderType, Shader*> _shaders;
	static Camera* _camera;
	static Viewport* _viewport;

	static Shader* _defaultShader;
};

