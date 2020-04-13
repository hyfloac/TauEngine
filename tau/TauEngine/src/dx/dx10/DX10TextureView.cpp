#include "dx/dx10/DX10TextureView.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10RenderingContext.hpp"
#include "TauEngine.hpp"
#include "TauConfig.hpp"

void DX10TextureView::generateMipmaps(IRenderingContext& context) noexcept
{
#if TAU_RTTI_CHECK
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }
#endif

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ctx.d3dDevice()->GenerateMips(_d3dShaderResourceView);
}
#endif
