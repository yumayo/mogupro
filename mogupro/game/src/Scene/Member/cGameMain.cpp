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
    skydome.setup( );
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
	const int active_player_id = Network::cMatchingMemberManager::getInstance( )->mPlayerID;

	std::vector<int> teams;

    //�v���C���[�B�̈ʒu
    std::vector<ci::vec3> positions;
	float get_map_top_pos = Game::cFieldManager::getInstance()->getBlockTopPosition(ci::vec3(0,0,0)).y;
	for (int i = 0; i < player_numbers; i++) {
		positions.push_back(ci::vec3(0, get_map_top_pos, i * 2));
        teams.emplace_back( 0 );
	}
    for ( auto& o : Network::cMatchingMemberManager::getInstance( )->mPlayerDatas )
    {
        teams.push_back( o.teamNum );
    }

    Game::cPlayerManager::getInstance( )->setup(positions, player_numbers, active_player_id, teams);

	int seed = 20171031;
	GemManager->setUp(vec3(0,0,0),
		              vec3(Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_X /2,
						   Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_Y,
						   Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_Z),1,1,100,seed);
    Collision::cCollisionManager::getInstance( )->setup( );
    //Network::cUDPClientManager::getInstance( )->open( );
    //Network::cUDPServerManager::getInstance( )->open( );
    //Network::cUDPClientManager::getInstance( )->connectOfflineServer( );
	sendEndSetup = false;
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
		if (sendEndSetup == false)
		{
			Network::cUDPClientManager::getInstance()->send(new Network::Packet::Request::cReqEndGamemainSetup());
			sendEndSetup = true;
		}

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
		Game::cPlayerManager::getInstance()->playerCollisionUpdate();
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
    Game::cFieldManager::getInstance( )->draw( );
    Game::cStrategyManager::getInstance( )->draw( );
	GemManager->draw();
    Collision::cCollisionManager::getInstance( )->draw( );
    skydome.draw( );
	CAMERA->unBind3D();

	//�v���C���[������2D�`��
	CAMERA->bind2D();
	gl::disableDepthRead();
	gl::disableDepthWrite();
	GemManager->drawFbo();
	CAMERA->unBind2D();

	CAMERA->bind3D();
	gl::enableDepthRead();
	gl::enableDepthWrite();
    Game::cPlayerManager::getInstance( )->draw( );
}

void cGameMain::draw2D( )
{
	//�v���C���[���O��2D�`��
    gl::enableFaceCulling( false );
    gl::disableDepthRead( );
    gl::disableDepthWrite( );
	
    n->entry_render( mat4( ) );
}

void cGameMain::resize( )
{

}


}
}
