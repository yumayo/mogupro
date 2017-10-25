#pragma once
#include <cinder/app/App.h>
#include <Utility/cSingletonAble.h>
#include <Game/Field/cUnderGround.h>

namespace Game
{
class cFieldManager : public ::Utility::cSingletonAble<cFieldManager>
{
public:

    cFieldManager();
    ~cFieldManager();

    void setup();
    void update( const float& delta_time );
    void draw();

public: //Player, Strategy が使う関数

    // ブロックを破壊する
    // position : 位置
    // radius    : 球の半径
    bool blockBreak( const ci::vec3& position, const float& radius = 0 );

    // 引数(vec3)で近くのブロックの上部中心位置を返す
    // target_position : 対象の位置
    ci::vec3 getBlockHighestPosition( const ci::vec3& target_position );

    // x,zで近くのブロックの位置を、yはブロックの一番高い固定値を返す
    // target_position : 対象の位置
    ci::vec3 getBlockTopPosition( const ci::vec3& target_position );

private: // Member
    Field::cUnderGround mUnderGround;
};
}