
#pragma once
#include "cinder/Vector.h"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include <Resource/cSoundManager.h>
namespace User
{
    enum HagurumaType {
        LEFTHAGURUMA, RIGHTHAGURUMA
    };
    enum UPDATETYPE
    {
        NOTUPDATE, STARTUPDATE, ENDUPDATE

    };
    class Haguruma {
    public:
        Haguruma( );
        Haguruma( cinder::vec2 pos, cinder::vec2 scale, float starttime, float endtime, float rotatespeed, HagurumaType type, bool playsound = true );
        void draw( );
        void update( );
        void setUpdate( UPDATETYPE updatetype ) {
            updatetype_ = updatetype;
        }
        void set_anglescale_t( float anglescale_t_ ) { this->anglescale_t_ = anglescale_t_; }
    private:
        cinder::vec2 pos_;
        cinder::vec2 scale_;
        Sound::cSE& se;
        bool playsound_;
        float starttime_;
        float endtime_;
        cinder::gl::TextureRef tex[6];
        UPDATETYPE updatetype_;
        float rotate[6];
        HagurumaType type_;
        float speed_;
        float rotatespeed_;
        float t_;
        float anglescale_t_;
        void drawTexture( cinder::vec2 trancepos, int number );
        void startUpdate( );
        void endUpdate( );
        void angleUpdate( );
    };
}