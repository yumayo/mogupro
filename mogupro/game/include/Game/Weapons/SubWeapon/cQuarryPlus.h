#pragma once
#include <Game/Weapons/SubWeapon/cSubWeaponBase.h>
#include <Collision/cAABBCollider.h>
#include <Game/cGemManager.h>
#include <Node/node.h>
#include <Game/Field/cBreakBlockType.h>
#include <Utility/cStateMachineBehaviour.h>
#include <cinder/gl/VboMesh.h>
namespace Game
{
namespace Weapons
{
namespace SubWeapon
{
class cQuarryPlus : public Game::Weapons::SubWeapon::cSubWeaponBase
{
public:
	cQuarryPlus( const ci::vec3 pos, const int objectId, const int playerId );
	~cQuarryPlus( );
public: //! override functions
	void setup( ) override;
	void update( const float& delta_time ) override;
	void updateCollisionAfterUpdate( const float& delta_time ) override;
	void draw( ) override;
	bool deleteThis( ) override;
public:
	void HitGem( const int gemId );
private:

	Utility::cStateMachineBehaviour mStateMachine;

	Utility::hardptr<Node::node> root;
	
	float mDeltaSecond = 0.0F;

	//
	//! ���b�V���B
	cinder::gl::VboMeshRef mVboAntennaReciever;
	cinder::gl::VboMeshRef mVboAntennaRod;
	cinder::gl::VboMeshRef mVboBase;
	cinder::gl::VboMeshRef mVboCone;
	cinder::gl::VboMeshRef mVboDrill;
	cinder::gl::VboMeshRef mVboLeg;
	
	//
	//�{�̌n
	
	Collision::cAABBCollider mMachineAABB;
	
	//
	//��[�̃h����

	ci::vec3 mDrillPos;
	ci::vec3 getNextEasingPos( ) const;
	bool iseasingfinished = true;
	int easingcount = 0;

	//
	//�W�F���֌W

	std::vector<std::shared_ptr<Game::Gem::cFragmentGem>> getgems;
	std::vector<ci::ColorA> gemcolors;
	Game::Field::cBreakBlockType mBreakType;
};
}
}
}