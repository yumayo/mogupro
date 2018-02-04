#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>
#include <Scene/cSceneManager.h>
#include <CameraManager/cCameraManager.h>
#include <Utility/cInput.h>
#include <Scene/Member/cBegin.h>
#include <Utility/cScheduler.h>
#include <Resource/cJsonManager.h>
class gameApp : public cinder::app::App
{
private:
    float prevSeconds = 0.0F;
public:
    void setup( ) override;
    void mouseDown( cinder::app::MouseEvent event ) override;
    void mouseMove( cinder::app::MouseEvent event ) override;
    void mouseDrag( cinder::app::MouseEvent event ) override;
    void mouseUp( cinder::app::MouseEvent event )   override;
    void keyDown( cinder::app::KeyEvent event )     override;
    void keyUp( cinder::app::KeyEvent event )       override;
    void update( ) override;
    void draw( ) override;
    void cleanup( ) override;
};
void gameApp::setup( )
{
	ENV->setup( );
    Scene::cSceneManager::getInstance( )->shift<Scene::Member::cBegin>( );
}
void gameApp::mouseDown( cinder::app::MouseEvent event )
{
	ENV->mouseDown(event);
}
void gameApp::mouseUp( cinder::app::MouseEvent event )
{
    ENV->mouseUp( event );
}
void gameApp::keyDown( cinder::app::KeyEvent event )
{
    ENV->keyDown( event );

	if ( event.getCode( ) == event.KEY_F5 )
	{
		Resource::cJsonManager::getInstance( )->reload( );
	}
}
void gameApp::keyUp( cinder::app::KeyEvent event )
{
    ENV->keyUp( event );
}
void gameApp::mouseMove( cinder::app::MouseEvent event )
{
    ENV->mouseMove( event );
}
void gameApp::mouseDrag( cinder::app::MouseEvent event )
{
    ENV->mouseDrag( event );
}
void gameApp::update( )
{
	float elapsedSeconds = cinder::app::getElapsedSeconds( );
    float delta = elapsedSeconds - prevSeconds;
    prevSeconds = elapsedSeconds;

	ENV->preUpdate( delta );

	try
	{
		Scene::cSceneManager::getInstance( )->update( delta );
		CAMERA->update( delta );
		Utility::cScheduler::getInstance( )->update( delta );
	}
	catch ( Scene::cSceneManager::SceneDeleted const& sceneDeleted )
	{

	}
}
void gameApp::draw( )
{
	cinder::gl::clear( cinder::Color( 0, 0, 0 ) );

    CAMERA->bind3D( );
	Scene::cSceneManager::getInstance( )->draw( );
    CAMERA->unBind3D( );

    CAMERA->bind2D( );
	Scene::cSceneManager::getInstance( )->draw2D( );
    CAMERA->unBind2D( );

    ENV->flashInput( );
}
void gameApp::cleanup()
{
	ENV->cleanup( );
	Scene::cSceneManager::removeInstance( );
}
CINDER_APP( gameApp, cinder::app::RendererGl, [ ] ( cinder::app::App::AppBase::Settings* s ) { s->setWindowSize( 1600, 900 ); } )
