#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "Scene/cSceneManager.h"
#include "Scene/Member/cGameMain.h"
#include <CameraManager/cCameraManager.h>
#include <Utility/cInput.h>
#include <Utility/cTimeMeasurement.h>
#include <Scene/Member/cMatching.h>
#include <Scene/Member/cMatchingServer.h>
#include <Scene/Member/cTitle.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class gameApp : public App
{
private:
    float prevSeconds = 0.0F;
public:
    void setup( ) override;
    void mouseDown( MouseEvent event ) override;
    void mouseMove( MouseEvent event ) override;
    void mouseDrag( MouseEvent event ) override;
    void mouseUp( MouseEvent event )   override;
    void keyDown( KeyEvent event )     override;
    void keyUp( KeyEvent event )       override;
    void update( ) override;
    void draw( ) override;
    void cleanup( ) override;
};

void gameApp::setup( )
{
    cSceneManager::getInstance( )->change<Scene::Member::cTitle>( );
    cSceneManager::getInstance( )->now( ).setup( );
}

void gameApp::mouseDown( MouseEvent event )
{
	ENV->mouseDown(event);
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

void gameApp::update( )
{
	float elapsedSeconds = app::getElapsedSeconds( );
    float delta = elapsedSeconds - prevSeconds;
    prevSeconds = elapsedSeconds;

	 float t = 0.0F;
	 if ( getAverageFps( ) == 0.0F )
	 {
		 t = 1.0F / getFpsSampleInterval( );
	 }
	 else
	 {
		 t = 1.0F / getAverageFps( );
	 }
    cSceneManager::getInstance( )->now( ).update( t );
	CAMERA->update(delta);
}

void gameApp::draw( )
{
    gl::clear( Color( 0, 0, 0 ) );

    CAMERA->bind3D( );
    cSceneManager::getInstance( )->now( ).draw( );
    CAMERA->unBind3D( );

    CAMERA->bind2D( );
    cSceneManager::getInstance( )->now( ).draw2D( );
    CAMERA->unBind2D( );

    ENV->flashInput( );
}

void gameApp::cleanup()
{
    cSceneManager::getInstance()->now().shutDown();
}

CINDER_APP( gameApp, RendererGl, [ ] ( App::AppBase::Settings* s ) { s->setWindowSize( 1600, 900 ); } )
