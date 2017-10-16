#pragma once
#include <map>
#include <memory>
#include <Utility/cSingletonAble.h>
#include <cinder/gl/VboMesh.h>
namespace Resource
{
class cObjectManager : public Utility::cSingletonAble<cObjectManager>
{
public:
    cObjectManager( );
    // —á: Assets/OBJ/player.obj ‚È‚ç
    // „: player.obj ‚Æ“ü—Í‚·‚éB
    cinder::gl::VboMeshRef& findObject( std::string const& underAssetsUnderSEUnderPath );
private:
    std::map<std::string, cinder::gl::VboMeshRef> mObjects;
};
}
