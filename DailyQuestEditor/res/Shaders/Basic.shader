#shader vertex
#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
out vec3 out_color;
void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	out_color = color;
};

#shader fragment
#version 330

in vec3 out_color;
layout(location = 0) out vec4 color;

void main()
{
	color = vec4(out_color, 1.0);
};