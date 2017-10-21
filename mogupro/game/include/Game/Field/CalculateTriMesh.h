#pragma once
#include <Game/Field/cBlock.h>
#include <Game/Field/FieldData.h>
namespace Game
{
namespace Field
{
class cChunkHolder;
class cChunk;

// チャンクに必要な情報を計算する
// x, z : 二次元配列で並んだチャンクのcell番号
cChunk calcChunkData( cChunk chunk );

inline uint multiplyIvec3( const ci::ivec3& t ) { return t.x * t.y * t.z; }

template<typename T>
inline T getVectorMaxElement( const std::vector<T>& target )
{
    return *std::max_element( target.cbegin(), target.cend() );
}

std::vector<ci::vec2> getUv( int side_num );

int getIndex( const ci::vec3 & vtx, const ci::vec3 & normal,
              const std::vector<ci::vec3>& vertices,
              const std::vector<ci::vec3>& normals );

}
}
