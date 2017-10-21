#pragma once
#include <Network/NetworkDefines.h>

namespace Network
{
bool exportInt(int& value, char* const buffer, int& offset, int totalSize);
bool importInt(int& value, const char* const buffer, int& offset, int totalSize);

bool exportUInt(unsigned int& value, char* const buffer, int& offset, int totalSize);
bool importUInt(unsigned int& value, const char* const buffer, int& offset, int totalSize);

bool importChar(char& value, const char* const buffer, int& offset, int totalSize);
bool exportChar(char& value, char* const buffer, int& offset, int totalSize);

bool importShort(short& value, const char* const buffer, int& offset, int totalSize);
bool exportShort(short& value, char* const buffer, int& offset, int totalSize);

bool importStr(char& value, const char* const buffer, int& offset, int totalSize);
bool exportStr(char* value, char* const buffer, int& offset, int totalSize, size_t valueSize);
bool importStr(char* value, const char* const buffer, int& offset, int totalSize, size_t valueSize);

//!@ LookMe : Float‚É‘Î‚·‚éhtonl,ntohl‚Í‚Å‚«‚È‚¢‚Ì‚Å‚Ç‚¤‚·‚é‚©
bool exportFloat(const float& value, char* const buffer, int& offset, int totalSize);
bool importFloat(float& value, const char* const buffer, int& offset, int totalSize);

// ˆÈ‰ºyumayo

ubyte2 convertFixedpoint( float value );
float convertFloat( ubyte2 value );

class cImporter
{
public:
    cImporter( char const* const buffer );
    template<class Ty>
    cImporter& operator>>( Ty& value );
private:
    ubyte2 mOffset;
    char const* const mBuffer;
};
class cExporter
{
public:
    cExporter( char* const buffer );
    template<class Ty>
    cExporter& operator<<( Ty const& value );
    operator ubyte2( ) const noexcept;
private:
    ubyte2 mOffset;
    char* const mBuffer;
};
}