#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <unordered_map>
#include <string>

#include <sstream>
#include <fstream>

#include "../Error.h"

class Shader
{
public:
	Shader() { program = 0; }
	~Shader();

	void Init(const std::string& shaderFile);
	void Init(const std::string& vertexFile, const std::string& fragmentFile);

	void Set(const char* name, int value);
	void Set(const char* name, float value);
	void Set(const char* name, double value);
	void Set(const char* name, int count, int* value);
	void Set(const char* name, int count, float* value);
	void Set(const char* name, int count, double* value);
	void SetMatrix4(const char* name, float* value);
	void SetMatrix3(const char* name, float* value);

	void Begin();
	void End();

private:
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	GLuint program;
	std::unordered_map<const char*, GLint> uniforms;

	GLint GetUniform(const char* name);

	void Compile(const std::string& vertexFileData, const std::string& fragmentFileData);

	static GLuint CompileShader(GLenum type, const char* source);

	static void ParseShader(const char* filePath, 
		std::string& vertexFileData, std::string& fragmentFileData);
};

