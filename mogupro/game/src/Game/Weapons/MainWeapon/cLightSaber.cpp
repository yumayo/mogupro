#include <Game/Weapons/MainWeapon/cLightSaber.h>
#include <cinder/gl/gl.h>
#include <Utility/cInput.h>
#include <CameraManager/cCameraManager.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>
#include <Resource/cSoundManager.h>
#include <Game/cGemManager.h>
#include <Particle/cParticleManager.h>
#include <Game/cClientAdapter.h>
#include <Math/Quat.h>
#include <cinder/gl/gl.h>
#include <Node/action.hpp>
#include <Game/cPlayerManager.h>
#include <Game/cLightManager.h>
using namespace Node::Action;
using namespace cinder;
namespace Game
{
namespace Weapons
{
namespace MainWeapon
{
cLightSaber::cLightSaber( Player::cPlayer& player )
	: cBase( player )
{

}
void cLightSaber::setup( )
{
	STATE_GENERATE( stateMachine, idle );
	STATE_GENERATE( stateMachine, slash_ready );
	STATE_GENERATE( stateMachine, left_slash );
	STATE_GENERATE( stateMachine, right_slash );
	STATE_GENERATE( stateMachine, chage_attack );
	STATE_GENERATE( stateMachine, chage_hold );
	STATE_GENERATE( stateMachine, shot );
	STATE_GENERATE( stateMachine, chage_hold_max );
	STATE_GENERATE( stateMachine, shot_max );

	// アイドル 
	{
		idle->join( slash_ready, [ this ] ( auto n )
		{
			return push;
		} );
		idle->onStateIn = [ this ] ( auto m )
		{
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 1, 0, 0 ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};
		};
	}
	// スラッシュか、チャージショットかの分岐
	{
		slash_ready->join( left_slash, [ this ] ( auto n )
		{
			// 0.2秒以内にクリックを離したらスラッシュ
			return ( n->time < 0.2F ) && pull;
		} );
		slash_ready->join( chage_attack, [ this ] ( auto n )
		{
			// 0.2秒以上ホールド状態が続いたらホールド
			return ( n->time >= 0.2F );
		} );
		slash_ready->onStateIn = [ this ] ( auto m )
		{
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 0.5F, 1, 0.5F ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};
		};
	}
	// 左スラッシュ
	{
		left_slash->join( right_slash, [ this ] ( auto n )
		{
			// 0.3 ~ 0.6秒の間にクリックをしたらもう一度スラッシュが発動
			return  ( n->time >= 0.3F ) && ( n->time < 0.6F ) && push;
		} );
		left_slash->join( idle, [ this ] ( auto n )
		{
			// 強スラッシュが発動する時間を過ぎたらアイドル
			return ( n->time >= 0.6F );
		} );
		left_slash->onStateIn = [ this ] ( auto m )
		{
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 0, 1, 0 ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};

			Resource::SE["Player/aura1.wav"].setLooping( false );
			Resource::SE["Player/aura1.wav"].stop( );

			Resource::SE["Player/swing2.wav"].setGain( 0.2f );
			Resource::SE["Player/swing2.wav"].play( );
		};
	}
	// 右スラッシュ
	{
		right_slash->join( idle, [ this ] ( auto n )
		{
			return ( n->time >= 0.3F );
		} );
		right_slash->onStateIn = [ this ] ( auto m )
		{
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 0, 0, 1 ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};
			Resource::SE["Player/swing2.wav"].setGain( 0.2f );
			Resource::SE["Player/swing2.wav"].play( );
		};
	}
	// チャージアタック
	{
		chage_attack->join( chage_hold, [ this ] ( auto n )
		{
			return n->time >= 1.0F;
		} );
		chage_attack->join( left_slash, [ this ] ( auto n )
		{
			return ( n->time < 1.0F ) && pull;
		}, [ &, this ] ( auto n )
		{
			return n->time / 1.0F;
		} );
		chage_attack->onStateIn = [ this ] ( auto m )
		{
			Resource::SE["Player/aura1.wav"].setGain( 0.2f );
			Resource::SE["Player/aura1.wav"].setLooping( true );
			Resource::SE["Player/aura1.wav"].play( );
			Particle::cParticleManager::getInstance( )->create( Particle::ParticleParam( )
																.position( pos )
																.scale( 0.3f )
																.vanishBeginTime( 0.f )
																.vanishTime( 15.f / 60.f )
																.vanishTimeRange( 0.0f )
																.easeTime( 15.f )
																.speed( 0.0f )
																.textureType( Particle::ParticleTextureType::SPARK )
																.color( ci::ColorA( 1, 1, 0 ) )
																.moveType( Particle::ParticleType::CONVERGE )
																.count( 3 )
																.isTrajectory( true )
																.effectTime( 0.3f )
																.easeType( EaseType::CircIn ) );
		};
	}
	// チャージショット準備(途中段階)
	{
		chage_hold->join( chage_hold_max, [ this ] ( auto n )
		{
			return n->time >= 1.0F;
		} );
		chage_hold->join( shot, [ this ] ( auto n )
		{
			// 途中で発射した場合。
			return ( n->time < 1.0F ) && pull;
		}, [ ] ( auto n )
		{
			return n->time / 1.0F;
		} );
		chage_hold->onStateIn = [ this ] ( auto m )
		{
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 1, 1, 0 ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};
			Resource::SE["Player/aura1.wav"].setGain( 0.2f );
			Resource::SE["Player/aura1.wav"].setLooping( true );
			Resource::SE["Player/aura1.wav"].play( );
			Particle::cParticleManager::getInstance( )->create( Particle::ParticleParam( )
																.position( pos )
																.scale( 0.3f )
																.vanishBeginTime( 0.f )
																.vanishTime( 15.f / 60.f )
																.vanishTimeRange( 0.0f )
																.easeTime( 15.f )
																.speed( 0.0f )
																.textureType( Particle::ParticleTextureType::SPARK )
																.color( player.getWhichTeam( ) == Player::Team::Red ? ci::ColorA( 1, 0, 0 ) : ci::ColorA( 0, 1, 1 ) )
																.moveType( Particle::ParticleType::CONVERGE )
																.count( 3 )
																.isTrajectory( true )
																.effectTime( 0.3f )
																.easeType( EaseType::CircIn )
			);
		};
	}
	// チャージショット発射(途中段階)
	{
		shot->join( idle, [ this ] ( auto n )
		{
			return n->time > 1.0F;
		} );
		shot->onStateIn = [ this ] ( auto m )
		{
			float power = boost::any_cast<float>( m );
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 0, 1, 1 ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};

			Resource::SE["Player/aura1.wav"].setLooping( false );
			Resource::SE["Player/aura1.wav"].stop( );

			Resource::SE["Player/katana-slash5.wav"].setGain( 0.2f );
			Resource::SE["Player/katana-slash5.wav"].play( );
		};
	}
	// チャージショット準備(最大)
	{
		chage_hold_max->join( shot_max, [ this ] ( auto n )
		{
			return pull;
		} );
		chage_hold_max->onStateIn = [ this ] ( auto m )
		{
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 1, 0, 1 ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};
			Resource::SE["Player/decision1.wav"].setGain( 0.2f );
			Resource::SE["Player/decision1.wav"].setLooping( false );
			Resource::SE["Player/decision1.wav"].play( );
		};
	}
	// チャージショット発射(最大)
	{
		shot_max->join( idle, [ this ] ( auto n )
		{
			return n->time > 1.0F;
		} );
		shot_max->onStateIn = [ this ] ( auto m )
		{
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 1, 0.5F, 0.5F ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};

			Resource::SE["Player/aura1.wav"].setLooping( false );
			Resource::SE["Player/aura1.wav"].stop( );

			Resource::SE["Player/iainuki1.wav"].setGain( 0.2f );
			Resource::SE["Player/iainuki1.wav"].setLooping( false );
			Resource::SE["Player/iainuki1.wav"].play( );
		};
	}

	stateMachine.setEntryNode( idle );
}
void cLightSaber::update( const float& delta_time )
{
	auto m = player.getWorldMatrixWeapon( );
	pos = vec3( m[3][0], m[3][1], m[3][2] );
	ci::app::console( ) << pos << std::endl;
	stateMachine.update( delta_time );
	ci::app::console() <<
	stateMachine.getCurrentStateName( ) << std::endl;
	flashInput( );
}
void cLightSaber::draw( )
{
	drawFunc( );
}
void cLightSaber::reset( )
{

}
}
}
}