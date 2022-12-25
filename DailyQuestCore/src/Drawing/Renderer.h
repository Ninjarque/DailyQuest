#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "../Error.h"

class Renderer
{
public:
	Renderer(int textures = 1)
	{
		this->textures = textures;
		width = 0;
		height = 0;
		depthID = 0;
		frameBufferID = 0;
	}
	~Renderer() { DeleteFramebuffer(frameBufferID, textureIDs, depthID); }

	void Init(int width, int height)
	{
		this->width = width;
		this->height = height;
		CreateFramebuffer(width, height, frameBufferID, textures, textureIDs, depthID);
	}

	void StartFrame(int width, int height, GLfloat* clearColor = nullptr)
	{
		if (textureIDs.size() != textures || width != this->width || height != this->height)
		{
			this->width = width;
			this->height = height;
			if (textureIDs.size() == textures)
				DeleteFramebuffer(frameBufferID, textureIDs, depthID);
			CreateFramebuffer(width, height, frameBufferID, textures, textureIDs, depthID);
		}
		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
		if (clearColor != nullptr)
		{
			glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
	std::vector<GLuint> EndFrame() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return textureIDs;
	}
private:
	int width;
	int height;
	int textures;
	std::vector<GLuint> textureIDs;
	GLuint depthID;
	GLuint frameBufferID;

	static void CreateFramebuffer(int width, int height, GLuint& frameBufferID, int textures,
		std::vector<GLuint>& textureIDs, GLuint& depthID)
	{
		glGenFramebuffers(1, &frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

		std::vector<GLenum> drawBuffers;
		for (int i = 0; i < textures; i++)
		{
			GLuint textureID;
			// The texture we're going to render to
			glGenTextures(1, &textureID);

			// "Bind" the newly created texture : all future texture functions will modify this texture
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Give an empty image to OpenGL ( the last "0" )
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

			// Poor filtering. Needed !
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			// Set "renderedTexture" as our colour attachement #0
			GLenum attachement = GL_COLOR_ATTACHMENT0 + i;
			glFramebufferTexture(GL_FRAMEBUFFER, attachement, textureID, 0);
			drawBuffers.push_back(attachement);

			textureIDs.push_back(textureID);
		}

		// The depth buffer
		glGenRenderbuffers(1, &depthID);
		glBindRenderbuffer(GL_RENDERBUFFER, depthID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthID);

		// Set the list of draw buffers.
		glDrawBuffers(1, drawBuffers.data()); // "1" is the size of DrawBuffers

		// Error handling
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Error::fatalError("Could not create renderer's framebuffer!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	static void DeleteFramebuffer(GLuint& frameBufferID, std::vector<GLuint>& textureIDs, GLuint& depthID)
	{
		for (auto textureID : textureIDs)
			glDeleteTextures(1, &textureID);
		textureIDs.clear();
		glDeleteRenderbuffers(1, &depthID);
		glDeleteFramebuffers(1, &frameBufferID);
	}
};

