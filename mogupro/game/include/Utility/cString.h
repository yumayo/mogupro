#pragma once
#include <string>
namespace Utility
{
class cString
{
public:
    static void replace( std::string* src, std::string const& prev, std::string const& write );
    static std::vector<std::string> split( std::string const& s, char delim );
    static std::string getDirectoryNameUnix( std::string const& path );
    static std::string getDirectoryNameWindows( std::string const& path );
    static std::string getExtensionName( std::string const& path );
    static std::string toHex( unsigned char value );
    static std::string toHex( unsigned int value );
    static std::string createUniqueIdentifier( );

    // ˆÈ‰º
    // http://nekko1119.hatenablog.com/entry/2017/01/02/054629

    static std::wstring multi_to_wide_winapi( std::string const& src );
    static std::string wide_to_multi_winapi( std::wstring const& src );
    static std::string wide_to_utf8_winapi( std::wstring const& src );
    static std::wstring utf8_to_wide_winapi( std::string const& src );
    static std::string multi_to_utf8_winapi( std::string const& src );
    static std::string utf8_to_multi_winapi( std::string const& src );
};
}
