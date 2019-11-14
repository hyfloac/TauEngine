#version 330 core

in VertexData 
{
    vec2 texCoord;
} vertexIn;

layout(location = 0) out vec4 fragColor;

uniform sampler2D textBMP;
uniform vec3 textColor;

void main()
{
    fragColor = vec4(textColor, texture(textBMP, vertexIn.texCoord).r);
}
