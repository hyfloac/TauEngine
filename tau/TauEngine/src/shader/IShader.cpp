#include "shader/IShader.hpp"
#include "RenderingMode.hpp"
#include "gl/GLShader.hpp"
#include "system/RenderingContext.hpp"

IResourceSelectorTransformer::ResIndex RenderingModeResourceSelectorTransformer::transform(const DynString& key) noexcept
{
    STR_SWITCH(key, {
        STR_CASE("DirectX10", { return 0; })
        STR_CASE("Vulkan",    { return 1; })
        STR_CASE("OpenGL",    { return 2; })
    }, { })
    return -1;
}

IResourceSelectorTransformer::ResIndex RenderingModeResourceSelectorTransformer::transform(const RenderingMode& rm) noexcept
{
    switch(rm.currentMode())
    {
	    case RenderingMode::Mode::DirectX9:
	    case RenderingMode::Mode::DirectX10:
	    case RenderingMode::Mode::DirectX11:
	    case RenderingMode::Mode::DirectX12:
	    case RenderingMode::Mode::DirectX12_1:
	        return 0;
	    case RenderingMode::Mode::Vulkan:
	        return 1;
	    case RenderingMode::Mode::OpenGL3:
	    case RenderingMode::Mode::OpenGL3_1:
	    case RenderingMode::Mode::OpenGL3_2:
	    case RenderingMode::Mode::OpenGL3_3:
	    case RenderingMode::Mode::OpenGL4:
	    case RenderingMode::Mode::OpenGL4_2:
	    case RenderingMode::Mode::OpenGL4_3:
	    case RenderingMode::Mode::OpenGL4_4:
	    case RenderingMode::Mode::OpenGL4_5:
	    case RenderingMode::Mode::OpenGL4_6:
	        return 2;
	    default: return -1;
    }
}

CPPRef<RenderingModeResourceSelectorTransformer> IShaderBuilder::rsTransformer = CPPRef<RenderingModeResourceSelectorTransformer>(new RenderingModeResourceSelectorTransformer);
