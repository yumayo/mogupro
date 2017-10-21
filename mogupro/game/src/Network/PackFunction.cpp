#include <Network/PackFunction.hpp>
#include <winsock2.h>

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

    // ˆÈ‰ºyumayo
    template<>
    ubyte2 imp<ubyte1>( ubyte1& value, const char* const buffer, ubyte2 offset )
    {
        memcpy( &value, buffer + offset, sizeof( ubyte1 ) );
        return offset += sizeof( ubyte1 );
    }
    template<>
    ubyte2 exp<ubyte1>( ubyte1 const& value, char* const buffer, ubyte2 offset )
    {
        memcpy( buffer + offset, &value, sizeof( ubyte1 ) );
        return offset += sizeof( ubyte1 );
    }
    template<>
    ubyte2 imp<ubyte2>( ubyte2& value, const char* const buffer, ubyte2 offset )
    {
        memcpy( &value, buffer + offset, sizeof( ubyte2 ) );
        return offset += sizeof( ubyte2 );
    }
    template<>
    ubyte2 exp<ubyte2>( ubyte2 const& value, char* const buffer, ubyte2 offset )
    {
        memcpy( buffer + offset, &value, sizeof( ubyte2 ) );
        return offset += sizeof( ubyte2 );
    }
    template<>
    ubyte2 imp<ubyte4>( ubyte4& value, const char* const buffer, ubyte2 offset )
    {
        memcpy( &value, buffer + offset, sizeof( ubyte4 ) );
        return offset += sizeof( ubyte4 );
    }
    template<>
    ubyte2 exp<ubyte4>( ubyte4 const& value, char* const buffer, ubyte2 offset )
    {
        memcpy( buffer + offset, &value, sizeof( ubyte4 ) );
        return offset += sizeof( ubyte4 );
    }
    ubyte2 convertFixedpoint( float value )
    {
        return ubyte2( );
    }
}