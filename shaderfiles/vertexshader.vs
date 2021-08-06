#version 330 core

layout(location = 0) in vec3 modelspace_position;
layout(location = 1) in vec3 modelspace_normal;

out vec3 FragPos;
out vec3 Normal;

// Values that stay constant for the whole mesh.

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  projection * view * model * vec4(modelspace_position, 1.0f);
	FragPos = vec3(model * vec4(modelspace_position, 1.0));
	Normal = mat3(transpose(inverse(model))) * modelspace_normal;
}
