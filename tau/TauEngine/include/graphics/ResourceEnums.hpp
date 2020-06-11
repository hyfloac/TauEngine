#pragma once

namespace EResource {

/**
 * Used to determine how a resource is used.
 */
enum class Type
{
    Buffer = 1,
    Texture1D,
    Texture2D,
    Texture3D
};

/**
 *   Used when mapping a resource to determine what to do with
 * the previous data in the buffer.
 */
enum class MapType
{
    /**
     * Doesn't affect the previous data.
     *
     * You are free to overwrite any part of it.
     */
    Default = 1,
    /**
     * Discard the previous data.
     *
     *   This usually involves being given a new buffer every time
     * it is mapped.
     */
    Discard,
    /**
     * Doesn't affect the previous data.
     *
     *   You promise not to touch any data that is currently in
     * use by the GPU. This is useful if you need to have a
     * constantly mapped buffer and have segmented parts of it
     * for each frame.
     */
    NoOverwrite,
    /**
     * You will be reading data from the GPU.
     */
    NoWrite
};

/**
 * Used to determine how the resource will be used.
 */
enum class UsageType
{
    /**
     * General use. Usually written once.
     */
    Default = 1,
    /**
     * Cannot be rewritten by either the GPU or CPU
     */
    Immutable,
    /**
     * Intended to be frequently updated by the CPU.
     */
    Dynamic,
    /**
     * Used for retrieving data from the GPU.
     */
    Readable
};

}
