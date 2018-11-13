#include <file/FileHandling.hpp>
#include <Utils.hpp>
#include <cstring>

void BufferedTextFileReader::bufferData() noexcept
{
    fgets(this->_buffer, BUFFER_SIZE, this->_file);
    this->_index = 0;
    if(feof(this->_file))
    {
        this->_finished = strlen(this->_buffer);
    }
}

BufferedTextFileReader::BufferedTextFileReader(const char* fileName) noexcept
    : _file(null), _fileName(fileName), _index(-1), _finished(-1)
{ 
    _file = fopen(fileName, "r");
    if(!_file) _finished = 1;
}

BufferedTextFileReader::~BufferedTextFileReader() noexcept
{
    fclose(this->_file);
}

char BufferedTextFileReader::readChar() noexcept
{
    if(this->_index == -1)
    {
        bufferData();
    }
    else if(this->_index == BUFFER_SIZE)
    {
        bufferData();
    }
    return this->_buffer[this->_index++];
}

u32 BufferedTextFileReader::readString(u32 len, char* buffer) noexcept
{
    if(this->_index == -1)
    {
        bufferData();
    }
    else if(this->_index == BUFFER_SIZE)
    {
        bufferData();
    }

    u32 index = 0;

    while(this->_index + len >= BUFFER_SIZE)
    {
        memcpy(buffer + index, this->_buffer + this->_index, BUFFER_SIZE - this->_index);
        len -= BUFFER_SIZE - this->_index;
        index += BUFFER_SIZE - this->_index;
        bufferData();
        this->_index = 0;
    }

    memcpy(buffer + index, this->_buffer + this->_index, len);

    index += len;

    buffer[index] = '\0';

    return index + 1;
}

u32 BufferedTextFileReader::readLine(char* buffer, u32 maxLen) noexcept
{
    u32 i = 0;
    for(; i < maxLen; ++i)
    {
        if(this->_index == -1)
        {
            bufferData();
        }
        else if(this->_index == BUFFER_SIZE)
        {
            bufferData();
        }

        if(this->_buffer[this->_index] == '\n')
        {
            this->_index++;
            break;
        }

        if(this->_buffer[this->_index] == '\r')
        {
            this->_index += 2;
            break;
        }

        buffer[i] = this->_buffer[this->_index++];
    }

    buffer[i] = '\0';
    return i + 1;
}
