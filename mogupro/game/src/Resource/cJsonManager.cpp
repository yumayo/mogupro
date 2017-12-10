#include <Resource/cJsonManager.h>
#include <Utility/cString.h>
#include <Utility/cSearchSystem.h>
#include <cinder/ObjLoader.h>
#include <cinder/app/App.h>
#include <boost/optional.hpp>
namespace Resource
{
cJsonManager::cJsonManager( )
{
    Utility::cSearchSystem search;
    search.search( Utility::cString::getAssetPath( ) + "JSON\\" );
    auto& files = search.unixNotationFullPaths( );
    for ( int i = 0, size = files.size( ); i < size; ++i )
    {
        auto const& fileName = files[i];
        auto extension = Utility::cString::getExtensionName( fileName );
        if ( extension == "json" )
        {
            auto underPos = fileName.find( "JSON/" ) + sizeof( "JSON/" ) - sizeof( '\0' );
            auto unferName = fileName.substr( underPos );
			std::stringstream ss;
			auto asset = cinder::app::loadAsset( "JSON/" + unferName );
			auto buffer = asset->getBuffer( );
			ss.write( static_cast<char const*>( buffer->getData( ) ), buffer->getSize( ) );
			Json::Value root;
			if ( Json::Reader( ).parse( ss, root ) )
			{
				mJsons.insert( std::make_pair( unferName, root ) );
			}
        }
    }
}
boost::optional<Json::Value const&> cJsonManager::find( std::string const & underAssetsUnderJSONUnderPath )const noexcept
{
	auto findItr = mJsons.find( underAssetsUnderJSONUnderPath );
	if ( findItr != mJsons.end( ) )
	{
		return findItr->second;
	}
	else
	{
		return boost::none;
	}
}
JsonSuppoter JSON;
}
