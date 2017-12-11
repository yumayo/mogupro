#include <Game/cGameManager.h>
#include <boost/format.hpp>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cTitle.h>
#include <Utility/cInput.h>
#include <Game/Field/FieldData.h>
#include <CameraManager/cCameraManager.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <Game/cUIManager.h>
#include <Game/Player/cPlayer.h>
#include <Game/Field/FieldData.h>
#include <Resource/cSoundManager.h>
#include <Game/cPlayerManager.h>
namespace pt = boost::posix_time;
namespace Game
{
cGameManager::cGameManager( )
{
	root = Node::node::create( );
	root->set_schedule_update( );
	root->set_content_size( cinder::app::getWindowSize( ) );
	root->set_scale( cinder::vec2( 1, -1 ) );
	root->set_position( root->get_content_size( ) * cinder::vec2( -0.5F, 0.5F ) );

	using namespace Node::Action;
	addPreUpdate( State::INIT, [ this ] ( float t )
	{
		next( );

		auto cameraPosition = root->add_child( Node::node::create( ) );
		cameraPosition->set_schedule_update( );
		cameraPosition->set_position_3d( Game::Field::WORLD_SIZE * cinder::vec3( 0.4F, 1.0F, 0.4F ) + cinder::vec3( 0, 1.0F, 0 ) );
		cameraPosition->run_action( move_by::create( 5.0F, cinder::vec3( 5.0F, 3.0F, 7.0F ) ) );
		cameraPosition->set_name( "cameraPosition" );

		auto cameraAngle = root->add_child( Node::node::create( ) );
		cameraAngle->set_schedule_update( );
		cameraAngle->set_position( cinder::vec2( -glm::pi<float>( ) / 2.0F, -0.2 ) );
		cameraAngle->run_action( move_by::create( 5.0F, cinder::vec2( -1.5F, 0.2F ) ) );
		cameraAngle->set_name( "cameraAngle" );

		auto fieldName = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
		fieldName->set_anchor_point( cinder::vec2( 0, 0 ) );
		fieldName->set_text( u8"もぐもぐコロニー" );
		fieldName->set_position( cinder::vec2( 50, 50 ) );

		auto rule = root->add_child( Node::node::create( ) );
		rule->set_position( root->get_content_size( ) / 2.0F );

		auto ruleSpeech = rule->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
		ruleSpeech->set_text( u8"たくさんジェムを大砲に入れろ" );
	} );
	addPreUpdate( State::LOAD, [ this ] ( float t )
	{
		if ( ENV->pushKey( cinder::app::MouseEvent::RIGHT_DOWN ) )
		{
			skipReady( );
		}
		// タイマーが来るまで待機します。
		if ( timeEmpty( ) ) return;
		if ( shiftSeconds[State::LOAD] < boost::posix_time::microsec_clock::local_time( ) )
		{
			next( );
			root->remove_all_children( );
		}
	} );
	addPreUpdate( State::MY_TEAM, [ this ] ( float t )
	{
		if ( shiftSeconds[State::MY_TEAM] < boost::posix_time::microsec_clock::local_time( ) )
		{
			next( );
		}
	} );
	addPreUpdate( State::ENEMY_TEAM, [ this ] ( float t )
	{
		if ( shiftSeconds[State::ENEMY_TEAM] < boost::posix_time::microsec_clock::local_time( ) )
		{
			next( );
			root->remove_all_children( );

			switch ( cPlayerManager::getInstance( )->getActivePlayerTeamId( ) )
			{
			case Player::Red:
				CAMERA->setCameraAngle( cinder::vec2( 0, 0 ) );
				break;
			case Player::Blue:
				CAMERA->setCameraAngle( cinder::vec2( -glm::pi<float>( ), 0 ) );
				break;
			default:
				break;
			}

			ENV->setMouseControl( true );
			auto ready = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 128 ) );
			ready->set_text( u8"ready" );
			ready->set_position( root->get_content_size( ) / 2.0F );
			ready->run_action( sequence::create( delay::create( 2.0F ), remove_self::create( ) ) );
		}
	} );
	addPreUpdate( State::READY, [ this ] ( float t )
	{
		if ( shiftSeconds[State::READY] < boost::posix_time::microsec_clock::local_time( ) )
		{
			next( );
			root->remove_all_children( );

			ENV->enableKeyButton( );
			ENV->enablePadAxis( );
			ENV->enablePadButton( );
			cUIManager::getInstance( )->enable( );
			auto go = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 128 ) );
			go->set_text( u8"GO!!" );
			go->set_position( root->get_content_size( ) / 2.0F );
			go->run_action( sequence::create( delay::create( 2.0F ), fade_out::create( 1.0F ), remove_self::create( ) ) );
			playBgm( "testbgm2.wav", 0.3f, 0.0f, 0.0f );
		}
	} );
	addPreUpdate( State::BATTLE, [ this ] ( float t )
	{
		if ( shiftSeconds[State::BATTLE] < boost::posix_time::microsec_clock::local_time( ) )
		{
			next( );

			ENV->disableMouseButton( );
			ENV->disableKeyButton( );
			ENV->disablePadButton( );
			ENV->disablePadAxis( );

			if ( root->get_child_by_name( "fader" ) ) return;
			auto n = root->add_child( Node::Renderer::rect::create( cinder::app::getWindowSize( ) ) );
			n->set_color( cinder::ColorA( 0, 0, 0, 0 ) );
			n->set_name( "fader" );
			n->set_position( root->get_content_size( ) / 2.0F );
			n->run_action( Node::Action::sequence::create( Node::Action::fade_in::create( 1.0F ), Node::Action::call_func::create( [ this ]
			{
				next( );

				ENV->enableKeyButton( );
				ENV->enableMouseButton( );
				ENV->enablePadButton( );
				ENV->enablePadAxis( );
				stopBgm( "testbgm2.wav" );
				cUIManager::getInstance( )->disable( );
				root->get_child_by_name( "fader" )->run_action( Node::Action::sequence::create( Node::Action::fade_out::create( 1.0F ), Node::Action::remove_self::create( ) ) );
				auto label = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
				label->set_position( root->get_content_size( ) / 2.0F );
				auto point = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
				point->set_position( root->get_content_size( ) / 2.0F + cinder::vec2( 0, 100 ) );
				auto p = cUIManager::getInstance( )->result( );
				switch ( cUIManager::getInstance( )->winTeam( ) )
				{
				case Game::Player::Red:
					label->set_text( "red team win" );
					point->set_text( "red: " + std::to_string( p.x ) + " <----> blue: " + std::to_string( p.y ) );
					break;
				case Game::Player::Blue:
					label->set_text( "blue team win" );
					point->set_text( "blue: " + std::to_string( p.y ) + " <----> red: " + std::to_string( p.x ) );
					break;
				default:
					break;
				}
			} ) ) );
		}
	} );
	addPreUpdate( State::BATTLE_END, [ this ] ( float t )
	{
		if ( shiftSeconds[State::BATTLE_END] < boost::posix_time::microsec_clock::local_time( ) )
		{
			next( );
		}
	} );
	addPreUpdate( State::RESULT, [ this ] ( float t )
	{

	} );
	addUpdate( State::INIT, [ this ] ( float t )
	{
	} );
	addUpdate( State::LOAD, [ this ] ( float t )
	{
		CAMERA->refPosition = root->get_child_by_name( "cameraPosition" )->get_position_3d( );
		CAMERA->setCameraAngle( root->get_child_by_name( "cameraAngle" )->get_position( ) );
	} );
	addUpdate( State::MY_TEAM, [ this ] ( float t )
	{
		switch ( cPlayerManager::getInstance( )->getActivePlayerTeamId( ) )
		{
		case Player::Red:
			CAMERA->refPosition = Field::CANNON_POINT[0];
			CAMERA->setCameraAngle( cinder::vec2( -glm::pi<float>( ), 0 ) );
			break;
		case Player::Blue:
			CAMERA->refPosition = Field::CANNON_POINT[1];
			CAMERA->setCameraAngle( cinder::vec2( 0, 0 ) );
			break;
		default:
			break;
		}
	} );
	addUpdate( State::ENEMY_TEAM, [ this ] ( float t )
	{
		switch ( cPlayerManager::getInstance( )->getActivePlayerTeamId( ) )
		{
		case Player::Red:
			CAMERA->refPosition = Field::CANNON_POINT[1];
			CAMERA->setCameraAngle( cinder::vec2( 0, 0 ) );
			break;
		case Player::Blue:
			CAMERA->refPosition = Field::CANNON_POINT[0];
			CAMERA->setCameraAngle( cinder::vec2( -glm::pi<float>( ), 0 ) );
			break;
		default:
			break;
		}
	} );
	addUpdate( State::READY, [ this ] ( float t )
	{
	} );
	addUpdate( State::BATTLE, [ this ] ( float t )
	{

	} );
	addUpdate( State::BATTLE_END, [ this ] ( float t )
	{

	} );
	addUpdate( State::RESULT, [ this ] ( float t )
	{
		CAMERA->refPosition = Game::Field::WORLD_SIZE * cinder::vec3( 0.5F, 2.5F, 0.5F );
		CAMERA->setCameraAngle( cinder::vec2( -glm::pi<float>( ) / 2.0F, -glm::pi<float>( ) ) );
		if ( ENV->pushKey( ) )
		{
			cSceneManager::getInstance( )->shift<Scene::Member::cTitle>( );
		}
	} );
}
void cGameManager::setTime( boost::posix_time::ptime loadTime )
{
	auto myTeam = loadTime + boost::posix_time::seconds( 2 );
	auto enemyTeam = myTeam + boost::posix_time::seconds( 2 );
	auto ready = enemyTeam + boost::posix_time::seconds( 2 );
	auto battle = ready + boost::posix_time::minutes( 5 );
	auto battleEnd = battle + boost::posix_time::seconds( 3 );
	auto result = battleEnd + boost::posix_time::seconds( 3 );

	shiftSeconds[State::LOAD] = loadTime;
	shiftSeconds[State::MY_TEAM] = myTeam;
	shiftSeconds[State::ENEMY_TEAM] = enemyTeam;
	shiftSeconds[State::READY] = ready;
	shiftSeconds[State::BATTLE] = battle;
	shiftSeconds[State::BATTLE_END] = battleEnd;
	shiftSeconds[State::RESULT] = result;
}
std::string cGameManager::getLeftBattleTime( )
{
	auto duration = shiftSeconds[State::BATTLE] - pt::microsec_clock::local_time( );
	if ( duration.is_negative( ) )
	{
		return "00:00";
	}
	return boost::str( boost::format( "%02d:%02d" ) % duration.minutes( ) % duration.seconds( ) );
}
void cGameManager::preUpdate( float delta )
{
	flash = false;
	mPreUpdates[state]( delta );
}
void cGameManager::update( float delta )
{
	root->entry_update( delta );
	mUpdates[state]( delta );
}
void cGameManager::draw( )
{
	root->entry_render( cinder::mat4( ) );
}
void cGameManager::skipReady( )
{
	auto now = boost::posix_time::microsec_clock::local_time( );
	shiftSeconds[State::LOAD] = now;
	shiftSeconds[State::MY_TEAM] = now;
	shiftSeconds[State::ENEMY_TEAM] = now;
	shiftSeconds[State::READY] = now;
	shiftSeconds[State::BATTLE] = now + boost::posix_time::minutes( 5 );
	shiftSeconds[State::BATTLE_END] = now + boost::posix_time::minutes( 5 ) + boost::posix_time::seconds( 3 );
	shiftSeconds[State::RESULT] = now + boost::posix_time::minutes( 5 ) + boost::posix_time::seconds( 3 ) + boost::posix_time::seconds( 3 );
}
void cGameManager::addPreUpdate( State state, std::function<void( float )> method )
{
	mPreUpdates.insert( std::make_pair( state, method ) );
}
void cGameManager::addUpdate( State state, std::function<void( float )> method )
{
	mUpdates.insert( std::make_pair( state, method ) );
}
void cGameManager::next( )
{
	flash = true;
	prevState = state;
	state = static_cast<State>( static_cast<int>( state ) + 1 );
}
void cGameManager::playBgm( const std::string& name, const float gain, const float loopbegin, const float loopend )
{
	Resource::cSoundManager::getInstance( )->findBgm( name ).setLooping( true );
	Resource::cSoundManager::getInstance( )->findBgm( name ).setGain( gain );
	Resource::cSoundManager::getInstance( )->findBgm( name ).play( );
}
void cGameManager::stopBgm( const std::string& name )
{
	Resource::cSoundManager::getInstance( )->findBgm( name ).stop( );
}
bool cGameManager::timeEmpty( )
{
	return shiftSeconds.find( state ) == shiftSeconds.end( );
}
}
