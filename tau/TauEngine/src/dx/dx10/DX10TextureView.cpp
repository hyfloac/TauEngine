#include "dx/dx10/DX10TextureView.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10RenderingContext.hpp"
#include "TauEngine.hpp"

void DX10TextureView::generateMipmaps(IRenderingContext& context) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    ctx.d3dDevice()->GenerateMips(_d3dShaderResourceView);
}
#endif
