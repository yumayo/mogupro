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
    void toRevival();
    ci::vec3 getPosition();
    BlockType getType();
    BlockType setType( const BlockType & type );
    int getIndex();
    bool isActive();

public: // Member

    int mId;
    Collision::cAABBCollider mCollider;
    BlockType mType;
};
}
}