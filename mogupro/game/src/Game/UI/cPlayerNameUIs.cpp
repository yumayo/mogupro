#include <Game/UI/cPlayerNameUIs.h>
#include <Node/renderer.hpp>
#include <Game/cPlayerManager.h>
#include <CameraManager/cCameraManager.h>
namespace Game
{
namespace UI
{
bool cPlayerNameUIs::init( )
{
	for ( auto& it : cPlayerManager::getInstance( )->getPlayers( ) )
	{
		if ( cPlayerManager::getInstance( )->getActivePlayerTeamId( ) != it->getWhichTeam( ) ) continue;
		if ( it->isWatching( ) ) continue;

		auto l = this->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32.0F ) );
		ci::Frustum fru( CAMERA->getCamera( ) );
		auto aabb = it->getAABB( );
		l->set_block_visible( !fru.intersects( aabb ) );
		auto pos2D = CAMERA->getCamera( ).worldToScreen( it->getPos( ) + ci::vec3( 0, aabb.getSize( ).y / 2.0F, 0 ), cinder::app::getWindowWidth( ), cinder::app::getWindowHeight( ) );
		l->set_position( pos2D + ci::vec2( 5 ) );
		l->set_tag( it->getPlayerId( ) );
		l->set_text( u8"‚à‚®‚ç" + std::to_string( it->getPlayerId( ) ) );
		l->set_color( ci::ColorA( 0, 0, 0, 1 ) );

		auto instance = l->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32.0F ) );
		instance->set_text( u8"‚à‚®‚ç" + std::to_string( it->getPlayerId( ) ) );
		instance->set_position( ci::vec2( -5 ) );
	}
	set_schedule_update( );
	return true;
}
void cPlayerNameUIs::update( float delta )
{
	this->sort_children( [ this ] ( hardptr<Node::node>& a, hardptr<Node::node>& b )
	{
		auto depthA = CAMERA->getCamera( ).worldToEyeDepth( cPlayerManager::getInstance( )->getPlayer( a->get_tag( ) )->getPos( ) );
		auto depthB = CAMERA->getCamera( ).worldToEyeDepth( cPlayerManager::getInstance( )->getPlayer( b->get_tag( ) )->getPos( ) );
		return depthA < depthB;
	} );

	for ( auto& it : cPlayerManager::getInstance( )->getPlayers( ) )
	{
		if ( cPlayerManager::getInstance( )->getActivePlayerTeamId( ) != it->getWhichTeam( ) ) continue;
		if ( it->isWatching( ) ) continue;
		auto p = this->get_child_by_tag( it->getPlayerId( ) );
		ci::Frustum fru( CAMERA->getCamera( ) );
		auto aabb = it->getAABB( );
		p->set_block_visible( !fru.intersects( aabb ) );
		auto pos2D = CAMERA->getCamera( ).worldToScreen( it->getPos( ) + ci::vec3( 0, aabb.getSize( ).y / 2.0F, 0 ), cinder::app::getWindowWidth( ), cinder::app::getWindowHeight( ) );
		p->set_position( pos2D + ci::vec2( 5 ) );
	}
}
}
}
