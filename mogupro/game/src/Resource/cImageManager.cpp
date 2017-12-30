#include <Resource/cImageManager.h>
#include <Utility/cString.h>
#include <Utility/cSearchSystem.h>
#include <cinder/ObjLoader.h>
#include <cinder/app/App.h>
namespace Resource
{
cImageManager::cImageManager( )
{
	Utility::cSearchSystem search;
	search.search( Utility::cString::getAssetPath( ) + "IMAGE\\" );
	mFilePaths = search.unixNotationFullPaths( );
}
cinder::gl::TextureRef& cImageManager::findObject( std::string const & underAssetsUnderImagePath )
{
	auto findItr = mImages.find( underAssetsUnderImagePath );
	if ( findItr != mImages.end( ) )
	{
		return findItr->second;
	}
	else
	{
		throw std::runtime_error( underAssetsUnderImagePath + ": ファイルが存在しません。" );
	}
}
void cImageManager::loadOne( )
{
	if ( isFinished( ) ) return;

	auto const& fileName = mFilePaths[mCurrentLoadIndex];
	mCurrentLoadIndex = std::min( mCurrentLoadIndex + 1, (int)mFilePaths.size( ) );
	auto extension = Utility::cString::getExtensionName( fileName );
	extension = Utility::cString::toUpper( extension );
	if ( extension == "PNG" || extension == "JPG" || extension == "JPEG" || extension == "BMP" )
	{
		auto underImagePos = fileName.find( "IMAGE/" ) + sizeof( "IMAGE/" ) - sizeof( '\0' );
		auto unferImageName = fileName.substr( underImagePos );
		auto image = cinder::gl::Texture::create( cinder::loadImage( cinder::app::loadAsset( "IMAGE/" + unferImageName ) ) );
		mImages.insert( std::make_pair( unferImageName, image ) );
	}
}
bool cImageManager::isFinished( )
{
	return mCurrentLoadIndex == mFilePaths.size( );
}
int cImageManager::maxNum( )
{
	return mFilePaths.size( );
}
int cImageManager::currentNum( )
{
	return mCurrentLoadIndex;
}
}
