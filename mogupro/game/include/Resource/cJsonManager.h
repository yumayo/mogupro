#pragma once
#include <map>
#include <memory>
#include <Utility/cSingletonAble.h>
#include <jsoncpp/json.h>
#include <boost/optional.hpp>
namespace Resource
{
class cJsonManager : public Utility::cSingletonAble<cJsonManager>
{
public:
	cJsonManager( );
    // 例: Assets/JSON/player.json なら
    // ＞: player.json と入力する。
	boost::optional<Json::Value const&> find( std::string const& underAssetsUnderJSONUnderPath ) const noexcept;
private:
    std::map<std::string, Json::Value> mJsons;
};
struct JsonSuppoter
{
	class JsonNotFound : public std::runtime_error
	{
	public:
		explicit JsonNotFound( std::string const& message )
			: std::runtime_error( message )
		{
		}
	};
	Json::Value const& operator[]( std::string const& underAssetsUnderJSONUnderPath )
	{
		if ( auto json = cJsonManager::getInstance( )->find( underAssetsUnderJSONUnderPath ) )
		{
			return *json;
		}
		else
		{
			throw JsonNotFound( "jsonファイルがみつかりません" );
		}
	}
};
extern JsonSuppoter JSON;
}
