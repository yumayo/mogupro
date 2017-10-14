#pragma once
#include"Utility\cSingletonAble.h"
#include"ObjectBase.h"
#include"Strategy\StrategyObject\cStrategyObjectBase.h"
#include<list>

namespace Strategy
{
class cStrategyManager : public Utility::cSingletonAble<cStrategyManager>
{
public:
    cStrategyManager( );
    ~cStrategyManager( );
	void setup();
	void draw();
	void update();
	template<class T>
	void CreateStrategyObject(T _object);
private:
	std::list<std::shared_ptr<Strategy::StrategyObject::cStrategyObjectBase>>strategyobjects;
	void deleteObject();
};

template<class T>
inline void cStrategyManager::CreateStrategyObject(T _object)
{
	strategyobjects.push_back(std::make_shared<T>(_object));
}

}
#define STRM Strategy::cStrategyManager::getInstance()