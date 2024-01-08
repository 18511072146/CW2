#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 Normal;
} vs_out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    vs_out.FragPos = vec3(model * vec4(position, 1.0f));

    vs_out.TexCoords = vec2(texCoords.x, texCoords.y);

    vs_out.Normal = mat3(transpose(inverse(model))) * normal;
}


