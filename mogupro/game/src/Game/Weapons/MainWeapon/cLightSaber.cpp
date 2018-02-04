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
class tumeBaseNode : public Node::node
{
protected:
	ci::gl::VboMeshRef vbo;
	bool init( Player::Team team )
	{
		set_color( team == Player::Team::Red ? ci::ColorA( 1, 0, 0 ) : ci::ColorA( 0, 0, 1 ) );
		vec3 col( get_color().r, get_color().g, get_color().b );
		light = cLightManager::getInstance( )->addPointLight( ci::vec3(0), col, 0.0F );
		vbo = Resource::OBJ["mainWeapon.obj"];
		return true;
	}
	Light::PointLightHandle light;
	float lightPower = 0.0F;
public:
	void addLightPower( float powerByOnesecond )
	{
		lightPower = glm::clamp( lightPower + powerByOnesecond, 0.0F, 1.5F );
	}
	void setLightPower( float powerByOnesecond )
	{
		lightPower = glm::clamp( powerByOnesecond, 0.0F, 1.5F );
	}
	void resetLightPower( )
	{
		lightPower = 0.0F;
		light->reAttachPositionWithRadius( ci::vec3(0), 0.0F );
	}
};
class tumeNode : public tumeBaseNode
{
public:
	CREATE_H( tumeNode, Player::Team team )
	{
		CREATE( tumeNode, team );
	}
	void update( float delta ) override
	{
		auto mat = get_world_matrix_3d( );
		auto pos = ci::vec3( mat[3][0], mat[3][1], mat[3][2] );
		light->reAttachPositionWithRadius( pos, lightPower );
	}
	bool init( Player::Team team )
	{
		if ( !__super::init( team ) ) return false;
		return true;
	}
	void render( ) override
	{
		gl::draw( vbo );
	}
};
class tumeBulletNode : public tumeBaseNode
{
public:
	CREATE_H( tumeBulletNode, Player::Team team )
	{
		CREATE( tumeBulletNode, team );
	}
	bool init( Player::Team team )
	{
		if ( !__super::init( team ) ) return false;
		ws.center( vec3( 0 ) ).radius( 0.7F );
		return true;
	}
	void update( float delta ) override
	{
		auto mat = get_world_matrix_3d( );
		auto pos = ci::vec3( mat[3][0], mat[3][1], mat[3][2] );
		light->reAttachPositionWithRadius( pos, lightPower );

		attackSphere.setCenter( pos );
		attackSphere.setRadius( 0.7F );
	}
	void render( ) override
	{
		gl::draw( vbo );
		gl::draw( ws );
	}
	float power = 0.0F;
	ci::geom::WireSphere ws;
	ci::Sphere attackSphere;
	std::vector<int> hitListPlayer;
	std::vector<int> hitListGem;
};
cLightSaber::cLightSaber( Player::cPlayer& player )
	: cBase( player )
{
}
void cLightSaber::setup( )
{
	tumeRoot = Node::node::create( );
	tumeHolder = tumeRoot->add_child( Node::node::create( ) );
	tume = tumeHolder->add_child( tumeNode::create( player.getWhichTeam( ) ) );
	tume->set_schedule_update( );
	tumeBulletRoot = Node::node::create( );

	animation( 0.1F, TumeFormat( ) );
	tumeRoot->set_block_visible( );

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
			animation( 0.1F, TumeFormat( ) );
			tumeRoot->set_block_visible( );
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
			tumeRoot->set_block_visible( false );
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
		left_slash->onStateStay = [ this ] ( auto n )
		{
			player.getPlayerAnimation( ).setAnimationIncrementTime( 0.075F );
		};
		left_slash->onStateIn = [ this ] ( auto m )
		{
			ci::geom::WireSphere attackSphere;
			auto center = glm::normalize( vec3( 1, 0, 1 ) );
			auto radius = 0.6F;
			attackSphere.center( center ).radius( radius );

			drawFunc = [ attackSphere, this ]
			{
				gl::ScopedColor col( ColorA( 0, 1, 0 ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
				gl::draw( attackSphere );
			};

			if ( player.getActiveUser( ) )
			{
				auto mat = tumeRoot->get_world_matrix_3d( );
				auto centerMat = glm::translate( mat, center );
				center = vec3( centerMat[3][0], centerMat[3][1], centerMat[3][2] );
				attackSphere.center( center );

				ci::Sphere s( center, radius );

				for ( auto& p : cPlayerManager::getInstance( )->getPlayers( ) )
				{
					if ( p->getPlayerId( ) == player.getPlayerId( ) ) continue;
					if ( p->getWhichTeam( ) == player.getWhichTeam( ) ) continue;
					if ( p->isWatching( ) ) continue;
					if ( s.intersects( p->getAABB( ) ) )
					{
						cClientAdapter::getInstance( )->sendDamage( p->getPlayerId( ), 40.0F );
					}
				}

				ci::Sphere s_plus( vec3( mat[3][0], mat[3][1], mat[3][2] ), 0.5F );
				for ( auto& gem_ref : cGemManager::getInstance( )->getGemStones( ) )
				{
					if ( !gem_ref->isActive( ) ) continue;
					auto gemAABB = gem_ref->getAabb( ).createAABB( gem_ref->getCenterPos( ) );
					if ( s.intersects( gemAABB ) ||
						 s_plus.intersects( gemAABB ) )
					{
						for ( auto& f_gem_ref : cGemManager::getInstance( )->breakGemStone( gem_ref->getId( ) ) )
						{
							player.getGems( f_gem_ref->getId( ) );
						}
					}
				}

				Resource::SE["Player/aura1.wav"].setLooping( false );
				Resource::SE["Player/aura1.wav"].stop( );

				Resource::SE["Player/swing2.wav"].setGain( 0.2f );
				Resource::SE["Player/swing2.wav"].play( );
			}

			player.getPlayerAnimation( ).animationChange( "mogura_attack" );

			animation( 0.15F,
					   TumeFormat( )
					   .tumeRotation( 0.15F, glm::pi<float>( ) / 2.0F )
					   .tumeRootRotation( 0.3F, glm::pi<float>( ) / 1.5F ) );
		};
	}
	// 右スラッシュ
	{
		right_slash->join( idle, [ this ] ( auto n )
		{
			return ( n->time >= 0.4F );
		} );
		right_slash->onStateStay = [ this ] (auto n)
		{
			player.getPlayerAnimation( ).setAnimationIncrementTime( 0.075F );
		};
		right_slash->onStateIn = [ this ] ( auto m )
		{
			ci::geom::WireSphere attackSphere;
			auto center = glm::normalize( vec3( -1, 0, 2 ) );
			auto radius = 0.8F;
			attackSphere.center( center ).radius( radius );

			drawFunc = [ attackSphere, this ]
			{
				gl::ScopedColor col( ColorA( 0, 0, 1 ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
				gl::draw( attackSphere );
			};

			if ( player.getActiveUser( ) )
			{
				auto mat = tumeRoot->get_world_matrix_3d( );
				auto centerMat = glm::translate( mat, center );
				center = vec3( centerMat[3][0], centerMat[3][1], centerMat[3][2] );
				attackSphere.center( center );

				ci::Sphere s( center, radius );
				for ( auto& p : cPlayerManager::getInstance( )->getPlayers( ) )
				{
					if ( p->getPlayerId( ) == player.getPlayerId( ) ) continue;
					if ( p->getWhichTeam( ) == player.getWhichTeam( ) ) continue;
					if ( p->isWatching( ) ) continue;
					if ( s.intersects( p->getAABB( ) ) )
					{
						cClientAdapter::getInstance( )->sendDamage( p->getPlayerId( ), 50.0F );
					}
				}
				ci::Sphere s_plus( vec3( mat[3][0], mat[3][1], mat[3][2] ), 0.5F );
				for ( auto& gem_ref : cGemManager::getInstance( )->getGemStones( ) )
				{
					if ( !gem_ref->isActive( ) ) continue;
					auto gemAABB = gem_ref->getAabb( ).createAABB( gem_ref->getCenterPos( ) );
					if ( s.intersects( gemAABB ) ||
						 s_plus.intersects( gemAABB ) )
					{
						for ( auto& f_gem_ref : cGemManager::getInstance( )->breakGemStone( gem_ref->getId( ) ) )
						{
							player.getGems( f_gem_ref->getId( ) );
						}
					}
				}

				Resource::SE["Player/swing2.wav"].setGain( 0.2f );
				Resource::SE["Player/swing2.wav"].play( );
			}

			player.getPlayerAnimation( ).animationChange( "mogura_attack" );

			animation( 0.1F,
					   TumeFormat( )
					   .tumeRotation( 0.1F, -glm::pi<float>( ) / 2.0F )
					   .tumeRootRotation( 0.3F, -glm::pi<float>( ) / 1.5F ) );
		};
	}

	
	// チャージアタックか、チャージショットの分岐
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

			animation( 0.1F, TumeFormat( ) );
			tume->run_action( sequence::create( delay::create( 0.15F ), call_func::create( [ this ] { tume->set_axis( vec3( 1, 0, 0 ) ); } ), rotate_to::create( 0.3F, -glm::pi<float>( ) / 2.0F ) ) );
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
			return n->time;
		} );
		chage_hold->onStateStay = [ this ] ( auto n )
		{
			auto t = tume.dynamicptr<tumeNode>( );
			t->addLightPower( delta );
		};
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
																.easeType( EaseType::CircIn ) );

		};
	}
	// チャージショット発射(途中段階)
	{
		shot->join( idle, [ this ] ( auto n )
		{
			return n->time > 0.5F;
		} );
		shot->onStateIn = [ this ] ( auto m )
		{
			float power = boost::any_cast<float>( m );
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 0, 1, 1 ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};

			auto t = tume.dynamicptr<tumeNode>( );
			t->resetLightPower( );

			Resource::SE["Player/aura1.wav"].setLooping( false );
			Resource::SE["Player/aura1.wav"].stop( );

			Resource::SE["Player/katana-slash5.wav"].setGain( 0.2f );
			Resource::SE["Player/katana-slash5.wav"].play( );

			addBullet( power );

			tumeRoot->set_block_visible( );
		};
	}
	// チャージショット準備(最大)
	{
		chage_hold_max->join( shot_max, [ this ] ( auto n )
		{
			return pull;
		} );
		chage_hold_max->onStateStay = [ this ] ( auto n )
		{
			// 一応まだライトは大きくなる。攻撃力などに変化はない。
			auto t = tume.dynamicptr<tumeNode>( );
			t->addLightPower( delta );
		};
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
			return n->time > 0.6F;
		} );
		shot_max->onStateIn = [ this ] ( auto m )
		{
			drawFunc = [ this ]
			{
				gl::ScopedColor col( ColorA( 1, 0.5F, 0.5F ) );
				gl::drawCube( vec3( 0 ), vec3( 0.1F ) );
			};

			auto t = tume.dynamicptr<tumeNode>( );
			t->resetLightPower( );

			Resource::SE["Player/aura1.wav"].setLooping( false );
			Resource::SE["Player/aura1.wav"].stop( );

			Resource::SE["Player/iainuki1.wav"].setGain( 0.2f );
			Resource::SE["Player/iainuki1.wav"].setLooping( false );
			Resource::SE["Player/iainuki1.wav"].play( );

			addBullet( 2.0F );

			tumeRoot->set_block_visible( );
		};
	}

	stateMachine.setEntryNode( idle );
}
void cLightSaber::update( const float& delta_time )
{
	delta = delta_time;
	auto m = player.getWorldMatrixWeapon( );
	pos = ci::vec3( m[3][0], m[3][1], m[3][2] );
	tumeRoot->set_matrix_3d( m );
	tumeRoot->entry_update( delta_time );
	tumeBulletRoot->entry_update( delta_time );

	if ( player.getActiveUser( ) )
	{
		// 爪Bulletとの当たり判定
		for ( auto& b_holder : tumeBulletRoot->get_children( ) )
		{
			auto b = std::dynamic_pointer_cast<tumeBulletNode>( b_holder->get_children( ).front( ) );

			// 爪Bulletとプレイヤーの当たり判定
			for ( auto& p : cPlayerManager::getInstance( )->getPlayers( ) )
			{
				if ( p->getPlayerId( ) == player.getPlayerId( ) ) continue;
				if ( p->getWhichTeam( ) == player.getWhichTeam( ) ) continue;
				if ( p->isWatching( ) ) continue;

				bool collision = false;
				for ( auto l : b->hitListPlayer )
				{
					if ( l == p->getPlayerId( ) )
					{
						collision = true;
						break;
					}
				}
				if ( collision == false )
				{
					if ( b->attackSphere.intersects( p->getAABB( ) ) )
					{
						b->hitListPlayer.emplace_back( p->getPlayerId( ) );
						cClientAdapter::getInstance( )->sendDamage( p->getPlayerId( ), b->power * 50.0F );
					}
				}
			}

			// 爪Bulletとジェムとの当たり判定
			for ( auto& gem_ref : cGemManager::getInstance( )->getGemStones( ) )
			{
				if ( !gem_ref->isActive( ) ) continue;

				bool collision = false;
				for ( auto l : b->hitListGem )
				{
					if ( l == gem_ref->getId( ) )
					{
						collision = true;
						break;
					}
				}
				if ( collision == false )
				{
					if ( b->attackSphere.intersects( gem_ref->getAabb( ).createAABB( gem_ref->getCenterPos( ) ) ) )
					{
						b->hitListGem.emplace_back( gem_ref->getId( ) );
						for ( auto& f_gem_ref : cGemManager::getInstance( )->breakGemStone( gem_ref->getId( ) ) )
						{
							player.getGems( f_gem_ref->getId( ) );
						}
					}
				}
			}
		}
	}
	
	stateMachine.update( delta_time );
	flashInput( );
}
void cLightSaber::draw( )
{
	drawFunc( );
	
	{
		gl::ScopedModelMatrix scp;
		tumeRoot->entry_render( mat4( ) );
	}

	{
		gl::ScopedModelMatrix scp;
		tumeBulletRoot->entry_render( mat4( ) );
	}
}
void cLightSaber::reset( )
{

}
void cLightSaber::animation( float t, TumeFormat const & tumeFormat )
{
	tumeHolder->remove_all_actions( );
	tumeHolder->run_action( rotate_to::create( tumeFormat.m_tumeRootRotationTime == -1.0F ? t : tumeFormat.m_tumeRootRotationTime, tumeFormat.m_tumeRootRotation ) );
	tumeHolder->run_action( axis_to::create( tumeFormat.m_tumeRootAxisTime == -1.0F ? t : tumeFormat.m_tumeRootAxisTime, tumeFormat.m_tumeRootAxis ) );
	tumeHolder->run_action( move_to::create( tumeFormat.m_tumeRootPosTime == -1.0F ? t : tumeFormat.m_tumeRootPosTime, tumeFormat.m_tumeRootPos ) );
	tume->remove_all_actions( );
	tume->run_action( rotate_to::create( tumeFormat.m_tumeRotationTime == -1.0F ? t : tumeFormat.m_tumeRotationTime, tumeFormat.m_tumeRotation ) );
	tume->run_action( axis_to::create( tumeFormat.m_tumeAxisTime == -1.0F ? t : tumeFormat.m_tumeAxisTime, tumeFormat.m_tumeAxis ) );
	tume->run_action( move_to::create( tumeFormat.m_tumePosTime == -1.0F ? t : tumeFormat.m_tumePosTime, tumeFormat.m_tumePos ) );
}
void cLightSaber::addBullet( float power )
{
	auto b_holder = tumeBulletRoot->add_child( Node::node::create( ) );
	{
		b_holder->set_matrix_3d( player.getWorldMatrixWeapon( ) );
		b_holder->run_action( sequence::create( delay::create( 0.6F + power * 0.2F ), remove_self::create( ) ) );
	}
	auto b = b_holder->add_child( tumeBulletNode::create( player.getWhichTeam( ) ) );
	{
		b->power = power;
		b->setLightPower( b->power );
		b->set_schedule_update( );
		b->set_matrix_3d( tume->get_local_matrix_3d( ) );
		b->run_action( sequence::create( move_by::create( 0.6F + power * 0.2F, vec3( 0, 0, 5 + power * 10 ) ) ) );
	}
}
}
}
}