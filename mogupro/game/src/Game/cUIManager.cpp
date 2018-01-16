#include <Game/cUIManager.h>
#include <Node/renderer.hpp>
#include <boost/date_time.hpp>
#include <cinder/gl/gl.h>
#include <Node/action.hpp>
#include <Game/cPlayerManager.h>
#include <Game/cGameManager.h>
#include <Resource/cImageManager.h>
#include <Resource/cObjectManager.h>
#include <Game/Weapons/SubWeapon/SubWeaponType.h>
//Weapons::SubWeapon::SubWeaponType
using namespace ci;
namespace Game
{
class ItemDefault : public Node::node
{
public:
	CREATE_H( ItemDefault )
	{
		CREATE( ItemDefault );
	}
	bool init( )
	{
		set_anchor_point_3d( vec3( 0.5F ) );
		set_pivot_3d( vec3( 0.5F ) );
		set_axis( vec3( 0, 1, -1 ) );

		using namespace Node::Action;
		run_action( repeat_forever::create( rotate_by::create( 1.0F, M_PI ) ) );

		return true;
	}
	void render( ) override
	{
		gl::ScopedDepth scpDepth( true );
		gl::ScopedColor col( Color( 1, 0, 0 ) );
		gl::drawColorCube( vec3( 0 ), vec3( 1 ) );
	}
};
class ItemQuarry : public Node::node 
{
public:
	CREATE_H( ItemQuarry )
	{
		CREATE( ItemQuarry );
	}
	bool init( )
	{
		set_anchor_point_3d( vec3( 0.5F ) );
		set_pivot_3d( vec3( 0.5F ) );
		set_axis( vec3( 0, 1, -1 ) );

		using namespace Node::Action;
		run_action( repeat_forever::create( rotate_by::create( 1.0F, M_PI ) ) );

		return true;
	}
	void render( ) override
	{
		gl::ScopedDepth scpDepth( true );
		gl::ScopedColor col( Color( 1, 1, 1 ) );
		gl::ScopedGlslProg glsl( gl::getStockShader( gl::ShaderDef().texture() ) );
		gl::ScopedTextureBind texture( Resource::IMAGE["in_game/quarry.png"] );
		gl::drawCube( vec3( 0 ), vec3( 1 ) );
	}
};
class ItemLightBomb : public Node::node
{
public:
	CREATE_H( ItemLightBomb )
	{
		CREATE( ItemLightBomb );
	}
	bool init( )
	{
		set_anchor_point_3d( vec3( 0.5F ) );
		set_pivot_3d( vec3( 0.5F ) );
		set_axis( vec3( 0, 1, -1 ) );

		using namespace Node::Action;
		run_action( repeat_forever::create( rotate_by::create( 1.0F, M_PI ) ) );

		return true;
	}
	void render( ) override
	{
		gl::ScopedDepth scpDepth( true );
		gl::ScopedColor col( Color( 1, 0, 0 ) );
		gl::draw( Resource::cObjectManager::getInstance()->findObject( "sphere.obj" ) );
	}
};
hardptr<Node::node> createItem( Weapons::SubWeapon::SubWeaponType type )
{
	switch ( type )
	{
	case Game::Weapons::SubWeapon::LIGHT_BOMB:
		return ItemLightBomb::create( );
		break;
	case Game::Weapons::SubWeapon::QUARRY:
		return ItemQuarry::create( );
		break;
	default:
		return ItemDefault::create( );
		break;
	}
}
class cannonMeter : public Node::node
{
public:
	CREATE_H( cannonMeter, vec2 contentSize, Player::Team playerTeam, Player::Team myTeam )
	{
		CREATE( cannonMeter, contentSize, playerTeam, myTeam );
	}
	bool init( vec2 contentSize, Player::Team playerTeam, Player::Team myTeam )
	{
		this->myTeam = myTeam;
		this->playerTeam = playerTeam;

		auto gauge = Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( "gameMainUI/meter_gauge.png" ) );
		this->set_scale( vec2( myTeam == playerTeam ? 1.0F : 0.75F ) );

		auto rectSize = vec2( 82, 572 );

		meter = this->add_child( Node::Renderer::rect::create( rectSize ) );
		meter->set_color( myTeam == Player::Team::Red ? ColorA( 0.8, 0.2, 0.2 ) : ColorA( 0.2, 0.2, 0.8 ) );

		this->set_content_size( gauge->get_content_size( ) );
		this->set_anchor_point( vec2( myTeam == playerTeam ? 0 : 1, 1 ) );

		this->set_position( contentSize * vec2( myTeam == playerTeam ? 0 : 1, 1.0F ) );
		
		auto rectStartPos = vec2( 47, 86 );

		meter->set_position( rectStartPos + meter->get_content_size( ) * vec2( 0.5F, 1.0F ) );
		meter->set_anchor_point( vec2( 0.5F, 1.0F ) );
		meter->set_scale( vec2( 1.0F, 0.0F ) );

		auto analogMeter = this->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance()->find( "gameMainUI/meter.png" ) ) );
		analogMeter->set_position( vec2( 84, 701 ) );
		line = this->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( "gameMainUI/meter_line.png" ) ) );
		line->set_position( vec2( 84, 731 ) );

		player1 = gauge->add_child( Node::node::create( ) );
		player1->set_position( vec2( 134, 404 + 70 * 0 ) );
		{
			auto live = player1->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( myTeam == Player::Team::Red ?
																														   "gameMainUI/live_red.png" : "gameMainUI/live_blue.png" ) ) );
			live->set_anchor_point( vec2( 0, 0 ) );
			live->set_name( "live" );
			auto dead = player1->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( myTeam == Player::Team::Red ?
																														   "gameMainUI/dead_red.png" : "gameMainUI/dead_blue.png" ) ) );
			dead->set_anchor_point( vec2( 0, 0 ) );
			dead->set_name( "dead" );
			auto watch = player1->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( "gameMainUI/watch.png" ) ) );
			watch->set_position( vec2( 135, 49 ) );
			watch->set_name( "watch" );
		}
		
		player2 = gauge->add_child( Node::node::create( ) );
		player2->set_position( vec2( 134, 404 + 70 * 1 ) );
		{
			auto live = player2->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( myTeam == Player::Team::Red ?
																														   "gameMainUI/live_red.png" : "gameMainUI/live_blue.png" ) ) );
			live->set_anchor_point( vec2( 0, 0 ) );
			live->set_name( "live" );
			auto dead = player2->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( myTeam == Player::Team::Red ?
																														   "gameMainUI/dead_red.png" : "gameMainUI/dead_blue.png" ) ) );
			dead->set_anchor_point( vec2( 0, 0 ) );
			dead->set_name( "dead" );
			auto watch = player2->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( "gameMainUI/watch.png" ) ) );
			watch->set_position( vec2( 135, 49 ) );
			watch->set_name( "watch" );
		}

		player3 = gauge->add_child( Node::node::create( ) );
		player3->set_position( vec2( 134, 404 + 70 * 2 ) );
		{
			auto live = player3->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( myTeam == Player::Team::Red ?
																														   "gameMainUI/live_red.png" : "gameMainUI/live_blue.png" ) ) );
			live->set_anchor_point( vec2( 0, 0 ) );
			live->set_name( "live" );
			auto dead = player3->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( myTeam == Player::Team::Red ?
																														   "gameMainUI/dead_red.png" : "gameMainUI/dead_blue.png" ) ) );
			dead->set_anchor_point( vec2( 0, 0 ) );
			dead->set_name( "dead" );
			auto watch = player3->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( "gameMainUI/watch.png" ) ) );
			watch->set_position( vec2( 135, 49 ) );
			watch->set_name( "watch" );
		}

		this->add_child( gauge );
		gauge->set_position( gauge->get_content_size( ) * vec2( 0.5F, 0.5F ) );
		gauge->set_pivot( vec2( 0, 0 ) );
		gauge->set_scale( vec2( myTeam == playerTeam ? 1.0F : -1.0F, 1.0F ) );

		return true;
	}
	void addPower( int value )
	{
		power += value;

		float scale = std::min( 1.0F, power / 100.0F );
		meter->remove_all_actions( );
		meter->run_action( Node::Action::ease<EaseOutCubic>::create( Node::Action::scale_to::create( 1.5F, vec2( 1.0F, scale ) ) ) );

		float maxDegree = 93.28F;

		auto degree = Node::Action::action::ease_liner( scale, 0.0F, maxDegree );
		line->remove_all_actions( );
		line->run_action( Node::Action::ease<EaseOutCubic>::create( Node::Action::rotate_to::create( 1.5F, cinder::toRadians( degree ) ) ) );
	}
	softptr<Node::node> meter;
	Player::Team playerTeam;
	Player::Team myTeam;
	softptr<Node::node> line;
	softptr<Node::node> player1;
	softptr<Node::node> player2;
	softptr<Node::node> player3;
	int power = 0;
};
class itemSlot : public Node::node
{
public:
	CREATE_H( itemSlot, vec2 contentSize )
	{
		CREATE( itemSlot, contentSize );
	}
	bool init( vec2 contentSize )
	{
		this->set_schedule_update( );
		this->set_position( contentSize * vec2( 1, 0 ) + vec2( 0, 0 ) );

		auto capsuleGauge = Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( "gameMainUI/item_gauge.png" ) );
		capsuleGauge->set_anchor_point( vec2( 1, 0 ) );

		auto capsuleBase = this->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( "gameMainUI/item_base.png" ) ) );
		capsuleBase->set_anchor_point( capsuleGauge->get_anchor_point( ) );

		capsules = this->add_child( Node::node::create( ) );
		capsules->set_content_size( capsuleGauge->get_content_size( ) );
		capsules->set_anchor_point( capsuleGauge->get_anchor_point( ) );
		capsules->set_pivot( vec2( 0, 0 ) );
		{
			auto c1 = capsules->add_child( Node::node::create( ) );
			c1->set_position( vec2( 258, 151 ) );
			c1->set_scale( vec2( 35.0F ) );
			c1->set_name( "next" );
			auto c2 = capsules->add_child( Node::node::create( ) );
			c2->set_position( vec2( 113, 92 ) );
			c2->set_scale( vec2( 62.0F ) );
			c2->set_name( "current" );
		}

		this->add_child( capsuleGauge );

		return true;
	}
	softptr<Node::node> capsules;
};
void cUIManager::awake( )
{
	// ルート作成
	mRoot = Node::node::create( );
	mRoot->set_content_size( app::getWindowSize( ) );
	mRoot->set_schedule_update( );
	mRoot->set_scale( vec2( 1, -1 ) );
	mRoot->set_position( mRoot->get_content_size( ) * vec2( -0.5F, 0.5F ) );

	//プレイヤーがダメージを受けた時の画面の周りの光
	mPlayerScreenEffect = mRoot->add_child( Node::node::create( ) );
	auto damage_effect = mPlayerScreenEffect->add_child( Node::Renderer::sprite::create( "Player/screeneffect.png" ) );
	damage_effect->set_name( "player_screen_effect" );
	damage_effect->set_position( mRoot->get_content_size( ) * vec2( 0.5F ) );

	// タイマー作成
	mTime = mRoot->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( "gameMainUI/timer.png" ) ) );
	mTime->set_anchor_point( vec2( 0.5F, 0.0F ) );
	mTime->set_position( mRoot->get_content_size( ) * vec2( 0.5F, 0.0F ) );
	mTime->set_pivot( vec2( 0.5F, 0.125F ) );
	auto l = mTime->add_child( Node::Renderer::letter::create( "AMEMUCHIGOTHIC-06.ttf", 36.0F ) );
	l->set_space( -9.0F );
	l->set_name( "letter" );
	l->set_text( "12:34" );
	l->set_color( ColorA( 0, 0, 0 ) );

	mSlot = mRoot->add_child( itemSlot::create( mRoot->get_content_size( ) ) );
}
void cUIManager::setup( )
{
	auto playerTeamId = ( Player::Team )cPlayerManager::getInstance( )->getActivePlayerTeamId( );
	mRedTeamCannonMeter = mRoot->add_child( cannonMeter::create( mRoot->get_content_size( ), playerTeamId, Player::Team::Red ) );
	mBlueTeamCannonMeter = mRoot->add_child( cannonMeter::create( mRoot->get_content_size( ), playerTeamId, Player::Team::Blue ) );
}
void cUIManager::update( float delta )
{
	mRoot->entry_update( delta );

	{
		auto l = mTime->get_child_by_name( "letter" ).dynamicptr<Node::Renderer::letter>( );
		l->set_text( cGameManager::getInstance( )->getLeftBattleTime( ) );
	}

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
				target = mRedTeamCannonMeter.dynamicptr<cannonMeter>( )->player1;
				break;
			case 1:
				target = mRedTeamCannonMeter.dynamicptr<cannonMeter>( )->player2;
				break;
			case 2:
				target = mRedTeamCannonMeter.dynamicptr<cannonMeter>( )->player3;
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
				target = mBlueTeamCannonMeter.dynamicptr<cannonMeter>( )->player1;
				break;
			case 5:
				target = mBlueTeamCannonMeter.dynamicptr<cannonMeter>( )->player2;
				break;
			case 6:
				target = mBlueTeamCannonMeter.dynamicptr<cannonMeter>( )->player3;
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
	mRedTeamCannonMeter.dynamicptr<cannonMeter>( )->addPower( value );
}
void cUIManager::setBlueCannonPower( int value )
{
	mBlueTeamCannonMeter.dynamicptr<cannonMeter>( )->addPower( value );
}
void cUIManager::setItem( boost::optional<int> currentItem, boost::optional<int> nextItem )
{
	auto capsules = mSlot.dynamicptr<itemSlot>( )->capsules;
	auto current = capsules->get_child_by_name( "current" );
	auto next = capsules->get_child_by_name( "next" );
	if ( currentItem && current->get_children( ).empty( ) )
	{
		current->add_child( createItem( static_cast<Weapons::SubWeapon::SubWeaponType>( *currentItem ) ) );
	}
	else if ( nextItem && next->get_children( ).empty( ) )
	{
		next->add_child( createItem( static_cast<Weapons::SubWeapon::SubWeaponType>( *nextItem ) ) );
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
	mRoot->set_block_visible( false );
}
void cUIManager::disable( )
{
	mRoot->set_block_visible( true );
}
}
