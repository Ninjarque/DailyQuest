#shader vertex
#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in float depth;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in float texIndex;

out vec2 v_position;
out vec4 v_color;
out vec2 v_texCoord;
out float v_texIndex;

uniform mat4 P;

void main()
{
	gl_Position = P * vec4(position, depth / 1000.0, 1.0);
	v_position = position;
	v_color = color;
	v_texCoord = texCoord;
	v_texIndex = texIndex;
};

#shader fragment
#version 330

in vec2 v_position;
in vec4 v_color;
in vec2 v_texCoord;
in float v_texIndex;

uniform sampler2D v_textures[8];

layout(location = 0) out vec4 color;

void main()
{
	int index = int(v_texIndex);
	color = texture(v_textures[index], vec2(v_texCoord.x, v_texCoord.y)) * v_color;//vec4(out_color, 1.0);
	//color = vec4(v_texCoord.x, v_texCoord.y, v_texIndex, 1.0);
	//color = vec4(0.0,0.0,0.0,1.0);
};