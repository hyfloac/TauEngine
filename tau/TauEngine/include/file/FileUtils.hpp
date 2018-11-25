#pragma once

#include <NumTypes.hpp>
#include <Safeties.hpp>

/**
 * The list of different file modes.
 */
enum FileMode : u8
{
    /** Specifies that the file is to be opened for text reading only. */
    READ = 1,
    /**
     * Specifies that the file is to be opened for text writing only.
     *
     * Text will be overwritten.
     *
     * Creates a new file if one does not exist.
     */
    WRITE,
    /**
     * Specifies that the file is to be opened for text appending only.
     *
     * Text will be appended.
     *
     * Creates a new file if one does not exist.
     */
    APPEND,
    /**
     * Specifies that the file is to be opened for text reading and writing.
     *
     * Text will be overwritten if written to.
     *
     * Creates a new file if one does not exist.
     */
    READ_WRITE,
    /**
     * Specifies that the file is to be opened for text reading and writing.
     *
     * All text is cleared when opened. You can't really read anything.
     *
     * Creates a new file if one does not exist.
     */
    CLEAR_WRITE,
    /**
     * Specifies that the file is to be opened for text reading and appending.
     *
     * Text will be appended. Reading begins at the file head.
     *
     * Creates a new file if one does not exist.
     */
    READ_APPEND,

    /** Specifies that the file is to be opened for binary reading only. */
    READ_BINARY = 9,
    /**
     * Specifies that the file is to be opened for binary writing only.
     *
     * Contents will be overwritten.
     *
     * Creates a new file if one does not exist.
     */
    WRITE_BINARY,
    /**
     * Specifies that the file is to be opened for binary appending only.
     *
     * Contents will be appended.
     *
     * Creates a new file if one does not exist.
     */
    APPEND_BINARY,
    /**
     * Specifies that the file is to be opened for binary reading and writing.
     *
     * Contents will be overwritten if written to.
     *
     * Creates a new file if one does not exist.
     */
    READ_WRITE_BINARY,
    /**
     * Specifies that the file is to be opened for binary reading and writing.
     *
     * All contents are cleared when opened. You can't really read anything.
     *
     * Creates a new file if one does not exist.
     */
    CLEAR_WRITE_BINARY,
    /**
     * Specifies that the file is to be opened for binary reading and appending.
     *
     * Contents will be appended. Reading begins at the file head.
     *
     * Creates a new file if one does not exist.
     */
    READ_APPEND_BINARY
};

/**
 * Converts a {@link FileMode} enum to its string mode.
 *
 * @param[in] fileMode
 *    The {@FileMode} to get the string variant for.
 * @return
 *    The string form of the {@link FileMode}.
 */
NonNull const char* getFileModeStr(FileMode fileMode) noexcept;

FileMode getFileMode(const char* fileMode) noexcept;
