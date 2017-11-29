#pragma once
#include <Game/Weapons/SubWeapon/cSubWeaponBase.h>
#include"Collision\cAABBCollider.h"
#include<Game/cGemManager.h>
#include<Node\node.h>
#include<Node\action.hpp>
namespace Game
{
namespace Weapons
{
namespace SubWeapon
{
class cQuarry : public Game::Weapons::SubWeapon::cSubWeaponBase
{
public:
	enum DrillType {
		Level1, Level2, Level3,
	};
    cQuarry(const ci::vec3 _pos, const int _objectid, const int playerid,const DrillType type);
    ~cQuarry( );
	void setup() override;
	void update(const float& delta_time) override;
	void updateCollisionAfterUpdate(const float& delta_time) override;
	void draw() override;
	bool deleteThis() override;
	void HitGem(const int _gemid);
	DrillType drilltype;
private:
	enum DrillState {
		DRILLMOVE, DRILLRETURN, DRILLSTOP
	};
	DrillState state = DrillState::DRILLMOVE;
private://本体系
	void drawBasket();
	void drawMachine();
	void setScale(const DrillType _type);
	Collision::cAABBCollider mMachineAABB;
	///machineのposはmPos
private://スロープ
	struct Slope {
		ci::vec3 pos;
		ci::vec3 scale;
		float rotate_y;
	};
	Slope slope;
	Collision::cAABBCollider mSlopeAABB;
	void updateSlope(float delttime);
	void createSlope();
private://先端のドリル
	Collision::cAABBCollider mDrillAABB;
	ci::vec3 mDrillPos;
	ci::vec3 mDrillScale;
	float mDrillrotate_y;
	void setupDrill();
	void collisionFieldGems();
	void Drillmove(float delttime);
	ci::vec3 getNextEasingPos();
	Utility::hardptr<Node::node> root;
	bool iseasingfinished = true;
	int easingcount = 0;

private://ジェム関係
	void moveGetGem(const float delttime);
	std::vector<std::shared_ptr<Game::Gem::cGemStone>>getgems;
private:
	void drawCube(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color);
	bool isretun = false;
	DrillState changeState();

	
	
};
}
}
}
