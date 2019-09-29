#include <cstring>
#include <cstdint>
#include <cstddef>
#include <new>

/**
 *  A simple string implemenation.
 *
 *    This is intended to replace the STL implementation
 *  due to its terrible code gen.
 */
class String final
{
public:
    /**
     *  Allows for safe indexing into the string.
     *
     *    When changing a character this allows one to use 
     *  a `=` operator to assign the character. The reason
     *  why this class is necessary is that a length index
     *  may fail, and thus the assignment could fail. This
     *  allows one to still attempt to assign the character
     *  without having to worry about accidentally trying 
     *  write into unallocated memory.
     *
     *  Example:
     *      String s = "test";
     *      s.at(7) = 'd';
     *    If the String didn't have this indexer it would
     *   attempt to write into memory it doesn't own. The
     *   indexer allows it safely perform the operation
     *   without consequences.
     *    This is equivalent to writing
     *      String s("test");
     *      String::Indexer _idx(s.at(7));
     *      _idx.operator=('d');
     */
    class Indexer final
    {
    private:
        char* _ptr;
        String& _parent;
    public:
        Indexer(char* ptr, String& parent) noexcept
            : _ptr(ptr), _parent(parent)
        { }

        ~Indexer() noexcept = default;

        Indexer(const Indexer& copy) noexcept = default;
        Indexer(Indexer&& move) noexcept = default;

        Indexer& operator =(const Indexer& copy) noexcept = delete;
        Indexer& operator =(Indexer&& move) noexcept = delete;

        operator char() const noexcept
        {
            if(!_ptr) { return '\0'; }
            return *_ptr;
        }

        String& operator =(const char c) noexcept
        {
            if(_ptr) { *_ptr = c; }
            return _parent;
        }
    };
private:
    size_t _length;
    char* _str;
public:
    String() noexcept
        : _length(0), _str(nullptr)
    { }

    String(const char* str) noexcept
    {
        if(str)
        {
            _length = strlen(str);
            _str = new(std::nothrow) char[_length + 1];
            memcpy(_str, str, _length + 1);
        }
        else
        {
            _length = 0;
            _str = nullptr;
        }
    }

    ~String() noexcept
    {
        /*   Technically the delete operator can safely handle 
         * null pointers
         *   But in this case we are using a nullptr as a signal 
         * that the String has been moved.
         */
        if(_str)
        {
            delete[] _str;
        }
    }

    String(const String& copy) noexcept
        : _length(copy._length)
    {
        if(copy._str)
        {
            _str = new(std::nothrow) char[_length + 1];
            memcpy(_str, copy._str, _length + 1);
        }
        else
        {
            _str = nullptr;
        }
    }

    String(String&& move) noexcept
        : _length(move._length), _str(move._str)
    {
        move._str = nullptr;
    }

    String& operator =(const String& copy) noexcept
    {
        _length = copy._length;
        if(copy._str)
        {
            _str = new(std::nothrow) char[_length + 1];
            memcpy(_str, copy._str, _length + 1);
        }
        else
        {
            _str = nullptr;
        }

        return *this;
    }

    String& operator =(String&& move) noexcept
    {
        _length = move._length;
        _str = move._str;
        move._str = nullptr;

        return *this;
    }

    String& operator =(const char*& str) noexcept
    {
        if(str)
        {
            _length = strlen(str);
            _str = new(std::nothrow) char[_length + 1];
            memcpy(_str, str, _length + 1);
        }
        else
        {
            _length = 0;
            _str = nullptr;
        }

        return *this;
    }
public:
    const char* c_str() const noexcept { return _str; }
    const char*   str() const noexcept { return _str; }
    operator const char*() const noexcept { return _str; }

    size_t length() const noexcept { return _length; }
    size_t size() const noexcept { return _length; }
    operator size_t() const noexcept { return _length; }

    char operator [](size_t index) const noexcept
    {
        return _str[index];
    }

    char& operator [](size_t index) noexcept
    {
        return _str[index];
    }

    char at(size_t index) const noexcept
    {
        if(index >= _length) { return '\0'; }
        return _str[index];
    }

    Indexer at(size_t index) noexcept
    {
        if(index >= _length) { return Indexer(nullptr, *this); }
        return Indexer(_str + index, *this);
    }
public:
    String& append(const char* str, const size_t len) noexcept
    {
        if(!str || len == 0) { return *this; }

        char* newStr = nullptr;
        size_t newLen = len;

        if(!_str)
        {
            newStr = new(std::nothrow) char[len];
            memcpy(newStr, str, len + 1);
        }
        else
        {
            newLen += _length;

            newStr = new(std::nothrow) char[newLen + 1];
            memcpy(newStr, _str, _length);
            memcpy(newStr + _length, str, len + 1);
        }

        delete[] _str;
        _str = newStr;
        _length = newLen;

        return *this;
    }

    String& append(const char* str) noexcept
    {
        if(!str) { return *this; }
        return append(str, strlen(str));
    }

    String& append(const String& str)
    {
        return append(str._str, str._length);
    }
};
