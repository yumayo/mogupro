#include <Scene/Member/cResult.h>
#include <Game/cGameManager.h>
#include <Node/renderer.hpp>
#include <Game/Player/cPlayer.h>
#include <CameraManager/cCameraManager.h>
#include <Game/Field/FieldData.h>
#include <Utility/cInput.h>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cTitle.h>
#include <Resource/cImageManager.h>
#include <Game/cPlayerManager.h>
using namespace Node::Renderer;
using namespace cinder;
namespace Scene
{
namespace Member
{
using namespace cinder;
void cResult::setup( )
{
	root = Node::node::create( );
	root->set_schedule_update( );
	root->set_content_size( app::getWindowSize( ) - ivec2( 100, 100 ) );
	root->set_scale( vec2( 1, -1 ) );
	root->set_position( root->get_content_size( ) * vec2( -0.5F, 0.5F ) );

	ColorA winTeamColor, loseTeamColor;
	switch ( Game::cGameManager::getInstance( )->winTeam( ) )
	{
	case Game::Player::Red:
		winTeamColor = ColorA( 1, 0, 0 );
		loseTeamColor = ColorA( 0, 0, 1 );
		break;
	case Game::Player::Blue:
		winTeamColor = ColorA( 0, 0, 1 );
		loseTeamColor = ColorA( 1, 0, 0 );
		break;
	default:
		break;
	}

	auto win = root->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( u8"result/win_board.png" ) ) );
	win->set_color( winTeamColor );
	win->set_position( root->get_content_size( ) * vec2( 1, 0 ) );
	win->set_anchor_point( vec2( 1, 0 ) );
	win->set_pivot( vec2( 0, 0 ) );
	auto winOffset = Game::cGameManager::getInstance( )->winTeam( ) == Game::Player::Red ? 0 : 4;
	auto winKillData = Game::cGameManager::getInstance( )->winTeam( ) == Game::Player::Red ? Game::cGameManager::getInstance( )->redTeamKillNum( ) : Game::cGameManager::getInstance( )->blueTeamKillNum( );
	auto winDeathData = Game::cGameManager::getInstance( )->winTeam( ) == Game::Player::Red ? Game::cGameManager::getInstance( )->redTeamDeathNum( ) : Game::cGameManager::getInstance( )->blueTeamDeathNum( );
	auto winAppendGemData = Game::cGameManager::getInstance( )->winTeam( ) == Game::Player::Red ? Game::cGameManager::getInstance( )->redTeamAppendGemNum( ) : Game::cGameManager::getInstance( )->blueTeamAppendGemNum( );
	for ( int i = 0; i < 3; ++i )
	{
		auto scr = win->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( u8"result/bar.png" ) ) );
		scr->set_anchor_point( vec2( 0, 0 ) );
		scr->set_pivot( vec2( 0, 0 ) );
		scr->set_position( vec2( 17, 138 + i * 70 ) );

		auto gem = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32 ) );
		gem->set_text( std::to_string( winAppendGemData[i + winOffset] ) );
		gem->set_anchor_point( vec2( 0, 0 ) );
		gem->set_position( vec2( 403, 12 ) );

		auto kill = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 18 ) );
		kill->set_text( std::to_string( winKillData[i + winOffset] ) );
		kill->set_anchor_point( vec2( 0, 0 ) );
		kill->set_position( vec2( 551, 8 ) );

		auto death = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 18 ) );
		death->set_text( std::to_string( winDeathData[i + winOffset] ) );
		death->set_anchor_point( vec2( 0, 0 ) );
		death->set_position( vec2( 551, 34 ) );

		auto name = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 36 ) );
		name->set_text( u8"‚à‚®‚ç" + std::to_string( i + winOffset ) );
		name->set_anchor_point( vec2( 0, 0 ) );
		name->set_position( vec2( 93, 12 ) );
	}

	auto lose = root->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( u8"result/lose_board.png" ) ) );
	lose->set_position( root->get_content_size( )* vec2( 1, 1 ) );
	lose->set_color( loseTeamColor );
	lose->set_anchor_point( vec2( 1, 1 ) );
	lose->set_pivot( vec2( 0, 0 ) );
	auto loseOffset = Game::cGameManager::getInstance( )->winTeam( ) == Game::Player::Red ? 4 : 0;
	auto loseKillData = Game::cGameManager::getInstance( )->winTeam( ) == Game::Player::Red ? Game::cGameManager::getInstance( )->blueTeamKillNum( ) : Game::cGameManager::getInstance( )->redTeamKillNum( );
	auto loseDeathData = Game::cGameManager::getInstance( )->winTeam( ) == Game::Player::Red ? Game::cGameManager::getInstance( )->blueTeamDeathNum( ) : Game::cGameManager::getInstance( )->redTeamDeathNum( );
	auto loseAppendGemData = Game::cGameManager::getInstance( )->winTeam( ) == Game::Player::Red ? Game::cGameManager::getInstance( )->blueTeamAppendGemNum( ) : Game::cGameManager::getInstance( )->redTeamAppendGemNum( );
	for ( int i = 0; i < 3; ++i )
	{
		auto scr = lose->add_child( Node::Renderer::sprite::create( Resource::cImageManager::getInstance( )->find( u8"result/bar.png" ) ) );
		scr->set_anchor_point( vec2( 0, 0 ) );
		scr->set_pivot( vec2( 0, 0 ) );
		scr->set_position( vec2( 17, 138 + i * 70 ) );

		auto gem = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32 ) );
		gem->set_text( std::to_string( loseAppendGemData[i + loseOffset] ) );
		gem->set_anchor_point( vec2( 0, 0 ) );
		gem->set_position( vec2( 403, 12 ) );

		auto kill = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 18 ) );
		kill->set_text( std::to_string( loseKillData[i + loseOffset] ) );
		kill->set_anchor_point( vec2( 0, 0 ) );
		kill->set_position( vec2( 551, 8 ) );

		auto death = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 18 ) );
		death->set_text( std::to_string( loseDeathData[i + loseOffset] ) );
		death->set_anchor_point( vec2( 0, 0 ) );
		death->set_position( vec2( 551, 34 ) );

		auto name = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 36 ) );
		name->set_text( u8"‚à‚®‚ç" + std::to_string( i + loseOffset ) );
		name->set_anchor_point( vec2( 0, 0 ) );
		name->set_position( vec2( 93, 12 ) );
	}
}
void cResult::shutDown( )
{
}
void cResult::resize( )
{
}
void cResult::update( float deltaTime )
{
	CAMERA->refPosition = Game::Field::WORLD_SIZE * cinder::vec3( 0.5F, 2.5F, 0.5F );
	CAMERA->setCameraAngle( cinder::vec2( -glm::pi<float>( ) / 2.0F, -glm::pi<float>( ) ) );
	root->entry_update( deltaTime );
	if ( ENV->pushKey( ) )
	{
		Scene::cSceneManager::getInstance( )->shift<Scene::Member::cTitle>( );
	}
}
void cResult::draw( )
{
}
void cResult::draw2D( )
{
	root->entry_render( ci::mat4( ) );
}
}
}
