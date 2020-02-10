#pragma once

namespace EShader {
enum class Stage
{
    Vertex = 0,
    TessellationControl,
    Hull = TessellationControl,
    TessellationEvaluation,
    Domain = TessellationEvaluation,
    Geometry,
    Pixel,
    Fragment = Pixel,
    Compute
};
}
