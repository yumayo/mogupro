#pragma once
#include <Game/Field/cBlock.h>
using uint = uint32_t;

namespace Game
{
namespace Field
{
class cUnderGround
{
public:

    cUnderGround();
    ~cUnderGround();

    void setup();
    void update();
    void draw();

public: // Player,StrategyÇ™égÇ§ä÷êî

    bool isOutOfRange( const ci::ivec3& cell_num );
    bool blockBreak( const ci::vec3& position, const float& radius );
    ci::vec3 getBlockCenterTopPosition( const ci::vec3& target_position );

private: // Method

    bool createUnderGround();
    bool createMesh();
    void blockMeshBlend( std::shared_ptr<cBlock> b );
    void blockMeshErase( std::shared_ptr<cBlock> b );
    bool blockDigged( const  ci::ivec3& cell_num );
    void blockAllClear();

    ci::ivec3 getCellNumFromPosition( const ci::vec3& position );

private: // Member

    std::vector<std::vector<std::vector<std::shared_ptr<cBlock>>>> blocks;
    int mNum;
    int mHeight;
    float mIntervalOffset;
    float mScale;
    ci::vec3 mOffset;

    std::vector<ci::vec3> mVertices;
    std::vector<uint> mIndices;
    std::vector<ci::vec2> mUv;
    std::vector<ci::vec3> mNormals;
    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVboMesh;
};
}
}