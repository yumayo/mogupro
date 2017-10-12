#include <Game\Field\cFieldManager.h>
namespace Game
{
namespace Field
{
cFieldManager::cFieldManager()
{

}
cFieldManager::~cFieldManager()
{
}
void cFieldManager::setup()
{
    mUnderGround.setup();
}
void cFieldManager::update(const int delta_time)
{
    mUnderGround.update();
}
void cFieldManager::draw()
{
    mUnderGround.draw();
}
void cFieldManager::blockBreak(ci::vec3 position, int range)
{
    mUnderGround.blockBreak(position, range);
}
}
}