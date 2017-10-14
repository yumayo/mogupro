#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include <Game/ProductionCamera/cProdactionCamera.h>
#include <Utility/cInput.h>
#include <Game/SkyDome/cSkyDome.h>
#include <Game/cFieldManager.h>
#include <Game/cStrategyManager.h>
#include <Game/Gem/cGemManager.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class gameApp : public App
{
private:
    float delta_time;
    ci::vec3 pos;
    ci::vec3 size;
    Game::SkyDome::cSkyDome skydome;
public:
    void setup() override;
    void mouseDown( MouseEvent event ) override;
    void mouseMove( MouseEvent event ) override;
    void mouseDrag( MouseEvent event ) override;
    void mouseUp( MouseEvent event )   override;
    void keyDown( KeyEvent event )     override;
    void keyUp( KeyEvent event )       override;
    void update() override;
    void draw() override;
};

void gameApp::setup()
{
    pos = vec3( 0 );
    size = vec3( 2 );
    skydome.setup();
    Game::cFieldManager::getInstance()->setup();
    //Game::Gem::cGemManager::getInstance()->SetUp(vec3(5, 5, 5), vec3(10, 10, 10), 10, 0);
    //Game::Gem::cGemManager::getInstance()->Create();
    CAMERA->followingCamera( &pos, 30 );
    CAMERA->setup();
    ENV->padSetup();

    Game::cStrategyManager::getInstance()->setup();

    gl::enableDepthRead();
    gl::enableDepthWrite();
}

void gameApp::mouseDown( MouseEvent event )
{
    CAMERA->shakeCamera( 0.1f, 0.1f );
}

void gameApp::mouseUp( MouseEvent event )
{
    ENV->mouseUp( event );
}

void gameApp::keyDown( KeyEvent event )
{
    ENV->keyDown( event );
}

void gameApp::keyUp( KeyEvent event )
{
    ENV->keyUp( event );
}

void gameApp::mouseMove( MouseEvent event )
{
    ENV->mouseMove( event );
}

void gameApp::mouseDrag( MouseEvent event )
{
    ENV->mouseDrag( event );
}

void gameApp::update()
{
    delta_time = std::abs( delta_time - timeline().getCurrentTime() );
    CAMERA->update( delta_time );
    Game::cFieldManager::getInstance()->update( delta_time );

    delta_time = timeline().getCurrentTime();
    ENV->padUpdate();
    ENV->padProcessEvent();

    //カメラのマウス操作ON　OFF
    if (ENV->pushKey( KeyEvent::KEY_ESCAPE ))
    {
        ENV->setMouseControl( true );
    }
    if (ENV->pushKey( KeyEvent::KEY_1 ))
    {
        ENV->setMouseControl( false );
    }

    if (ENV->isPadPush( ENV->BUTTON_1 ))
    {
        CAMERA->shakeCamera( 0.1f, 0.1f );
    }
    CAMERA->setCameraAngle( ci::vec2( ENV->getPadAxis( 2 )*(-0.05f), ENV->getPadAxis( 3 )*(-0.05f) ) );

    if (ENV->pressKey( KeyEvent::KEY_w ))
        pos.z++;

    if (ENV->pressKey( KeyEvent::KEY_s ))
        pos.z--;

    if (ENV->pressKey( KeyEvent::KEY_d ))
        pos.x--;

    if (ENV->pressKey( KeyEvent::KEY_a ))
        pos.x++;

    if (ENV->pressKey( KeyEvent::KEY_q ))
        pos.y--;

    if (ENV->pressKey( KeyEvent::KEY_e ))
        pos.y++;

    if (ENV->pressKey( KeyEvent::KEY_UP ))
        CAMERA->setCameraAngle( ci::vec2( 0, 0.05f ) );

    if (ENV->pressKey( KeyEvent::KEY_DOWN ))
        CAMERA->setCameraAngle( ci::vec2( 0, -0.05f ) );

    if (ENV->pressKey( KeyEvent::KEY_RIGHT ))
        CAMERA->setCameraAngle( ci::vec2( 0.05f, 0 ) );

    if (ENV->pressKey( KeyEvent::KEY_LEFT ))
        CAMERA->setCameraAngle( ci::vec2( -0.05f, 0 ) );

    Game::cStrategyManager::getInstance()->update();
}

void gameApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );


    gl::setMatrices( CAMERA->getCamera() );
    Game::cFieldManager::getInstance()->draw();
    //Game::Gem::cGemManager::getInstance()->Draw();
    /*auto lambert = gl::ShaderDef().lambert();
    auto shader = gl::getStockShader(lambert);
    shader->bind();*/
    gl::drawCube( pos, size );
    Game::cStrategyManager::getInstance()->draw();
    skydome.draw();

    ENV->flashInput();
}

CINDER_APP( gameApp, RendererGl, []( App::AppBase::Settings* s ) { s->setWindowSize( 1600, 900 ); } )
