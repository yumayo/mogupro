#include <Node/Renderer/obj3d.h>
#include <cinder/gl/gl.h>
namespace Node
{
namespace Renderer
{
CREATE_CPP( obj3d, cinder::TriMeshRef mesh_handle, cinder::gl::VboMeshRef object_handle, cinder::gl::TextureRef texture_handle )
{
	CREATE( obj3d, mesh_handle, object_handle, texture_handle );
}
bool obj3d::init( cinder::TriMeshRef mesh_handle, cinder::gl::VboMeshRef object_handle, cinder::gl::TextureRef texture_handle )
{
	set_anchor_point_3d( { 0.5F, 0.5F, 0.5F } );
	set_pivot_3d( { 0.5F, 0.5F, 0.5F } );

	_triMesh = mesh_handle;
	_vboMesh = object_handle;
	_texture = texture_handle;

	auto aabb = _triMesh->calcBoundingBox( );
	set_content_size_3d( aabb.getSize( ) );

	return true;
}
void obj3d::render( )
{
	cinder::gl::ScopedTextureBind tex( _texture );
	cinder::gl::draw( _vboMesh );
}
}
}
