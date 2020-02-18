#pragma once

namespace EShader {
enum class Stage
{
    Vertex = 1,
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
