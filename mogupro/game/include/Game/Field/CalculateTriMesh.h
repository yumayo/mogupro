#pragma once
#include <Game/Field/cBlock.h>
namespace Game
{
namespace Field
{
class cChunk;

// チャンクに必要な情報を計算する
// chunk_size : ブロックの数 ivec3(x, y, z)個
// chunk_num : 二次元配列で並んだチャンクのcell番号
// block_scale : ブロック一個のscale
cChunk calcChunk( const ci::ivec3& chunk_size,
                  const ci::ivec2& chunk_num,
                  const uint& count,
                  const float& block_scale = 1 );

uint multiplyIvec3( ci::ivec3 t ) { return t.x * t.y * t.z; }

template<typename T>
inline T getVectorMaxElement( const std::vector<T>& target )
{
    return *std::max_element( target.begin(), target.end() );
}

std::vector<ci::vec2> getUv( int side_num );

int getIndex( const ci::vec3 & vtx, const ci::vec3 & normal,
              const std::vector<ci::vec3>& vertices,
              const std::vector<ci::vec3>& normals );

}
}
