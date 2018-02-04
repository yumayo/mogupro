#include <Game/cGameManager.h>
#include <Network/cUDPClientManager.h>
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
#include <Sound/Wav.h>
#include <Game/cResultManager.h>
using namespace cinder;
using namespace Node::Action;
namespace Game
{
cGameManager::cGameManager( )
{
	root = Node::node::create( );
	root->set_schedule_update( );
	root->set_content_size( cinder::app::getWindowSize( ) );
	root->set_scale( cinder::vec2( 1, -1 ) );
	root->set_position( root->get_content_size( ) * cinder::vec2( -0.5F, 0.5F ) );

	auto bgm = Sound::Wav( cinder::app::getAssetDirectories().front().string() + "/BGM/testbgm2.wav" );
	introloopBGM.create( bgm.data( ), bgm.size( ), 9.63499F, 69.45829F );
	introloopBGM.gain( 0.3F );

	STATE_GENERATE( sMac, load );
	STATE_GENERATE( sMac, my_team );
	STATE_GENERATE( sMac, enemy_team );
	STATE_GENERATE( sMac, ready );
	STATE_GENERATE( sMac, battle );
	STATE_GENERATE( sMac, battle_end );
	STATE_GENERATE( sMac, result );

	load->join( my_team, [ this ] ( auto n )
	{
		return Network::cUDPClientManager::getInstance( )->getServerTime( ) > allUserloadFinishedTime;
	} );
	load->onStateIn = [ this ] ( auto m )
	{
		auto cameraPosition = root->add_child( Node::node::create( ) );
		cameraPosition->set_schedule_update( );
		cameraPosition->set_position_3d( Game::Field::WORLD_SIZE * cinder::vec3( 0.4F, 1.0F, 0.4F ) + cinder::vec3( 0, 1.0F, 0 ) );
		cameraPosition->run_action( move_by::create( 15.0F, cinder::vec3( 5.0F, 3.0F, 7.0F ) ) );
		cameraPosition->set_name( "cameraPosition" );

		auto cameraAngle = root->add_child( Node::node::create( ) );
		cameraAngle->set_schedule_update( );
		cameraAngle->set_position( cinder::vec2( -glm::pi<float>( ) / 2.0F, -0.2 ) );
		cameraAngle->run_action( move_by::create( 5.0F, cinder::vec2( -1.5F, 0.2F ) ) );
		cameraAngle->set_name( "cameraAngle" );

		auto fieldName = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
		fieldName->set_anchor_point( cinder::vec2( 1, 1 ) );
		fieldName->set_text( u8"もぐもぐコロニー" );
		fieldName->set_position( root->get_content_size( ) - cinder::vec2( 50, 50 ) );

		auto rule = root->add_child( Node::node::create( ) );
		rule->set_position( root->get_content_size( ) / 2.0F );

		auto ruleSpeech = rule->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
		ruleSpeech->set_text( u8"たくさんジェムを大砲に入れろ" );
	};
	load->onStateOut = [ this ] ( )
	{
		root->remove_all_children( );
	};
	load->onStateStay = [ this ] ( auto n )
	{
		CAMERA->refPosition = root->get_child_by_name( "cameraPosition" )->get_position_3d( );
		CAMERA->setCameraAngle( root->get_child_by_name( "cameraAngle" )->get_position( ) );
	};
	my_team->join( enemy_team, [ this ] ( auto n )
	{
		return n->time > 2.0F;
	} );
	my_team->onStateStay = [ this ] ( auto n )
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
	};
	enemy_team->join( ready, [ this ] ( auto n )
	{
		return n->time > 2.0F;
	} );
	enemy_team->onStateOut = [ this ] ( )
	{
		root->remove_all_children( );
	};
	enemy_team->onStateStay = [ this ] ( auto n )
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
	};
	ready->join( battle, [ this ] ( auto n )
	{
		return n->time > 2.0F;
	} );
	ready->onStateIn = [ this ]( auto m )
	{
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
	};
	ready->onStateOut = [ this ] ( )
	{
		root->remove_all_children( );
	};
	battle->join( battle_end, [ this ] ( auto n )
	{
		return n->time > 60.0F * 5.0F;
	} );
	battle->onStateIn = [ this ] ( auto m )
	{
		battleStartTime = Network::cUDPClientManager::getInstance( )->getServerTime( );

		ENV->enableKeyButton( );
		ENV->enablePadAxis( );
		ENV->enablePadButton( );
		ENV->enableMouseButton( );
		cUIManager::getInstance( )->enable( );

		auto go = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 128 ) );
		go->set_text( u8"GO!!" );
		go->set_position( root->get_content_size( ) / 2.0F );
		go->run_action( sequence::create( delay::create( 2.0F ), fade_out::create( 1.0F ), remove_self::create( ) ) );
		introloopBGM.play( );
	};
	battle->onStateOut = [ this ] ( )
	{
		ENV->disableMouseButton( );
		ENV->disableKeyButton( );
		ENV->disablePadButton( );
		ENV->disablePadAxis( );
	};
	battle_end->join( result, [ this ] ( auto n )
	{
		return n->time > 1.0F;
	} );
	battle_end->onStateIn = [ this ] ( auto m )
	{
		auto n = root->add_child( Node::Renderer::rect::create( cinder::app::getWindowSize( ) ) );
		n->set_color( cinder::ColorA( 0, 0, 0, 0 ) );
		n->set_position( root->get_content_size( ) / 2.0F );
		n->set_name( "battle_end_fader" );
		n->run_action( Node::Action::fade_in::create( 1.0F ) );
	};
	battle->onStateOut = [ this ] ( )
	{
		ENV->setMouseControl( false );
		ENV->enableKeyButton( );
		ENV->enableMouseButton( );
		ENV->enablePadButton( );
		ENV->enablePadAxis( );
		introloopBGM.stop( );
		cUIManager::getInstance( )->disable( );
	};
	result->onStateIn = [ this ] ( auto m )
	{
		root->get_child_by_name( "battle_end_fader" )->run_action( Node::Action::sequence::create( Node::Action::fade_out::create( 1.0F ), Node::Action::remove_self::create( ) ) );
		cResultManager::getInstance( )->setup( );
	};
	result->onStateStay = [ this ] ( auto n )
	{
		cResultManager::getInstance( )->update( delta );
	};

	sMac.setEntryNode( load );
}
cGameManager::~cGameManager( )
{
	cResultManager::removeInstance( );
}
void cGameManager::setTime( float allUserloadFinishedTime )
{
	this->allUserloadFinishedTime = allUserloadFinishedTime;
}
std::string cGameManager::getLeftBattleTime( )
{
	auto duration = ( battleStartTime + 60.0F * 5.0F ) - Network::cUDPClientManager::getInstance()->getServerTime( );
	if ( duration < 0.0F )
	{
		return "00:00";
	}
	return boost::str( boost::format( "%02d:%02d" ) % (int)( duration / 60.0F ) % (int)( std::fmodf( duration, 60.0F ) ) );
}
void cGameManager::update( float delta )
{
	this->delta = delta;

	root->entry_update( delta );
	introloopBGM.update( delta );
	sMac.update( delta );
}
void cGameManager::draw( )
{
	root->entry_render( cinder::mat4( ) );
	if ( sMac.isCurrentState( "result" ) )
	{
		cResultManager::getInstance( )->draw( );
	}
}
bool cGameManager::isInGame( )
{
	return sMac.isCurrentState( "battle" );
}
void cGameManager::addRedCannonPower( int value )
{
	redCannonPower += value;
	cUIManager::getInstance( )->setRedCannonPower( value );
}
void cGameManager::addBlueCannonPower( int value )
{
	blueCannonPower += value;
	cUIManager::getInstance( )->setBlueCannonPower( value );
}
void cGameManager::appendItem( int type )
{
	if ( nextItem )
	{
		return;
	}
	if ( currentItem )
	{
		nextItem = type;
	}
	else
	{
		currentItem = type;
	}
	cUIManager::getInstance( )->setItem( currentItem, nextItem );
}
void cGameManager::useItem( )
{
	if ( !currentItem )
	{
		return;
	}
	if(nextItem)
	{
		currentItem = nextItem;
		nextItem = boost::none;
	}
	else if ( currentItem )
	{
		currentItem = boost::none;
	}
	cUIManager::getInstance( )->setItem( currentItem, nextItem );
}
void cGameManager::kill( int playerId )
{
	killNum[playerId]++;
}
void cGameManager::death( int playerId )
{
	deathNum[playerId]++;
}
void cGameManager::appendGem( int playerId, int gemNum )
{
	appendGemNum[playerId] += gemNum;
}
std::pair<int, int> cGameManager::getResult( )
{
	return std::make_pair( redCannonPower, blueCannonPower );
}
std::map<int, int> cGameManager::redTeamKillNum( )
{
	std::map<int, int> ret;
	for ( int i = 0; i < 3; ++i )
	{
		ret[i] = killNum[i];
	}
	return ret;
}
std::map<int, int> cGameManager::redTeamDeathNum( )
{
	std::map<int, int> ret;
	for ( int i = 0; i < 3; ++i )
	{
		ret[i] = deathNum[i];
	}
	return ret;
}
std::map<int, int> cGameManager::redTeamAppendGemNum( )
{
	std::map<int, int> ret;
	for ( int i = 0; i < 3; ++i )
	{
		ret[i] = appendGemNum[i];
	}
	return ret;
}
std::map<int, int> cGameManager::blueTeamKillNum( )
{
	std::map<int, int> ret;
	for ( int i = 4; i < 7; ++i )
	{
		ret[i] = killNum[i];
	}
	return ret;
}
std::map<int, int> cGameManager::blueTeamDeathNum( )
{
	std::map<int, int> ret;
	for ( int i = 4; i < 7; ++i )
	{
		ret[i] = deathNum[i];
	}
	return ret;
}
std::map<int, int> cGameManager::blueTeamAppendGemNum( )
{
	std::map<int, int> ret;
	for ( int i = 4; i < 7; ++i )
	{
		ret[i] = appendGemNum[i];
	}
	return ret;
}
int cGameManager::winTeam( )
{
	return ( redCannonPower > blueCannonPower ) ? Game::Player::Red : Game::Player::Blue;
}
}
