#include <Resource/cSoundManager.h>
#include <Utility/cString.h>
#include <Utility/cSearchSystem.h>
namespace Resource
{
cSoundManager::cSoundManager( )
{
	{
		Utility::cSearchSystem search;
		search.search( Utility::cString::getAssetPath( ) + "SE\\" );
		mSeFilePaths = search.unixNotationFullPaths( );
	}

	{
		Utility::cSearchSystem search;
		search.search( Utility::cString::getAssetPath( ) + "BGM\\" );
		mBgmFilePaths = search.unixNotationFullPaths( );
	}
}
Sound::cSE& cSoundManager::findSe( std::string const & underAssetsUnderSEUnderPath )
{
	auto findItr = mSes.find( underAssetsUnderSEUnderPath );
	if ( findItr != mSes.end( ) )
	{
		return findItr->second;
	}
	else
	{
		throw std::runtime_error( underAssetsUnderSEUnderPath + ": ファイルが存在しません。" );
	}
}
Sound::cBGM& cSoundManager::findBgm( std::string const & underAssetsUnderBGMUnderPath )
{
	auto findItr = mBgms.find( underAssetsUnderBGMUnderPath );
	if ( findItr != mBgms.end( ) )
	{
		return findItr->second;
	}
	else
	{
		throw std::runtime_error( underAssetsUnderBGMUnderPath + ": ファイルが存在しません。" );
	}
}
void cSoundManager::loadOne( )
{
	if ( !isSeFinished( ) )
	{
		loadOneSe( );
	}
	else
	{
		loadOneBgm( );
	}
}
bool cSoundManager::isFinished( )
{
	return isSeFinished( ) && isBgmFinished( );
}
void cSoundManager::loadOneSe( )
{
	if ( isSeFinished( ) ) return;

	auto const& fileName = mSeFilePaths[mSeCurrentLoadIndex];
	mSeCurrentLoadIndex = std::min( mSeCurrentLoadIndex + 1, (int)mSeFilePaths.size( ) );
	auto extension = Utility::cString::getExtensionName( fileName );
	if ( extension == "wav" )
	{
		auto underBgmPos = fileName.find( "SE/" ) + sizeof( "SE/" ) - sizeof( '\0' );
		auto unferBgmName = fileName.substr( underBgmPos );
		mSes.insert( std::make_pair( unferBgmName, std::move( Sound::cSE( "SE/" + unferBgmName ) ) ) );
	}
}
bool cSoundManager::isSeFinished( )
{
	return mSeCurrentLoadIndex == mSeFilePaths.size( );
}
void cSoundManager::loadOneBgm( )
{
	if ( isBgmFinished( ) ) return;

	auto const& fileName = mBgmFilePaths[mBgmCurrentLoadIndex];
	mBgmCurrentLoadIndex = std::min( mBgmCurrentLoadIndex + 1, (int)mBgmFilePaths.size( ) );
	auto extension = Utility::cString::getExtensionName( fileName );
	if ( extension == "wav" )
	{
		auto underBgmPos = fileName.find( "BGM/" ) + sizeof( "BGM/" ) - sizeof( '\0' );
		auto unferBgmName = fileName.substr( underBgmPos );
		mBgms.insert( std::make_pair( unferBgmName, std::move( Sound::cBGM( "BGM/" + unferBgmName ) ) ) );
	}
}
bool cSoundManager::isBgmFinished( )
{
	return mBgmCurrentLoadIndex == mBgmFilePaths.size( );
}
int cSoundManager::maxNum( )
{
	return mSeFilePaths.size( ) + mBgmFilePaths.size( );
}
int cSoundManager::currentNum( )
{
	return mSeCurrentLoadIndex + mBgmCurrentLoadIndex;
}
}
