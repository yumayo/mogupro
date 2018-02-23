#pragma once
#include <Node/node.h>
#include <Node/action.hpp>
#include <cinder/gl/gl.h>
#include <Resource/cObjectManager.h>
namespace Game
{
namespace UI
{
class cItemLightBomb : public Node::node
{
public:
	CREATE_H( cItemLightBomb )
	{
		CREATE( cItemLightBomb );
	}
	bool init( )
	{
		auto mesh = Resource::cObjectManager::getInstance()->findMesh("bombred.obj");
		cinder::AxisAlignedBox aabb( mesh->calcBoundingBox() );
		set_content_size_3d(aabb.getSize());
		set_anchor_point_3d( cinder::vec3( 0.0F, 0.6F, 0.0F ) );
		set_pivot_3d( cinder::vec3( 0.5F ) );
		set_axis( cinder::vec3( 0, 1, -1 ) );
		set_scale_3d( cinder::vec3(1.0F / 32.0F) );

		using namespace Node::Action;
		run_action( repeat_forever::create( rotate_by::create( 1.0F, M_PI ) ) );

		return true;
	}
	void render( ) override
	{
		cinder::gl::ScopedDepth scpDepth( true );
		cinder::gl::ScopedColor col( cinder::Color( 1, 1, 1 ) );
		cinder::gl::ScopedGlslProg glsl(cinder::gl::getStockShader(cinder::gl::ShaderDef().texture()));
		cinder::gl::ScopedTextureBind tex(Resource::IMAGE["in_game/bombred.png"]);
		cinder::gl::draw( Resource::cObjectManager::getInstance( )->findObject( "bombred.obj" ) );
	}
};
}
}
