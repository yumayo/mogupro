#pragma once
#include <Resource/cFbxManager.h>

namespace Game
{
namespace Animation
{


class cAnimation
{
public:

    cAnimation();
    ~cAnimation();

    void create( const std::string& name,
                 const bool &is_just_once = false,
                 const bool &is_idle_animation = false );
    void update(const float& delta_time = 0.016f);
    void draw();

    void animationChange( const std::string& name );
    bool isCrrentAnimationEnd();
    void setAnimationStopTime( const std::string& name, const float& stop_time);
    void animationTimeReturn( std::shared_ptr<Resource::Anim> anim );
    void animationTimeReset( std::shared_ptr<Resource::Anim> anim );
    void allReset();
    void setAnimationIncrementTime( const double &time = 0.016 ) { mAnimationIncrementTime = time; }
    std::shared_ptr<Resource::Anim> getActiveAnimation();

private:

    std::unordered_map<std::string, std::shared_ptr<Resource::Anim>> mAnims;
    std::vector<std::string> mJustOnceAnim;
    std::string mIdleAnimationName;
    std::string mActiveName;
    double mAnimationIncrementTime = 1 / 60.0;
};
}
}
