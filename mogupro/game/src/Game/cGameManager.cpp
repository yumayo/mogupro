#include <Game/cGameManager.h>
#include <boost/format.hpp>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cTitle.h>
#include <Utility/cInput.h>
#include <Game/Field/FieldData.h>
#include <CameraManager/cCameraManager.h>
namespace pt = boost::posix_time;
namespace Game
{
cGameManager::cGameManager( )
{
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
		if ( shiftSeconds[State::RESULT] < boost::posix_time::microsec_clock::local_time( ) )
		{
			shift( State::RESULT );
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
	mUpdates.insert( std::make_pair( State::RESULT, [ this ] ( float t )
	{
		ci::vec3 worldSize = ci::vec3( Game::Field::CHUNK_RANGE_X, Game::Field::CHUNK_RANGE_Y, Game::Field::CHUNK_RANGE_Z ) * Game::Field::BLOCK_SIZE * (float)Game::Field::CHUNK_SIZE;
		CAMERA->refPosition = worldSize * cinder::vec3( 0.5F, 2.5F, 0.5F );
		CAMERA->setCameraAngle( cinder::vec2( -M_PI / 2, -M_PI ) );

		if ( ENV->anyKey( ) )
		{
			cSceneManager::getInstance( )->shift<Scene::Member::cTitle>( );
		}
	} ) );
}
void cGameManager::setup( boost::posix_time::ptime ready, boost::posix_time::ptime battle, boost::posix_time::ptime result )
{
	shiftSeconds.insert( std::make_pair( State::READY, ready ) );
	shiftSeconds.insert( std::make_pair( State::BATTLE, battle ) );
	shiftSeconds.insert( std::make_pair( State::RESULT, result ) );
}
std::string cGameManager::getLeftBattleTime( )
{
	auto duration = shiftSeconds[State::RESULT] - pt::microsec_clock::local_time( );
	if ( duration.is_negative( ) )
	{
		return "00:00";
	}
	return boost::str( boost::format( "%02d:%02d" ) % duration.minutes( ) % duration.seconds( ) );
}
void cGameManager::preUpdate( float delta )
{
	flash = false;
	mPreUpdates[state]( delta );
}
void cGameManager::update( float delta )
{
	mUpdates[state]( delta );
}
void cGameManager::shift( State state )
{
	flash = true;
	prevState = this->state;
	this->state = state;
}
}
