#pragma once
#include <string>
#include <vector>
#include <jsoncpp/json.h>
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
    static std::string getWritablePath( );
    static std::string getAssetPath( );
    static std::string loadString( std::string const & relativePath );
    // printfしたものが帰ってきます。
    static std::string format( char const * str, ... );
    // printfと同等。
    static void log( char const * str, ... );
    // UTF-8
    static bool directoryExists( std::string fullPathWithDirectoryName );
    // UTF-8
    static bool fileExists( std::string fullPathWithFileName );
    // UTF-8
    static bool makeDirectory( std::string fullPathWithDirectoryName );
    // 設定されたディレクトリを再帰的に検索してなかったら自動で作成されます。
    // 作れなかった場合はthrowが飛んできます。
    // またディレクトリが作られた後、ファイルを作れなかった場合もthrowが飛んできます。
    // 引数: データのサイズ, データの先頭ポインタ, AppData以下の階層とファイル名(UTF-8)
    static void saveData( size_t writeByteSize, char const* str, std::string const& appDataUnderPathAndFileName );
    // 設定されたディレクトリを再帰的に検索してなかったら自動で作成されます。
    // 作れなかった場合はthrowが飛んできます。
    // またディレクトリが作られた後、ファイルを作れなかった場合もthrowが飛んできます。
    // 引数: 文字列, AppData以下の階層とファイル名(UTF-8)
    static void saveData( std::string const& data, std::string const& appDataUnderPathAndFileName );
    // 設定されたディレクトリを再帰的に検索してなかったら自動で作成されます。
    // 作れなかった場合はthrowが飛んできます。
    // またディレクトリが作られた後、ファイルを作れなかった場合もthrowが飛んできます。
    // 引数: Jsonデータ, AppData以下の階層とファイル名(UTF-8)
    static void saveData( Json::Value const& root, std::string const& appDataUnderPathAndFileName );

    // 以下
    // http://nekko1119.hatenablog.com/entry/2017/01/02/054629

    static std::wstring multiToWide( std::string const& src );
    static std::string wideToMulti( std::wstring const& src );
    static std::string wideToUtf8( std::wstring const& src );
    static std::wstring utf8ToWide( std::string const& src );
    static std::string multiToUtf8( std::string const& src );
    static std::string utf8ToMulti( std::string const& src );
};
}
