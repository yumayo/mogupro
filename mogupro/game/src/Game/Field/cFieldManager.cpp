#include <Game\Field\cFieldManager.h>
namespace Game
{
namespace Field
{
cFieldManager::cFieldManager()
{
    mUnderGround.setup();
}
cFieldManager::~cFieldManager()
{
}
void cFieldManager::update()
{
    mUnderGround.update();
}
void cFieldManager::draw()
{
    mUnderGround.draw();
}
}
}