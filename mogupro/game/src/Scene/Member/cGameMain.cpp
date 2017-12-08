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
#include <Game/cGameManager.h>
#include <Game/Field/RespawnPoint.h>
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

    Shader::cShadowManager::getInstance( )->setup( );
    Game::cFieldManager::getInstance( )->setup( );
    Game::cStrategyManager::getInstance( )->setup( );

	int player_numbers = 8;

	const int active_player_id = Network::cMatchingMemberManager::getInstance( )->mPlayerID;

	// チーム決め
	std::vector<int> teams;
	teams.emplace_back( Game::Player::Red );
	teams.emplace_back( Game::Player::Red );
	teams.emplace_back( Game::Player::Red );
	teams.emplace_back( Game::Player::Red );
	teams.emplace_back( Game::Player::Blue );
	teams.emplace_back( Game::Player::Blue );
	teams.emplace_back( Game::Player::Blue );
	teams.emplace_back( Game::Player::Blue );
    for ( auto& o : Network::cMatchingMemberManager::getInstance( )->mPlayerDatas )
    {
        teams[o.playerID] = o.teamNum;
    }
	teams[active_player_id] = Network::cMatchingMemberManager::getInstance()->mPlayerTeamNum;
	
	// リスポーン位置の設定。
	std::vector<ci::vec3> positions = Game::Field::RESPAWN_POINT;
    Game::cPlayerManager::getInstance( )->setup(positions, player_numbers, active_player_id, teams);
	for ( int i = 4; i < positions.size( ); ++i )
	{
		Game::cPlayerManager::getInstance( )->getPlayer( i )->move( vec3( 0, 0, -0.01F ) );
	}

	int seed = 20171031;
	GemManager->setUp(vec3(0,0,0),
		              vec3(Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_X /2,
						   Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_Y,
						   Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_Z),Game::Field::BLOCK_SIZE,0.5,100,seed);
    Collision::cCollisionManager::getInstance( )->setup( );
	Game::cLightManager::getInstance( )->setup( );
	Game::cShaderManager::getInstance( )->setup( );
	Game::cCapsuleManager::getInstance()->setup();
	Game::cSubWeaponManager::getInstance()->setup();
	Game::cUIManager::getInstance( )->setup( );

	auto now = boost::posix_time::microsec_clock::local_time( );
	auto ready = now + boost::posix_time::seconds( 9 );
	auto battle = ready + boost::posix_time::seconds( 2 );
	auto result = battle + boost::posix_time::minutes( 5 );
	Game::cGameManager::getInstance( )->setup( ready, battle, result );

	sendEndSetup = false;
	endTimer = false;
    gl::enableDepthRead( );
    gl::enableDepthWrite( );
	gameStartTimer = -1.0f;

	ENV->disableKeyWithMouseButton( );
	ENV->disablePadButton();
	ENV->disablePadAxis();
	Game::cUIManager::getInstance( )->disable( );
}

void cGameMain::shutDown( )
{
	Network::cUDPClientManager::removeInstance( );
	Network::cUDPServerManager::removeInstance( );
	Game::cDebugManager::removeInstance( );
	Game::cClientAdapter::removeInstance( );
	Game::cServerAdapter::removeInstance( );
	Game::cUIManager::removeInstance( );
	Game::cStrategyManager::removeInstance( );
	Game::cCapsuleManager::removeInstance( );
	Game::cSubWeaponManager::removeInstance( );
	Game::cPlayerManager::removeInstance( );
	Game::cShaderManager::removeInstance( );
	Particle::cParticleManager::removeInstance( );
	Game::cFieldManager::getInstance( )->shutdown( );
	Game::cFieldManager::removeInstance( );
	Game::cGemManager::removeInstance( );
	Game::cLightManager::removeInstance( );
	Collision::cCollisionManager::removeInstance( );
	Game::cGameManager::removeInstance( );
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
		
		// 他のアップデートよりも先に行います。
		Game::cGameManager::getInstance( )->preUpdate( deltaTime );

		Game::cDebugManager::getInstance( )->update( deltaTime );
        Game::cClientAdapter::getInstance( )->update( );
        Game::cServerAdapter::getInstance( )->update( );
        Game::cUIManager::getInstance( )->update( deltaTime );
        Game::cFieldManager::getInstance( )->update( deltaTime );
        Game::cPlayerManager::getInstance( )->update( deltaTime );
        Game::cStrategyManager::getInstance( )->update( deltaTime );
		Game::cCapsuleManager::getInstance()->update(deltaTime);
		Game::cSubWeaponManager::getInstance()->update(deltaTime);
        Collision::cCollisionManager::getInstance( )->update( deltaTime );
		Game::cPlayerManager::getInstance()->playerCollisionAfterUpdate( deltaTime );
		Game::cSubWeaponManager::getInstance()->updateCollisionAfterUpdate(deltaTime);
        GemManager->update(deltaTime);
		Game::cLightManager::getInstance( )->update( );
        Game::cShaderManager::getInstance( )->update( std::bind( &cGameMain::drawShadow, this ) );
        Particle::cParticleManager::getInstance()->update( deltaTime );
		GemManager->lateUpdate(deltaTime);
		Game::cGameManager::getInstance( )->update( deltaTime );
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
	Game::cGameManager::getInstance( )->draw( );
	Game::cDebugManager::getInstance( )->draw2d( );
}

void cGameMain::resize( )
{

}


}
}
