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
    // printf�������̂��A���Ă��܂��B
    static std::string format( char const * str, ... );
    // printf�Ɠ����B
    static void log( char const * str, ... );
    // UTF-8
    static bool directoryExists( std::string fullPathWithDirectoryName );
    // UTF-8
    static bool fileExists( std::string fullPathWithFileName );
    // UTF-8
    static bool makeDirectory( std::string fullPathWithDirectoryName );
    // �ݒ肳�ꂽ�f�B���N�g�����ċA�I�Ɍ������ĂȂ������玩���ō쐬����܂��B
    // ���Ȃ������ꍇ��throw�����ł��܂��B
    // �܂��f�B���N�g�������ꂽ��A�t�@�C�������Ȃ������ꍇ��throw�����ł��܂��B
    // ����: �f�[�^�̃T�C�Y, �f�[�^�̐擪�|�C���^, AppData�ȉ��̊K�w�ƃt�@�C����(UTF-8)
    static void saveData( size_t writeByteSize, char const* str, std::string const& appDataUnderPathAndFileName );
    // �ݒ肳�ꂽ�f�B���N�g�����ċA�I�Ɍ������ĂȂ������玩���ō쐬����܂��B
    // ���Ȃ������ꍇ��throw�����ł��܂��B
    // �܂��f�B���N�g�������ꂽ��A�t�@�C�������Ȃ������ꍇ��throw�����ł��܂��B
    // ����: ������, AppData�ȉ��̊K�w�ƃt�@�C����(UTF-8)
    static void saveData( std::string const& data, std::string const& appDataUnderPathAndFileName );
    // �ݒ肳�ꂽ�f�B���N�g�����ċA�I�Ɍ������ĂȂ������玩���ō쐬����܂��B
    // ���Ȃ������ꍇ��throw�����ł��܂��B
    // �܂��f�B���N�g�������ꂽ��A�t�@�C�������Ȃ������ꍇ��throw�����ł��܂��B
    // ����: Json�f�[�^, AppData�ȉ��̊K�w�ƃt�@�C����(UTF-8)
    static void saveData( Json::Value const& root, std::string const& appDataUnderPathAndFileName );

    // �ȉ�
    // http://nekko1119.hatenablog.com/entry/2017/01/02/054629

    static std::wstring multiToWide( std::string const& src );
    static std::string wideToMulti( std::wstring const& src );
    static std::string wideToUtf8( std::wstring const& src );
    static std::wstring utf8ToWide( std::string const& src );
    static std::string multiToUtf8( std::string const& src );
    static std::string utf8ToMulti( std::string const& src );
};
}
