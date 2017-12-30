#pragma once
#include <map>
#include <memory>
#include <Utility/cSingletonAble.h>
#include <cinder/gl/Texture.h>
#include <boost/optional.hpp>
namespace Resource
{
class cImageManager : public Utility::cSingletonAble<cImageManager>
{
public:
	cImageManager( );
	// —á: Assets/Image/player.png ‚È‚ç
	// „: player.png ‚Æ“ü—Í‚·‚éB
	cinder::gl::TextureRef& findObject( std::string const& underAssetsUnderImagePath );
	void loadOne( );
	bool isFinished( );
	int maxNum( );
	int currentNum( );
private:
	std::map<std::string, cinder::gl::TextureRef> mImages;
	int mCurrentLoadIndex = 0;
	std::vector<std::string> mFilePaths;
};
}
