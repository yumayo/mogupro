#include <Game/UI/cCannonMeter.h>
#include <Node/renderer.hpp>
#include <Resource/cImageManager.h>
using namespace cinder;
namespace Game
{
namespace UI
{
bool cCannonMeter::init( cinder::vec2 contentSize, Player::Team playerTeam, Player::Team myTeam )
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

	auto analogMeter = this->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( "gameMainUI/meter.png" ) ) );
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
void cCannonMeter::addPower( int value )
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
}
}
