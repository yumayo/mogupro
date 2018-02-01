#pragma once
#include <Node/node.h>
#include <cinder/gl/Texture.h>
#include <cinder/gl/VboMesh.h>
namespace Node
{
namespace Renderer
{
class obj3d : public Node::node
{
public:
	virtual ~obj3d( ) { }
	CREATE_H( obj3d, cinder::TriMeshRef mesh_handle, cinder::gl::VboMeshRef object_handle, cinder::gl::TextureRef texture_handle );
	bool init( cinder::TriMeshRef mesh_handle, cinder::gl::VboMeshRef object_handle, cinder::gl::TextureRef texture_handle );
	virtual void render( ) override;
protected:
	cinder::gl::TextureRef _texture;
	cinder::TriMeshRef _triMesh;
	cinder::gl::VboMeshRef _vboMesh;
};
}
}
