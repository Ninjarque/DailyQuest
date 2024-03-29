#shader vertex
#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in float depth;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in float texIndex;

layout(location = 5) in vec2 origin;
layout(location = 6) in float angle;

out vec2 v_position;
out vec4 v_color;
out vec2 v_texCoord;
out float v_texIndex;

uniform mat4 P;

vec2 rotate(vec2 position, vec2 origin, float angle)
{
	vec2 res = position - origin;

	res = vec2(res.x * cos(angle) - res.y * sin(angle), res.y * cos(angle) + res.x * sin(angle));

	return res + origin;
}

void main()
{
	vec2 rotatedPos = rotate(position, origin, angle);
	gl_Position = P * vec4(rotatedPos, depth / 1000.0, 1.0);
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

uniform float char_width = 0.35;
uniform float char_edge = 0.09;

uniform float border_width = 0.1;
uniform float border_edge = 0.09;
uniform vec4 second_color = vec4(0.0,0.0,0.0,0.0);

uniform vec2 shadow_offset = vec2(0.0,0.0);

uniform float metrics_ratio;

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}
float screenPxRange()
{
	return 2.0;
}

void main()
{
	int index = int(v_texIndex);
	vec4 mtsdf = texture(v_textures[index], vec2(v_texCoord.x, v_texCoord.y));//vec4(out_color, 1.0);
	vec4 mtsdf2 = texture(v_textures[index], vec2(v_texCoord.x + shadow_offset.x, v_texCoord.y + shadow_offset.y));//vec4(out_color, 1.0);
	//color = mtsdf;
	//return;
	float edge = char_edge;
	vec3 msd = mtsdf.rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float distance = 1.0 - mtsdf.a;
	float alpha = 1.0 - smoothstep(char_width, char_width + edge, distance);

	float bedge = 0.1;//border_edge;
	vec3 msd2 = mtsdf2.rgb;
	float sd2 = median(msd2.r, msd2.g, msd2.b);
	float distance2 = 1.0 - mtsdf2.a;
	float outline_alpha = 1.0 - smoothstep(border_width, border_width + bedge, distance2);

	float overallAlpha = alpha + (1.0 - alpha) * outline_alpha;
	vec4 overallColor = mix(second_color, v_color, alpha / overallAlpha);

	float screenPxDistance = screenPxRange() * (sd - 0.5);
	float opacity = smoothstep(edge, 1.0, screenPxDistance + 0.5);//clamp(screenPxDistance + 0.5, edge, 1.0);
	color = overallColor;//vec4(v_color.rgb, max(alpha, opacity));
};