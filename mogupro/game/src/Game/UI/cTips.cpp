#include <Game/UI/cTips.h>
#include <Resource/cImageManager.h>
#include <Node/renderer.hpp>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cFieldManager.h>
namespace Game
{
namespace UI
{
class cTargetCannon : public Node::Renderer::sprite
{
public:
	CREATE_H( cTargetCannon )
	{
		CREATE( cTargetCannon );
	}
	bool init( )
	{
		return __super::init( Resource::IMAGE["in_game/allow.png"] );
	}
	void update( float delta ) override
	{
		auto& camera = CAMERA->getCamera( );
		auto& cannons = cStrategyManager::getInstance( )->getCannons( );
		auto& cannon = cannons.at( cPlayerManager::getInstance( )->getActivePlayerTeamId( ) );
		auto targetPos = cannon->getGemStorePos( );
		targetPos.y = Field::WORLD_SIZE.y;

		ci::vec3 targetVec = targetPos - cPlayerManager::getInstance( )->getActivePlayer( )->getPos( );
		targetVec = ci::normalize( targetVec );
		ci::vec3 cameraVec = camera.getViewDirection( );
		cameraVec = ci::normalize( cameraVec );

		targetPos = targetVec + cPlayerManager::getInstance( )->getActivePlayer( )->getPos( );

		auto screenPos = camera.worldToScreen( targetPos, ci::app::getWindowWidth( ), ci::app::getWindowHeight( ) );

		set_position( screenPos );

		ci::vec2 vec = screenPos - ci::app::getWindowCenter( );
		float angle = atan2( vec.y, vec.x );
		set_rotation( angle );
	}
};
bool cTips::init( cinder::vec2 baseContentSize, Player::Team team )
{
	const auto messageBox = this->add_child( Node::Renderer::sprite::create( Resource::IMAGE[team == Player::Team::Red ? "in_game/mes6_03_b.png" : "in_game/mes6_05_b.png"] ) );
	messageBox->set_position( baseContentSize * ci::vec2( 0.5F, 0.8F ) );
	message = messageBox->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 48 ) );
	set_schedule_update( );

	using namespace Utility;

	STATE_GENERATE( mStateMachine, idle );
	STATE_GENERATE( mStateMachine, nearBlock );
	STATE_GENERATE( mStateMachine, nearGem );
	STATE_GENERATE( mStateMachine, transGem );
	STATE_GENERATE( mStateMachine, nice );

	idle->onStateIn = [ this ] ( auto n )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"" );
	};
	idle->join( nearBlock, [ ] ( auto n )
	{
		return cFieldManager::getInstance( )->isBreakBlock( cPlayerManager::getInstance( )->getActivePlayer( )->getPos( ) + ( glm::normalize( CAMERA->getCamera( ).getViewDirection( ) ) * ci::vec3( cPlayerManager::getInstance( )->getActivePlayer( )->getStatus( ).drill_speed / 3 ) ), 1 );
	} );

	nearBlock->onStateIn = [ this ] ( auto n )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"左クリックで地面を掘れるぞ" );
	};
	nearBlock->join( nearGem, [ ] ( auto n ) 
	{
		bool isHit = false;
		auto const& gems = cGemManager::getInstance( )->getGemStones( );
		for ( auto& gem : gems )
		{
			auto pos = cPlayerManager::getInstance( )->getActivePlayer( )->getPos( ) + glm::normalize( CAMERA->getCamera( ).getViewDirection( ) * ci::vec3( 1, 0, 1 ) ) * 1.5F;
			if ( glm::distance( pos, gem->getPos( ) ) < 2.0F )
			{
				isHit = true;
				break;
			}
		}
		return isHit;
	} );

	nearGem->onStateIn = [ this ] ( auto n )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"右クリックで宝石を採れるぞ" );
	};
	nearGem->join( nearBlock, [ ] ( auto n )
	{ 
		bool isHit = false;
		auto const& gems = cGemManager::getInstance( )->getGemStones( );
		for ( auto& gem : gems )
		{
			auto pos = cPlayerManager::getInstance( )->getActivePlayer( )->getPos( ) + glm::normalize( CAMERA->getCamera( ).getViewDirection( ) * ci::vec3( 1, 0, 1 ) ) * 1.5F;
			if ( glm::distance( pos, gem->getPos( ) ) < 2.0F )
			{
				isHit = true;
				break;
			}
		}
		return !isHit;
	} );
	nearGem->join( transGem, [ ] ( auto n )
	{ 
		return !cPlayerManager::getInstance( )->getActivePlayer( )->getgems.empty( );
	} );

	transGem->onStateIn = [ this ] ( auto m )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"宝石を大砲に持って帰ろう" );
		auto hintTargetCannon = this->add_child( cTargetCannon::create( ) );
		hintTargetCannon->set_name( "hintTargetCannon" );
		hintTargetCannon->set_schedule_update( );
	};
	transGem->onStateOut = [ this ] ( )
	{
		this->remove_child_by_name( "hintTargetCannon" );
	};
	transGem->join( nice, [ ] ( auto n ) 
	{
		return cPlayerManager::getInstance( )->getActivePlayer( )->getgems.empty( );
	} );

	nice->onStateIn = [ this ] ( auto m )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"よくやった" );
		message.dynamicptr<Node::Renderer::label>( )->run_action( Node::Action::sequence::create( Node::Action::delay::create( 1.5F ), Node::Action::call_func::create( [ this ] { message.dynamicptr<Node::Renderer::label>( )->set_name( "animationFinished" ); } ) ) );
	};
	nice->onStateOut = [ this ] ( )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_name( u8"" );
	};
	nice->join( idle, [ this ] ( auto n ) 
	{
		return message.dynamicptr<Node::Renderer::label>( )->get_name( ) == "animationFinished";
	} );

	mStateMachine.setEntryNode( idle );

	return true;
}
void cTips::update( float delta )
{
	mStateMachine.update( delta );
}
}
}
