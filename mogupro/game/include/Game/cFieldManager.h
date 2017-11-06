#pragma once
#include <cinder/app/App.h>
#include <Utility/cSingletonAble.h>
#include <Game/Field/cUnderGround.h>
#include <Game/Field/cBreakBlockType.h>

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
    bool blockBreak( const ci::vec3& position, const float& radius, const Field::cBreakBlockType& type = Field::BlockType::NORMAL );

    // ブロックを破壊する(ネットワーク用)
    // position : 位置
    // radius    : 球の半径
    bool blockBreakNetwork( const ci::vec3& position, const float& radius, const Field::cBreakBlockType& type = Field::BlockType::NORMAL );

    // 引数(vec3)で近くのブロックの上部中心位置を返す
    // target_position : 対象の位置
    ci::vec3 getBlockHighestPosition( const ci::vec3& target_position );

    // x,zで近くのブロックの位置を、yはブロックの一番高い固定値を返す
    // target_position : 対象の位置
    ci::vec3 getBlockTopPosition( const ci::vec3& target_position );

    // 影のシェーダー
    // チャンク一個一個にシェーダーを作る予定
    void setPointLight( ci::gl::GlslProgRef shader, ci::vec3 position );
    void erasePointLight( ci::vec3 position );

    // シャットダウン
    void shutdown();

private: // Member
    std::shared_ptr<Field::cUnderGround> mUnderGround;
};
}