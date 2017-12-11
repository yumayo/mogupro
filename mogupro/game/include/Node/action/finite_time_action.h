#pragma once
#include <Node/Action/action.h>
namespace Node
{
namespace Action
{
class finite_time_action : public action
{
public:
    CREATE_H( finite_time_action, float duration );
    virtual ~finite_time_action( ) { }

public:
    bool init( float duration );
public:
    virtual bool is_done( ) override;
    virtual float update( float delta ) override;
public:
    // 新しいバーチャル関数
    virtual void restart( );

protected:
    float _time = 0.0F;
public:
    float set_time( float value );
    float get_time( );

protected:
    float _duration = 0.0F;
public:
    void set_duration( float value );
    float get_duration( );
};
}
}