#include<Utility/cString.h>
#include <sstream>
#include <iomanip>
#include <random>
#include <direct.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
namespace Utility
{
void cString::replace( std::string * src, std::string const & prev, std::string const & write )
{
    std::string::size_type offset( src->find( prev ) );
    while ( offset != std::string::npos )
    {
        src->replace( offset, prev.length( ), write );
        offset = src->find( prev, offset + write.length( ) );
    }
}
std::vector<std::string> cString::split( std::string const& s, char delim )
{
    std::string elem;
    std::vector<std::string> elems;
    std::stringstream ss( s );
    while ( getline( ss, elem, delim ) )
    {
        if ( !elem.empty( ) ) elems.push_back( elem );
    }
    return elems;
}
std::string cString::getDirectoryNameUnix( std::string const & path )
{
    std::string p = path;
    replace( &p, "\\", "/" );
    std::string::size_type end( p.rfind( '/' ) );
    return ( end != std::string::npos ) ? p.substr( 0, end ).c_str( ) : std::string( );
}
std::string cString::getDirectoryNameWindows( std::string const & path )
{
    std::string p = path;
    replace( &p, "/", "\\" );
    std::string::size_type end( path.rfind( '\\' ) );
    return ( end != std::string::npos ) ? path.substr( 0, end ).c_str( ) : std::string( );
}
std::string cString::getExtensionName( std::string const & path )
{
    std::string::size_type pos( path.rfind( '.' ) );
    return ( pos != std::string::npos ) ? path.substr( pos + 1 ).c_str( ) : std::string( );
}
std::string cString::toHex( unsigned char value )
{
    std::stringstream ss;
    ss << std::hex << std::setw( 2 ) << std::setfill( '0' ) << static_cast<unsigned int>( value );
    return ss.str( );
}
std::string cString::toHex( unsigned int value )
{
    std::stringstream ss;
    ss << std::hex << std::setw( 8 ) << std::setfill( '0' ) << value;
    return ss.str( );
}
std::string cString::createUniqueIdentifier( )
{
    std::string uniqueIdentifier;
    std::random_device rd;
    for ( int i = 0; i < 24 / 4; ++i ) uniqueIdentifier += toHex( rd( ) );
    return uniqueIdentifier;
}
inline std::wstring cString::multi_to_wide_winapi( std::string const & src )
{
    auto const dest_size = ::MultiByteToWideChar( CP_ACP, 0U, src.data( ), -1, nullptr, 0U );
    std::vector<wchar_t> dest( dest_size, L'\0' );
    if ( ::MultiByteToWideChar( CP_ACP, 0U, src.data( ), -1, dest.data( ), dest.size( ) ) == 0 ) {
        throw std::system_error { static_cast<int>( ::GetLastError( ) ), std::system_category( ) };
    }
    return std::wstring( dest.begin( ), dest.end( ) );
}
inline std::string cString::wide_to_multi_winapi( std::wstring const & src )
{
    auto const dest_size = ::WideCharToMultiByte( CP_ACP, 0U, src.data( ), -1, nullptr, 0, nullptr, nullptr );
    std::vector<char> dest( dest_size, '\0' );
    if ( ::WideCharToMultiByte( CP_ACP, 0U, src.data( ), -1, dest.data( ), dest.size( ), nullptr, nullptr ) == 0 ) {
        throw std::system_error { static_cast<int>( ::GetLastError( ) ), std::system_category( ) };
    }
    return std::string( dest.begin( ), dest.end( ) );
}
inline std::string cString::wide_to_utf8_winapi( std::wstring const & src )
{
    auto const dest_size = ::WideCharToMultiByte( CP_UTF8, 0U, src.data( ), -1, nullptr, 0, nullptr, nullptr );
    std::vector<char> dest( dest_size, '\0' );
    if ( ::WideCharToMultiByte( CP_UTF8, 0U, src.data( ), -1, dest.data( ), dest.size( ), nullptr, nullptr ) == 0 ) {
        throw std::system_error { static_cast<int>( ::GetLastError( ) ), std::system_category( ) };
    }
    return std::string( dest.begin( ), dest.end( ) );
}
inline std::wstring cString::utf8_to_wide_winapi( std::string const & src )
{
    auto const dest_size = ::MultiByteToWideChar( CP_UTF8, 0U, src.data( ), -1, nullptr, 0U );
    std::vector<wchar_t> dest( dest_size, L'\0' );
    if ( ::MultiByteToWideChar( CP_UTF8, 0U, src.data( ), -1, dest.data( ), dest.size( ) ) == 0 ) {
        throw std::system_error { static_cast<int>( ::GetLastError( ) ), std::system_category( ) };
    }
    return std::wstring( dest.begin( ), dest.end( ) );
}
inline std::string cString::multi_to_utf8_winapi( std::string const & src )
{
    auto const wide = multi_to_wide_winapi( src );
    return wide_to_utf8_winapi( wide );
}
inline std::string cString::utf8_to_multi_winapi( std::string const & src )
{
    auto const wide = utf8_to_wide_winapi( src );
    return wide_to_multi_winapi( wide );
}
}
