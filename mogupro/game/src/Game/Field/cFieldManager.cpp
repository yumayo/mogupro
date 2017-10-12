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
void cFieldManager::blockBreak(ci::ivec3 block_num)
{
    mUnderGround.blockBreak(block_num);
}
void cFieldManager::blockBreak(int x, int y, int z)
{
    mUnderGround.blockBreak(ci::ivec3(x, y, z));
}
}
}