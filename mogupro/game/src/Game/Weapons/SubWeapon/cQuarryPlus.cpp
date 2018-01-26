#include <Game/Weapons/SubWeapon/cQuarryPlus.h>
#include <cinder/gl/gl.h>
#include <Game/cFieldManager.h>
#include <Game/cStrategyManager.h>
#include <Game/cGemManager.h>
#include <Network.hpp>
#include <cinder/gl/scoped.h>
#include <Resource/cImageManager.h>
#include <Game/cClientAdapter.h>
#include <Game/cStrategyManager.h>
#include <cinder/Rand.h>
#include <Game/cPlayerManager.h>
#include <Particle/cParticleManager.h>
#include <Game/cStrategyManager.h>
#include <Node/action.hpp>
#include <Resource/cObjectManager.h>
using namespace ci;
using namespace ci::app;
using namespace Node;
using namespace Node::Action;
namespace Game
{
namespace Weapons
{
namespace SubWeapon
{
cQuarryPlus::cQuarryPlus( const ci::vec3 pos, const int objectId, const int playerId )
	: mMachineAABB( pos, ci::vec3( 1 ) )
{
	ismyobject = ( Game::cPlayerManager::getInstance( )->getActivePlayerId( ) == playerId );
	mObjectId = objectId;
	mPlayerId = playerId;
	mScale = vec3( 3, 1, 3 );
	mDrillPos = mPos = pos;
	root = Node::node::create( );
	root->set_schedule_update( );
	root->set_position_3d( mDrillPos );
	iseasingfinished = true;
}
cQuarryPlus::~cQuarryPlus( )
{
	mMachineAABB.removeWorld( );

	Particle::cParticleManager::getInstance( )->create(
		Particle::ParticleParam( )
		.position( mMachineAABB.getPosition( ) + ci::vec3( 0, mScale.y / 2.f, 0 ) )
		.moveType( Particle::ParticleType::ABSORB )
		.textureType( Particle::ParticleTextureType::SPARK )
		.colors( gemcolors )
		.convergePoint( Game::cStrategyManager::getInstance( )->getCannons( )[Game::cPlayerManager::getInstance( )->getPlayers( )[mPlayerId]->getWhichTeam( )]->getReStorePos( ) )
		.speed( 1.5f )
		.swellEndTime( 0.1f )
		.swellWaitTime( 30.0f )
		.easeTime( 60.0f )
		.count( gemcolors.size( ) )
		.effectTime( 0 )
		.vanishTime( 10.0 )
		.randomEaseTypes( { EaseType::BackIn,EaseType::BackOut,EaseType::CircIn,EaseType::CircOut,EaseType::Linear,EaseType::CubicIn } )
	);
	int num = getgems.size( );

	cGemManager::getInstance( )->deleteFragmentGems( getgems );

	Game::cStrategyManager::getInstance( )->getCannons( )[int( cPlayerManager::getInstance( )->getPlayers( )[mPlayerId]->getWhichTeam( ) )]->receiveQuarryGem( num, mPlayerId, ismyobject );
}
void cQuarryPlus::setup( )
{
	mMachineAABB.addWorld( );
	mMachineAABB.setSize( mScale );

	mVboAntennaReciever = Resource::OBJ["quarry_plus/antenna_reciever.obj"];
	mVboAntennaRod      = Resource::OBJ["quarry_plus/antenna_rod.obj"];
	mVboBase            = Resource::OBJ["quarry_plus/base.obj"];
	mVboCone            = Resource::OBJ["quarry_plus/cone.obj"];
	mVboDrill           = Resource::OBJ["quarry_plus/drill.obj"];
	mVboLeg             = Resource::OBJ["quarry_plus/leg.obj"];

	mBreakType.add( Field::BlockType::NORMAL );
	mBreakType.add( Field::BlockType::HARD );

	auto drillBuild = mStateMachine.generate( "build" );
	auto drillMove = mStateMachine.generate( );
	auto drillReturn = mStateMachine.generate( );
	auto drillStop = mStateMachine.generate( "stop" );

	auto build = root->add_child( node::create( ) );

	drillBuild->join( drillMove, [ this ] ( auto n )
	{
		return true;
	} );
	drillBuild->onStateStay = [ this ] ( auto n )
	{
	};
	drillMove->join( drillReturn, [ this ] ( auto n )
	{
		return mDrillPos.y < 0.0F;
	} );
	drillMove->onStateStay = [ this ] ( auto n )
	{
		{
			vec3 pos_ = root->get_position_3d( );
			if ( iseasingfinished )
			{
				iseasingfinished = false;
				root->run_action( sequence::create( ease<EaseOutExpo>::create( move_to::create( 0.02F, getNextEasingPos( ) ) ), call_func::create( [ this ]
				{
					iseasingfinished = true;
					easingcount++;
				} ) ) );
			}
			mDrillPos = pos_;
		}
		if ( ismyobject )
		{
			float drillScale = 1.0F;

			Game::cFieldManager::getInstance( )->blockBreak( mDrillPos, drillScale, mBreakType );

			AxisAlignedBox drill_aabb( mDrillPos - cinder::vec3( drillScale ), mDrillPos + cinder::vec3( drillScale ) );

			for ( int i = 0; i < int( GemManager->getGemStones( ).size( ) ); i++ )
			{
				if ( !GemManager->getGemStone( i )->isActive( ) )continue;
				vec3 gempos = GemManager->getGemStone( i )->getPos( );
				vec3 gemscale = GemManager->getGemStone( i )->getScale( );

				AxisAlignedBox gem_aabb( gempos - gemscale / 2.f, gempos + gemscale / 2.f );

				if ( STRM->isAABB( drill_aabb, gem_aabb ) )
				{
					cClientAdapter::getInstance( )->sendGetGemQuarry( mObjectId, GemManager->getGemStone( i )->getId( ) );
				}
			}
		}
	};
	drillReturn->join( drillStop, [ this ] ( auto n )
	{
		return mPos.y == mDrillPos.y;
	} );
	drillReturn->onStateStay = [ this ] ( auto n )
	{
		mDrillPos.x = mPos.x;
		mDrillPos.y = std::min( mDrillPos.y + mDeltaSecond * 5.0f, mPos.y );
		mDrillPos.z = mPos.z;
	};

	mStateMachine.setEntryNode( drillBuild );
}
void cQuarryPlus::update( const float & delta_time )
{
	mDeltaSecond = delta_time;
	root->entry_update( mDeltaSecond );
	mStateMachine.update( );
	for ( int i = 0; i < int( getgems.size( ) ); i++ )
	{
		if ( getgems[i]->getPos( ).y < mPos.y + mScale.y / 2.f + 1.f ) {
			getgems[i]->node->entry_update( mDeltaSecond );
			vec3 p = getgems[i]->node->get_position_3d( );

			float rotate_speed = mDeltaSecond * 6.f;

			getgems[i]->setSinRotate( getgems[i]->getSinRotate( ) + rotate_speed );

			getgems[i]->setPos( vec3( getgems[i]->getPutPos( ).x + ( mScale.x / 2.f )*cos( getgems[i]->getSinRotate( ) ),
										p.y,
										getgems[i]->getPutPos( ).z + ( mScale.z / 2.f )*sin( getgems[i]->getSinRotate( ) ) ) );

		}
	}
}
void cQuarryPlus::updateCollisionAfterUpdate( const float & delta_time )
{

}
void cQuarryPlus::draw( )
{
	{
		cinder::gl::ScopedTextureBind a( Resource::IMAGE["in_game/quarry.png"] );
		{
			gl::pushModelMatrix( );
			gl::translate( mPos );
			gl::scale( mScale );
			gl::draw( mVboBase );
			gl::popModelMatrix( );
		}
	}

	if ( !mStateMachine.isCurrentState( "build" ) )
	{
		// ÉhÉäÉãñ_
		{
			gl::pushModelView( );
			auto drillRootPos = mDrillPos;
			drillRootPos.y = mPos.y;
			gl::translate( drillRootPos );
			float d = glm::distance( drillRootPos, mDrillPos );
			gl::scale( 1.0F, d, 1.0F );
			gl::draw( mVboDrill );
			gl::popModelView( );
		}
		// êÊí[
		{
			gl::pushModelView( );
			gl::translate( mDrillPos );
			gl::draw( mVboCone );
			gl::popModelView( );
		}
	}
}
bool cQuarryPlus::deleteThis( )
{
	return mStateMachine.isCurrentState( "stop" );
}
void cQuarryPlus::HitGem( const int gemId )
{
	std::vector<std::shared_ptr<Game::Gem::cFragmentGem>> breakGemStones = GemManager->breakGemStone( gemId );
	GemManager->getGemStone( gemId )->setIsActive( false );

	for ( int i = 0; i < breakGemStones.size( ); ++i ) {
		
		auto targetGem = breakGemStones[i];

		getgems.emplace_back( targetGem );

		targetGem->setIsRigid( false );
		targetGem->setSinRotate( atan2f( targetGem->getPos( ).z - mPos.z, ( targetGem->getPos( ).x - mPos.x ) ) );
		targetGem->setPutPos( vec3( mPos.x, mDrillPos.y, mPos.z ) );
		targetGem->node = Node::node::create( );
		targetGem->node->set_schedule_update( );
		targetGem->node->run_action( sequence::create( float_to::create( ( mPos.y - targetGem->getPos( ).y ) * 0.1f,
																		 targetGem->getPos( ).y, mPos.y + mScale.y + 1.f,
																		 [ this, targetGem ] ( float t )
		{
			auto p = targetGem->node->get_position_3d( );
			p.y = t;
			targetGem->node->set_position_3d( p );
		} ) ) );
		gemcolors.push_back( targetGem->getColorA( ) );
		gemcolors.push_back( targetGem->getColorA( ) );
		targetGem->setIsActive( false );
	}
}
ci::vec3 cQuarryPlus::getNextEasingPos( ) const
{
	int num = mScale.x;
	vec3 buf;
	int z_direction = easingcount / ( num*num ) % 2 == 0 ? 1 : -1;
	int direction = 1 * z_direction;
	if ( ( easingcount % ( num*num ) / num ) % 2 == 1 ) {
		direction = -1 * z_direction;
	}

	if ( easingcount / ( num*num ) % 2 == 0 ) {
		buf = vec3( mPos.x - direction * ( ( num - 1 ) / 2 ) + direction * ( easingcount%num ),
					mPos.y - ( easingcount / ( num*num ) ),
					mPos.z - ( ( num - 1 ) / 2 ) + z_direction * ( ( easingcount % ( num*num ) ) / num ) );
	}
	else {
		buf = vec3( mPos.x - direction * ( ( num - 1 ) / 2 ) + direction * ( easingcount%num ),
					mPos.y - ( easingcount / ( num*num ) ),
					mPos.z + ( ( num - 1 ) / 2 ) - ( ( easingcount % ( num*num ) ) / num ) );
	}

	return buf;
}
}
}
}
