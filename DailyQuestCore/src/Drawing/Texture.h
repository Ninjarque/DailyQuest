#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
	Texture(GLuint id, int width, int height)
	{
		_id = id;
		_width = width;
		_height = height;
	}
	void Dispose()
	{
		glDeleteTextures(1, &_id);
		_id = 0;
	}

	int GetWidth() { return _width; }
	int GetHeight() { return _height; }

	GLuint GetID() const { return _id; }
	
	operator GLuint() const { return GetID(); }

private:
	GLuint _id;
	int _width;
	int _height;
};