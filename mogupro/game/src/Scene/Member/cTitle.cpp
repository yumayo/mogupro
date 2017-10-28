#include <Scene/Member/cTitle.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <CameraManager/cCameraManager.h>
#include <Utility/cInput.h>
#include <Scene/Member/cGameMain.h>
#include <Scene/Member/cMatching.h>
#include <Scene/Member/cMatchingServer.h>
#include <Scene/cSceneManager.h>
#include <Network.hpp>
using namespace Node::Action;
namespace Scene
{
namespace Member
{
cTitle::cTitle( )
{

}
cTitle::~cTitle( )
{

}
void cTitle::setup( )
{
    mCamPos = ci::vec3( 0 );
    CAMERA->followingCamera( &mCamPos, 100 );
    CAMERA->setup( );

    mSelectTag = 0;

    mRoot = Node::node::create( );
    mRoot->set_schedule_update( );

    auto toMatching = Node::Renderer::rect::create( ci::vec2( 200, 200 ) );
    toMatching->set_position( ci::vec2( -250, 0 ) );
    toMatching->set_color( ci::ColorA( 1, 0, 0 ) );
    toMatching->set_tag( 0 );
    toMatching->set_schedule_update( );
    mRoot->add_child( toMatching );
    {
        auto f = Node::Renderer::label::create( "sawarabi-gothic-medium.ttf", 32 );
        f->set_text( u8"マッチング" );
        f->set_scale( glm::vec2( 1, -1 ) );
        toMatching->add_child( f );
    }
    mScenes.emplace_back( [ ] { cSceneManager::getInstance( )->shift<Scene::Member::cMatching>( ); } );

    auto toMatchingServer = Node::Renderer::rect::create( ci::vec2( 200, 200 ) );
    toMatchingServer->set_position( ci::vec2( 0, 0 ) );
    toMatchingServer->set_color( ci::ColorA( 0, 1, 0 ) );
    toMatchingServer->set_tag( 1 );
    toMatchingServer->set_schedule_update( );
    mRoot->add_child( toMatchingServer );
    {
        auto f = Node::Renderer::label::create( "sawarabi-gothic-medium.ttf", 32 );
        f->set_text( u8"マッチングサーバー" );
        f->set_scale( glm::vec2( 1, -1 ) );
        toMatchingServer->add_child( f );
    }
    mScenes.emplace_back( [ ] { cSceneManager::getInstance( )->shift<Scene::Member::cMatchingServer>( ); } );

    auto toGameMain = Node::Renderer::rect::create( ci::vec2( 200, 200 ) );
    toGameMain->set_position( ci::vec2( 250, 0 ) );
    toGameMain->set_color( ci::ColorA( 0, 0, 1 ) );
    toGameMain->set_tag( 2 );
    toGameMain->set_schedule_update( );
    mRoot->add_child( toGameMain );
    {
        auto f = Node::Renderer::label::create( "sawarabi-gothic-medium.ttf", 32 );
        f->set_text( u8"ゲームメイン" );
        f->set_scale( glm::vec2( 1, -1 ) );
        toGameMain->add_child( f );
    }
    mScenes.emplace_back( [ ] { 
		Network::cUDPClientManager::getInstance()->open();
		Network::cUDPServerManager::getInstance()->open();
		Network::cUDPClientManager::getInstance()->connectOfflineServer();
		cSceneManager::getInstance( )->shift<Scene::Member::cGameMain>( ); 
	} );

    mRoot->get_child_by_tag( mSelectTag )->run_action(
        repeat_forever::create(
            sequence::create(
                ease<ci::EaseInOutCirc>::create( scale_by::create( 0.26F, ci::vec2( 0.2F ) ) ),
                ease<ci::EaseInOutCirc>::create( scale_by::create( 0.26F, ci::vec2( -0.2F ) ) )
            )
        )
    );
}
void cTitle::shutDown( )
{
}
void cTitle::resize( )
{
}
void cTitle::update( float deltaTime )
{
    int prevSelectTag = mSelectTag;
    if ( ENV->pushKey( ci::app::KeyEvent::KEY_RIGHT ) )
    {
        mSelectTag = std::min( mSelectTag + 1, 2 );
    }
    if ( ENV->pushKey( ci::app::KeyEvent::KEY_LEFT ) )
    {
        mSelectTag = std::max( mSelectTag - 1, 0 );
    }

    if ( ENV->pushKey( ci::app::KeyEvent::KEY_RETURN ) )
    {
        mScenes[mSelectTag]( );
        return;
    }

    if ( prevSelectTag != mSelectTag )
    {
        mRoot->get_child_by_tag( prevSelectTag )->remove_all_actions( );
        mRoot->get_child_by_tag( prevSelectTag )->set_scale( ci::vec2( 1.0F ) );
        mRoot->get_child_by_tag( mSelectTag )->run_action(
            repeat_forever::create(
                sequence::create(
                    ease<ci::EaseInOutCirc>::create( scale_by::create( 0.26F, ci::vec2( 0.2F ) ) ),
                    ease<ci::EaseInOutCirc>::create( scale_by::create( 0.26F, ci::vec2( -0.2F ) ) )
                )
            )
        );
    }

    mRoot->entry_update( deltaTime );
}
void cTitle::draw( )
{
}
void cTitle::draw2D( )
{
    ci::gl::enableFaceCulling( false );
    ci::gl::disableDepthRead( );
    ci::gl::disableDepthWrite( );
    mRoot->entry_render( cinder::mat4( ) );
}
}
}
