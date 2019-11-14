#version 330 core

in VertexData
{
    vec2 mTexCoord;
    vec4 mColor;
} vertexIn;

void main(void)
{
    gl_FragColor = vertexIn.mColor;
}
