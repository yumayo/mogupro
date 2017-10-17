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
    bool blockBreak( const ci::vec3& position, const float& radius );

    // セルがマップの範囲外かどうか
    // cell_num : セル番号
    bool isOutOfRange( const ci::ivec3& cell_num );

    // 引数(vec3)で近くのブロックの上部中心位置を返す
    // position : 対象の位置
    ci::vec3 getBlockCenterTopPosition( const ci::vec3& target_position );

private: // Member
    Field::cUnderGround mUnderGround;
};
}