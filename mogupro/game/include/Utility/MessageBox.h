#pragma once
#include <string>
#include <functional>
namespace Utility
{
void MessageBoxYesNo( std::string const& message, std::function<void( )> const& calledYes, std::function<void( )> const& calledNo );
void MessageBoxOk( std::string const& message, std::function<void( )> const& calledOk );
void MessageAssetNothing( std::string const& underAssetsPath );
}
