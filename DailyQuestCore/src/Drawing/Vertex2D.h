#pragma once

struct Vertex2D
{
	float Position[2] = { 0.0f, 0.0f };
	float Depth[1] = { 0.0f };
	float Color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float TexCoords[2] = { 0.0f, 0.0f };
	float TextureID[1] = { 0.0f };

	Vertex2D(float x, float y, float depth, float r, float g, float b, float a,
		float texCoordsX, float texCoordsY, float textureID)
	{
		Position[0] = x;
		Position[1] = y;
		Depth[0] = depth;
		Color[0] = r;
		Color[1] = g;
		Color[2] = b;
		Color[3] = a;
		TexCoords[0] = texCoordsX;
		TexCoords[1] = texCoordsY;
		TextureID[0] = textureID;
	};
};