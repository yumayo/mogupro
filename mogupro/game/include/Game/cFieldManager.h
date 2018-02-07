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
    // radius   : 範囲の半径
    bool blockBreak( const ci::vec3& position, const float& radius, const Field::cBreakBlockType& type = Field::BlockType::NORMAL );

    // ブロックを破壊する(ネットワーク用)
    // position : 位置
    // radius   : 範囲の半径
    bool blockBreakNetwork( const ci::vec3& position, const float& radius, const Field::cBreakBlockType& type = Field::BlockType::NORMAL );

    // ブロックを破壊できるか確認する
    // position : 位置
    // radius   : 範囲の半径
    bool isBreakBlock( const ci::vec3& position, const float& radius );

    // 引数(vec3)で近くのブロックの上部中心位置を返す
    // target_position : 対象の位置
    ci::vec3 getBlockHighestPosition( const ci::vec3& target_position );

    // x,zで近くのブロックの位置を、yはブロックの一番高い固定値を返す
    // target_position : 対象の位置
    ci::vec3 getBlockTopPosition( const ci::vec3& target_position );

    // 座標からチャンクの id を返す
    // position : 座標
    // radius   : 範囲の半径
    std::vector<int> getChunkId( const ci::vec3& position, const float& radius );

    bool isUnderCannon( const int& chunk_x, const int& chunk_y, const int& chunk_z );

    // ブロックのリセット
    void blockAllReset();

    // シャットダウン
    void shutdown();

public:

    void addToBreakType( const Field::BlockType& type );
    void clearToBreakType();

public:

    void playBreakBlockSounds( const ci::vec3& position, const std::string& key );

private: // Member
    std::shared_ptr<Field::cUnderGround> mUnderGround;
    std::set<Field::BlockType> mToBreakBlocksType;
};
}