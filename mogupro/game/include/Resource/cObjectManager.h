#pragma once
#include <map>
#include <memory>
#include <Utility/cSingletonAble.h>
#include <cinder/gl/VboMesh.h>
#include <vector>
#include <string>
namespace Resource
{
class cObjectManager : public Utility::cSingletonAble<cObjectManager>
{
public:
    cObjectManager( );
    // ó·: Assets/OBJ/player.obj Ç»ÇÁ
    // ÅÑ: player.obj Ç∆ì¸óÕÇ∑ÇÈÅB
    cinder::gl::VboMeshRef& findObject( std::string const& underAssetsUnderSEUnderPath );
	void loadOne( );
	bool isFinished( );
	int maxNum( );
	int currentNum( );
private:
    std::map<std::string, cinder::gl::VboMeshRef> mObjects;
	int mCurrentLoadIndex = 0;
	std::vector<std::string> mFilePaths;
};
}
