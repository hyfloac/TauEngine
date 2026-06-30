#pragma once

#include <NumTypes.hpp>
#include <com/TauObject.hpp>

namespace tau::graphics {

struct CommandListCapabilities final
{
    b8 nativeCommandListSupport : 1;
    b8 bundleInheritsState : 1;
};

struct ShaderCapabilities final
{
    b8 supportsGeometry : 1;
    b8 supportsTessellation : 1;
    b8 supportsMesh : 1;
    b8 supportsRayTracing : 1;
    b8 supportsCompute : 1;
};

struct HeapCapabilities final
{
    b8 supportsUserHeap : 1;
    b8 supportsMultiType : 1;
};

struct ResourceCapabilities final
{
    b8 supportsAliasing : 1;
    b8 supportsDirectModify : 1;
};

}

TAU_DECL_UUID(::tau::graphics::CommandListCapabilities, 0xFDD86A96DFA14EAFull, 0x9FDD85D91399DF45ull);
TAU_DECL_UUID(::tau::graphics::ShaderCapabilities, 0x02ECEACB32754FE9ull, 0x8C18632D02668A4Full);
TAU_DECL_UUID(::tau::graphics::HeapCapabilities, 0x613A0281A66641F3ull, 0x93C1DC405235823Eull);
TAU_DECL_UUID(::tau::graphics::ResourceCapabilities, 0x40647DEBB8E24FC5ull, 0x99F6292B48B3274Eull);
