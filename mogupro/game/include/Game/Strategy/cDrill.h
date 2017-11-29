#pragma once
#include"Game/Strategy/cStrategyObjectBase.h"
#include"Game/cGemManager.h"
#include"Node\node.h"
#include"Node\action.hpp"
namespace Game
{
namespace Strategy
{
	

class cDrill : public cStrategyObjectBase
{
public:
	enum DrillType {
		Level1, Level2, Level3,
	};
    cDrill( const ci::vec3 _pos,const int _id,const DrillType type,const bool ismyobject);
    ~cDrill();
    void draw() override;
    void update( const float& delta_time ) override;
	void updateCollisionAfterUpdate(const float& delta_time)override;
    void setup() override;
    bool DeleteThis() override;
	void setField(const ci::vec3 pos) override;
	void HitGem(const int _gemid);
	DrillType drilltype;
private:
	enum DrillState{
		DRILLMOVE,DRILLRETURN,DRILLSTOP
	};
    struct DrillSlope
    {
        ci::vec3 pos;
        ci::vec3 scale;
        float rotate;
    };
	DrillState state = DrillState::DRILLMOVE;
	void setScale(const DrillType _type);
    void drawCube( const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color );
	void collisionFieldGems();
    DrillSlope drillslopes;
	std::vector<std::shared_ptr<Game::Gem::cGemStone>>getgems;
	void drawBasket();
    void createDrills();
    void move(float delttime);
	void moveGetGem(const float delttime);
	void updateSlope(const float direction,float delttime);
    float drillspeed = 10.0f;
    float y_rotate = 0.0f;
	void setting(const ci::vec3 beginpos);
    ci::vec3 beginpos;
    ci::vec3 machinescale;
    ci::vec3 slope;
    float drillslope;
	bool isretun = false;
	DrillState changeState();
	void DigEasing();
	void drawMachine();
	Utility::hardptr<Node::node> root;
	bool iseasingfinished = true;
	int easingcount = 0;
	ci::vec3 getNextEasingPos();
};
}
}

