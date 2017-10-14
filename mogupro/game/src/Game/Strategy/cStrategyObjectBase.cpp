#include<Game/Strategy/cStrategyObjectBase.h>
namespace Game
{
namespace Strategy
{
cStrategyObjectBase::cStrategyObjectBase()
{

};
cStrategyObjectBase::~cStrategyObjectBase()
{

}
ci::vec3 cStrategyObjectBase::getPos()
{
    return pos;
}
ci::vec3 cStrategyObjectBase::getScale()
{
    return scale;
}
}
}
