#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D sprite;
uniform vec4 color;
uniform bool useTexture;

void main()
{
    if (useTexture)
        FragColor = texture(sprite, TexCoord);
    else
        FragColor = vec4(color);
}