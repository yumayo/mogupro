#pragma once
#include <map>
#include <memory>
#include <Utility/cSingletonAble.h>
#include <Sound/cSE.h>
#include <Sound/cBGM.h>
namespace Resource
{
class cSoundManager : public Utility::cSingletonAble<cSoundManager>
{
public:
    cSoundManager( );
    // —á: Assets/SE/player/attack.wav ‚È‚ç
    // „: player/attack.wav ‚Æ“ü—Í‚·‚éB
    Sound::cSE& findSe( std::string const& underAssetsUnderSEUnderPath );
    // —á: Assets/BGM/main/buttle1.wav ‚È‚ç
    // „: main/buttle1.wav ‚Æ“ü—Í‚·‚éB
    Sound::cBGM& findBgm( std::string const& underAssetsUnderBGMUnderPath );
private:
    void loadSe( );
    void loadBgm( );
private:
    std::map<std::string, Sound::cSE> mSes;
    std::map<std::string, Sound::cBGM> mBgms;
};
}
