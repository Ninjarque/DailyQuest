#include "Model.h"

void Model::Init(std::vector<float> vertexData,
	std::vector<int> componentDimensions,
	std::vector<bool> normalize)
{
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	int stride = 0;
	std::vector<int> offsets;
	for (auto size : componentDimensions)
	{
		offsets.push_back(stride);
		stride += size * sizeof(float);
	}

	vertexes = vertexData.size() * sizeof(float) / stride;

	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	for (int index = 0; index < componentDimensions.size(); index++)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, componentDimensions[index], GL_FLOAT,
			normalize[index] ? GL_TRUE : GL_FALSE, stride, (const void*)offsets[index]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::Init(std::vector<float> vertexData, 
	std::vector<int> componentDimensions,
	std::vector<bool> normalize,
	std::vector<unsigned int> indices)
{
	Init(vertexData, componentDimensions, normalize);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	ibos = indices.size();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::StartModel(int* elementCount, ModelDrawMode* mode)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	if (ibos > 0)
	{
		*elementCount = ibos;
		*mode = ModelDrawMode::Elements;
	}
	else
	{
		*elementCount = vertexes;
		*mode = ModelDrawMode::Arrays;
	}
}

void Model::EndModel() 
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::Dispose()
{
	glDeleteBuffers(1, &buffer);
}
