#include <Utility/cSearchSystem.h>
#include <algorithm>
#include <Windows.h>
#include <Utility/cString.h>
namespace Utility
{
void cSearchSystem::search( std::string const & path )
{
    mWindowsNotationRoot = path;
    cString::replace( &mWindowsNotationRoot, "/", "\\" );
    auto lastBackslashUnderPos = mWindowsNotationRoot.rfind( "\\" ) + sizeof( "\\" ) - sizeof( '\0' );

    // C:\\hoge\\test\\;
    if ( lastBackslashUnderPos == mWindowsNotationRoot.size( ) )
    {
        // ok
    }
    // C:\\hoge\\test;
    else
    {
        mWindowsNotationRoot += "\\";
    }
    mUnixNotationRoot = mWindowsNotationRoot;
    cString::replace( &mUnixNotationRoot, "\\", "/" );
    searchPath( mWindowsNotationRoot );
}
std::vector<std::string> const & cSearchSystem::windowsNotationFiles( )
{
    return mWindowsNotationFiles;
}
std::vector<std::string> const & cSearchSystem::windowsNotationFullPaths( )
{
    return mWindowsNotationFullPaths;
}
std::vector<std::string> const & cSearchSystem::windowsNotationDirectorys( )
{
    return mWindowsNotationDirectorys;
}
std::vector<std::string> const & cSearchSystem::unixNotationFiles( )
{
    return mUnixNotationFiles;
}
std::vector<std::string> const & cSearchSystem::unixNotationFullPaths( )
{
    return mUnixNotationFullPaths;

}
std::vector<std::string> const & cSearchSystem::unixNotationDirectorys( )
{
    return mUnixNotationDirectorys;
}
void cSearchSystem::searchDirectory( std::tr2::sys::path const & path )
{
    std::string src = path.string( ).substr( mWindowsNotationRoot.size( ) );
    src = cString::multiToUtf8( src );
    mWindowsNotationDirectorys.emplace_back( src );
    cString::replace( &src, "\\", "/" );
    mUnixNotationDirectorys.emplace_back( src );
    searchPath( path );
}
void cSearchSystem::searchFile( std::tr2::sys::path const & path )
{
    auto writablePath = path.string( );
    mWindowsNotationFullPaths.emplace_back( writablePath );
    cString::replace( &writablePath, "\\", "/" );
    mUnixNotationFullPaths.emplace_back( writablePath );

    std::string src = path.string( ).substr( mWindowsNotationRoot.size( ) );
    src = cString::multiToUtf8( src );
    mWindowsNotationFiles.emplace_back( src );
    cString::replace( &src, "\\", "/" );
    mUnixNotationFiles.emplace_back( src );
}
void cSearchSystem::searchPath( std::tr2::sys::path const & path )
{
    std::tr2::sys::path p( path );
    std::for_each( std::tr2::sys::directory_iterator( p ), std::tr2::sys::directory_iterator( ), [ this ] ( std::tr2::sys::path const& p )
    {
        if ( std::tr2::sys::is_regular_file( p ) )
        {
            searchFile( p );
        }
    } );

    p = std::tr2::sys::path( path );
    std::for_each( std::tr2::sys::directory_iterator( p ), std::tr2::sys::directory_iterator( ), [ this ] ( std::tr2::sys::path const& p )
    {
        if ( std::tr2::sys::is_directory( p ) )
        {
            searchDirectory( p );
        }
    } );
}
}
