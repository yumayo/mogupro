#include <Network/PackFunction.hpp>
#include <winsock2.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
#include <cinder/Matrix.h>
#include <vector>

namespace Network
{
	bool exportInt(int& value, char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(int)))
			return false;
		if (buffer == NULL)return false;
		int c = htonl(value);
		memcpy(buffer + offset, &c, sizeof(int));
		offset += sizeof(int);
		return true;
	}

	bool importInt(int& value, const char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(int)))
			return false;
		int c;
		memcpy(&c, buffer + offset, sizeof(int));
		value = ntohl(c);
		offset += sizeof(int);
		return true;
	}

	bool exportUInt(unsigned int& value, char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(unsigned int)))
			return false;
		if (buffer == NULL)return false;
		unsigned int c = htonl(value);
		memcpy(buffer + offset, &c, sizeof(unsigned int));
		offset += sizeof(unsigned int);
		return true;
	}

	bool importUInt(unsigned int& value, const char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(unsigned int)))
			return false;
		unsigned int c;
		memcpy(&c, buffer + offset, sizeof(unsigned int));
		value = ntohl(c);
		offset += sizeof(unsigned int);
		return true;
	}

	bool importChar(char& value, const char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(char)))
			return false;
		char c;
		memcpy(&c, buffer + offset, sizeof(char));
		value = c;
		offset += sizeof(char);
		return true;
	}

	bool exportChar(char& value, char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(char)))
			return false;
		if (buffer == NULL)return false;
		memcpy(buffer + offset, &value, sizeof(char));
		offset += sizeof(char);
		return true;
	}

	bool exportShort(short& value, char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(short)))
			return false;
		if (buffer == NULL)return false;
		short c = htons(value);
		memcpy(buffer + offset, &c, sizeof(short));
		offset += sizeof(short);
		return true;
	}

	bool importShort(short& value, const char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(short)))
			return false;
		short c;
		memcpy(&c, buffer + offset, sizeof(short));
		value = ntohs(c);
		offset += sizeof(short);
		return true;
	}

	bool importStr(char& value, const char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(char)))
			return false;
		memcpy(&value, buffer + offset, sizeof(char));
		offset += sizeof(char);
		return true;
	}

	bool exportStr(char* value, char* const buffer, int& offset, int totalSize, size_t valueSize)
	{
		if (totalSize < int(offset + sizeof(valueSize)))
			return false;
		if (buffer == NULL)return false;
		memcpy(buffer + offset, value, sizeof(valueSize));
		offset += sizeof(valueSize);
		return true;
	}

	bool importStr(char* value, const char* const buffer, int& offset, int totalSize, size_t valueSize)
	{
		if (totalSize < int(offset + sizeof(valueSize)))
			return false;
		memcpy(value, buffer + offset, sizeof(valueSize));
		offset += sizeof(valueSize);
		return true;
	}

	//!@ LookMe : Float‚É‘Î‚·‚éhtonl,ntohl‚Í‚Å‚«‚È‚¢‚Ì‚Å‚Ç‚¤‚·‚é‚©
	bool exportFloat(const float& value, char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(float)))
			return false;
		if (buffer == NULL)return false;
		memcpy(buffer + offset, &value, sizeof(float));
		offset += sizeof(float);
		return true;
	}

	bool importFloat(float& value, const char* const buffer, int& offset, int totalSize)
	{
		if (totalSize < int(offset + sizeof(float)))
			return false;
		memcpy(&value, buffer + offset, sizeof(float));
		offset += sizeof(float);
		return true;
	}

    ubyte2 convertFixedpoint( float value )
    {
        ubyte2 result;
        result = static_cast<ubyte2>( value ) << 7;
        result += static_cast<ubyte2>( ( value - static_cast<unsigned int>( value ) ) * 100.0F );
        return result;
    }

    float convertFloat( ubyte2 value )
    {
        float result;
        result = value >> 7;
        result += static_cast<float>( value & 0x007F ) / 100.0F;
        return result;
    }

    // ˆÈ‰ºyumayo
    template<>
    cImporter& cImporter::operator>><ubyte1>( ubyte1& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( ubyte1 ) );
        mOffset += sizeof( ubyte1 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<ubyte1>( ubyte1 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( ubyte1 ) );
        mOffset += sizeof( ubyte1 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><ubyte2>( ubyte2& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( ubyte2 ) );
        mOffset += sizeof( ubyte2 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<ubyte2>( ubyte2 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( ubyte2 ) );
        mOffset += sizeof( ubyte2 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><ubyte4>( ubyte4& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( ubyte4 ) );
        mOffset += sizeof( ubyte4 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<ubyte4>( ubyte4 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( ubyte4 ) );
        mOffset += sizeof( ubyte4 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><byte1>( byte1& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( byte1 ) );
        mOffset += sizeof( byte1 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<byte1>( byte1 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( byte1 ) );
        mOffset += sizeof( byte1 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><byte2>( byte2& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( byte2 ) );
        mOffset += sizeof( byte2 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<byte2>( byte2 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( byte2 ) );
        mOffset += sizeof( byte2 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><byte4>( byte4& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( byte4 ) );
        mOffset += sizeof( byte4 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<byte4>( byte4 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( byte4 ) );
        mOffset += sizeof( byte4 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><float>( float& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( float ) );
        mOffset += sizeof( float );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<float>( float const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( float ) );
        mOffset += sizeof( float );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><cinder::vec2>( cinder::vec2& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( cinder::vec2 ) );
        mOffset += sizeof( cinder::vec2 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<cinder::vec2>( cinder::vec2 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( cinder::vec2 ) );
        mOffset += sizeof( cinder::vec2 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><cinder::vec3>( cinder::vec3& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( cinder::vec3 ) );
        mOffset += sizeof( cinder::vec3 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<cinder::vec3>( cinder::vec3 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( cinder::vec3 ) );
        mOffset += sizeof( cinder::vec3 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><cinder::vec4>( cinder::vec4& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( cinder::vec4 ) );
        mOffset += sizeof( cinder::vec4 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<cinder::vec4>( cinder::vec4 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( cinder::vec4 ) );
        mOffset += sizeof( cinder::vec4 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><cinder::quat>( cinder::quat& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( cinder::quat ) );
        mOffset += sizeof( cinder::quat );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<cinder::quat>( cinder::quat const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( cinder::quat ) );
        mOffset += sizeof( cinder::quat );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><cinder::mat3>( cinder::mat3& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( cinder::mat3 ) );
        mOffset += sizeof( cinder::mat3 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<cinder::mat3>( cinder::mat3 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( cinder::mat3 ) );
        mOffset += sizeof( cinder::mat3 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><cinder::mat4>( cinder::mat4& value )
    {
        memcpy( &value, mBuffer + mOffset, sizeof( cinder::mat4 ) );
        mOffset += sizeof( cinder::mat4 );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<cinder::mat4>( cinder::mat4 const& value )
    {
        memcpy( mBuffer + mOffset, &value, sizeof( cinder::mat4 ) );
        mOffset += sizeof( cinder::mat4 );
        return *this;
    }
    template<>
    cImporter& cImporter::operator>><std::string>( std::string& value )
    {
        value.reserve( PACKET_MAX_BYTE );
        strcpy( value._Get_data( )._Myptr( ), mBuffer + mOffset );
        value._Get_data( )._Mysize = strlen( value.c_str( ) );
        value.shrink_to_fit( );
        mOffset += static_cast<ubyte2>( value.size( ) + 1U );
        return *this;
    }
    template<>
    cExporter& cExporter::operator<<<std::string>( std::string const& value )
    {
        strcpy( mBuffer + mOffset, value.c_str( ) );
        mOffset += static_cast<ubyte2>( value.size( ) + 1U );
        return *this;
    }
    cImporter::cImporter( char const * const buffer )
        : mBuffer( buffer )
        , mOffset( 0U )
    {
    }
    cExporter::cExporter( char * const buffer )
        : mBuffer( buffer )
        , mOffset( 0U )
    {
    }
    cExporter::operator ubyte2( ) const noexcept
    {
        return mOffset;
    }
}