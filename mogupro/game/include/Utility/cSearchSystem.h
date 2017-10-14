#pragma once
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <map>
#include <Windows.h>
#include <Utility/cString.h>
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
    std::vector<std::string> mWindowsNotationDirectorys;
    std::vector<std::string> mUnixNotationFiles;
    std::vector<std::string> mUnixNotationDirectorys;
public:
    void search( std::string const& path );
    std::vector<std::string> const& windowsNotationFiles( );
    std::vector<std::string> const& windowsNotationDirectorys( );
    std::vector<std::string> const& unixNotationFiles( );
    std::vector<std::string> const& unixNotationDirectorys( );
private:
    void searchDirectory( std::tr2::sys::path const& path );
    void searchFile( std::tr2::sys::path const& path );
    void searchPath( std::tr2::sys::path const& path );
};
}