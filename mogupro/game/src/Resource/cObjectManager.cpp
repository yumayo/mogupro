#include <Resource/cObjectManager.h>
#include <Utility/cString.h>
#include <Utility/cSearchSystem.h>
#include <cinder/ObjLoader.h>
#include <cinder/app/App.h>
namespace Resource
{
cObjectManager::cObjectManager( )
{
    Utility::cSearchSystem search;
    search.search( Utility::cString::getAssetPath( ) + "OBJ\\" );
    auto& files = search.unixNotationFullPaths( );
    for ( int i = 0, size = files.size( ); i < size; ++i )
    {
        auto const& fileName = files[i];
        auto extension = Utility::cString::getExtensionName( fileName );
        if ( extension == "obj" )
        {
            auto underObjPos = fileName.find( "OBJ/" ) + sizeof( "OBJ/" ) - sizeof( '\0' );
            auto unferObjName = fileName.substr( underObjPos );
            cinder::ObjLoader loader( cinder::app::loadAsset( "OBJ/" + unferObjName ) );
            auto mesh = cinder::TriMesh::create( loader );
            auto vbo = cinder::gl::VboMesh::create( *mesh );
            mObjects.insert( std::make_pair( unferObjName, vbo ) );
        }
    }
}
cinder::gl::VboMeshRef & cObjectManager::findObject( std::string const & underAssetsUnderSEUnderPath )
{
    auto findItr = mObjects.find( underAssetsUnderSEUnderPath );
    if ( findItr != mObjects.end( ) )
    {
        return findItr->second;
    }
    else
    {
        throw std::runtime_error( underAssetsUnderSEUnderPath + ": ファイルが存在しません。" );
    }
}
}
