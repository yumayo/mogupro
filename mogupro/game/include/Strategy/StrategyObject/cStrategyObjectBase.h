#pragma once
#include"ObjectBase.h"
namespace Strategy
{
namespace StrategyObject
{
class cStrategyObjectBase : public ObjectBase
{
public:
    cStrategyObjectBase( );
    virtual ~cStrategyObjectBase( );
	ci::vec3 getPos();
	ci::vec3 getScale();
	virtual void setup() = 0;
	virtual void update(const float& delta_time) = 0;
	virtual void draw() = 0;
	virtual bool DeleteThis() = 0;
protected:
	ci::vec3 pos;
	ci::dvec3 scale;
private:
};
}
}
