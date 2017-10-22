#pragma once
#include <functional>
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>

namespace Game
{
namespace Field
{
class cBlock;
class cChunk;

using uint = uint32_t;
// tuple‰»ŒŸ“¢
using ChunkMap = std::unordered_map<ci::ivec3, Game::Field::cChunk>;

constexpr int
BLOCK_SIZE = 1,
CHUNK_SIZE = 4,
CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE,
CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
constexpr float
OFFSET_POSITION = 0.5f;

}
}

namespace std
{
template<>
struct hash<ci::ivec3>
{
    size_t operator()( const ci::ivec3& vect ) const noexcept
    {
        std::hash<decltype( vect.x )> hasher;

        auto hash1 = hasher( vect.x );
        auto hash2 = hasher( vect.y );
        auto hash3 = hasher( vect.z );

        return std::hash<decltype( vect.x )>{}( ( hash1 ^ hash2 ^ hash3 ) >> 3 );
    }
};
}


