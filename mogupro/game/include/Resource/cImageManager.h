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
	// 例: Assets/Image/player.png なら
	// ＞: player.png と入力する。
	cinder::gl::TextureRef find( std::string const& underAssetsUnderImagePath );
	void loadOne( );
	bool isFinished( );
	int maxNum( );
	int currentNum( );
private:
	std::map<std::string, cinder::gl::TextureRef> mImages;
	int mCurrentLoadIndex = 0;
	std::vector<std::string> mFilePaths;
};
struct ImageSuppoter
{
	class ImageNotFound : public std::runtime_error
	{
	public:
		ImageNotFound( ) : std::runtime_error( "imageファイルが見つかりません。" )
		{
		}
	};
	cinder::gl::TextureRef operator[]( std::string const& underAssetsUnderIMAGEUnderPath ) const
	{
		try
		{
			return cImageManager::getInstance( )->find( underAssetsUnderIMAGEUnderPath );
		}
		catch ( ... )
		{
			throw ImageNotFound( );
		}
	}
};
extern ImageSuppoter const IMAGE;
}
