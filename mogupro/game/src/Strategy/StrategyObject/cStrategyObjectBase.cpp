#include<Strategy/StrategyObject/cStrategyObjectBase.h>
namespace Strategy
{
namespace StrategyObject
{
cStrategyObjectBase::cStrategyObjectBase( )
{

};
cStrategyObjectBase::~cStrategyObjectBase( )
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
;
}
}
