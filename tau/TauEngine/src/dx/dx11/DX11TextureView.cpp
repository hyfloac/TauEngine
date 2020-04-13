#include "dx/dx11/DX11TextureView.hpp"

#ifdef _WIN32
#include "dx/dx11/DX11RenderingContext.hpp"
#include "TauEngine.hpp"
#include "TauConfig.hpp"

void DX11TextureView::generateMipmaps(IRenderingContext& context) noexcept
{
#if TAU_RTTI_CHECK
    if(!RTT_CHECK(context, DX11RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }
#endif

    auto& ctx = reinterpret_cast<DX11RenderingContext&>(context);

    ctx.d3d11DeviceContext()->GenerateMips(_d3dShaderResourceView);
}
#endif
