#include "Renderer2D.h"

int Renderer2D::MaxQuadCount;
int Renderer2D::MaxVertexCount;
int Renderer2D::MaxIndexCount;
int Renderer2D::MaxTextureCount;

Camera* Renderer2D::_currentCamera;
Camera* Renderer2D::_defaultCamera = nullptr;

Shader* Renderer2D::_currentShader;

RendererData Renderer2D::data;
RendererStats Renderer2D::stats;

void Renderer2D::Init(int maxQuadCount, int maxTextureCount)
{
	MaxQuadCount = maxQuadCount;
	MaxVertexCount = MaxQuadCount * 4;
	MaxIndexCount = MaxQuadCount * 6;
	if (maxTextureCount == 0)
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max);
		maxTextureCount = max;
	}
	MaxTextureCount = maxTextureCount;

	data.quadBuffer = new Vertex2D[MaxVertexCount];

	glGenVertexArrays(1, &data.quadVAO);
	glBindVertexArray(data.quadVAO);

	glGenBuffers(1, &data.quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, data.quadVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	//glEnableVertexArrayAttrib(data.quadVAO, 0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, Position));

	glEnableVertexAttribArray(1);
	//glEnableVertexArrayAttrib(data.quadVAO, 1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, Depth));

	glEnableVertexAttribArray(2);
	//glEnableVertexArrayAttrib(data.quadVAO, 2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, Color));

	glEnableVertexAttribArray(3);
	//glEnableVertexArrayAttrib(data.quadVAO, 3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, TexCoords));

	glEnableVertexAttribArray(4);
	//glEnableVertexArrayAttrib(data.quadVAO, 4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, TextureID));


	glEnableVertexAttribArray(5);
	//glEnableVertexArrayAttrib(data.quadVAO, 4);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, Origin));

	glEnableVertexAttribArray(6);
	//glEnableVertexArrayAttrib(data.quadVAO, 4);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, Angle));

	unsigned int* indices = new unsigned int[MaxIndexCount];
	int offset = 0;
	for (int i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i] = offset;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;
		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset;
		offset += 4;
	}
	glGenBuffers(1, &data.quadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.quadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	delete[] indices;

	//1 x 1 white square
	glGenTextures(1, &data.defaultTexture);
	glBindTexture(GL_TEXTURE_2D, data.defaultTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GLuint color = 0xffffffff;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
	glBindTexture(GL_TEXTURE_2D, 0);

	data.textureSlotsMap[data.defaultTexture] = data.defaultTextureSlot;
	data.textureSlotIndex = 1;
}

void Renderer2D::Dispose()
{
	glDeleteVertexArrays(1, &data.quadVAO);
	glDeleteBuffers(1, &data.quadVBO);
	glDeleteBuffers(1, &data.quadIBO);

	glDeleteTextures(1, &data.defaultTexture);

	delete[] data.quadBuffer;
}

void Renderer2D::Begin(Camera* camera, Shader* shader, bool enableDepth)
{
	if (enableDepth)
		glEnable(GL_DEPTH_TEST);
	Begin(camera, shader);
}

void Renderer2D::Begin(Camera* camera, Shader* shader)
{
	_currentCamera = camera;
	_currentShader = shader;

	data.quadBufferPtr = data.quadBuffer;
	data.textureSlotsMap.clear();
	data.textureSlotsMap[data.defaultTexture] = data.defaultTextureSlot;

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetUniforms(_currentShader);
}

void Renderer2D::End()
{
	End(true);
}
void Renderer2D::End(bool flush)
{
	GLsizeiptr size = (uint8_t*)data.quadBufferPtr - (uint8_t*)data.quadBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, data.quadVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data.quadBuffer);
	if (flush)
		Flush();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer2D::Flush()
{
	for (auto textureSlot : data.textureSlotsMap)
	{
		glBindTextureUnit(textureSlot.second, textureSlot.first);
	}
	glBindVertexArray(data.quadVAO);
	glDrawElements(GL_TRIANGLES, data.indexCount, GL_UNSIGNED_INT, nullptr);
	stats.drawCount++;

	data.indexCount = 0;
	data.textureSlotIndex = 1;

	glBindVertexArray(0);
}


void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float depth, glm::vec4 color,
	glm::vec2 origin, float angle)
{
	DrawQuad(position, size, depth, color, data.defaultTexture, origin, angle);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float depth, GLuint textureID,
	glm::vec2 origin, float angle)
{
	DrawQuad(position, size, depth, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), textureID, origin, angle);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float depth, GLuint textureID, glm::vec2 uv_position, glm::vec2 uv_size,
	glm::vec2 origin, float angle)
{
	DrawQuad(position, size, depth, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), textureID, uv_position, uv_size, origin, angle);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float depth, Texture* texture,
	glm::vec2 origin, float angle)
{
	GLuint id = 0; if (texture != nullptr) id = *texture;
	DrawQuad(position, size, depth, id, origin, angle);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float depth, Texture* texture, glm::vec2 uv_position, glm::vec2 uv_size,
	glm::vec2 origin, float angle)
{
	GLuint id = 0; if (texture != nullptr) id = *texture;
	DrawQuad(position, size, depth, id, uv_position, uv_size, origin, angle);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float depth, glm::vec4 color, Texture* texture,
	glm::vec2 origin, float angle)
{
	GLuint id = 0; if (texture != nullptr) id = *texture;
	DrawQuad(position, size, depth, color, id, origin, angle);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float depth, glm::vec4 color, Texture* texture, glm::vec2 uv_position, glm::vec2 uv_size,
	glm::vec2 origin, float angle)
{
	GLuint id = 0; if (texture != nullptr) id = *texture;
	DrawQuad(position, size, depth, color, id, glm::vec2(0.0f), glm::vec2(1.0f), origin, angle);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float depth, glm::vec4 color, GLuint textureID,
	glm::vec2 origin, float angle)
{
	DrawQuad(position, size, depth, color, textureID, glm::vec2(0.0f), glm::vec2(1.0f), origin, angle);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, float depth, glm::vec4 color, GLuint textureID, glm::vec2 uv_position, glm::vec2 uv_size,
	glm::vec2 origin, float angle)
{
	if (data.indexCount >= MaxIndexCount || data.textureSlotIndex >= MaxTextureCount)
	{
		End(false);
		Flush();
		Begin(_currentCamera, _currentShader);
	}
	if (textureID == 0) textureID = data.defaultTexture;
	if (!data.textureSlotsMap.count(textureID))
	{
		data.textureSlotsMap[textureID] = data.textureSlotIndex;
		data.textureSlotIndex++;
	}
	float texture = data.textureSlotsMap[textureID];

	*data.quadBufferPtr =
		Vertex2D(position.x, position.y, depth, color.x, color.y, color.z, color.w,
			uv_position.x, uv_position.y, texture, origin.x, origin.y, angle);
	data.quadBufferPtr++;

	*data.quadBufferPtr =
		Vertex2D(position.x + size.x, position.y, depth, color.x, color.y, color.z, color.w,
			uv_position.x + uv_size.x, uv_position.y, texture, origin.x, origin.y, angle);
	data.quadBufferPtr++;

	*data.quadBufferPtr =
		Vertex2D(position.x + size.x, position.y + size.y, depth, color.x, color.y, color.z, color.w,
			uv_position.x + uv_size.x, uv_position.y + uv_size.y, texture, origin.x, origin.y, angle);
	data.quadBufferPtr++;

	*data.quadBufferPtr =
		Vertex2D(position.x, position.y + size.y, depth, color.x, color.y, color.z, color.w,
			uv_position.x, uv_position.y + uv_size.y, texture, origin.x, origin.y, angle);
	data.quadBufferPtr++;

	data.indexCount += 6;
	stats.quadCount++;
}

void Renderer2D::SetUniforms(Shader* shader)
{
	static std::vector<int> sampler(MaxTextureCount);
	int i = 0;
	for (auto& s : sampler)
	{
		s = i; i++;
	}
	shader->Set("v_textures", sampler.size(), sampler.data());
	shader->SetMatrix4("P", _currentCamera->GetTransforms());
}

void Renderer2D::GetStats(int& drawCount, int& quadCount)
{
	drawCount = stats.drawCount;
	quadCount = stats.quadCount;
}

void Renderer2D::ResetStats()
{
	stats = RendererStats();
}