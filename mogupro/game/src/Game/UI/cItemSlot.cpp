#include <Game/UI/cItemSlot.h>
#include <Resource/cImageManager.h>
#include <Node/renderer.hpp>
using namespace cinder;
namespace Game
{
namespace UI
{
bool cItemSlot::init( cinder::vec2 contentSize )
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
}
}
