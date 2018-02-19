#include <Scene/Member/cBegin.h>
#include <Resource/cObjectManager.h>
#include <Resource/cSoundManager.h>
#include <Resource/cImageManager.h>
#include <Resource/TextureManager.h>
#include <Resource/cJsonManager.h>
#include <Resource/cFbxManager.h>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cTitle.h>
#include <Sound/cALContextManager.h>
#include <Node/renderer.hpp>
#include <CameraManager/cCameraManager.h>
namespace Scene
{
namespace Member
{
void cBegin::setup( )
{
	CameraManager::cCameraManager::getInstance( )->setup( );

	Sound::cALContextManager::getInstance( );

	Resource::cJsonManager::getInstance( );

	Resource::cObjectManager::getInstance( );
	Resource::cImageManager::getInstance( );
	Resource::cSoundManager::getInstance( );
	Resource::cFbxManager::getInstance( );

	root = Node::node::create( );
	root->set_content_size( ci::app::getWindowSize( ) );
	root->set_scale( ci::vec2( 1, -1 ) );
	root->set_position( root->get_content_size( ) / 2.0F * ci::vec2( -1, 1 ) );

	root->add_child( Node::Renderer::sprite::create( "IMAGE/loading/logo.png" ) )->set_position( root->get_content_size( ) * ci::vec2( 0.5F ) );

	loadBar = root->add_child( Node::node::create( ) );
	loadBar->set_position( root->get_content_size( ) * ci::vec2( 0.5F, 0.8F ) );
	auto base = loadBar->add_child( Node::Renderer::rect::create( ci::vec2( 1000, 30 ) ) );
	auto bar = loadBar->add_child( Node::Renderer::rect::create( ci::vec2( 1000, 30 ) ) );
	bar->set_name( "bar" );
	bar->set_color( ci::ColorA( 1, 0, 0, 1 ) );
	bar->set_position( ci::vec2(-500, 0) );
	bar->set_anchor_point( ci::vec2( 0.0F, 0.5F) );
	auto edge = loadBar->add_child( Node::Renderer::rect_edge::create( ci::vec2( 1000, 30 ) ) );
}
void cBegin::shutDown( )
{
}
void cBegin::update( float t )
{
	float maxSum = Resource::cObjectManager::getInstance( )->maxNum( ) +
		Resource::cImageManager::getInstance( )->maxNum( ) +
		Resource::cSoundManager::getInstance( )->maxNum( ) +
		Resource::cFbxManager::getInstance( )->maxNum( );
	float currentSum = Resource::cObjectManager::getInstance( )->currentNum( ) +
		Resource::cImageManager::getInstance( )->currentNum( ) +
		Resource::cSoundManager::getInstance( )->currentNum( ) +
		Resource::cFbxManager::getInstance( )->currentNum( );

	auto bar = loadBar->get_child_by_name( "bar" );
	bar->set_scale( ci::vec2( currentSum / maxSum, 1.0F ) );

	if ( !Resource::cObjectManager::getInstance( )->isFinished( ) )
	{
		Resource::cObjectManager::getInstance( )->loadOne( );
	}
	else if ( !Resource::cImageManager::getInstance( )->isFinished( ) )
	{
		Resource::cImageManager::getInstance( )->loadOne( );
	}
	else if ( !Resource::cSoundManager::getInstance( )->isFinished( ) )
	{
		Resource::cSoundManager::getInstance( )->loadOne( );
	}
	else if ( !Resource::cFbxManager::getInstance( )->isFinished( ) )
	{
		Resource::cFbxManager::getInstance( )->loadOne( );
	}
	else
	{
		cSceneManager::getInstance( )->shift<cTitle>( );
	}
}
void cBegin::draw( )
{
}
void cBegin::draw2D( )
{
	root->entry_render( ci::mat4( ) );
}
void cBegin::resize( )
{
}
}
}
