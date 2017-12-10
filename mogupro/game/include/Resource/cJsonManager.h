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
	boost::optional<Json::Value const&> find( std::string const& underAssetsUnderSEUnderPath ) const noexcept;
private:
    std::map<std::string, Json::Value> mJsons;
};
}
