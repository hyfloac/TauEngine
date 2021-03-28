#pragma once

#include <fstream>

#include "MathUtils.hpp"

class GraphicsInfoWriter final
{
private:
    ::std::ofstream _stream;
    int _indent;
    ::std::string _indentCache;
public:
    GraphicsInfoWriter(const ::std::wstring& fileName)
        : _stream(fileName)
        , _indent(0)
        , _indentCache("")
    { begin(); }

    ~GraphicsInfoWriter()
    { end(); }

    GraphicsInfoWriter(const GraphicsInfoWriter& copy) = delete;
    GraphicsInfoWriter(GraphicsInfoWriter&& move) = delete;
    
    GraphicsInfoWriter& operator=(const GraphicsInfoWriter& copy) = delete;
    GraphicsInfoWriter& operator=(GraphicsInfoWriter&& move) = delete;

    void write(const ::std::string& str)
    { _stream << str; }

    void writeKey(const ::std::string& key)
    { _stream << _indentCache << '"' << key << "\": "; }

    void beginObject(bool indent = true)
    {
        if(indent)
        {
            _stream << _indentCache;
        }
        _stream << '{' << ::std::endl;
        increaseIndent();
    }
    
    void beginObject(const ::std::string& key)
    {
        writeKey(key);
        beginObject(false);
    }
    
    void beginObject(const char* const key)
    {
        writeKey(key);
        beginObject(false);
    }

    void endObject()
    {
        decreaseIndent();
        _stream << _indentCache << "}," << ::std::endl;
    }

    void beginArray(bool indent = true)
    {
        if(indent)
        {
            _stream << _indentCache;
        }
        _stream << '[' << ::std::endl;
        increaseIndent();
    }

    void beginArray(const ::std::string& key)
    {
        writeKey(key);
        beginArray(false);
    }

    void beginArray(const char* const key)
    {
        writeKey(key);
        beginArray(false);
    }

    void endArray()
    {
        decreaseIndent();
        _stream << _indentCache << "]," << ::std::endl;
    }

    void writeString(const ::std::string& key, const ::std::string& value)
    {
        writeKey(key);
        _stream << '"' << value << "\"," << ::std::endl;
    }

    void writeInt(const ::std::string& key, const i32 i)
    {
        writeKey(key);
        _stream << i << ',' << ::std::endl;
    }

    void writeInt(const ::std::string& key, const u32 i)
    {
        writeKey(key);
        _stream << i << ',' << ::std::endl;
    }

    void writeInt(const ::std::string& key, const i64 i)
    {
        writeKey(key);
        _stream << i << ',' << ::std::endl;
    }

    void writeInt(const ::std::string& key, const u64 i)
    {
        writeKey(key);
        _stream << i << ',' << ::std::endl;
    }

    void writeBool(const ::std::string& key, const bool b)
    {
        writeKey(key);
        _stream << (b ? "true" : "false") << ',' << ::std::endl;
    }

    void writeFloat(const ::std::string& key, const float f)
    {
        writeKey(key);
        _stream << f << ',' << ::std::endl;
    }

    void writeFloat(const ::std::string& key, const double f)
    {
        writeKey(key);
        _stream << f << ',' << ::std::endl;
    }
private:
    void begin()
    { beginObject(false); }

    void end()
    { endObject(); }

    void increaseIndent()
    {
        ++_indent;
        _indentCache = "";
        for(int i = 0; i < _indent; ++i)
        {
            _indentCache += "    ";
        }
    }

    void decreaseIndent()
    {
        --_indent;
        _indentCache = "";
        for(int i = 0; i < _indent; ++i)
        {
            _indentCache += "    ";
        }
    }
};
