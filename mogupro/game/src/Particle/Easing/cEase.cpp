#include "Particle/Easing/cEase.h"

EaseOrigin::EaseOrigin( float begin, float end, float end_frame, EaseType type )
{
    setup( begin, end, end_frame, type );
}

void EaseOrigin::setup( float begin, float end, float end_frame, EaseType type )
{
    begin_ = begin;
    end_ = end;
    end_frame_ = static_cast<float>( end_frame );
    easefunc_ = getEaseFunc( type );
    count_ = 0.0f;
}

void EaseOrigin::update( const float& delta_time )
{
    count_ += delta_time * 60;

    if ( count_ > end_frame_ )
        count_ = end_frame_;
}

float EaseOrigin::currentTargetValue()
{
    if ( !easefunc_ )
        return begin_;
    return easefunc_( count_ / end_frame_, begin_, end_ );
}

bool EaseOrigin::isDone()
{
    return count_ >= end_frame_;
}

////////////////////////////////////////////////////////////////////////////////////////////
// RunEase
////////////////////////////////////////////////////////////////////////////////////////////

RunEase::RunEase()
{
    is_stop = false;
    is_loop = false;
}

void RunEase::update( const float& delta_time )
{
    if ( is_stop )return;
    if ( ease_accum.empty() )return;
    action( delta_time );
}

void RunEase::add( float & target, float end, float end_frame, EaseType ease_type )
{
    target_ = &target;
    ease_accum.push_back( EaseOrigin( target, end, end_frame, ease_type ) );
}

void RunEase::addWait( float & target, float wait )
{
    target_ = &target;
    ease_accum.push_back( EaseOrigin( target, target, wait, EaseType::NONE ) );
}

void RunEase::clear()
{
    ease_accum.clear();
}

void RunEase::action( const float& delta_time )
{
    auto ease_begin = ease_accum.begin();
    if ( ease_begin->isDone() )
    {
        ease_begin++;
        if ( ease_begin != ease_accum.end() )
            ease_begin->setBegin( *target_ );
        if ( is_loop )
            loop();
        else
            pop();
    }
    else
    {
        *target_ = ease_begin->currentTargetValue();
        ease_begin->update( delta_time );
    }
}

void RunEase::pop()
{
    ease_accum.pop_front();
}

void RunEase::loop()
{
    EaseOrigin temp = ease_accum.front();
    temp.countReset();
    ease_accum.pop_front();
    ease_accum.push_back( temp );
}

////////////////////////////////////////////////////////////////////////////////////////////
// Ease イージングを管理するクラス
////////////////////////////////////////////////////////////////////////////////////////////

void cEase::update( const float& delta_time )
{
    for ( auto it = ease.begin(); it != ease.end();)
    {
        if ( it->second.isEaseEnd() )
        {
            it = ease.erase( it );
            continue;
        }
        it->second.update( delta_time );
        it++;
    }
}

void cEase::add( float & target, const float& end, float end_frame, EaseType ease_type )
{
    ease[&target].add( target, end, end_frame, ease_type );
}

void cEase::add( ci::vec3 & target, const ci::vec3& end, float end_frame, EaseType ease_type )
{
    ease[&target.x].add( target.x, end.x, end_frame, ease_type );
    ease[&target.y].add( target.y, end.y, end_frame, ease_type );
    ease[&target.z].add( target.z, end.z, end_frame, ease_type );
}

void cEase::add( ci::vec2 & target, const ci::vec2 & end, float end_frame, EaseType ease_type )
{
    ease[&target.x].add( target.x, end.x, end_frame, ease_type );
    ease[&target.y].add( target.y, end.y, end_frame, ease_type );
}

void cEase::wait( float & target, float wait_count )
{
    ease[&target].addWait( target, wait_count );
}

void cEase::wait( ci::vec3 & target, float wait_count )
{
    ease[&target.x].addWait( target.x, wait_count );
    ease[&target.y].addWait( target.y, wait_count );
    ease[&target.z].addWait( target.z, wait_count );
}

void cEase::wait( ci::vec2 & target, float wait_count )
{
    ease[&target.x].addWait( target.x, wait_count );
    ease[&target.y].addWait( target.y, wait_count );
}

void cEase::stop( float & target, const bool& is_enable )
{
    ease[&target].setIsStop( is_enable );
}

void cEase::stop( ci::vec3 & target, const bool& is_enable )
{
    ease[&target.x].setIsStop( is_enable );
    ease[&target.y].setIsStop( is_enable );
    ease[&target.z].setIsStop( is_enable );
}

void cEase::stop( ci::vec2 & target, const bool& is_enable )
{
    ease[&target.x].setIsStop( is_enable );
    ease[&target.y].setIsStop( is_enable );
}


void cEase::loop( float & target, const bool& is_enable )
{
    ease[&target].setIsLoop( is_enable );
}

void cEase::loop( ci::vec3 & target, const bool& is_enable )
{
    ease[&target.x].setIsLoop( is_enable );
    ease[&target.y].setIsLoop( is_enable );
    ease[&target.z].setIsLoop( is_enable );
}

void cEase::loop( ci::vec2 & target, const bool& is_enable )
{
    ease[&target.x].setIsLoop( is_enable );
    ease[&target.y].setIsLoop( is_enable );
}

void cEase::kill( float & target )
{
    ease[&target].clear();
}

void cEase::kill( ci::vec3 & target )
{
    ease[&target.x].clear();
    ease[&target.y].clear();
    ease[&target.z].clear();
}

void cEase::kill( ci::vec2 & target )
{
    ease[&target.x].clear();
    ease[&target.y].clear();
}

bool cEase::isEaseEnd( float & target )
{
    return ease[&target].isEaseEnd();
}

bool cEase::isEaseEnd( ci::vec3 & target )
{
    if ( ease.find( &target.x ) == ease.end() ||
         ease.find( &target.y ) == ease.end() ||
         ease.find( &target.z ) == ease.end() )
        return true;

    return ease[&target.x].isEaseEnd()
        && ease[&target.y].isEaseEnd()
        && ease[&target.z].isEaseEnd();
}

void cEase::allClear()
{
    for ( auto & it : ease )
    {
        it.second.clear();
    }
    ease.clear();
}