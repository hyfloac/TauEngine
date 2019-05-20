#version 330 core

layout(location = 0) vec2 pos;
layout(location = 1) vec2 uv;
layout(location = 2) vec4 color;

out vec2 f_uv;
out vec4 f_color;

uniform mat4 projectionMatrix;

void main(void)
{
    f_uv = uv;
    f_color = color;
    gl_Position = projectionMatrix * vec4(pos.xy, 0.0, 1.0);
}
