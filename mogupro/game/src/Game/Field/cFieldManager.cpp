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
void cFieldManager::update( const float delta_time )
{
    mUnderGround.update();
}
void cFieldManager::draw()
{
    ci::gl::enableFaceCulling( true);
    mUnderGround.draw();
    ci::gl::enableFaceCulling( false );
}
void cFieldManager::blockBreak( ci::vec3 position, float  radius )
{
    mUnderGround.blockBreak( position, radius );
}
}
}