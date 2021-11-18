#vertex shader
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec4 override_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

out VS_OUT{
	vec4 color;
	vec2 tex_coord;
} vs_out;

void main(void)
{
	gl_Position = projection * view * model * position;
	vs_out.color = override_color * color;
	vs_out.tex_coord = tex_coords;
	// vs_out.color = position * 2.0f;
}

#fragment shader
#version 450 core

out vec4 color;

uniform sampler2D texKitten;
uniform sampler2D texPuppy;

in VS_OUT{
	vec4 color;
	vec2 tex_coord;
} fs_in;

void main(void)
{
	// color = vec4(1.0, 0.0, 0.0, 1.0);
	color = fs_in.color;
	// color = texture(texKitten, fs_in.tex_coord);
	// color = fs_in.color * mix(texture(texPuppy, fs_in.tex_coord), texture(texKitten, fs_in.tex_coord), 0.5f);
}

