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
// tuple������
using cChunkRef = std::shared_ptr<Game::Field::cChunk>;
using ChunkMap = std::unordered_map<ci::ivec3, cChunkRef>;
using cBlockRef = std::shared_ptr<Game::Field::cBlock>;

constexpr float
#if _DEBUG
BLOCK_SIZE = 0.5f,
#else
BLOCK_SIZE = 0.25f,
#endif
OFFSET_POSITION = BLOCK_SIZE / 2;
constexpr int
CHUNK_SIZE = 16,
CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE,
CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE,
#if _DEBUG
CHUNK_RANGE_X = 4, CHUNK_RANGE_Y = 1, CHUNK_RANGE_Z = 4;
#else
CHUNK_RANGE_X = 8, CHUNK_RANGE_Y = 4, CHUNK_RANGE_Z = 16;
#endif
extern cinder::vec3 const WORLD_SIZE;
extern cinder::vec3 const WORLD_COLLISION_SIZE;
extern std::vector<cinder::vec3> const RESPAWN_POINT;
extern std::vector<cinder::quat> const RESPAWN_ROTATION;
extern std::vector<cinder::vec3> const CANNON_POINT;
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
