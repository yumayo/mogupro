#include <Utility/utf8.h>
namespace Utility
{
bool utf8::isLegalUTF8( const utf8_head *source, int length )
{
    utf8_head a;
    const utf8_head *srcptr = source + length;
    switch ( length ) {
    default: return false;
    case 4: if ( ( a = ( *--srcptr ) ) < 0x80 || a > 0xBF ) return false;
    case 3: if ( ( a = ( *--srcptr ) ) < 0x80 || a > 0xBF ) return false;
    case 2: if ( ( a = ( *--srcptr ) ) < 0x80 || a > 0xBF ) return false;

        switch ( *source ) {
        case 0xE0: if ( a < 0xA0 ) return false; break;
        case 0xED: if ( a > 0x9F ) return false; break;
        case 0xF0: if ( a < 0x90 ) return false; break;
        case 0xF4: if ( a > 0x8F ) return false; break;
        default:   if ( a < 0x80 ) return false;
        }

    case 1: if ( *source >= 0x80 && *source < 0xC2 ) return false;
    }
    if ( *source > 0xF4 ) return false;
    return true;
}
int utf8::getUTF8StringLength( const utf8_head* utf8 )
{
    const utf8_head** source = &utf8;
    const utf8_head* sourceEnd = utf8 + strlen( (const char*)utf8 );
    int ret = 0;
    while ( *source != sourceEnd ) {
        int length = trailingBytesForUTF8[**source] + 1;
        if ( length > sourceEnd - *source || !isLegalUTF8( *source, length ) )
            return 0;
        *source += length;
        ++ret;
    }
    return ret;
}
size_t utf8::getNumBytesForUTF8( utf8_head first )
{
    return trailingBytesForUTF8[first] + 1;
}
utf8::utf8( )
{
    /*nothing*/
}
utf8::utf8( const std::string& newStr )
{
	set_string( newStr );
}
bool utf8::set_string( const std::string & newStr )
{
	clear( );
	if ( !newStr.empty( ) )
	{
		utf8_head* sequenceUtf8 = (utf8_head*)newStr.c_str( );

		int lengthString = getUTF8StringLength( sequenceUtf8 );

		if ( lengthString == 0 )
		{
			_is_utf8 = false;
			return false;
		}

		while ( *sequenceUtf8 )
		{
			std::size_t lengthChar = getNumBytesForUTF8( *sequenceUtf8 );

			std::string charUTF8;
			charUTF8.append( (char*)sequenceUtf8, lengthChar );
			sequenceUtf8 += lengthChar;

			push_back( charUTF8 );
		}

		_text.clear( );
		for ( auto& obj : *this )
		{
			_text += obj;
		}
	}
	return true;
}
bool utf8::is_utf8( )
{
    return _is_utf8;
}
char const* utf8::c_str( )
{
    return _text.c_str( );
}
std::string utf8::string( )
{
    return _text;
}
}