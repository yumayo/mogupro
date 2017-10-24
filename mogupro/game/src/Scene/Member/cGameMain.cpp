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
#include <Network.hpp>
#include <Game/cClientAdapter.h>
#include <Game/cServerAdapter.h>
#include <Resource/TextureManager.h>
#include <Shader/cShadowManager.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
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

    n = Node::node::create( );
    auto font = Node::Renderer::label::create( "sawarabi-gothic-medium.ttf", 64 );
    font->set_text( u8"���{��ق��ق�" );
    font->set_scale( vec2( 1, -1 ) );
    using namespace Node::Action;
    n->run_action( repeat_forever::create( sequence::create(
        ease<EaseOutCubic>::create( move_to::create( 1.0F, vec2( 360, 200 ) ) ),
        ease<EaseOutCubic>::create( move_to::create( 1.0F, vec2( 360, -200 ) ) ),
        ease<EaseOutCubic>::create( move_to::create( 1.0F, vec2( -360, -200 ) ) ),
        ease<EaseOutCubic>::create( move_to::create( 1.0F, vec2( -360, 200 ) ) )
    ) ) );
    n->set_schedule_update( );
    n->add_child( font );

    Shader::cShadowManager::getInstance( )->setup( );
    Game::cFieldManager::getInstance( )->setup( );
    Game::cStrategyManager::getInstance( )->setup( );

	
	//�v���C���[�̐l��
	int player_numbers = 8;
	//��������P�Ȃ̂�
	const int active_player_id = 1;
	for (int i = 0; i < player_numbers; i++) {
		positions.push_back(ci::vec3(0, 10, i * 2));
	}
    Game::cPlayerManager::getInstance( )->setup(positions, player_numbers, active_player_id);

	int seed = 20171031;
	GemManager->setUp(vec3(0,0,0),vec3(8,6,16),1,1,10,seed);
    Collision::cCollisionManager::getInstance( )->setup( );
    Network::cUDPClientManager::getInstance( )->open( );
    Network::cUDPServerManager::getInstance( )->open( );
    Network::cUDPClientManager::getInstance( )->connectOfflineServer( );

    gl::enableDepthRead( );
    gl::enableDepthWrite( );
}

void cGameMain::shutDown( )
{

}

void cGameMain::update( float deltaTime )
{
    n->entry_update( deltaTime );
    Shader::cShadowManager::getInstance( )->update( std::bind( &cGameMain::drawShadow, this ) );
    Game::cFieldManager::getInstance( )->update( deltaTime );
    ENV->padUpdate( );
    ENV->padProcessEvent( );
    Game::cPlayerManager::getInstance( )->update( deltaTime );
    Game::cStrategyManager::getInstance( )->update( deltaTime );
    Collision::cCollisionManager::getInstance( )->update( );
	GemManager->update();
    Network::cUDPClientManager::getInstance( )->update( deltaTime );
    Network::cUDPServerManager::getInstance( )->update( deltaTime );
    Game::cClientAdapter::getInstance( )->update( );
    Game::cServerAdapter::getInstance( )->update( );
}

void cGameMain::draw( )
{
    Shader::cShadowManager::getInstance( )->draw( std::bind( &cGameMain::drawShadow, this ) );
}

void cGameMain::drawShadow( )
{
    Game::cPlayerManager::getInstance( )->draw( );
    Game::cFieldManager::getInstance( )->draw( );
    Game::cStrategyManager::getInstance( )->draw( );
	GemManager->draw();
    skydome.draw( );
}

void cGameMain::draw2D( )
{
    gl::enableFaceCulling( false );
    gl::disableDepthRead( );
    gl::disableDepthWrite( );
    //gl::draw( TEX->get( "sky_dome" ), ci::Rectf( 0, 300, 300, 0 ) );
	GemManager->drawFbo();
    n->entry_render( mat4( ) );
}

void cGameMain::resize( )
{

}


}
}
