#include <Game/cTutorialManager.h>
#include <Network/cUDPClientManager.h>
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
#include <Game/Field/FieldData.h>
#include <Resource/cSoundManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cClientAdapter.h>
namespace Game
{
cTutorialManager::cTutorialManager( )
{
	root = Node::node::create( );
	root->set_schedule_update( );
	root->set_content_size( cinder::app::getWindowSize( ) );
	root->set_scale( cinder::vec2( 1, -1 ) );
	root->set_position( root->get_content_size( ) * cinder::vec2( -0.5F, 0.5F ) );

	using namespace Node::Action;
	addPreUpdate( State::Description, [ this ] ( float t )
	{
		next();
		ui.setup(dess::SceneName::TUTORIAL);
		root->remove_all_children();

		switch (cPlayerManager::getInstance()->getActivePlayerTeamId())
		{
		case Player::Red:
			CAMERA->setCameraAngle(cinder::vec2(0, 0));
			break;
		case Player::Blue:
			CAMERA->setCameraAngle(cinder::vec2(-glm::pi<float>(), 0));
			break;
		default:
			break;
		}
		
		ENV->setMouseControl(true);

		
		cUIManager::getInstance()->enable();

		playBgm("ƒgƒƒsƒJƒ‹–³E.wav", 0.3f, 0.0f, 0.0f);
	} );
	addPreUpdate( State::LOAD, [ this ] ( float t )
	{
		if (!ui.ui_data["à–¾”Â"]->getActive())
		{
			next();
			ENV->enableKeyButton();
			ENV->enablePadAxis();
			ENV->enablePadButton();
			ENV->enableMouseButton();
		}
	} );
	addPreUpdate( State::MY_TEAM, [ this ] ( float t )
	{
		next();
	} );
	addPreUpdate( State::ENEMY_TEAM, [ this ] ( float t )
	{
		next();
		
	} );
	addPreUpdate( State::READY, [ this ] ( float t )
	{
		if ( shiftSeconds[State::READY] < 10 )
		{
			next( );
			
		}
	} );
	addPreUpdate( State::BATTLE, [ this ] ( float t )
	{
		if ( shiftSeconds[State::BATTLE] < 0 )
		{
			next( );

			ENV->disableMouseButton( );
			ENV->disableKeyButton( );
			ENV->disablePadButton( );
			ENV->disablePadAxis( );

			if ( root->get_child_by_name( "fader" ) ) return;
			auto n = root->add_child( Node::Renderer::rect::create( cinder::app::getWindowSize( ) ) );
			n->set_color( cinder::ColorA( 0, 0, 0, 0 ) );
			n->set_name( "fader" );
			n->set_position( root->get_content_size( ) / 2.0F );
			n->run_action( Node::Action::sequence::create( Node::Action::fade_in::create( 1.0F ), Node::Action::call_func::create( [ this ]
			{
				next( );

				ENV->enableKeyButton( );
				ENV->enableMouseButton( );
				ENV->enablePadButton( );
				ENV->enablePadAxis( );
				stopBgm( "testbgm2.wav" );
				cUIManager::getInstance( )->disable( );
				root->get_child_by_name( "fader" )->run_action( Node::Action::sequence::create( Node::Action::fade_out::create( 1.0F ), Node::Action::remove_self::create( ) ) );
				auto label = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
				label->set_position( root->get_content_size( ) / 2.0F );
				auto point = root->add_child( Node::Renderer::label::create( "AMEMUCHIGOTHIC-06.ttf", 64 ) );
				point->set_position( root->get_content_size( ) / 2.0F + cinder::vec2( 0, 100 ) );
				
			} ) ) );
		}
	} );
	addPreUpdate( State::BATTLE_END, [ this ] ( float t )
	{
		if ( shiftSeconds[State::BATTLE_END] < Network::cUDPClientManager::getInstance()->getServerTime( ) )
		{
			next( );
		}
	} );
	addPreUpdate( State::RESULT, [ this ] ( float t )
	{

	} );
	addUpdate( State::Description, [ this ] ( float t )
	{
	} );
	addUpdate( State::LOAD, [ this ] ( float t )
	{

	} );
	addUpdate( State::MY_TEAM, [ this ] ( float t )
	{
		switch ( cPlayerManager::getInstance( )->getActivePlayerTeamId( ) )
		{
		case Player::Red:
			CAMERA->refPosition = Field::CANNON_POINT[0];
			CAMERA->setCameraAngle( cinder::vec2( -glm::pi<float>( ), 0 ) );
			break;
		case Player::Blue:
			CAMERA->refPosition = Field::CANNON_POINT[1];
			CAMERA->setCameraAngle( cinder::vec2( 0, 0 ) );
			break;
		default:
			break;
		}
	} );
	addUpdate( State::ENEMY_TEAM, [ this ] ( float t )
	{
		switch ( cPlayerManager::getInstance( )->getActivePlayerTeamId( ) )
		{
		case Player::Red:
			CAMERA->refPosition = Field::CANNON_POINT[1];
			CAMERA->setCameraAngle( cinder::vec2( 0, 0 ) );
			break;
		case Player::Blue:
			CAMERA->refPosition = Field::CANNON_POINT[0];
			CAMERA->setCameraAngle( cinder::vec2( -glm::pi<float>( ), 0 ) );
			break;
		default:
			break;
		}
	} );
	addUpdate( State::READY, [ this ] ( float t )
	{
	} );
	addUpdate( State::BATTLE, [ this ] ( float t )
	{

	} );
	addUpdate( State::BATTLE_END, [ this ] ( float t )
	{

	} );
	addUpdate( State::RESULT, [ this ] ( float t )
	{
		CAMERA->refPosition = Game::Field::WORLD_SIZE * cinder::vec3( 0.5F, 2.5F, 0.5F );
		CAMERA->setCameraAngle( cinder::vec2( -glm::pi<float>( ) / 2.0F, -glm::pi<float>( ) ) );
		if ( ENV->pushKey( ) )
		{
			Scene::cSceneManager::getInstance( )->shift<Scene::Member::cTitle>( );
		}
	} );
}
void cTutorialManager::setTime( float loadTime )
{
	auto myTeam = loadTime + 2.0F;
	auto enemyTeam = myTeam + 2.0F;
	auto ready = enemyTeam + 2.0F;
	auto battle = ready + 60.0F * 5.0F;
	auto battleEnd = battle + 3.0F;
	auto result = battleEnd + 3.0F;

	shiftSeconds[State::LOAD] = loadTime;
	shiftSeconds[State::MY_TEAM] = myTeam;
	shiftSeconds[State::ENEMY_TEAM] = enemyTeam;
	shiftSeconds[State::READY] = ready;
	shiftSeconds[State::BATTLE] = battle;
	shiftSeconds[State::BATTLE_END] = battleEnd;
	shiftSeconds[State::RESULT] = result;
}
std::string cTutorialManager::getLeftBattleTime( )
{
	auto duration = shiftSeconds[State::BATTLE] - Network::cUDPClientManager::getInstance()->getServerTime( );
	if ( duration < 0.0F )
	{
		return "00:00";
	}
	return boost::str( boost::format( "%02d:%02d" ) % (int)( duration / 60.0F ) % (int)( std::fmodf( duration, 60.0F ) ) );
}
void cTutorialManager::preUpdate( float delta )
{
	flash = false;
	mPreUpdates[state]( delta );
}
void cTutorialManager::update( float delta )
{
	root->entry_update( delta );
	mUpdates[state]( delta );
	ui.update(delta);
	c_Easing::update(delta);
}
void cTutorialManager::draw( )
{
	ui.draw();
	root->entry_render( cinder::mat4( ) );
	
}
void cTutorialManager::skipReady( )
{
	auto now = Network::cUDPClientManager::getInstance()->getServerTime( );
	shiftSeconds[State::LOAD] = shiftSeconds[State::MY_TEAM] = shiftSeconds[State::ENEMY_TEAM] = shiftSeconds[State::READY] = now;
	shiftSeconds[State::BATTLE] = shiftSeconds[State::READY] + 60.0F * 5.0F;
	shiftSeconds[State::BATTLE_END] = shiftSeconds[State::BATTLE] + 3.0F;
	shiftSeconds[State::RESULT] = shiftSeconds[State::BATTLE_END] + 3.0F;
}
void cTutorialManager::addPreUpdate( State state, std::function<void( float )> method )
{
	mPreUpdates.insert( std::make_pair( state, method ) );
}
void cTutorialManager::addUpdate( State state, std::function<void( float )> method )
{
	mUpdates.insert( std::make_pair( state, method ) );
}
void cTutorialManager::next( )
{
	flash = true;
	prevState = state;
	state = static_cast<State>( static_cast<int>( state ) + 1 );
}
void cTutorialManager::playBgm( const std::string& name, const float gain, const float loopbegin, const float loopend )
{
	Resource::cSoundManager::getInstance( )->findBgm( name ).setLooping( true );
	Resource::cSoundManager::getInstance( )->findBgm( name ).setGain( gain );
	Resource::cSoundManager::getInstance( )->findBgm( name ).play( );
}
void cTutorialManager::stopBgm( const std::string& name )
{
	Resource::cSoundManager::getInstance( )->findBgm( name ).stop( );
}
bool cTutorialManager::timeEmpty( )
{
	return shiftSeconds.find( state ) == shiftSeconds.end( );
}
}
