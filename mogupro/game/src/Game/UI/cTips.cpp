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
	return true;
}
void cTips::update( float delta )
{
	const auto act_p = cPlayerManager::getInstance()->getActivePlayer( );

	//! 地面を掘る時のTIPS表示
	if ( cFieldManager::getInstance( )->isBreakBlock( act_p->getPos( ) + ( glm::normalize( CAMERA->getCamera( ).getViewDirection( ) ) * ci::vec3( act_p->getStatus( ).drill_speed / 3 ) ), 1 ) )
	{
		if ( !mHintNearBlock )
		{
			mHintNearBlock = true;
			if ( !mHintNearGemStone && !mHintTransportGem )
			{
				message.dynamicptr<Node::Renderer::label>( )->set_text( u8"左クリックで地面を掘れるぞ" );
			}
		}
	}
	//! 宝石を採れる時のTIPS表示
	static int i = 0; i++; 
	if ( i % 5 != 0 )
	{
		bool isHit = false;
		auto const& gems = cGemManager::getInstance( )->getGemStones( );
		for ( auto& gem : gems )
		{
			auto pos = act_p->getPos( ) + glm::normalize( CAMERA->getCamera( ).getViewDirection( ) * ci::vec3( 1, 0, 1 ) ) * 3.0F;
			if ( glm::distance( pos, gem->getPos( ) ) < 2.0F )
			{
				isHit = true;
				break;
			}
		}
		if ( isHit )
		{
			if ( !mHintNearGemStone && !mHintTransportGem )
			{
				mHintNearGemStone = true;
				message.dynamicptr<Node::Renderer::label>( )->set_text( u8"右クリックで宝石を採れるぞ" );
			}
		}
		else if( !mHintTransportGem )
		{
			mHintNearGemStone = false;
			mHintNearBlock = false;
		}
	}
	//! 宝石を大砲に持って帰った時のTIPS表示
	if ( mHintTransportGem && act_p->getgems.empty( ) )
	{
		mHintTransportGem = false;
		mHintNearBlock = false;
		mHintNearGemStone = false;
		this->remove_child_by_name( "hintTargetCannon" );
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"よくやった" );
		message->run_action( Node::Action::sequence::create( Node::Action::delay::create( 1.5F ) ) );
	}
	//! 宝石を大砲に持って帰る時のTIPS表示
	if ( !act_p->getgems.empty( ) )
	{
		if ( !mHintTransportGem )
		{
			mHintTransportGem = true;
			message.dynamicptr<Node::Renderer::label>( )->set_text( u8"宝石を大砲に持って帰ろう" );
			auto hintTargetCannon = this->add_child( cTargetCannon::create( ) );
			hintTargetCannon->set_name( "hintTargetCannon" );
			hintTargetCannon->set_schedule_update( );
		}
	}
}
}
}
