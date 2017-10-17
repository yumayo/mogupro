#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>
#include <cinder/Timeline.h>
#include <Utility/cInput.h>
#include <Scene/Member/cGameMain.h>
#include <Game/cFieldManager.h>
#include <Game/cStrategyManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cGemManager.h>
#include <Collision/cCollisionManager.h>
using namespace ci;
using namespace ci::app;
using namespace std;
namespace Scene
{
namespace Member
{
void cGameMain::setup( )
{
    pos = vec3( 0 );
    size = vec3( 2 );
    skydome.setup( );
    CAMERA->followingCamera( &pos, 30 );
    CAMERA->setup( );
    ENV->padSetup( );
    Game::cFieldManager::getInstance( )->setup( );
    //Game::Gem::cGemManager::getInstance()->SetUp(vec3(5, 5, 5), vec3(10, 10, 10), 10, 0);
    //Game::Gem::cGemManager::getInstance()->Create();
	
    Game::cStrategyManager::getInstance( )->setup( );
	Game::cPlayerManager::getInstance()->setup();
    Collision::cCollisionManager::getInstance( )->setup( );

    gl::enableDepthRead( );
    gl::enableDepthWrite( );
}

void cGameMain::shutDown( )
{

}

void cGameMain::update(float deltaTime)
{ 
    Game::cFieldManager::getInstance( )->update(deltaTime);
    ENV->padUpdate( );
    ENV->padProcessEvent( );
	Game::cPlayerManager::getInstance()->update(deltaTime);
    Game::cStrategyManager::getInstance( )->update(deltaTime);
    Collision::cCollisionManager::getInstance( )->update( );
}

void cGameMain::draw( )
{
	Game::cPlayerManager::getInstance()->draw();
    Game::cFieldManager::getInstance( )->draw( );
    Game::cStrategyManager::getInstance( )->draw( );
    Collision::cCollisionManager::getInstance( )->draw( );
    skydome.draw( );
}

void cGameMain::draw2D( )
{

}

void cGameMain::resize( )
{

}


}
}
