#include <Game/UI/cTips.h>
#include <Resource/cImageManager.h>
#include <Node/renderer.hpp>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cFieldManager.h>
#include <Game/Weapons/MainWeapon/cLightSaber.h>
namespace Game
{
namespace UI
{
using namespace cinder;
bool cTips::init( cinder::vec2 baseContentSize, Player::Team team )
{
	set_schedule_update( );

	const auto messageBox = this->add_child( Node::Renderer::sprite::create( Resource::IMAGE[team == Player::Team::Red ? "in_game/mes6_03_b.png" : "in_game/mes6_05_b.png"] ) );
	messageBox->set_position( baseContentSize * ci::vec2( 0.5F, 0.8F ) );
	messageBox->set_pivot( vec2(0, 0 ) );
	message = messageBox->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 48 ) );
	message->set_anchor_point( vec2(0, 0) );

	dig = messageBox->add_child(Node::Renderer::sprite::create(Resource::IMAGE["in_game/dig.png"] ) );
	dig->set_position(vec2(86, 25));
	dig->set_anchor_point(vec2(0));
	dig->set_visible(false);

	attack = messageBox->add_child(Node::Renderer::sprite::create(Resource::IMAGE["in_game/attack.png"]));
	attack->set_position(vec2(86, 25));
	attack->set_anchor_point(vec2(0));
	attack->set_visible(false);

	block = messageBox->add_child(Node::Renderer::sprite::create(Resource::IMAGE["in_game/block.png"]));
	block->set_position(vec2(361, 25));
	block->set_anchor_point(vec2(0));
	block->set_visible(false);

	gem = messageBox->add_child(Node::Renderer::sprite::create(Resource::IMAGE["in_game/gem.png"]));
	gem->set_position(vec2(361, 25));
	gem->set_anchor_point(vec2(0));
	gem->set_visible(false);

	STATE_GENERATE( mStateMachine, idle );
	STATE_GENERATE( mStateMachine, nearBlock );
	STATE_GENERATE( mStateMachine, nearGem );
	STATE_GENERATE( mStateMachine, transGem );
	STATE_GENERATE( mStateMachine, nice );

	auto gemHoldFunc = [](auto)
	{
		return !cPlayerManager::getInstance()->getActivePlayer()->getgems.empty();
	};
	auto isHitGemFunc = []
	{
		if (auto lightSaber = dynamic_cast<Weapons::MainWeapon::cLightSaber*>(
			cPlayerManager::getInstance()->getActivePlayer()->getMainWeapon()))
		{
			if (lightSaber->isHitGem())
			{
				return true;
			}
		}
		return false;
	};

	// 特に何もない時の動作
	idle->onStateIn = [ this ] ( auto n )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"" );
		message->set_position(vec2(0));
	};
	idle->join( nearBlock, [ ] ( auto n )
	{
		return cFieldManager::getInstance( )->isBreakBlock( cPlayerManager::getInstance( )->getActivePlayer( )->getPos( ) + ( glm::normalize( CAMERA->getCamera( ).getViewDirection( ) ) * ci::vec3( cPlayerManager::getInstance( )->getActivePlayer( )->getStatus( ).drill_speed / 3 ) ), 1 );
	} );
	idle->join(transGem, gemHoldFunc);
	// ～特に何もない時の動作

	// 近くに掘れるブロックがあった時の動作
	nearBlock->onStateIn = [ this ] ( auto n )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"　　　で地面　　　を掘れるぞ" );
		message->set_position(vec2(50, 70));
		dig->set_visible(true);
		block->set_visible(true);
	};
	nearBlock->onStateOut = [this]
	{
		dig->set_visible(false);
		block->set_visible(false);
	};
	nearBlock->join( nearGem, [isHitGemFunc] ( auto n )
	{
		return isHitGemFunc();
	} );
	nearBlock->join(transGem, gemHoldFunc);
	// ～近くに掘れるブロックがあった時の動作

	// 近くに採れるジェムがあった時の動作
	nearGem->onStateIn = [ this ] ( auto n )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"　　　で宝石　　　を採れるぞ" );
		message->set_position(vec2(50, 72));
		attack->set_visible(true);
		gem->set_visible(true);
	};
	nearGem->onStateOut = [this]
	{
		attack->set_visible(false);
		gem->set_visible(false);
	};
	nearGem->join( nearBlock, [isHitGemFunc] ( auto n )
	{ 
		return !isHitGemFunc();
	} );
	nearGem->join( transGem, gemHoldFunc);
	// ～近くに採れるジェムがあった時の動作

	// ジェムを持っていた時の動作
	transGem->onStateIn = [ this ] ( auto m )
	{
		message.dynamicptr<Node::Renderer::label>( )->set_text( u8"宝石を大砲に持って帰ろう" );
		message->set_position(vec2(120, 60));
	};
	transGem->onStateOut = [ this ] ( )
	{
		this->remove_child_by_name( "hintTargetCannon" );
	};
	transGem->join( nice, [ ] ( auto n ) 
	{
		return cPlayerManager::getInstance( )->getActivePlayer( )->getgems.empty( );
	} );
	// ～ジェムを持っていた時の動作

	// ジェムを大砲に納めた時の動作
	nice->onStateIn = [this](auto m)
	{
		message.dynamicptr<Node::Renderer::label>()->set_text(u8"よくやった");
		message->set_position(vec2(260, 60));
		message.dynamicptr<Node::Renderer::label>()->run_action(Node::Action::sequence::create(Node::Action::delay::create(1.5F), Node::Action::call_func::create([this]
		{
			mYokuyatta = true;
		})));
	};
	nice->join(idle, [this](auto) 
	{ 
		return mYokuyatta;
	});
	nice->onStateOut = [this]
	{
		mYokuyatta = false;
	};
	// ～ジェムを大砲に納めた時の動作

	mStateMachine.setEntryNode( idle );

	return true;
}
void cTips::update( float delta )
{
	mStateMachine.update( delta );
}
}
}
