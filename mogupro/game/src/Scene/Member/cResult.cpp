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

	auto* gm = Game::cGameManager::getInstance( );

	{
		std::vector<std::string> redPlayerName;
		bool win = gm->winTeam( ) == Game::Player::Red;
		for ( int i = 0; i < 3; ++i )
		{
			redPlayerName.emplace_back( u8"‚à‚®‚ç" + std::to_string( i ) );
		}
		auto n = root->add_child( createScoreBoard( Game::Player::Red,
													gm->winTeam( ) == Game::Player::Red,
													redPlayerName,
													gm->getRedTeamAppendGemData( ),
													gm->getRedTeamKillData( ),
													gm->getRedTeamDeathData( ) ) );
		n->set_position( root->get_content_size( ) * ( win ? vec2( 1, 0 ) : vec2( 1, 1 ) ) );
		n->set_anchor_point( win ? vec2( 1, 0 ) : vec2( 1, 1 ) );
	}
	{
		std::vector<std::string> bluePlayerName;
		bool win = gm->winTeam( ) == Game::Player::Blue;
		for ( int i = 4; i < 7; ++i )
		{
			bluePlayerName.emplace_back( u8"‚à‚®‚ç" + std::to_string( i ) );
		}
		auto n = root->add_child( createScoreBoard( Game::Player::Blue,
													gm->winTeam( ) == Game::Player::Blue,
													bluePlayerName,
													gm->getBlueTeamAppendGemData( ),
													gm->getBlueTeamKillData( ),
													gm->getBlueTeamDeathData( ) ) );
		n->set_position( root->get_content_size( ) * ( win ? vec2( 1, 0 ) : vec2( 1, 1 ) ) );
		n->set_anchor_point( win ? vec2( 1, 0 ) : vec2( 1, 1 ) );
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
hardptr<Node::node> cResult::createScoreBoard( int team, bool win, std::vector<std::string> playerNameData, std::vector<int> pointData, std::vector<int> killData, std::vector<int> deathData )
{
	auto board = Node::Renderer::sprite::create( Resource::IMAGE[win ? "result/win_board.png" : "result/lose_board.png"] );
	board->set_color( team == Game::Player::Red ? ColorA( 1, 0, 0 ) : ColorA( 0, 0, 1 ) );
	board->set_pivot( vec2( 0, 0 ) );
	for ( int i = 0; i < 3; ++i )
	{
		auto scr = board->add_child( Node::Renderer::sprite::create( Resource::IMAGE["result/bar.png"] ) );
		scr->set_anchor_point( vec2( 0, 0 ) );
		scr->set_pivot( vec2( 0, 0 ) );
		scr->set_position( vec2( 17, 138 + i * 70 ) );

		auto gem = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 32 ) );
		gem->set_text( std::to_string( pointData[i] ) );
		gem->set_anchor_point( vec2( 0, 0 ) );
		gem->set_position( vec2( 403, 12 ) );

		auto kill = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 18 ) );
		kill->set_text( std::to_string( killData[i] ) );
		kill->set_anchor_point( vec2( 0, 0 ) );
		kill->set_position( vec2( 551, 8 ) );

		auto death = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 18 ) );
		death->set_text( std::to_string( deathData[i] ) );
		death->set_anchor_point( vec2( 0, 0 ) );
		death->set_position( vec2( 551, 34 ) );

		auto name = scr->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 36 ) );
		name->set_text( playerNameData[i] );
		name->set_anchor_point( vec2( 0, 0 ) );
		name->set_position( vec2( 93, 12 ) );
	}
	return board;
}
}
}
