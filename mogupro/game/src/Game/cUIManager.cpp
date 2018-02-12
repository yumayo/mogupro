#include <Game/cUIManager.h>
#include <Node/renderer.hpp>
#include <boost/date_time.hpp>
#include <cinder/gl/gl.h>
#include <Node/action.hpp>
#include <Game/cPlayerManager.h>
#include <Game/cGameManager.h>
#include <Resource/cImageManager.h>
#include <Resource/cObjectManager.h>
#include <Game/UI/cItemUIGenerator.h>
#include <Game/UI/cCannonMeter.h>
#include <Game/UI/cItemSlot.h>
#include <Game/UI/cPlayerNameUIs.h>
#include <Game/UI/cTips.h>
#include <Scene/cSceneManager.h>
#include <Scene/Member/Tutorial.h>
#include <Game/UI/cTargetCannon.h>
#include <Game/UI/cTimer.h>
#include <cinder/Rand.h>
//Weapons::SubWeapon::SubWeaponType
using namespace Node::Action;
using namespace ci;
namespace Game
{
void cUIManager::awake( )
{
	// ルート作成
	mRoot = Node::node::create( );
	mRoot->set_content_size( app::getWindowSize( ) );
	mRoot->set_schedule_update( );
	mRoot->set_scale( vec2( 1, -1 ) );
	mRoot->set_position( mRoot->get_content_size( ) * vec2( -0.5F, 0.5F ) );

	// 文字生成が重い気がするのでキャッシュ。
	auto l = mRoot->add_child(Node::Renderer::label::create("AMEMUCHIGOTHIC-06.ttf", 340.0F));
	l->set_name("count_down");
	l->set_color(ColorA(1, 1, 1, 0.5F));
	l->set_position(mRoot->get_content_size() / 2.0F);

	rander.seed(20180212);

	//プレイヤーがダメージを受けた時の画面の周りの光
	mPlayerScreenEffect = mRoot->add_child( Node::node::create( ) );
	auto damage_effect = mPlayerScreenEffect->add_child( Node::Renderer::sprite::create( "Player/screeneffect.png" ) );
	damage_effect->set_name( "player_screen_effect" );
	damage_effect->set_position( mRoot->get_content_size( ) * vec2( 0.5F ) );

	mSlot = mRoot->add_child( UI::cItemSlot::create( mRoot->get_content_size( ) ) );
}
void cUIManager::setup( )
{
	auto playerTeamId = ( Player::Team )cPlayerManager::getInstance( )->getActivePlayerTeamId( );
	mRedTeamCannonMeter = mRoot->add_child( UI::cCannonMeter::create( mRoot->get_content_size( ), playerTeamId, Player::Team::Red ) );
	mBlueTeamCannonMeter = mRoot->add_child( UI::cCannonMeter::create( mRoot->get_content_size( ), playerTeamId, Player::Team::Blue ) );

	mRoot->add_child( UI::cPlayerNameUIs::create( ) );

	mTargetCannon = mRoot->add_child( UI::cTargetCannon::create( playerTeamId ) );

	if (!Scene::cSceneManager::getInstance()->isCurrentScene<Scene::Member::cTutorial>())
	{
		// タイマー作成
		auto timer = mRoot->add_child(UI::cTimer::create(mRoot->get_content_size()));
		mTimer = timer;
		timer->lastOneMinutesCall = [this]
		{
			auto tex1 = mRoot->add_child( Node::Renderer::sprite::create( Resource::IMAGE["in_game/last_one_minutes.png"] ) );
			tex1->set_position( mRoot->get_content_size() / 2.0F );
			tex1->set_color( ColorA( 1, 1, 1, 0.5F ) );
			tex1->run_action(sequence::create(delay::create(2.0F), fade_out::create(1.0F), remove_self::create()));

			auto tex2 = mRoot->add_child(Node::Renderer::sprite::create(Resource::IMAGE["in_game/last_one_minutes.png"]));
			tex2->set_position( mRoot->get_content_size() / 2.0F );
			tex2->set_color(ColorA(1, 1, 1, 0.5F));
			tex2->run_action(ease<ci::EaseOutCubic>::create( scale_by::create(1.0F, vec2(1.0F))));
			tex2->run_action(fade_out::create( 1.0F ) );
			tex2->run_action(sequence::create( delay::create(1.0F ), remove_self::create()));
		};
		timer->countDownCall = [this](auto number)
		{
			auto l = mRoot->get_child_by_name("count_down").dynamicptr<Node::Renderer::label>();
			l->set_text(number);
			l->set_rotation(rander.nextFloat(-0.2F, 0.2F));
			l->set_scale( vec2(1.0F) );
			l->run_action( scale_by::create( 0.7F, vec2(0.2F) ) );
			l->set_opacity( 0.5F );
			l->run_action(sequence::create( delay::create(0.1F), fade_out::create(0.5F)));
		};

		mTips = mRoot->add_child(UI::cTips::create(mRoot->get_content_size(), playerTeamId));
	}

	ui[mTimer] = FixedPosition{ mTimer->get_position(), mTimer->get_position() + vec2( 0, mTimer->get_content_size().y + 100 ) * -1.0F };
	ui[mTips] = FixedPosition{ mTips->get_position(), mTips->get_position() + vec2(0, mTips->get_content_size().y + 300) * 1.0F };
	ui[mSlot] = FixedPosition{ mSlot->get_position(), mSlot->get_position() + vec2(0, mSlot->get_content_size().y + 300) * -1.0F };
	ui[mRedTeamCannonMeter] = FixedPosition{ mRedTeamCannonMeter->get_position(), mRedTeamCannonMeter->get_position() + vec2(mRedTeamCannonMeter->get_content_size().x + 200, 0) * -1.0F };
	ui[mBlueTeamCannonMeter] = FixedPosition{ mBlueTeamCannonMeter->get_position(), mBlueTeamCannonMeter->get_position() + vec2(mBlueTeamCannonMeter->get_content_size().x + 200, 0) * 1.0F };

	disable();
	mRoot->entry_update(1.0F);
}
void cUIManager::update( float delta )
{
	mRoot->entry_update(delta);

	for ( auto player : cPlayerManager::getInstance( )->getPlayers( ) )
	{
		auto const pID = player->getPlayerId( );
		if ( ( pID == 3U ) || ( pID == 7U ) )
			continue;

		softptr<Node::node> target;

		if ( pID >= 0.0F && pID <= 2 ) //! red
		{
			switch ( pID )
			{
			case 0:
				target = mRedTeamCannonMeter.dynamicptr<UI::cCannonMeter>( )->player1;
				break;
			case 1:
				target = mRedTeamCannonMeter.dynamicptr<UI::cCannonMeter>( )->player2;
				break;
			case 2:
				target = mRedTeamCannonMeter.dynamicptr<UI::cCannonMeter>( )->player3;
				break;
			default:
				break;
			}
		}
		else //! blue
		{
			switch ( pID )
			{
			case 4:
				target = mBlueTeamCannonMeter.dynamicptr<UI::cCannonMeter>( )->player1;
				break;
			case 5:
				target = mBlueTeamCannonMeter.dynamicptr<UI::cCannonMeter>( )->player2;
				break;
			case 6:
				target = mBlueTeamCannonMeter.dynamicptr<UI::cCannonMeter>( )->player3;
				break;
			default:
				break;
			}
		}
		
		target->get_child_by_name( "live" )->set_visible( !player->isDead( ) );
		target->get_child_by_name( "dead" )->set_visible( player->isDead( ) );
		target->get_child_by_name( "watch" )->set_visible( cPlayerManager::getInstance( )->getWatchingTargetPlayerId( ) == player->getPlayerId( ) );
	}

	//プレイヤーがダメージを受けた時の画面の周りの光
	int player_hp = cPlayerManager::getInstance( )->getActivePlayer( )->getStatus( ).hp;
	mPlayerScreenEffect->get_child_by_name( "player_screen_effect" )->set_color( ci::ColorA( 1, 1, 1, 1 - ( player_hp / 100 ) ) );

}
void cUIManager::draw( )
{
	mRoot->entry_render( mat4( ) );
}
void cUIManager::setRedCannonPower( int value )
{
	mRedTeamCannonMeter.dynamicptr<UI::cCannonMeter>( )->addPower( value );
}
void cUIManager::setBlueCannonPower( int value )
{
	mBlueTeamCannonMeter.dynamicptr<UI::cCannonMeter>( )->addPower( value );
}
void cUIManager::setItem( boost::optional<int> currentItem, boost::optional<int> nextItem )
{
	auto capsules = mSlot.dynamicptr<UI::cItemSlot>( )->capsules;
	auto current = capsules->get_child_by_name( "current" );
	auto next = capsules->get_child_by_name( "next" );
	if ( currentItem && current->get_children( ).empty( ) )
	{
		current->add_child( UI::cItemUIGenerator::createItem( static_cast<Weapons::SubWeapon::SubWeaponType>( *currentItem ) ) );
	}
	else if ( nextItem && next->get_children( ).empty( ) )
	{
		next->add_child( UI::cItemUIGenerator::createItem( static_cast<Weapons::SubWeapon::SubWeaponType>( *nextItem ) ) );
	}
	else if ( !nextItem && !next->get_children( ).empty( ) )
	{
		current->remove_all_children( );
		next->get_children( ).front( )->set_parent( current );
	}
	else
	{
		current->remove_all_children( );
	}
}
void cUIManager::enable( )
{
	if (visible) return;
	for (auto& u : ui)
	{
		u.first->remove_all_actions();
		u.first->set_schedule_update( true );
		u.first->run_action(ease<ci::EaseOutCubic>::create(move_to::create(0.2F, u.second.enable)));
	}
	mTargetCannon->set_block_visible( false );
	visible = true;
}
void cUIManager::disable( )
{
	if (!visible) return;
	for (auto& u : ui)
	{
		u.first->remove_all_actions();
		u.first->set_schedule_update( false );
		u.first->run_action(ease<ci::EaseOutCubic>::create(move_to::create(0.2F, u.second.disable)));
	}
	mTargetCannon->set_block_visible( true );
	visible = false;
}
}
