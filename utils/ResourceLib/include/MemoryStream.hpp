#pragma once

#include "IStream.hpp"

#include <DynArray.hpp>
#include <Safeties.hpp>
#include <String.hpp>
#include <TauCOM.impl.hpp>

namespace tau {

class DynamicMemoryStream final : public IStream
{
    DEFAULT_DESTRUCT(DynamicMemoryStream);
    DELETE_CM(DynamicMemoryStream);
    TAU_COM_IMPL_REF_COUNT_CUSTOM();
public:
    struct ConstructionInfo final : com::BaseConstructionInfo
    {
        DEFAULT_CONSTRUCT_PU(ConstructionInfo);
        DEFAULT_DESTRUCT_O(ConstructionInfo);
        DEFAULT_CM_PU(ConstructionInfo);
    public:
        uSys Capacity;

        ConstructionInfo(
            const uSys capacity
        ) noexcept
            : Capacity(capacity)
        { }
    };
public:
    DynamicMemoryStream()
        : DynamicMemoryStream(0)
    { }

    DynamicMemoryStream(
        const uSys capacity
    ) noexcept
        : m_Buffer(capacity)
        , m_Position(0)
        , m_Length(0)
    { }

    [[nodiscard]] const DynArray<u8>& Buffer() const noexcept { return m_Buffer; }

    [[nodiscard]] DynArray<u8> ToArray() const noexcept
    {
        DynArray<u8> array(m_Length);
        array.MemCpyCountFrom(m_Buffer.Array(), m_Length);
        return array;
    }

    // IUnknown

    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override
    {
        using namespace com;

        if(!pInterface)
        {
            return RC_NullParam;
        }

        if(iid == iid_of<IUnknown> || iid == iid_of<IStream>)
        {
            *pInterface = this;
        }
        else
        {
            return RC_InterfaceNotFound;
        }

        AddReference();
        return RC_Success;
    }

    // IStream

    /**
     * @return Whether this stream supports reading.
     */
    [[nodiscard]] bool CanRead() const noexcept override { return true; }

    /**
     * @return Whether this stream supports writing.
     */
    [[nodiscard]] bool CanWrite() const noexcept override { return true; }

    /**
     * @return Whether this stream supports seeking.
     */
    [[nodiscard]] bool CanSeek() const noexcept override { return true; }

    /**
     * @return The length of the stream.
     */
    [[nodiscard]] i64 Length() const noexcept override { return m_Length; }

    [[nodiscard]] i64 Position() const noexcept override { return m_Position; }

    i64 Position(const i64 pos, const ESeekOrigin origin) noexcept override
    {
        switch(origin)
        {
            case ESeekOrigin::Begin:
                return SeekCore(pos, 0);
            case ESeekOrigin::Current:
                return SeekCore(pos, m_Position);
            case ESeekOrigin::End:
                return SeekCore(pos, m_Length);
            default:
                assert(false);
        }
    }

    std::expected<uSys, ErrorCode> Read(void* const buffer, const uSys count) override
    {
        assert(buffer);

        iSys n = m_Length - m_Position;

        if(n > count)
        {
            n = count;
        }

        if(n <= 0)
        {
            return 0;
        }

        assert(m_Position + n >= 0);

        (void) ::std::memcpy(buffer, m_Buffer.Array() + m_Position, n);
        m_Position += n;

        return n;
    }

    [[nodiscard]] std::expected<u8, ErrorCode> ReadByte() override
    {
        if(m_Position >= m_Length)
        {
            return ::std::unexpected(ErrorCode::EndOfFile);
        }

        return m_Buffer[m_Position++];
    }

    void Write(const void* const buffer, const uSys count) override
    {
        const iSys i = m_Position + static_cast<iSys>(count);

        assert(i >= 0);

        if(i > m_Length)
        {
            bool mustZero = m_Position > m_Length;

            if(i > m_Buffer.Length())
            {
                const bool allocatedNewArray = EnsureCapacity(i);
                if(allocatedNewArray)
                {
                    mustZero = false;
                }
            }

            if(mustZero)
            {
                (void) ::std::memset(m_Buffer.Array() + i - m_Length, 0, m_Length);
            }

            m_Length = i;
        }


        (void) ::std::memcpy(m_Buffer.Array() + m_Position, buffer, i);
        m_Position = i;
    }

    void Flush() override { }
private:
    iSys SeekCore(const iSys offset, const iSys loc)
    {
        assert(offset <= IntMaxMin<i64>::Max - loc);

        m_Position = loc + offset;
        return m_Position;
    }

    bool EnsureCapacity(const iSys count)
    {
        assert(count >= 0);

        if(count <= m_Buffer.Length())
        {
            return false;
        }

        iSys newCapacity = maxT(count, 256);

        // We are ok with this overflowing since the next statement will deal
        // with the cases where _capacity*2 overflows.
        if(newCapacity < m_Buffer.Length() * 2)
        {
            newCapacity = m_Buffer.Length() * 2;
        }

        // We want to expand the array up to Array.MaxLength.
        // And we want to give the user the value that they asked for
        if(m_Buffer.Length() * 2 > IntMaxMin<uSys>::Max)
        {
            newCapacity = maxT(count, IntMaxMin<uSys>::Max);
        }

        DynArray<u8> newBuffer(newCapacity);
        newBuffer.MemCpyCountFrom(m_Buffer.Array(), m_Length);
        m_Buffer = ::std::move(newBuffer);

        return true;
    }

    void DestroySelf() noexcept;

    static com::EResultCode Factory(
        const com::UUID& iid,
        void** const pInterface,
        const com::BaseConstructionInfo* const pConstructionInfo
    ) noexcept;
private:
    DynArray<u8> m_Buffer;
    iSys m_Position;
    iSys m_Length;
};

class MemoryStream final : public IStream
{
    DEFAULT_DESTRUCT(MemoryStream);
    DELETE_CM(MemoryStream);
    TAU_COM_IMPL_REF_COUNT_CUSTOM();
public:
    struct ConstructionInfo final : com::BaseConstructionInfo
    {
        DEFAULT_CONSTRUCT_PU(ConstructionInfo);
        DEFAULT_DESTRUCT_O(ConstructionInfo);
        DEFAULT_CM_PU(ConstructionInfo);
    public:
        u8* Buffer;
        uSys Length;
        bool Writable = true;

        ConstructionInfo(
            u8* const buffer,
            const uSys length,
            const bool writable
        ) noexcept
            : Buffer(buffer)
            , Length(length)
            , Writable(writable)
        { }

        ConstructionInfo(
            u8* const buffer,
            const uSys length
        )
            : ConstructionInfo(
                buffer,
                length,
                true
            )
        { }
    };
public:
    MemoryStream(
        u8* const buffer,
        const uSys length,
        const bool writable
    ) noexcept
        : m_Buffer(buffer)
        , m_Position(0)
        , m_Length(length)
        , m_Writable(writable)
    { }

    MemoryStream(
        u8* const buffer,
        const uSys length
    ) noexcept
        : MemoryStream(
            buffer,
            length,
            true
        )
    { }

    // IUnknown

    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override
    {
        using namespace com;

        if(!pInterface)
        {
            return RC_NullParam;
        }

        if(iid == iid_of<IUnknown> || iid == iid_of<IStream>)
        {
            *pInterface = this;
        }
        else
        {
            return RC_InterfaceNotFound;
        }

        AddReference();
        return RC_Success;
    }

    // IStream

    /**
     * @return Whether this stream supports reading.
     */
    [[nodiscard]] bool CanRead() const noexcept override { return true; }

    /**
     * @return Whether this stream supports writing.
     */
    [[nodiscard]] bool CanWrite() const noexcept override { return m_Writable; }

    /**
     * @return Whether this stream supports seeking.
     */
    [[nodiscard]] bool CanSeek() const noexcept override { return true; }

    /**
     * @return The length of the stream.
     */
    [[nodiscard]] i64 Length() const noexcept override { return m_Length; }

    [[nodiscard]] i64 Position() const noexcept override { return m_Position; }

    i64 Position(const i64 pos, const ESeekOrigin origin) noexcept override
    {
        switch(origin)
        {
            case ESeekOrigin::Begin:
                return SeekCore(pos, 0);
            case ESeekOrigin::Current:
                return SeekCore(pos, m_Position);
            case ESeekOrigin::End:
                return SeekCore(pos, m_Length);
            default:
                assert(false);
        }
    }

    std::expected<uSys, ErrorCode> Read(void* const buffer, const uSys count) override
    {
        assert(buffer);

        iSys n = m_Length - m_Position;

        if(n > count)
        {
            n = count;
        }

        if(n <= 0)
        {
            return 0;
        }

        assert(m_Position + n >= 0);

        (void) ::std::memcpy(buffer, m_Buffer + m_Position, n);
        m_Position += n;

        return n;
    }

    [[nodiscard]] std::expected<u8, ErrorCode> ReadByte() override
    {
        if(m_Position >= m_Length)
        {
            return ::std::unexpected(ErrorCode::EndOfFile);
        }

        return m_Buffer[m_Position++];
    }

    void Write(const void* const buffer, const uSys count) override
    {
        assert(buffer);
        assert(m_Writable);

        const iSys i = static_cast<iSys>(m_Position + count);

        assert(i >= 0);

        assert(i <= m_Length);

        (void) ::std::memcpy(m_Buffer + m_Position, buffer, i);
        m_Position = i;
    }
private:
    iSys SeekCore(const iSys offset, const iSys loc)
    {
        assert(offset <= IntMaxMin<i64>::Max - loc);

        m_Position = loc + offset;
        return m_Position;
    }

    void DestroySelf() noexcept;

    static com::EResultCode Factory(
        const com::UUID& iid,
        void** const pInterface,
        const com::BaseConstructionInfo* const pConstructionInfo
    ) noexcept;
private:
    u8* m_Buffer;
    iSys m_Position;
    iSys m_Length;
    bool m_Writable;
};

}

TAU_DECL_UUID(tau::DynamicMemoryStream, 0x44AA2BDC7DBF4F46ull, 0x9ACD5D334095CCE2ull);
TAU_DECL_UUID(tau::MemoryStream, 0xDD7012E8E0004219ull, 0xA5583E6DF682CCB5ull);
