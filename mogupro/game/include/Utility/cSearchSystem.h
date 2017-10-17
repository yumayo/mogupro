#pragma once
#include <vector>
#include <string>
#include <filesystem>
namespace Utility
{
//
// http://qiita.com/episteme/items/0e3c2ee8a8c03780f01e
// 上記のサイトより

class cSearchSystem
{
    std::string mWindowsNotationRoot;
    std::string mUnixNotationRoot;
    std::vector<std::string> mWindowsNotationFiles;
    std::vector<std::string> mWindowsNotationFullPaths;
    std::vector<std::string> mWindowsNotationDirectorys;
    std::vector<std::string> mUnixNotationFiles;
    std::vector<std::string> mUnixNotationFullPaths;
    std::vector<std::string> mUnixNotationDirectorys;
public:
    void search( std::string const& path );
    std::vector<std::string> const& windowsNotationFiles( );
    std::vector<std::string> const& windowsNotationFullPaths( );
    std::vector<std::string> const& windowsNotationDirectorys( );
    std::vector<std::string> const& unixNotationFiles( );
    std::vector<std::string> const& unixNotationFullPaths( );
    std::vector<std::string> const& unixNotationDirectorys( );
private:
    void searchDirectory( std::tr2::sys::path const& path );
    void searchFile( std::tr2::sys::path const& path );
    void searchPath( std::tr2::sys::path const& path );
};
}