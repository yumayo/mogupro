#pragma once
#include <Network/NetworkDefines.h>
#include <vector>

namespace Network
{
bool exportInt( int& value, char* const buffer, int& offset, int totalSize );
bool importInt( int& value, const char* const buffer, int& offset, int totalSize );

bool exportUInt( unsigned int& value, char* const buffer, int& offset, int totalSize );
bool importUInt( unsigned int& value, const char* const buffer, int& offset, int totalSize );

bool importChar( char& value, const char* const buffer, int& offset, int totalSize );
bool exportChar( char& value, char* const buffer, int& offset, int totalSize );

bool importShort( short& value, const char* const buffer, int& offset, int totalSize );
bool exportShort( short& value, char* const buffer, int& offset, int totalSize );

bool importStr( char& value, const char* const buffer, int& offset, int totalSize );
bool exportStr( char* value, char* const buffer, int& offset, int totalSize, size_t valueSize );
bool importStr( char* value, const char* const buffer, int& offset, int totalSize, size_t valueSize );

//!@ LookMe : Float‚É‘Î‚·‚éhtonl,ntohl‚Í‚Å‚«‚È‚¢‚Ì‚Å‚Ç‚¤‚·‚é‚©
bool exportFloat( const float& value, char* const buffer, int& offset, int totalSize );
bool importFloat( float& value, const char* const buffer, int& offset, int totalSize );

// ˆÈ‰ºyumayo

ubyte2 convertFixedpoint( float value );
float convertFloat( ubyte2 value );

class cImporter
{
public:
    cImporter( char const* const buffer );
    template<class Ty>
    cImporter& operator>>( Ty& value );
    template<class Ty>
    cImporter& operator>>( std::vector<Ty> const& value )
    {
        ubyte2 size;
        *this >> size;
        for ( ubyte2 i = 0; i < size; ++i )
        {
            Ty type;
            *this >> type;
            value.emplace_back( std::move( type ) );
        }
    }
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
    template<class Ty>
    cExporter& operator<<( std::vector<Ty> const& value )
    {
        *this << static_cast<ubyte2>( value.size( ) );
        for ( auto const& o : value )
        {
            *this << o;
        }
    }
    operator ubyte2( ) const noexcept;
private:
    ubyte2 mOffset;
    char* const mBuffer;
};
}