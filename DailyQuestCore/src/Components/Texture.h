#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Components
{
	class Texture
	{
	public:
		Texture(GLuint id, int width, int height)
		{
			_id = id;
			_width = width;
			_height = height;
		}
		Texture(const Texture& texture)
		{
			_id = texture._id;
			_width = texture._width;
			_height = texture._height;
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
}