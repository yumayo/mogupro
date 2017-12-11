#include <Scene/Member/cBegin.h>
#include <Resource/cObjectManager.h>
#include <Resource/cSoundManager.h>
#include <Resource/TextureManager.h>
#include <Resource/cJsonManager.h>
#include <Resource/cFbxManager.h>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cTitle.h>
namespace Scene
{
namespace Member
{
void cBegin::setup( )
{
	Resource::cObjectManager::getInstance( );
	Resource::cSoundManager::getInstance( );
	Resource::cJsonManager::getInstance( );
	cSceneManager::getInstance( )->shift<cTitle>( );
    Resource::cFbxManager::getInstance( );
}
void cBegin::shutDown( )
{
}
void cBegin::update( float t )
{
}
void cBegin::draw( )
{
}
void cBegin::draw2D( )
{
}
void cBegin::resize( )
{
}
}
}
