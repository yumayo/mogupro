#include <Game/Animation/cAnimation.h>
using namespace ci;
using namespace ci::app;
using namespace Resource;

namespace Game
{
namespace Animation
{
std::shared_ptr<Anim> errAnim = std::make_shared<Anim>();
cAnimation::cAnimation() :
    mActiveName( "" )
    , mIdleAnimationName( "" )
{

}

cAnimation::~cAnimation()
{

}

void cAnimation::create( const std::string & name,
                         const bool &is_just_once,
                         const bool &is_idle_animation )
{
    if ( mAnims.find( name ) != mAnims.end() )
        return;

 // cFbxManager::getInstance()->create( name );

    std::shared_ptr<Anim> anim = std::make_shared<Anim>();
    cFbxManager::getInstance()->createAnimation( name, *anim );
    animationTimeReset( anim );

    if ( is_just_once == true )
        mJustOnceAnim.push_back( name );
    if ( is_idle_animation == true )
        mIdleAnimationName = name;

    mAnims.insert( std::make_pair( name, std::move( anim ) ) );
}
void cAnimation::update()
{
    if ( mAnims.find( mActiveName ) == mAnims.end() )
        return;

    auto anim = getActiveAnimation();
    // アニメーション経過時間を進める
    anim->animation_time += mAnimationIncrementTime;

    if ( isCrrentAnimationEnd() )
    {
        // 一度だけ実行するアニメーションだった場合、デフォルトのアニメーションに戻す
        if ( std::any_of( mJustOnceAnim.begin(), mJustOnceAnim.end(),
                          [&]( std::string t ) { return t == mActiveName; } ) &&
             mIdleAnimationName != "" )
        {
            animationChange( mIdleAnimationName );
        }
        else
            animationTimeReturn( anim );
    }
}

void cAnimation::draw()
{
    cFbxManager::getInstance()->draw( mActiveName,
                                      getActiveAnimation()->animation_time );
}

void cAnimation::animationChange( const std::string & name )
{
    if ( mAnims.find( name ) == mAnims.end() )
        return;
    mActiveName = name;
    allReset();
}

bool cAnimation::isCrrentAnimationEnd()
{
    auto anim = getActiveAnimation();
    return anim->animation_time > anim->animation_stop;
}

void cAnimation::setAnimationStopTime( const std::string& name, const float& stop_time )
{
    if ( mAnims.find( name ) == mAnims.end() )
        return;
    mAnims[name]->animation_stop = stop_time;
}

void cAnimation::animationTimeReturn( std::shared_ptr<Resource::Anim> anim )
{
    anim->animation_time = anim->animation_start + anim->animation_time - anim->animation_stop;
}

void cAnimation::animationTimeReset( std::shared_ptr<Anim> anim )
{
    anim->animation_time = anim->animation_start;
}

void cAnimation::allReset()
{
    for ( auto& it : mAnims )
        animationTimeReset( it.second );
}

std::shared_ptr<Resource::Anim> cAnimation::getActiveAnimation()
{
    if ( mAnims.find( mActiveName ) == mAnims.end() )
        return errAnim;
    return mAnims[mActiveName];
}

}
}
