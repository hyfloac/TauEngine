#pragma once

#include <Objects.hpp>

/**
 * An opaque descriptor layout.
 *
 *   This is used during resource binding to know where each
 * descriptor should be bound to within the pipeline.
 */
DECL_OPAQUE_TYPE(DescriptorLayout);

/**
 * A view into a texture.
 *
 * This is used for binding textures to the shader pipeline.
 *
 * The exact implementation details are up to the driver.
 */
DECL_OPAQUE_TYPE(TextureView);

/**
 * A view into a uniform buffer resource.
 *
 *   This is used for binding uniform buffers to the shader
 * pipeline.
 */
DECL_OPAQUE_TYPE(UniformBufferView);

/**
 * A table of descriptors.
 *
 *   Descriptors are things like texture views and uniform
 * buffer views. The exact implementation of this is opaque and
 * heavily reliant on the graphics API.
 */
DECL_OPAQUE_TYPE(DescriptorTable);

/**
 * A table of texture samplers.
 *
 *   Normally descriptors are little more than a memory
 * address, but samplers are a bit more involved. Thus they get
 * their own table.
 */
DECL_OPAQUE_TYPE(DescriptorSamplerTable);

DECL_OPAQUE_TYPE(ShaderProgram);
