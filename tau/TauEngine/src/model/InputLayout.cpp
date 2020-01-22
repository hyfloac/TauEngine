#include "model/InputLayout.hpp"


// ReSharper disable once CppMemberFunctionMayBeConst
const InputLayoutDescriptor* IInputLayoutBuilder::getLayoutDescriptor(const uSys index) const noexcept
{
    if(index < _descriptors.size())
    {
        return &_descriptors[index];
    }

    return null;
}

// ReSharper disable once CppMemberFunctionMayBeConst
InputLayoutDescriptor* IInputLayoutBuilder::getLayoutDescriptor(const uSys index) noexcept
{
    if(index < _descriptors.size())
    {
        return &_descriptors[index];
    }

    return null;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void IInputLayoutBuilder::setLayoutDescriptor(const uSys index, const ShaderDataType::Type type, const ShaderSemantic::Semantic semantic) noexcept
{
    if(index < _descriptors.size())
    {
        _descriptors[index].type() = type;
        _descriptors[index].semantic() = semantic;
    }
}
