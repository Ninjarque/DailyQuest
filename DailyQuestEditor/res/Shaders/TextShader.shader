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
	v_texCoord = vec2(texCoord.x, texCoord.y);
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

const float width = 0.35;
const float edge = 0.09;

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}
float screenPxRange()
{
	return 32.0;
}

void main()
{
	int index = int(v_texIndex);
	vec4 mtsdf = texture(v_textures[index], vec2(v_texCoord.x, v_texCoord.y));//vec4(out_color, 1.0);
	//color = mtsdf;
	//return;
	vec3 msd = mtsdf.rgb;
	float distance = 1.0 - mtsdf.a;
	float alpha = 1.0 - smoothstep(width, width + edge, distance);
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange() * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	color = vec4(v_color.rgb, alpha * v_color.a * opacity);
	//color = vec4(v_texCoord.x, v_texCoord.y, v_texIndex, 1.0);
	//color = vec4(0.0,0.0,0.0,1.0);
};