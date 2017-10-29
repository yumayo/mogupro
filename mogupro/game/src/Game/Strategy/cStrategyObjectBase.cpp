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
ci::vec3 cStrategyObjectBase::getScale()
{
    return scale;
}
int cStrategyObjectBase::getId()
{
	return id;
}
bool cStrategyObjectBase::getIsSetting()
{
	return issetting;

}
}
}
