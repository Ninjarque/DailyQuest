#include "Shader.h"

Shader::~Shader()
{
	glDeleteProgram(program);
	//for (auto uniform : uniforms) 
	//	delete[] uniform.first; 
	uniforms.clear();
}

void Shader::Init(const std::string& shaderFile)
{
	std::string vertexFileData;
	std::string fragmentFileData;
	ParseShader(shaderFile.c_str(), vertexFileData, fragmentFileData);
	Compile(vertexFileData, fragmentFileData);
}
void Shader::Init(const std::string& vertexFile, const std::string& fragmentFile)
{
	std::ifstream vertexF(vertexFile);
	std::ifstream fragmentF(fragmentFile);

	std::stringstream vertextBuffer;
	vertextBuffer << vertexF.rdbuf();
	std::stringstream fragmentBuffer;
	fragmentBuffer << fragmentF.rdbuf();
	Compile(vertextBuffer.str(), fragmentBuffer.str());
}


void Shader::Set(const char* name, int value) { glUniform1i(GetUniform(name), value); }
void Shader::Set(const char* name, float value) { glUniform1f(GetUniform(name), value); }
void Shader::Set(const char* name, double value) { glUniform1d(GetUniform(name), value); }
void Shader::Set(const char* name, int count, int* value) { glUniform1iv(GetUniform(name), count, value); }
void Shader::Set(const char* name, int count, float* value) { glUniform1fv(GetUniform(name), count, value); }
void Shader::Set(const char* name, int count, double* value) { glUniform1dv(GetUniform(name), count, value); }
void Shader::SetMatrix4(const char* name, glm::mat4 value)
{
	glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::SetMatrix3(const char* name, glm::mat3 value)
{
	glUniformMatrix3fv(GetUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::Begin() { glUseProgram(program); }
void Shader::End() { glUseProgram(0); }

GLint Shader::GetUniform(const char* name)
{
	if (!uniforms.count(name))
		uniforms[name] = glGetUniformLocation(program, name);
	return uniforms[name];
}

void Shader::Compile(const std::string& vertexFileData, const std::string& fragmentFileData)
{
	program = glCreateProgram();
	GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexFileData.c_str());
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentFileData.c_str());

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLuint Shader::CompileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_alloca(length * sizeof(char));
		glGetShaderInfoLog(shader, length, &length, message);
		std::string shaderType = (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
		Error::warning("Failed to compile " + shaderType + " shader!");
		Error::warning(message);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void Shader::ParseShader(const char* filePath, std::string& vertexFileData, std::string& fragmentFileData)
{
	std::ifstream file(filePath);

	std::string line;
	ShaderType type = ShaderType::NONE;;
	std::stringstream ss[2];
	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
			else
			{
				type = ShaderType::NONE;
			}
		}
		else
		{
			int t = (int)type;
			if (t != -1)
			{
				ss[t] << line << '\n';
			}
		}
	}
	vertexFileData = ss[0].str();
	fragmentFileData = ss[1].str();
}
