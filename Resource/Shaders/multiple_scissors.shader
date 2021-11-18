#vertex shader
#version 450 core

layout(location = 0) in vec4 position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out VS_OUT
{
	vec4 color;
} vs_out;

void main(void)
{
	gl_Position = position;
	vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}

#geometry shader
#version 420 core

layout(triangles, invocations = 4) in;
layout(triangle_strip, max_vertices = 3) out;

layout(std140, binding = 0) uniform transform_block
{
	mat4 view[4];
	mat4 projection;
	mat4 model;
};

in VS_OUT
{
	vec4 color;
} gs_in[];

out GS_OUT
{
	vec4 color;
} gs_out;

void main(void)
{
	for (int i = 0; i < gl_in.length(); i++)
	{
		gs_out.color = gs_in[i].color;
		gl_Position = projection * view[gl_InvocationID] * model * gl_in[i].gl_Position;
		gl_ViewportIndex = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
}

#fragment shader
#version 420 core

out vec4 color;

in GS_OUT
{
	vec4 color;
} fs_in;

void main(void)
{
	color = fs_in.color;
}