#include <Scene/Member/cBegin.h>
#include <Resource/cObjectManager.h>
#include <Resource/cSoundManager.h>
#include <Resource/TextureManager.h>
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
}
void cBegin::shutDown( )
{
}
void cBegin::update( float t )
{
	cSceneManager::getInstance( )->shift<cTitle>( );
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
