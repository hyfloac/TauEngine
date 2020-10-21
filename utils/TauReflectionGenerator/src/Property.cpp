#include "Property.hpp"

namespace tau {

PropertyDeclaration::~PropertyDeclaration() noexcept
{
    _attributes.iterate([](AttributeSet::Node* const node) { node->value.handler()->destroyData(node->value); });
}

}
