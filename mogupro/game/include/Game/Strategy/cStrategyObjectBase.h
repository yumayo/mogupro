#pragma once
#include"Game\cObjectBase.h"
namespace Game
{
namespace Strategy
{
class cStrategyObjectBase : public Game::cObjectBase
{
public:
    cStrategyObjectBase();
    virtual ~cStrategyObjectBase();
    ci::vec3 getScale();
	int getId();
    virtual void setup() = 0;
    virtual void update( const float& delta_time ) = 0;
	virtual void updateCollisionAfterUpdate(const float& delta_time) = 0;
    virtual void draw() = 0;
    virtual bool DeleteThis() = 0;
	virtual void setField(const ci::vec3 pos) = 0;
	bool ismyobject;
protected:
    ci::vec3 scale;
	int id;
	bool issetting = false;
	bool getIsSetting();
private:
};
}
}