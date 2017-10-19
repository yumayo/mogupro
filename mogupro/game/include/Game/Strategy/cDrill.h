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
    cDrill( const ci::vec3 _pos, const ci::vec3 _scale,const int _id);
    ~cDrill();
    void draw() override;
    void update( const float& delta_time ) override;
    void setup() override;
    bool DeleteThis() override;
	void setField(const ci::vec3 pos) override;
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
    void drawCube( const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color );
	void collisionFieldGems();
    std::vector<DrillSlope>drillslopes;
	std::vector<std::shared_ptr<Game::Gem::cGem>>getgems;
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
	float test;
	hardptr<Node::node>root;
};
}
}

