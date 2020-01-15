#include <texture/Texture.hpp>
#include <gl/GLTexture.hpp>
#include <dx/DXTexture.hpp>
#include <system/RenderingContext.hpp>
#include <Utils.hpp>

class DX9RenderingContext;

// ITexture* ITexture::create(IRenderingContext& context, const u32 width, const u32 height, const ETexture::Format format, const ETexture::Type textureType) noexcept
// {
//     switch(context.mode().currentMode())
//     {
//         case RenderingMode::Mode::DirectX9:
//         {
// #ifdef _WIN32
//             if(textureType == ETexture::Type::T2D)
//             {
//                 return new(std::nothrow) D3D9Texture2D(width, height, format, (DX9RenderingContext&) context);
//             }
//             else
//             { return null; }
// #else
//             return null;
// #endif
//         }
//         case RenderingMode::Mode::DirectX11:
//         case RenderingMode::Mode::DirectX12:
//         case RenderingMode::Mode::DirectX12_1:
// #ifdef _WIN32
//             return null;
// #else
//             return null;
// #endif
//         case RenderingMode::Mode::Vulkan:
//             return null;
//         case RenderingMode::Mode::OpenGL2:
//         case RenderingMode::Mode::OpenGL3:
//         case RenderingMode::Mode::OpenGL3_1:
//         case RenderingMode::Mode::OpenGL3_2:
//         case RenderingMode::Mode::OpenGL3_3:
//         case RenderingMode::Mode::OpenGL4:
//         case RenderingMode::Mode::OpenGL4_2:
//         case RenderingMode::Mode::OpenGL4_3:
//         case RenderingMode::Mode::OpenGL4_4:
//         case RenderingMode::Mode::OpenGL4_5:
//         case RenderingMode::Mode::OpenGL4_6:
//         {
//             if(textureType == ETexture::Type::T2D)
//             {
//                 return new(std::nothrow) GLTexture2D(width, height, format, 0);
//             }
//             else if(textureType == ETexture::Type::Depth)
//             {
//                 return new(std::nothrow) GLDepthTexture(width, height, format, 0);
//             }
//             else if(textureType == ETexture::Type::Cube)
//             {
//                 return new(std::nothrow) GLTextureCube(width, height, format);
//             }
//             else
//             { return null; }
//         }
//         default: return null;
//     }
// }
//
// ITextureCube* ITextureCube::create(IRenderingContext& context, u32 width, u32 height, ETexture::Format format) noexcept
// {
//     return reinterpret_cast<ITextureCube*>(ITexture::create(context, width, height, format, ETexture::Type::Cube));
// }
