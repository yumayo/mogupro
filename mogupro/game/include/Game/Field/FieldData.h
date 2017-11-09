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
class cChunkLayer;

using uint = uint32_t;
// tuple‰»ŒŸ“¢
using cChunkRef = std::shared_ptr<Game::Field::cChunk>;
using ChunkMap = std::unordered_map<ci::ivec3, cChunkRef>;
using cBlockRef = std::shared_ptr<Game::Field::cBlock>;

constexpr float
BLOCK_SIZE = 0.25f,
OFFSET_POSITION = BLOCK_SIZE / 2;
constexpr int
CHUNK_SIZE = 16,
CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE,
CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE,
CHUNK_RANGE_X = 2, CHUNK_RANGE_Y = 2, CHUNK_RANGE_Z = 2;
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
