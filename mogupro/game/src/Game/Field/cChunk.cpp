#include <Game/Field/cChunk.h>
namespace Game
{
namespace Field
{
cChunk::cChunk()
{

}
cChunk::~cChunk()
{

}
ci::TriMeshRef cChunk::createTriMesh()
{
    mMesh = ci::TriMesh::create();
    if ( mVertices.size() > 0 )
        mMesh->appendPositions( &mVertices[0], mVertices.size() );
    if ( mIndices.size() > 0 )
        mMesh->appendIndices( &mIndices[0], mIndices.size() );
    if ( mUv.size() > 0 )
        mMesh->appendTexCoords0( &mUv[0], mUv.size() );
    if ( mNormals.size() > 0 )
        mMesh->appendNormals( &mNormals[0], mNormals.size() );
    return mMesh;
}
}
}
