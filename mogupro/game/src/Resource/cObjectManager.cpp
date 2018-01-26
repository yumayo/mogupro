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
    mFilePaths = search.unixNotationFullPaths( );
}
cinder::gl::VboMeshRef cObjectManager::findObject( std::string const & underAssetsUnderSEUnderPath )
{
    auto findItr = mObjects.find( underAssetsUnderSEUnderPath );
    if ( findItr != mObjects.end( ) )
    {
        return findItr->second.vbo;
    }
    else
    {
        throw std::runtime_error( underAssetsUnderSEUnderPath + ": ファイルが存在しません。" );
    }
}
cinder::TriMeshRef cObjectManager::findMesh( std::string const & underAssetsUnderSEUnderPath )
{
	auto findItr = mObjects.find( underAssetsUnderSEUnderPath );
	if ( findItr != mObjects.end( ) )
	{
		return findItr->second.mesh;
	}
	else
	{
		throw std::runtime_error( underAssetsUnderSEUnderPath + ": ファイルが存在しません。" );
	}
}
void cObjectManager::loadOne( )
{
	if ( isFinished( ) ) return;

	auto const& fileName = mFilePaths[mCurrentLoadIndex];
	mCurrentLoadIndex = std::min( mCurrentLoadIndex + 1, (int)mFilePaths.size( ) );
	auto extension = Utility::cString::getExtensionName( fileName );
	if ( extension == "obj" )
	{
		auto underObjPos = fileName.find( "OBJ/" ) + sizeof( "OBJ/" ) - sizeof( '\0' );
		auto unferObjName = fileName.substr( underObjPos );
		cinder::ObjLoader loader( cinder::app::loadAsset( "OBJ/" + unferObjName ) );
		auto mesh = cinder::TriMesh::create( loader );
		auto vbo = cinder::gl::VboMesh::create( *mesh );
		mObjects.insert( std::make_pair( unferObjName, Mesh{ vbo, mesh } ) );
	}
}
bool cObjectManager::isFinished( )
{
	return mCurrentLoadIndex == mFilePaths.size( );
}
int cObjectManager::maxNum( )
{
	return mFilePaths.size( );
}
int cObjectManager::currentNum( )
{
	return mCurrentLoadIndex;
}
ObjectSuppoter const OBJ;
MeshSuppoter const MESH;
}
