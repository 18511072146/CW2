#version 330 core
in vec3 Normal;
in vec3 Position;

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{   
    color = vec4(texture(texture_diffuse1, TexCoords));
}
