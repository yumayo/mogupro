#include<Utility/cSearchSystem.h>
namespace Utility
{
void cSearchSystem::search( std::string const & path )
{
    mWindowsNotationRoot = path;
    cString::replace( &mWindowsNotationRoot, "/", "\\" );
    mUnixNotationRoot = path;
    cString::replace( &mUnixNotationRoot, "\\", "/" );
    searchPath( mWindowsNotationRoot );
}
std::vector<std::string> const & cSearchSystem::windowsNotationFiles( )
{
    return mWindowsNotationFiles;
}
std::vector<std::string> const & cSearchSystem::windowsNotationDirectorys( )
{
    return mWindowsNotationDirectorys;
}
std::vector<std::string> const & cSearchSystem::unixNotationFiles( )
{
    return mUnixNotationFiles;
}
std::vector<std::string> const & cSearchSystem::unixNotationDirectorys( )
{
    return mUnixNotationDirectorys;
}
void cSearchSystem::searchDirectory( std::tr2::sys::path const & path )
{
    std::string src = path.string( ).substr( mWindowsNotationRoot.size( ) + 1 );
    src = cString::multi_to_utf8_winapi( src );
    mWindowsNotationDirectorys.emplace_back( src );
    cString::replace( &src, "\\", "/" );
    mUnixNotationDirectorys.emplace_back( src );
    searchPath( path );
}
void cSearchSystem::searchFile( std::tr2::sys::path const & path )
{
    std::string src = path.string( ).substr( mWindowsNotationRoot.size( ) + 1 );
    src = cString::multi_to_utf8_winapi( src );
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
