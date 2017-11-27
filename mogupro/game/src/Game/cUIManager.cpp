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
	CREATE_H( lambertCube, vec3 size )
	{
		CREATE( lambertCube, size );
	}
	bool init( vec3 size )
	{
		set_content_size_3d( size );
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
		gl::drawColorCube( get_content_size_3d( ) / 2.0F, get_content_size_3d( ) );
	}
};
void cUIManager::setup( )
{
	mRoot = Node::node::create( );
	mRoot->set_content_size( app::getWindowSize( ) - ivec2( 100 ) );
	mRoot->set_anchor_point( vec2( 0.5F ) );
	mRoot->set_pivot( vec2( 0.5F ) );

	mTime = mRoot->add_child( Node::Renderer::rect::create( vec2( 150, 60 ) ) );
	mTime->set_anchor_point( vec2( 0.0F, 1.0F ) );
	mTime->set_position( vec2( -mRoot->get_content_size( ).x / 2.0F, mRoot->get_content_size( ).y / 2.0F ) );
	auto l = mTime->add_child( Node::Renderer::label::create( "sawarabi-gothic-medium.ttf", 60.0F ) );
	l->set_name( "label" );
	l->set_text( "12:34" );
	l->set_color( ColorA( 0, 0, 0 ) );
	l->set_scale( vec2( 1, -1 ) );

	mLive = mRoot->add_child( Node::node::create( ) );
	mLive->set_position( mRoot->get_content_size( ) * vec2( 0.0F, 0.5F ) - vec2(0.0F, 64.0F * 0.5F) );
	int offset = cPlayerManager::getInstance( )->getPlayers( ).size( ) / 2 * -1;
	for ( auto player : cPlayerManager::getInstance( )->getPlayers( ) )
	{
		auto ikiteru = mLive->add_child( Node::Renderer::sprite::create( "ikiteru.png" ) );
		ikiteru->set_scale( vec2( 1, -1 ) );
		ikiteru->set_name( "ikiteru" + std::to_string( player->getPlayerId( ) ) );
		ikiteru->set_position( ikiteru->get_content_size( ) * vec2( offset, 0.0F ) );
		auto yarareta = mLive->add_child( Node::Renderer::sprite::create( "yarareta.png" ) );
		yarareta->set_scale( vec2( 1, -1 ) );
		yarareta->set_name( "yarareta" + std::to_string( player->getPlayerId( ) ) );
		yarareta->set_position( yarareta->get_content_size( ) * vec2( offset, 0.0F ) );
		yarareta->set_visible( false );
		if ( ++offset == 0 ) offset++;
	}

	mCapsule1 = mRoot->add_child( Node::Renderer::circle::create( 30.0F ) );
	mCapsule1->set_anchor_point( vec2( 1.0F, 1.0F ) );
	mCapsule1->set_position( mRoot->get_content_size( ) / 2.0F - vec2( mCapsule1->get_content_size( ).x, 0.0F ) - vec2( 10.0F * 1, 0.0F ) );
	{
		auto c = mCapsule1->add_child( lambertCube::create( vec3( 30.0F ) ) );
	}

	mCapsule2 = mRoot->add_child( Node::Renderer::circle::create( 30.0F ) );
	mCapsule2->set_anchor_point( vec2( 1.0F, 1.0F ) );
	mCapsule2->set_position( mRoot->get_content_size( ) / 2.0F );
	{
		auto c = mCapsule2->add_child( lambertCube::create( vec3( 30.0F ) ) );
	}

	mMyTeamCannonPower = mRoot->add_child( Node::Renderer::rect::create( vec2( 30, 300 ) ) );
	mMyTeamCannonPower->set_anchor_point( vec2( 0.5F, 0.0F ) );
	mMyTeamCannonPower->set_pivot( vec2( 0.5F, 0.0F ) );
	mMyTeamCannonPower->set_position( vec2( -mRoot->get_content_size( ).x / 2.0F + mMyTeamCannonPower->get_content_size( ).x / 2.0F,
											-mMyTeamCannonPower->get_content_size( ).y / 2.0F ) );
	{
		auto g = mMyTeamCannonPower->add_child( Node::Renderer::rect::create( mMyTeamCannonPower->get_content_size( ) ) );
		g->set_name( "gauge" );
		g->set_scale( vec2( 1.0F, 0.0F ) );
		g->set_color( ColorA( 0.8, 0.2, 0.2 ) );
		g->set_anchor_point( vec2( 0.5F, 0.0F ) );
	}

	mEnemyTeamCannonPower = mRoot->add_child( Node::Renderer::rect::create( vec2( 30, 300 ) ) );
	mEnemyTeamCannonPower->set_anchor_point( vec2( 0.5F, 0.0F ) );
	mEnemyTeamCannonPower->set_pivot( vec2( 0.5F, 0.0F ) );
	mEnemyTeamCannonPower->set_position( vec2( mRoot->get_content_size( ).x / 2.0F - mEnemyTeamCannonPower->get_content_size( ).x / 2.0F,
											   -mEnemyTeamCannonPower->get_content_size( ).y / 2.0F ) );
	{
		auto g = mEnemyTeamCannonPower->add_child( Node::Renderer::rect::create( mEnemyTeamCannonPower->get_content_size( ) ) );
		g->set_name( "gauge" );
		g->set_scale( vec2( 1.0F, 0.0F ) );
		g->set_color( ColorA( 0.2, 0.2, 0.8 ) );
		g->set_anchor_point( vec2( 0.5F, 0.0F ) );
	}
}
void cUIManager::update( float delta )
{
	mRoot->entry_update( delta );

	{
		auto l = mTime->get_child_by_name( "label" ).dynamicptr<Node::Renderer::label>( );
		l->set_text( cGameManager::getInstance( )->getLeftBattleTime( ) );
	}

	static float myA = 1.0F; myA += delta;
	{
		auto g = mMyTeamCannonPower->get_child_by_name( "gauge" );
		g->set_scale( vec2( 1.0F, sin( myA ) * 0.5F + 0.5F ) );
	}

	static float enA = 0.0F; enA += delta;
	{
		auto g = mEnemyTeamCannonPower->get_child_by_name( "gauge" );
		g->set_scale( vec2( 1.0F, sin( enA ) * 0.5F + 0.5F ) );
	}

	for ( auto player : cPlayerManager::getInstance( )->getPlayers( ) )
	{
		mLive->get_child_by_name( "ikiteru" + std::to_string( player->getPlayerId( ) ) )->set_visible( !player->isDead( ) );
		mLive->get_child_by_name( "yarareta" + std::to_string( player->getPlayerId( ) ) )->set_visible( player->isDead( ) );
	}
}
void cUIManager::draw( )
{
	mRoot->entry_render( mat4( ) );
}
}
