#include "graphics/Resource.debug.hpp"

#if TAU_RESOURCE_DEBUG_DATA

namespace tau { namespace debug {

ResourceDebugCategory ResourceDebugCategory::PositionVertexBuffer(L"Position Vertex Buffer");
ResourceDebugCategory ResourceDebugCategory::NormalVertexBuffer(L"Normal Vertex Buffer");
ResourceDebugCategory ResourceDebugCategory::TangentVertexBuffer(L"Tangent Vertex Buffer");
ResourceDebugCategory ResourceDebugCategory::BitangentVertexBuffer(L"Bitangent Vertex Buffer");
ResourceDebugCategory ResourceDebugCategory::TextureUVVertexBuffer(L"Texture UV Vertex Buffer");
ResourceDebugCategory ResourceDebugCategory::IndexBuffer(L"Index Buffer");
ResourceDebugCategory ResourceDebugCategory::UniformBuffer(L"Uniform Buffer");
ResourceDebugCategory ResourceDebugCategory::UploadBuffer(L"Upload Buffer");
                      
ResourceDebugCategory ResourceDebugCategory::DiffuseTexture(L"Diffuse Texture");
ResourceDebugCategory ResourceDebugCategory::NormalTexture(L"Normal Texture");
ResourceDebugCategory ResourceDebugCategory::HeightTexture(L"Height Texture");
ResourceDebugCategory ResourceDebugCategory::AmbientOcclusionTexture(L"Ambient Occlusion Texture");
                      
ResourceDebugCategory ResourceDebugCategory::MetalnessTexture(L"Metalness Texture");
ResourceDebugCategory ResourceDebugCategory::RoughnessTexture(L"Roughness Texture");
ResourceDebugCategory ResourceDebugCategory::MetalnessF0ControlTexture(L"Metalness/Roughness Workflow Specular F0 Modifier Texture");
                      
ResourceDebugCategory ResourceDebugCategory::SpecularTexture(L"Specular Texture");
ResourceDebugCategory ResourceDebugCategory::GlossinessTexture(L"Glossiness Texture");
                      
ResourceDebugCategory ResourceDebugCategory::OtherBuffer(L"Other Buffer");
ResourceDebugCategory ResourceDebugCategory::OtherTexture(L"Other Texture");
ResourceDebugCategory ResourceDebugCategory::Other(L"Other Resource");

} }

#endif
