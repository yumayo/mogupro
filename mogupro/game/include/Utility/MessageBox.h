#pragma once
#include <string>
#include <functional>
namespace Utility
{
void MessageBoxYesNo( std::string const& message, std::function<void( )> const& calledYes, std::function<void( )> const& calledNo );
void MessageBoxOk( std::string const& message, std::function<void( )> const& calledOk );
void MessageBoxError( std::string const& message, std::string const& fileName, int line, std::function<void( )> const& calledOk );
void MessageAssetNothing( std::string const& underAssetsPath );
}
#define MES_ERR(message, calledOk) Utility::MessageBoxError( message, __FILE__, __LINE__, calledOk );
