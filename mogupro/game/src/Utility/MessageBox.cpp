#include <Utility/MessageBox.h>
#include <sstream>
#include <Windows.h>
// http://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/sec7-1msgbox.html
namespace Utility
{
void MessageBoxYesNo( std::string const & message, std::function<void( )> const & calledYes, std::function<void( )> const & calledNo )
{
    switch ( MessageBoxA( nullptr, message.c_str( ), "もぐプロ", MB_YESNO ) )
    {
    case IDYES:
        calledYes( );
        break;
    default:
        calledNo( );
        break;
    }
}
void MessageBoxOk( std::string const & message, std::function<void( )> const & calledOk )
{
    switch ( MessageBoxA( nullptr, message.c_str( ), "もぐプロ", MB_OK ) )
    {
    case IDOK:
        // nothing break
    default:
        calledOk( );
        break;
    }
}
void MessageBoxError( std::string const & message, std::string const & fileName, int line, std::function<void( )> const & calledOk )
{
    MessageBoxOk( message + "\n" + fileName + " : " + std::to_string( line ), calledOk );
}
void MessageAssetNothing( std::string const & underAssetsPath )
{
    std::stringstream message;
    message << underAssetsPath << "が見つかりません。" << std::endl;
    message << "そのままゲームを続行します。" << std::endl;
    switch ( MessageBoxA( nullptr, message.str( ).c_str( ), "もぐプロ", MB_OK ) )
    {
    case IDOK:
        // nothing break
    default:
        break;
    }
}
}
