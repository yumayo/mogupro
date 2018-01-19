#pragma once
#include <Node/node.h>
#include <Node/action.hpp>
#include <cinder/gl/gl.h>
namespace Game
{
namespace UI
{
class cItemDefault : public Node::node
{
public:
	CREATE_H( cItemDefault )
	{
		CREATE( cItemDefault );
	}
	bool init( )
	{
		set_anchor_point_3d( cinder::vec3( 0.5F ) );
		set_pivot_3d( cinder::vec3( 0.5F ) );
		set_axis( cinder::vec3( 0, 1, -1 ) );

		using namespace Node::Action;
		run_action( repeat_forever::create( rotate_by::create( 1.0F, M_PI ) ) );

		return true;
	}
	void render( ) override
	{
		cinder::gl::ScopedDepth scpDepth( true );
		cinder::gl::ScopedColor col( cinder::Color( 1, 0, 0 ) );
		cinder::gl::drawColorCube( cinder::vec3( 0 ), cinder::vec3( 1 ) );
	}
};
}
}
