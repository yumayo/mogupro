#pragma once
#include <cinder/app/AppBase.h>
#include <cinder/gl/gl.h>
#include <Collision/cAABBCollider.h>
#include <Game/Field/FieldData.h>
#include <Game/Field/cBreakBlockType.h>
namespace Game
{
namespace Field
{
class cBlock
{
public:

    cBlock();
    cBlock( const ci::vec3& position, const float& scale, const uint & id );
    ~cBlock();

    void setup();

public: // Method

    void clear();
    void toBreak();

public: // Member

    uint mId;
    ci::vec3 mPosition;
    float mScale;
    bool mIsActive = false;
    Collision::cAABBCollider mCollider;
    BlockType mType;
};
}
}