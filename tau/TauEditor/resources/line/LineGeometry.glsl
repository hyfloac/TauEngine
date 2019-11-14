#version 330 core;

layout(lines_adjecency) in;
layout(triangle_strip, max_vertices = 7) out;

in VertexData
{
    vec4 mColor;
} vertexIn[4];

out VertexData
{
    vec2 mTexCoord;
    vec4 mColor;
} vertexOut;

uniform float lineThickness;
uniform vec4 viewport;
uniform float miterLimit;

vec2 toScreenSpace(vec4 vertex)
{
    return vec2(vertex.xy / vertex.w) * viewport.zw;
}

float toZValue(vec4 vertex)
{
    return vertex.z / vertex.w;
}

void drawSegment(vec2 points[4], vec4 colors[4], float zValues[4])
{
    vec2 p0 = points[0];
    vec2 p1 = points[1];
    vec2 p2 = points[2];
    vec2 p3 = points[3];

    // perform naive culling
    vec2 area = viewport.zw * 4;
    if(p1.x < -area.x || p1.x > area.x) { return; }
    if(p1.y < -area.y || p1.y > area.y) { return; }
    if(p2.x < -area.x || p2.x > area.x) { return; }
    if(p2.y < -area.y || p2.y > area.y) { return; }

    // determine the direction of each of the 3 segments (prev, curr, next)
    vec2 v0 = normalize(p1 - p0);
    vec2 v1 = normalize(p2 - p1);
    vec2 v2 = normalize(p3 - p2);

    // determine the normal of each of the 3 segments
    vec2 n0 = vec2(-v0.y, v0.x);
    vec2 n1 = vec2(-v1.y, v1.x);
    vec2 n2 = vec2(-v2.y, v2.x);

    // determine miter lines by averagine the normals of the 2 segments
    vec2 miter_a = normalize(n0 + n1);
    vec2 miter_b = normalize(n1 + n2);
}

void main(void)
{
    vec4 points[4];
    points[0] = gl_in[0].gl_Position;
    points[1] = gl_in[1].gl_Position;
    points[2] = gl_in[2].gl_Position;
    points[3] = gl_in[3].gl_Position;

    vec4 colors[4];
    colors[0] = vertexIn[0].mColor;
    colors[1] = vertexIn[1].mColor;
    colors[2] = vertexIn[2].mColor;
    colors[3] = vertexIn[3].mColor;

    vec2 screenPoints[4];
    screenPoints[0] = toScreenSpace(points[0]);
    screenPoints[1] = toScreenSpace(points[1]);
    screenPoints[2] = toScreenSpace(points[2]);
    screenPoints[3] = toScreenSpace(points[3]);

    float zValues[4];
    zValues[0] = toZValue(points[0]);
    zValues[1] = toZValue(points[1]);
    zValues[2] = toZValue(points[2]);
    zValues[3] = toZValue(points[3]);

    drawSegment(screenPoints, colors, zValues);
}
