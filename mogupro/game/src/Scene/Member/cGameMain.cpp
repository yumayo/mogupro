#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>
#include <cinder/Timeline.h>
#include <Utility/cInput.h>
#include <Scene/Member/cGameMain.h>
#include <Game/cFieldManager.h>
#include <Game/cStrategyManager.h>
#include <Camera/cCameraManager.h>
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

    if ( ENV->pressKey( KeyEvent::KEY_w ) )
        pos.z++;

    if ( ENV->pressKey( KeyEvent::KEY_s ) )
        pos.z--;

    if ( ENV->pressKey( KeyEvent::KEY_d ) )
        pos.x--;

    if ( ENV->pressKey( KeyEvent::KEY_a ) )
        pos.x++;

    if ( ENV->pressKey( KeyEvent::KEY_q ) )
        pos.y--;

    if ( ENV->pressKey( KeyEvent::KEY_e ) )
        pos.y++;

    if ( ENV->pressKey( KeyEvent::KEY_UP ) )
        CAMERA->setCameraAngle( ci::vec2( 0, 0.05f ) );

    if ( ENV->pressKey( KeyEvent::KEY_DOWN ) )
        CAMERA->setCameraAngle( ci::vec2( 0, -0.05f ) );

    if ( ENV->pressKey( KeyEvent::KEY_RIGHT ) )
        CAMERA->setCameraAngle( ci::vec2( 0.05f, 0 ) );

    if ( ENV->pressKey( KeyEvent::KEY_LEFT ) )
        CAMERA->setCameraAngle( ci::vec2( -0.05f, 0 ) );

    Game::cStrategyManager::getInstance( )->update(deltaTime);
    Collision::cCollisionManager::getInstance( )->update( );
}

void cGameMain::draw( )
{
    Game::cFieldManager::getInstance( )->draw( );
    //Game::Gem::cGemManager::getInstance()->Draw();
    /*auto lambert = gl::ShaderDef().lambert();
    auto shader = gl::getStockShader(lambert);
    shader->bind();*/
    gl::drawCube( pos, size );
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
