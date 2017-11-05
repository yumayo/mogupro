#include <Game/cDebugManager.h>
#include <Utility/cInput.h>
#include <Game/cPlayerManager.h>
#include <cinder/app/App.h>
namespace Game
{
void cDebugManager::setup( )
{
	mParam = cinder::params::InterfaceGl::create( "Status", cinder::ivec2( 300, cinder::app::getWindowHeight( ) - 24 * 2 ) );
	mParam->setPosition( cinder::ivec2( 24, 24 ) );
	mParam->hide( );
	mParam->addParam( "FPS", &mFps, true );
	mParam->addParam( "delta", &mDelta, true );
	mParam->addParam( "playerPosition", &mPlayerPosition, true );
}
void cDebugManager::update( float delta )
{
	if ( ENV->pushKey( cinder::app::KeyEvent::KEY_F3 ) )
	{
		if ( !mParam->isVisible( ) )
		{
			mParam->show( );
		}
		else
		{
			mParam->hide( );
		}
	}

	//
	mFps = 1.0F / delta;

	//
	mDelta = delta;

	//
	mPlayerPosition = cPlayerManager::getInstance( )->getActivePlayer( )->getPos( );
}
void cDebugManager::draw2d( )
{
	mParam->draw( );
}
}
