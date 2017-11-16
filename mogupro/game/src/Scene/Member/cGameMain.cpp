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
#include <Game/cShaderManager.h>
#include <Game/cDebugManager.h>
#include <Game/cLightManager.h>
#include <Game/cUIManager.h>
#include <Particle/cParticleManager.h>
#include<Game/cCapsuleManager.h>
#include<Game/cSubWeaponManager.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>
using namespace ci;
using namespace ci::app;
using namespace std;
namespace Scene
{
namespace Member
{
void cGameMain::setup( )
{
	Game::cDebugManager::getInstance( )->setup( );

	glsl = cinder::gl::GlslProg::create( cinder::app::loadAsset( "Shader/world.vert" ), 
										 cinder::app::loadAsset( "Shader/world.frag" ) );

    skydome.setup( );
    CAMERA->setup( );
    ENV->padSetup( );

    Game::cUIManager::getInstance( )->setup( );

    Shader::cShadowManager::getInstance( )->setup( );
    Game::cFieldManager::getInstance( )->setup( );
    Game::cStrategyManager::getInstance( )->setup( );

	int player_numbers = 8;

	const int active_player_id = Network::cMatchingMemberManager::getInstance( )->mPlayerID;

	std::vector<int> teams;

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
						   Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_Z),Game::Field::BLOCK_SIZE,0.5,100,seed);
    Collision::cCollisionManager::getInstance( )->setup( );
    //Network::cUDPClientManager::getInstance( )->open( );
    //Network::cUDPServerManager::getInstance( )->open( );
    //Network::cUDPClientManager::getInstance( )->connectOfflineServer( );
	Game::cLightManager::getInstance( )->setup( );
	Game::cShaderManager::getInstance( )->setup( );
	Game::cCapsuleManager::getInstance()->setup();
	Game::cSubWeaponManager::getInstance()->setup();

	sendEndSetup = false;
	endTimer = false;
    gl::enableDepthRead( );
    gl::enableDepthWrite( );
	gameStartTimer = -1.0f;
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
		if (endTimer == false)
		{
			while (auto resSetGamestartTimer = Network::cResponseManager::getInstance()->getResSetGamestartTimer())
			{
				boost::posix_time::ptime nowTime = boost::posix_time::second_clock::universal_time();
				 startTime = boost::posix_time::from_iso_string(resSetGamestartTimer->mTimerStr);
				gameStartTimer = (startTime.time_of_day().total_milliseconds() - nowTime.time_of_day().total_milliseconds()) / 1000.0f;
				continue;
			}

			gameStartTimer -= deltaTime;
			if (gameStartTimer < 0.0f)
			{
				endTimer = true;
				Network::cUDPClientManager::getInstance()->send(new Network::Packet::Request::cReqEndStartTimer());
			}
		}
		


		Game::cDebugManager::getInstance( )->update( deltaTime );
        Game::cClientAdapter::getInstance( )->update( );
        Game::cServerAdapter::getInstance( )->update( );
        Game::cUIManager::getInstance( )->update( deltaTime );
        Game::cFieldManager::getInstance( )->update( deltaTime );
        ENV->padUpdate( );
        ENV->padProcessEvent( );
        Game::cPlayerManager::getInstance( )->update( deltaTime );
        Game::cStrategyManager::getInstance( )->update( deltaTime );
		Game::cCapsuleManager::getInstance()->update(deltaTime);
		Game::cSubWeaponManager::getInstance()->update(deltaTime);
        Collision::cCollisionManager::getInstance( )->update( deltaTime );
		Game::cPlayerManager::getInstance()->playerCollisionAfterUpdate( deltaTime );
		Game::cSubWeaponManager::getInstance()->updateCollisionAfterUpdate(deltaTime);
        GemManager->update( );
		Game::cLightManager::getInstance( )->update( );
        Game::cShaderManager::getInstance( )->update( std::bind( &cGameMain::drawShadow, this ) );
        Particle::cParticleManager::getInstance()->update( deltaTime );
    }
}

void cGameMain::draw( )
{
	Game::cShaderManager::getInstance( )->draw( [ this ]
	{
		ci::gl::ScopedColor scpCol( ColorA( 1.0F, 1.0F, 1.0F, 1.0F ) );

		gl::enableDepthRead( );
		gl::enableDepthWrite( );
		Game::cFieldManager::getInstance( )->draw( );
		Game::cShaderManager::getInstance( )->uniformUpdate( );
		Game::cStrategyManager::getInstance( )->draw( );
		Game::cSubWeaponManager::getInstance()->draw();
		Game::cCapsuleManager::getInstance()->draw();
		GemManager->draw( );
		skydome.draw( );
		CAMERA->unBind3D( );

		CAMERA->bind2D( );
		gl::disableDepthRead( );
		gl::disableDepthWrite( );
		GemManager->drawFbo( );
		CAMERA->unBind2D( );

		CAMERA->bind3D( );
		gl::enableDepthRead( );
		gl::enableDepthWrite( );
		Game::cPlayerManager::getInstance( )->draw( );

		Particle::cParticleManager::getInstance( )->draw( );
	} );

	Collision::cCollisionManager::getInstance( )->draw( );
}

void cGameMain::drawShadow( )
{
	ci::gl::ScopedColor scpCol( ColorA( 1.0F, 1.0F, 1.0F, 1.0F ) );
	gl::enableDepthRead( );
	gl::enableDepthWrite( );
	Game::cFieldManager::getInstance( )->draw( );
	Game::cStrategyManager::getInstance( )->draw( );
	Game::cPlayerManager::getInstance( )->draw( );
}

void cGameMain::draw2D( )
{
    gl::enableFaceCulling( false );
    gl::disableDepthRead( );
    gl::disableDepthWrite( );
	
	Game::cUIManager::getInstance( )->draw( );
	Game::cDebugManager::getInstance( )->draw2d( );
}

void cGameMain::resize( )
{

}


}
}
