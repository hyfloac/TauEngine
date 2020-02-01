#version 430 core

in VertexData 
{
    vec2 texCoord;
} vertexIn;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform Color
{
    vec4 textColor;
};

layout(location = 2) uniform sampler2D textBMP;

// uniform sampler2D textBMP;
// uniform vec3 textColor;

void main()
{
    fragColor = vec4(textColor.xyz, texture(textBMP, vertexIn.texCoord).r);
}
