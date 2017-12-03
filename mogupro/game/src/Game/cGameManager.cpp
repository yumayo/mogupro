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
namespace pt = boost::posix_time;
namespace Game
{
cGameManager::cGameManager( )
{
	root = Node::node::create( );
	root->set_schedule_update( );

	mPreUpdates.insert( std::make_pair( State::STAND_BY, [ this ] ( float t )
	{
		if ( shiftSeconds[State::READY] < boost::posix_time::microsec_clock::local_time( ) )
		{
			shift( State::READY );
		}
	} ) );
	mPreUpdates.insert( std::make_pair( State::READY, [ this ] ( float t )
	{
		if ( shiftSeconds[State::BATTLE] < boost::posix_time::microsec_clock::local_time( ) )
		{
			shift( State::BATTLE );
		}
	} ) );
	mPreUpdates.insert( std::make_pair( State::BATTLE, [ this ] ( float t )
	{
		if ( shiftSeconds[State::BATTLE_END] < boost::posix_time::microsec_clock::local_time( ) )
		{
			shift( State::BATTLE_END );
		}
	} ) );
	mPreUpdates.insert( std::make_pair( State::BATTLE_END, [ this ] ( float t )
	{
		if ( ENV->pushKey( ) )
		{
			shiftResult( );
		}
		if ( isShiftResult )
		{
			if ( root->get_child_by_name( "fader" ) ) return;
			auto n = root->add_child( Node::Renderer::rect::create( cinder::app::getWindowSize( ) ) );
			n->set_color( cinder::ColorA( 0, 0, 0, 0 ) );
			n->set_name( "fader" );
			n->run_action( Node::Action::sequence::create( Node::Action::fade_in::create( 1.0F ), Node::Action::call_func::create( [ this ] {
				shift( State::RESULT );
				cUIManager::getInstance( )->disable( );
				root->get_child_by_name( "fader" )->run_action( Node::Action::sequence::create( Node::Action::fade_out::create( 1.0F ), Node::Action::remove_self::create( ) ) );
			} ) ) );
		}
	} ) );
	mPreUpdates.insert( std::make_pair( State::RESULT, [ this ] ( float t )
	{

	} ) );
	mUpdates.insert( std::make_pair( State::STAND_BY, [ this ] ( float t )
	{

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
		ci::vec3 worldSize = ci::vec3( Game::Field::CHUNK_RANGE_X, Game::Field::CHUNK_RANGE_Y, Game::Field::CHUNK_RANGE_Z ) * Game::Field::BLOCK_SIZE * (float)Game::Field::CHUNK_SIZE;
		CAMERA->refPosition = worldSize * cinder::vec3( 0.5F, 2.5F, 0.5F );
		CAMERA->setCameraAngle( cinder::vec2( -M_PI / 2, -M_PI ) );

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
