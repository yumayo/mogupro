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
#include <Network/cMatchingMemberManager.h>
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
    font->set_text( u8"日本語ほげほげ" );
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

	//プレイヤーの人数
	int player_numbers = 8;

	//自分が何Pなのか
	const int active_player_id = Network::cMatchingMemberManager::getInstance( )->mPlayerID;

	std::vector<int> teams;

    //プレイヤー達の位置
    std::vector<ci::vec3> positions;

	for (int i = 0; i < player_numbers; i++) {
		positions.push_back(ci::vec3(0, 20, i * 2));
        teams.emplace_back( 0 );
	}
    for ( auto& o : Network::cMatchingMemberManager::getInstance( )->mPlayerDatas )
    {
        teams.push_back( o.teamNum );
    }

    Game::cPlayerManager::getInstance( )->setup(positions, player_numbers, active_player_id, teams);

	int seed = 20171031;
	GemManager->setUp(vec3(0,-15,0),vec3(30,30,60),1,1,100,seed);
    Collision::cCollisionManager::getInstance( )->setup( );
    //Network::cUDPClientManager::getInstance( )->open( );
    //Network::cUDPServerManager::getInstance( )->open( );
    //Network::cUDPClientManager::getInstance( )->connectOfflineServer( );
	Network::cUDPClientManager::getInstance()->send(new Network::Packet::Request::cReqEndGamemainSetup());
    gl::enableDepthRead( );
    gl::enableDepthWrite( );
}

void cGameMain::shutDown( )
{
    Game::cFieldManager::getInstance()->shutdown();
}

void cGameMain::update( float deltaTime )
{
    Network::cUDPClientManager::getInstance( )->update( deltaTime );
    Network::cUDPServerManager::getInstance( )->update( deltaTime );

    if ( Network::cUDPClientManager::getInstance( )->isConnected( ) )
    {
        Game::cClientAdapter::getInstance( )->update( );
        Game::cServerAdapter::getInstance( )->update( );
        n->entry_update( deltaTime );
        //Shader::cShadowManager::getInstance( )->update( std::bind( &cGameMain::drawShadow, this ) );
        Game::cFieldManager::getInstance( )->update( deltaTime );
        ENV->padUpdate( );
        ENV->padProcessEvent( );
        Game::cPlayerManager::getInstance( )->update( deltaTime );
        Game::cStrategyManager::getInstance( )->update( deltaTime );
        Collision::cCollisionManager::getInstance( )->update( deltaTime );
        GemManager->update( );
    }
}

void cGameMain::draw( )
{
    drawShadow( );
    //Shader::cShadowManager::getInstance( )->draw( std::bind( &cGameMain::drawShadow, this ) );
}

void cGameMain::drawShadow( )
{
    gl::enableDepthRead( );
    gl::enableDepthWrite( );
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
