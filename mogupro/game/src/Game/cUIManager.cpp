#include <Game/cUIManager.h>
#include <Node/renderer.hpp>
#include <boost/date_time.hpp>
#include <cinder/gl/gl.h>
#include <Node/action.hpp>
#include <Game/cPlayerManager.h>
#include <Game/cGameManager.h>
using namespace ci;
namespace Game
{
class lambertCube : public Node::node
{
public:
	CREATE_H( lambertCube )
	{
		CREATE( lambertCube );
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
void cUIManager::awake( )
{
	redCannonPower = 0;
	blueCannonPower = 0;

	mRoot = Node::node::create( );
	mRoot->set_content_size( app::getWindowSize( ) - ivec2( 100, 100 ) );
	mRoot->set_schedule_update( );
	mRoot->set_scale( vec2( 1, -1 ) );
	mRoot->set_position( mRoot->get_content_size( ) * vec2( -0.5F, 0.5F ) );

	mDisableSlot = mRoot->get_content_size( ) * vec2( 1, 0 ) + vec2( 0, -300 );
	mEnableSlot = mRoot->get_content_size( ) * vec2( 1, 0 ) + vec2( 0, 0 );

	//プレイヤーがダメージを受けた時の画面の周りの光
	mPlayerScreenEffect = mRoot->add_child( Node::node::create( ) );
	auto damage_effect = mPlayerScreenEffect->add_child( Node::Renderer::sprite::create( "Player/screeneffect.png" ) );
	damage_effect->set_name( "player_screen_effect" );
	damage_effect->set_position( mRoot->get_content_size( ) * vec2( 0.5F ) );

	mTime = mRoot->add_child( Node::Renderer::rect::create( vec2( 200, 60 ) ) );
	mTime->set_anchor_point( vec2( 0.0F, 0.0F ) );
	auto l = mTime->add_child( Node::Renderer::letter::create( "AMEMUCHIGOTHIC-06.ttf", 50.0F ) );
	l->set_space( -15.0F );
	l->set_name( "letter" );
	l->set_text( "12:34" );
	l->set_color( ColorA( 0, 0, 0 ) );

	mLive = mRoot->add_child( Node::node::create( ) );
	mLive->set_position( mRoot->get_content_size( ) * vec2( 0.5F, 0.0F ) );

	mSlot = mRoot->add_child( Node::node::create( ) );
	mSlot->set_schedule_update( );
	mSlot->set_position( mDisableSlot );
	auto mCapsuleGauge = Node::Renderer::sprite::create( "itemGauge.png" );
	mCapsuleGauge->set_anchor_point( vec2( 1, 0 ) );
	mCapsuleGauge->set_pivot( vec2( 0, 0 ) );
	mCapsule = mSlot->add_child( Node::node::create( ) );
	mCapsule->set_content_size( mCapsuleGauge->get_content_size( ) );
	mCapsule->set_anchor_point( mCapsuleGauge->get_anchor_point( ) );
	mCapsule->set_pivot( mCapsuleGauge->get_pivot( ) );
	{
		auto c1 = mCapsule->add_child( Node::node::create( ) );
		c1->set_position( vec2( 182, 47 ) );
		c1->set_scale( vec2( 35.0F ) );
		c1->set_name( "subSlot" );
		auto c2 = mCapsule->add_child( Node::node::create( ) );
		c2->set_position( vec2( 87, 123 ) );
		c2->set_scale( vec2( 62.0F ) );
		c2->set_name( "mainSlot" );
	}
	mSlot->add_child( mCapsuleGauge );

	mRedTeamCannonPower = mRoot->add_child( Node::Renderer::rect::create( vec2( 30, 300 ) ) );
	mRedTeamCannonPower->set_anchor_point( vec2( 0.5F, 0.0F ) );
	mRedTeamCannonPower->set_pivot( vec2( 0.5F, 1.0F ) );
	mRedTeamCannonPower->set_position( mRoot->get_content_size( ) * vec2( 0, 0.5F ) + vec2( mRedTeamCannonPower->get_content_size( ).x / 2.0F,
																							-mRedTeamCannonPower->get_content_size( ).y / 2.0F ) );
	{
		auto g = mRedTeamCannonPower->add_child( Node::Renderer::rect::create( mRedTeamCannonPower->get_content_size( ) ) );
		g->set_name( "gauge" );
		g->set_scale( vec2( 1.0F, 0.0F ) );
		g->set_color( ColorA( 0.8, 0.2, 0.2 ) );
		g->set_anchor_point( vec2( 0.5F, 1.0F ) );
	}

	mBlueTeamCannonPower = mRoot->add_child( Node::Renderer::rect::create( vec2( 30, 300 ) ) );
	mBlueTeamCannonPower->set_anchor_point( vec2( 0.5F, 0.0F ) );
	mBlueTeamCannonPower->set_pivot( vec2( 0.5F, 1.0F ) );
	mBlueTeamCannonPower->set_position( mRoot->get_content_size( ) * vec2( 1, 0.5F ) + vec2( -mBlueTeamCannonPower->get_content_size( ).x / 2.0F,
																							 -mBlueTeamCannonPower->get_content_size( ).y / 2.0F ) );
	{
		auto g = mBlueTeamCannonPower->add_child( Node::Renderer::rect::create( mBlueTeamCannonPower->get_content_size( ) ) );
		g->set_name( "gauge" );
		g->set_scale( vec2( 1.0F, 0.0F ) );
		g->set_color( ColorA( 0.2, 0.2, 0.8 ) );
		g->set_anchor_point( vec2( 0.5F, 1.0F ) );
	}
}
void cUIManager::setup( )
{
	int offset = cPlayerManager::getInstance( )->getPlayers( ).size( ) / 2 * -1;
	for ( auto player : cPlayerManager::getInstance( )->getPlayers( ) )
	{
		auto ikiteru = mLive->add_child( Node::Renderer::sprite::create( "ikiteru.png" ) );
		ikiteru->set_name( "ikiteru" + std::to_string( player->getPlayerId( ) ) );
		ikiteru->set_anchor_point( vec2( 0.5F, 0 ) );
		ikiteru->set_position( ikiteru->get_content_size( ) * vec2( offset, 0.0F ) );
		auto yarareta = mLive->add_child( Node::Renderer::sprite::create( "yarareta.png" ) );
		yarareta->set_name( "yarareta" + std::to_string( player->getPlayerId( ) ) );
		yarareta->set_anchor_point( vec2( 0.5F, 0 ) );
		yarareta->set_position( yarareta->get_content_size( ) * vec2( offset, 0.0F ) );
		yarareta->set_visible( false );
		if ( ++offset == 0 ) offset++;
	}
	if ( cPlayerManager::getInstance( )->getActivePlayerTeamId( ) == Player::Blue )
	{
		auto tmp = mBlueTeamCannonPower->get_position( );
		mBlueTeamCannonPower->set_position( mRedTeamCannonPower->get_position( ) );
		mRedTeamCannonPower->set_position( tmp );
	}
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
		mLive->get_child_by_name( "ikiteru" + std::to_string( player->getPlayerId( ) ) )->set_visible( !player->isDead( ) );
		mLive->get_child_by_name( "yarareta" + std::to_string( player->getPlayerId( ) ) )->set_visible( player->isDead( ) );
	}

	//プレイヤーがダメージを受けた時の画面の周りの光
	int player_hp = cPlayerManager::getInstance( )->getActivePlayer( )->getStatus( ).hp;
	mPlayerScreenEffect->get_child_by_name( "player_screen_effect" )->set_color( ci::ColorA( 1, 1, 1, 1 - ( player_hp / 100 ) ) );

}
void cUIManager::draw( )
{
	mRoot->entry_render( mat4( ) );
}
void cUIManager::addRedCannonPower( int value )
{
	auto g = mRedTeamCannonPower->get_child_by_name( "gauge" );
	auto scale = g->get_scale( );
	redCannonPower += value;
	scale.y = redCannonPower / 100.0F;
	scale.y = clamp( scale.y, 0.0F, 1.0F );
	g->set_scale( scale );
}
void cUIManager::addBlueCannonPower( int value )
{
	auto g = mBlueTeamCannonPower->get_child_by_name( "gauge" );
	auto scale = g->get_scale( );
	blueCannonPower += value;
	scale.y = blueCannonPower / 100.0F;
	scale.y = clamp( scale.y, 0.0F, 1.0F );
	g->set_scale( scale );
}
void cUIManager::appendItem( int type )
{
	auto mainSlot = mCapsule->get_child_by_name( "mainSlot" );
	auto subSlot = mCapsule->get_child_by_name( "subSlot" );
	if ( mainSlot->get_children( ).empty( ) )
	{
		mainSlot->add_child( lambertCube::create( ) );
		mSlot->remove_all_actions( );
		mSlot->run_action( Node::Action::ease<cinder::EaseOutCubic>::create( Node::Action::move_to::create( 1.0F, mEnableSlot ) ) );
	}
	else if ( subSlot->get_children( ).empty( ) )
	{
		subSlot->add_child( lambertCube::create( ) );
	}
}
int cUIManager::winTeam( )
{
	return ( redCannonPower > blueCannonPower ) ? Game::Player::Red : Game::Player::Blue;
}
void cUIManager::useItem( )
{
	auto mainSlot = mCapsule->get_child_by_name( "mainSlot" );
	auto subSlot = mCapsule->get_child_by_name( "subSlot" );
	if ( !mainSlot->get_children( ).empty( ) )
	{
		mainSlot->remove_all_children( );
		if ( !subSlot->get_children( ).empty( ) )
		{
			subSlot->get_children( ).front( )->set_parent( mainSlot );
		}
		else
		{
			mSlot->remove_all_actions( );
			mSlot->run_action( Node::Action::sequence::create( Node::Action::delay::create( 1.0F ),
															   Node::Action::ease<cinder::EaseOutCubic>::create( Node::Action::move_to::create( 1.0F, mDisableSlot ) ) ) );
		}
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
