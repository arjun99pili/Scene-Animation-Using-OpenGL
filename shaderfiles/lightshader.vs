#version 330 core
layout (location = 0) in vec3 modelspace_position;
layout (location = 1) in vec3 modelspace_normal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(modelspace_position, 1.0));
    Normal = mat3(transpose(inverse(model))) * modelspace_normal;  //why transpose?
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}