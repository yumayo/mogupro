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
    // ��: Assets/SE/player/attack.wav �Ȃ�
    // ��: player/attack.wav �Ɠ��͂���B
    Sound::cSE& findSe( std::string const& underAssetsUnderSEUnderPath );
    // ��: Assets/BGM/main/buttle1.wav �Ȃ�
    // ��: main/buttle1.wav �Ɠ��͂���B
    Sound::cBGM& findBgm( std::string const& underAssetsUnderBGMUnderPath );
private:
    void loadSe( );
    void loadBgm( );
private:
    std::map<std::string, Sound::cSE> mSes;
    std::map<std::string, Sound::cBGM> mBgms;
};
}
