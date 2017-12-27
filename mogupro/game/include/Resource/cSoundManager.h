#pragma once
#include <map>
#include <memory>
#include <Utility/cSingletonAble.h>
#include <Sound/cSE.h>
#include <Sound/cBGM.h>
#include <vector>
#include <string>
namespace Resource
{
class cSoundManager : public Utility::cSingletonAble<cSoundManager>
{
public:
    cSoundManager( );
    // 例: Assets/SE/player/attack.wav なら
    // ＞: player/attack.wav と入力する。
    Sound::cSE& findSe( std::string const& underAssetsUnderSEUnderPath );
    // 例: Assets/BGM/main/buttle1.wav なら
    // ＞: main/buttle1.wav と入力する。
    Sound::cBGM& findBgm( std::string const& underAssetsUnderBGMUnderPath );
	void loadOne( );
	bool isFinished( );
	int maxNum( );
	int currentNum( );
private:
	void loadOneSe( );
	bool isSeFinished( );
	void loadOneBgm( );
	bool isBgmFinished( );
private:
    std::map<std::string, Sound::cSE> mSes;
    std::map<std::string, Sound::cBGM> mBgms;
	std::vector<std::string> mSeFilePaths;
	std::vector<std::string> mBgmFilePaths;
	int mSeCurrentLoadIndex = 0;
	int mBgmCurrentLoadIndex = 0;
};
}
