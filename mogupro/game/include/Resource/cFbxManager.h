#pragma once
#ifdef _MSC_VER
// TIPS:FBX SDKはDLLを使う
#define FBXSDK_SHARED
#endif
#include <cinder/app/App.h>
#include <cinder/Camera.h>
#include <cinder/Arcball.h>
#include <cinder/TriMesh.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/ip/Flip.h>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <fbxsdk.h>                // FBX SDK
#include <cinder/ImageIo.h>
#include <Utility/cSingletonAble.h>
namespace Resource
{
#ifdef _MSC_VER
// cp932 → UTF-8
std::string getUTF8Path( const std::string& path );
#endif

// FBX SDKサンプルから拝借
// Get the geometry offset to a node. It is never inherited by the children.
FbxAMatrix GetGeometry( FbxNode* pNode );

// FbxAMatrix → Matrix44
glm::mat4 getMatrix44( const FbxAMatrix& matrix );

// スケルタルアニメーション用の情報
struct Skin
{
    bool has_skins;
    std::vector<std::vector<float>> weights;
    std::vector<FbxAMatrix> base_inv;
};

// 表示用のメッシュ
struct Mesh
{
    ci::TriMesh tri_mesh;
    Skin skin;
    ci::TriMesh deformed_mesh;
};

// FBXから取り出した情報を保持しておくための定義
// マテリアル
struct Material
{
    //ci::gl::Material material;
    boost::optional<ci::gl::Texture> texture;
};

struct Anim
{
    // アニメーションの経過時間
    double animation_time = 0.0;
    double animation_start;
    double animation_stop;

    // シーン内のアニメーション数
    int animation_stack_count;
    int current_animation_stack = 0;

    Anim() :
        animation_time( 0.0 ),
        animation_start( 0.0 ),
        animation_stop( 0.0 ),
        animation_stack_count( 0 ),
        current_animation_stack( 0 )
    {

    }

    static Anim zero() { return Anim(); }
};

class cFbx
{
public:

    cFbx();
    ~cFbx();

    void setup( FbxManager *manager, const std::string& filename );
    void update();
    void draw();
    void draw( const double &animation_time );

    void createAnimation( Anim &anim );

private:

    void drawFbx( FbxNode *node, FbxTime &time );
    Mesh createMesh( FbxMesh *mesh );
    Skin createSkin( FbxMesh *mesh );
    ci::TriMesh getDeformedTriMesh( FbxMesh *mesh,
                                    const Mesh& src_mesh,
                                    FbxAMatrix& parent_matrix,
                                    FbxTime& time );
    const ci::TriMesh& getTriMesh( FbxMesh *mesh,
                                   Mesh& src_mesh,
                                   FbxAMatrix &parent_matrix,
                                   FbxTime& time );

    void setAnimation( const int index, Anim &anim );

    FbxScene* scene;
    FbxNode* root_node;

    // 表示物の情報を名前で管理
    std::map<std::string, Mesh> meshes;
    std::map<std::string, Material> materials;

};

class cFbxManager : public Utility::cSingletonAble<cFbxManager>
{
public:

    cFbxManager();
    ~cFbxManager();

    void setup();
    void update();
    void draw();
    void draw( const std::string &name, const double &animation_time );
    void create( const std::string &name );
    void createAnimation( const std::string &name,
                          Anim &anim );

private:

    // FBX情報
    FbxManager* manager;
    std::unordered_map<std::string, std::shared_ptr<cFbx>> models;
};
}
