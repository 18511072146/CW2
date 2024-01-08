#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube bodybox;

void main()
{
    color = texture(bodybox, TexCoords);
}
