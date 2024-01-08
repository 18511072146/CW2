#version 330 core

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    vec3 Normal;
} fs_in;

out vec4 FragColor;

void main()
{           
    FragColor = vec4(0.0f, 0.8f, 0.0f, 1.0f);
}
