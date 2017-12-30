#include <Utility/cString.h>
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <cstdlib>
#include <iostream>
#include <random>
#include <locale>
#include <direct.h>
#include <system_error>
#include <winstring.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <Utility/cRecursionUsableMutex.h>
#include <Utility/cScopedMutex.h>
#include <cinder/app/App.h>
#include <cinder/DataTarget.h>
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
std::string cString::getFilename( const std::string & path )
{
	return path.substr( path.rfind( '/' ) + 1, path.length( ) );
}
std::string cString::getFileNameWithoutExtension( const std::string & path )
{
	std::string name = getFilename( path );
	return name.substr( 0, name.rfind( '.' ) );
}
std::string cString::toUpper( std::string const & str )
{
	std::string lower = str;
	transform( lower.begin( ), lower.end( ), lower.begin( ), ::toupper );
	return lower;
}
std::string cString::toLower( std::string const & str )
{
	std::string lower = str;
	transform( lower.begin( ), lower.end( ), lower.begin( ), ::tolower );
	return lower;
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
static const int max_string_length = 65536;
static cRecursionUsableMutex gloval_app_console_mutex;
std::string cString::format( char const * str, ... )
{
    cScopedMutex console( gloval_app_console_mutex );

    static char buf[max_string_length];
    va_list args;
    va_start( args, str );
    vsnprintf( buf, max_string_length, str, args );
    va_end( args );

    return buf;
}
void cString::log( char const * str, ... )
{
    cScopedMutex console( gloval_app_console_mutex );

    static char buf[max_string_length];
    va_list args;
    va_start( args, str );
    vsnprintf( buf, max_string_length, str, args );
    va_end( args );

    cinder::app::console( ) << buf << std::endl;
}
bool cString::directoryExists( std::string fullPathWithDirectoryName )
{
    return PathIsDirectory( utf8ToWide( fullPathWithDirectoryName ).c_str( ) );
}
bool cString::fileExists( std::string fullPathWithFileName )
{
    return PathFileExists( utf8ToWide( fullPathWithFileName ).c_str( ) );
}
bool cString::makeDirectory( std::string fullPathWithDirectoryName )
{
    return CreateDirectory( utf8ToWide( fullPathWithDirectoryName ).c_str( ), nullptr );
}
void cString::saveData( size_t writeByteSize, char const * str, std::string const & appDataUnderPathAndFileName )
{
    auto const writablePath = getWritablePath( );
    auto directorys = getDirectoryNameWindows( appDataUnderPathAndFileName );
    auto splited = split( directorys, '\\' );
    for ( auto& o : splited )
    {
        if ( !directoryExists( writablePath + o ) )
        {
            if ( makeDirectory( writablePath + o ) )
            {
                throw std::runtime_error( writablePath + o + ": フォルダを作成できませんでした。" );
            }
        }
    }
    auto dataRef = cinder::writeFile( writablePath + appDataUnderPathAndFileName );
    if ( !dataRef )
    {
        throw std::runtime_error( writablePath + appDataUnderPathAndFileName + ": ファイルを作成できませんでした。" );
    }
    dataRef->getStream( )->writeData( str, writeByteSize );
}
void cString::saveData( std::string const & data, std::string const & appDataUnderPathAndFileName )
{
    saveData( data.size( ), data.c_str( ), appDataUnderPathAndFileName );
}
void cString::saveData( Json::Value const & root, std::string const & appDataUnderPathAndFileName )
{
    Json::FastWriter writer;
    auto data = writer.write( root );
    saveData( data.size( ), data.c_str( ), appDataUnderPathAndFileName );
}
std::string cString::getWritablePath( )
{
    auto writablePath = std::string( getenv( "APPDATA" ) ) + "\\mogupro\\";
    if ( !makeDirectory( writablePath ) )
    {
        throw std::runtime_error( writablePath + ": フォルダを作成できませんでした。" );
    }
    return writablePath;
}
std::string cString::getAssetPath( )
{
    return cinder::app::getAssetDirectories( ).front( ).string( ) + "\\";
}
std::string cString::loadString( std::string const & relativePath )
{
    return static_cast<char*>( cinder::app::loadAsset( relativePath )->getBuffer( )->getData( ) );
}
std::wstring cString::multiToWide( std::string const & src )
{
    auto const dest_size = ::MultiByteToWideChar( CP_ACP, 0, src.data( ), -1, nullptr, 0 );
    std::vector<wchar_t> dest( dest_size, L'\0' );
    if ( ::MultiByteToWideChar( CP_ACP, 0, src.data( ), -1, dest.data( ), dest.size( ) ) == 0 ) {
        throw std::system_error { static_cast<int>( ::GetLastError( ) ), std::system_category( ) };
    }
    return std::wstring( dest.begin( ), dest.end( ) );
}
std::string cString::wideToMulti( std::wstring const & src )
{
    auto const dest_size = ::WideCharToMultiByte( CP_ACP, 0, src.data( ), -1, nullptr, 0, nullptr, nullptr );
    std::vector<char> dest( dest_size, '\0' );
    if ( ::WideCharToMultiByte( CP_ACP, 0, src.data( ), -1, dest.data( ), dest.size( ), nullptr, nullptr ) == 0 ) {
        throw std::system_error { static_cast<int>( ::GetLastError( ) ), std::system_category( ) };
    }
    return std::string( dest.begin( ), dest.end( ) );
}
std::string cString::wideToUtf8( std::wstring const & src )
{
    auto const dest_size = ::WideCharToMultiByte( CP_UTF8, 0, src.data( ), -1, nullptr, 0, nullptr, nullptr );
    std::vector<char> dest( dest_size, '\0' );
    if ( ::WideCharToMultiByte( CP_UTF8, 0, src.data( ), -1, dest.data( ), dest.size( ), nullptr, nullptr ) == 0 ) {
        throw std::system_error { static_cast<int>( ::GetLastError( ) ), std::system_category( ) };
    }
    return std::string( dest.begin( ), dest.end( ) );
}
std::wstring cString::utf8ToWide( std::string const & src )
{
    auto const dest_size = ::MultiByteToWideChar( CP_UTF8, 0, src.data( ), -1, nullptr, 0 );
    std::vector<wchar_t> dest( dest_size, L'\0' );
    if ( ::MultiByteToWideChar( CP_UTF8, 0, src.data( ), -1, dest.data( ), dest.size( ) ) == 0 ) {
        throw std::system_error { static_cast<int>( ::GetLastError( ) ), std::system_category( ) };
    }
    return std::wstring( dest.begin( ), dest.end( ) );
}
std::string cString::multiToUtf8( std::string const & src )
{
    auto const wide = multiToWide( src );
    return wideToUtf8( wide );
}
std::string cString::utf8ToMulti( std::string const & src )
{
    auto const wide = utf8ToWide( src );
    return wideToMulti( wide );
}
}
