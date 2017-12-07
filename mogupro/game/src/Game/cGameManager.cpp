#include <Game/cGameManager.h>
#include <boost/format.hpp>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cTitle.h>
#include <Utility/cInput.h>
#include <Game/Field/FieldData.h>
#include <CameraManager/cCameraManager.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <Game/cUIManager.h>
#include <Game/Player/cPlayer.h>
#include <Game/Field/RespawnPoint.h>
namespace pt = boost::posix_time;
namespace Game
{
cGameManager::cGameManager( )
{
	using namespace Node::Action;
	root = Node::node::create( );
	root->set_schedule_update( );
	root->set_content_size( cinder::app::getWindowSize( ) );
	root->set_scale( cinder::vec2( 1, -1 ) );
	root->set_position( root->get_content_size( ) * cinder::vec2( -0.5F, 0.5F ) );
	auto cameraTarget = root->add_child( Node::node::create( ) );
	cameraTarget->set_schedule_update( );
	cameraTarget->set_position_3d( Game::Field::WORLD_SIZE * cinder::vec3( 0.4F, 1.0F, 0.4F ) + cinder::vec3( 0, 1.0F, 0 ) );
	cameraTarget->run_action( sequence::create( move_by::create( 5.0F, cinder::vec3( 5.0F, 3.0F, 7.0F ) ),
												call_func::create( [ this ] { root->get_child_by_name( "cameraPosition" )->set_position_3d( Field::CANNON_POINT[0] ); } ),
												delay::create( 2.0F ),
												call_func::create( [ this ] { root->get_child_by_name( "cameraPosition" )->set_position_3d( Field::CANNON_POINT[1] ); } ) ) );
	cameraTarget->set_name( "cameraPosition" );
	auto cameraPosition = root->add_child( Node::node::create( ) );
	cameraPosition->set_schedule_update( );
	cameraPosition->set_position( cinder::vec2( -glm::pi<float>( ) / 2.0F, -0.2 ) );
	cameraPosition->run_action( sequence::create( move_by::create( 5.0F, cinder::vec2( -1.5F, 0.2F ) ),
												  call_func::create( [ this ] { root->get_child_by_name( "cameraAngle" )->set_position( cinder::vec2( -glm::pi<float>( ), 0 ) ); } ),
												  delay::create( 2.0F ),
												  call_func::create( [ this ] { root->get_child_by_name( "cameraAngle" )->set_position( cinder::vec2( 0, 0 ) ); } ) ) );
	cameraPosition->set_name( "cameraAngle" );
	auto fieldName = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
	fieldName->set_anchor_point( cinder::vec2( 0, 0 ) );
	fieldName->set_text( u8"もぐもぐコロニー" );
	fieldName->set_position( cinder::vec2( 50, 50 ) );
	fieldName->run_action( sequence::create( delay::create( 5.0F ), remove_self::create( ) ) );
	auto rule = root->add_child( Node::node::create( ) );
	rule->set_position( root->get_content_size( ) / 2.0F );
	rule->run_action( sequence::create( delay::create( 5.0F ), remove_self::create( ) ) );
	auto ruleSpeech = rule->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
	ruleSpeech->set_text( u8"たくさんジェムを大砲に入れろ" );
	mPreUpdates.insert( std::make_pair( State::STAND_BY, [ this ] ( float t )
	{
		if ( shiftSeconds[State::READY] < boost::posix_time::microsec_clock::local_time( ) )
		{
			shift( State::READY );
			ENV->setMouseControl( true );
			auto ready = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 128 ) );
			ready->set_text( u8"ready" );
			ready->set_position( root->get_content_size( ) / 2.0F );
			ready->run_action( sequence::create( delay::create( 2.0F ), remove_self::create( ) ) );
		}
	} ) );
	mPreUpdates.insert( std::make_pair( State::READY, [ this ] ( float t )
	{
		if ( shiftSeconds[State::BATTLE] < boost::posix_time::microsec_clock::local_time( ) )
		{
			shift( State::BATTLE );
			ENV->enableKeyWithMouseButton( );
			cUIManager::getInstance( )->enable( );
			auto go = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 128 ) );
			go->set_text( u8"GO!!" );
			go->set_position( root->get_content_size( ) / 2.0F );
			go->run_action( sequence::create( delay::create( 2.0F ), fade_out::create( 1.0F ), remove_self::create( ) ) );
		}
	} ) );
	mPreUpdates.insert( std::make_pair( State::BATTLE, [ this ] ( float t )
	{
		if ( shiftSeconds[State::BATTLE_END] < boost::posix_time::microsec_clock::local_time( ) )
		{
			shift( State::BATTLE_END );
			ENV->disableKeyWithMouseButton( );
		}
	} ) );
	mPreUpdates.insert( std::make_pair( State::BATTLE_END, [ this ] ( float t )
	{
		if ( shiftSeconds[State::RESULT] < boost::posix_time::microsec_clock::local_time( ) )
		{
			shiftResult( );
		}
		if ( isShiftResult )
		{
			if ( root->get_child_by_name( "fader" ) ) return;
			auto n = root->add_child( Node::Renderer::rect::create( cinder::app::getWindowSize( ) ) );
			n->set_color( cinder::ColorA( 0, 0, 0, 0 ) );
			n->set_name( "fader" );
			n->set_position( root->get_content_size( ) / 2.0F );
			n->run_action( Node::Action::sequence::create( Node::Action::fade_in::create( 1.0F ), Node::Action::call_func::create( [ this ] {
				shift( State::RESULT );
				ENV->enableKeyWithMouseButton( );
				cUIManager::getInstance( )->disable( );
				root->get_child_by_name( "fader" )->run_action( Node::Action::sequence::create( Node::Action::fade_out::create( 1.0F ), Node::Action::remove_self::create( ) ) );
				auto label = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
				label->set_position( root->get_content_size( ) / 2.0F );
				auto point = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
				point->set_position( root->get_content_size( ) / 2.0F + cinder::vec2( 0, 100 ) );
				auto p = cUIManager::getInstance( )->result( );
				switch ( cUIManager::getInstance( )->winTeam( ) )
				{
				case Game::Player::Red:
					label->set_text( "red team win" );
					point->set_text( "red: " + std::to_string( p.x ) + " <----> blue: " + std::to_string( p.y ) );
					break;
				case Game::Player::Blue:
					label->set_text( "blue team win" );
					point->set_text( "blue: " + std::to_string( p.y ) + " <----> red: " + std::to_string( p.x ) );
					break;
				default:
					break;
				}
			} ) ) );

		}
	} ) );
	mPreUpdates.insert( std::make_pair( State::RESULT, [ this ] ( float t )
	{

	} ) );
	mUpdates.insert( std::make_pair( State::STAND_BY, [ this ] ( float t )
	{
		CAMERA->refPosition = root->get_child_by_name( "cameraPosition" )->get_position_3d( );
		CAMERA->setCameraAngle( root->get_child_by_name( "cameraAngle" )->get_position( ) );
	} ) );
	mUpdates.insert( std::make_pair( State::READY, [ this ] ( float t )
	{
	} ) );
	mUpdates.insert( std::make_pair( State::BATTLE, [ this ] ( float t )
	{

	} ) );
	mUpdates.insert( std::make_pair( State::BATTLE_END, [ this ] ( float t )
	{

	} ) );
	mUpdates.insert( std::make_pair( State::RESULT, [ this ] ( float t )
	{
		CAMERA->refPosition = Game::Field::WORLD_SIZE * cinder::vec3( 0.5F, 2.5F, 0.5F );
		CAMERA->setCameraAngle( cinder::vec2( -glm::pi<float>( ) / 2.0F, -glm::pi<float>( ) ) );
		if ( ENV->pushKey( ) )
		{
			cSceneManager::getInstance( )->shift<Scene::Member::cTitle>( );
		}
	} ) );
}
void cGameManager::setup( boost::posix_time::ptime ready, boost::posix_time::ptime battle, boost::posix_time::ptime battleEnd )
{
	shiftSeconds.insert( std::make_pair( State::READY, ready ) );
	shiftSeconds.insert( std::make_pair( State::BATTLE, battle ) );
	shiftSeconds.insert( std::make_pair( State::BATTLE_END, battleEnd ) );
	shiftSeconds.insert( std::make_pair( State::RESULT, battleEnd + boost::posix_time::seconds( 3 ) ) );
}
std::string cGameManager::getLeftBattleTime( )
{
	auto duration = shiftSeconds[State::BATTLE_END] - pt::microsec_clock::local_time( );
	if ( duration.is_negative( ) )
	{
		return "00:00";
	}
	return boost::str( boost::format( "%02d:%02d" ) % duration.minutes( ) % duration.seconds( ) );
}
void cGameManager::shiftResult( )
{
	isShiftResult = true;
}
void cGameManager::preUpdate( float delta )
{
	flash = false;
	mPreUpdates[state]( delta );
}
void cGameManager::update( float delta )
{
	mUpdates[state]( delta );
	root->entry_update( delta );
}
void cGameManager::draw( )
{
	root->entry_render( cinder::mat4( ) );
}
void cGameManager::shift( State state )
{
	flash = true;
	prevState = this->state;
	this->state = state;
}
}
