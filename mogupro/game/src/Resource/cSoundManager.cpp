#include <Resource/cSoundManager.h>
#include <Utility/cString.h>
#include <Utility/cSearchSystem.h>
namespace Resource
{
cSoundManager::cSoundManager( )
{
    loadSe( );
    loadBgm( );
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
void cSoundManager::loadSe( )
{
    Utility::cSearchSystem search;
    search.search( Utility::cString::getAssetPath( ) + "SE\\" );
    auto& files = search.unixNotationFullPaths( );
    for ( int i = 0, size = files.size( ); i < size; ++i )
    {
        auto const& fileName = files[i];
        auto extension = Utility::cString::getExtensionName( fileName );
        if ( extension == "wav" )
        {
            auto underBgmPos = fileName.find( "SE/" ) + sizeof( "SE/" ) - sizeof( '\0' );
            auto unferBgmName = fileName.substr( underBgmPos );
            mSes.insert( std::make_pair( unferBgmName, std::move( Sound::cSE( "SE/" + unferBgmName ) ) ) );
        }
    }
}
void cSoundManager::loadBgm( )
{
    Utility::cSearchSystem search;
    search.search( Utility::cString::getAssetPath( ) + "BGM\\" );
    auto& files = search.unixNotationFullPaths( );
    for ( int i = 0, size = files.size( ); i < size; ++i )
    {
        auto const& fileName = files[i];
        auto extension = Utility::cString::getExtensionName( fileName );
        if ( extension == "wav" )
        {
            auto underBgmPos = fileName.find( "BGM/" ) + sizeof( "BGM/" ) - sizeof( '\0' );
            auto unferBgmName = fileName.substr( underBgmPos );
            mBgms.insert( std::make_pair( unferBgmName, std::move( Sound::cBGM( "BGM/" + unferBgmName ) ) ) );
        }
    }
}
}
