# pragma once

#include <Resource/cSoundManager.h>

#include"../Utilitys/Hirasawa.h"
#include"../Utilitys/TitleCircle.h"
#include"../Utilitys/Haguruma.h"

#include <functional>

namespace User
{
    class SceneTitle
    {
    public:
        SceneTitle( );
        ~SceneTitle( );
    public:
        void update( );
        void draw( );
		std::function<void()> on_cross_se_play;
    private:
        void resize( );
        void select( );
        void beginDrawUI( );
        void drawUI( );
        void endDrawUI( );
    private:
        // スラッシュでの操作を行いますので、インスタンス化します。
        // Yumayo.h
        enum TitleEasing {
            KURIKOBACK, KURIKOBACKDELAY, KURIKO, KURIKODELAY,
            BOSSBACK, BOSSBACKDELAY, BOSS, BOSSDELAY, ONEDELAY,
            CROSS, SLASH, FLASH, UP, LOGOSLIDE, TitleMAX
        };
        enum TexType {
            T_KURIKO, T_BOSS, T_KURIKOBACK, T_BOSSBACK, T_SHURIKEN,
            T_KUNAI, T_SLASH, T_BACKGROUND, T_LOGO, T_MAINKURIKO,
            T_START, T_MAINKUNAI, T_SUBKUNAI, TEXMAX
        };
        enum SE_END {
            SE_CROSS, SE_MAX
        };

        // Yumayo.h のフォントを扱うクラスです。
        float endt_;
        float t_[TitleEasing::TitleMAX];
        int marucount;
        cinder::gl::TextureRef textures[TexType::TEXMAX];
        bool se_ends[SE_END::SE_MAX];
        Sound::cBGM& bgm;
        Sound::cSE& cross_se;
        Sound::cSE& start_se;
        std::vector<Haguruma> haguruma;
        // シーン遷移に使います。
        std::vector<TitleCircle> kuriko_shuriken;
        std::vector<TitleCircle> boss_kunai;
        bool enablenext;
        bool isEnd;
        bool isStartTouch( );
        bool isblackout;
        float iconangle;
        float kunaiangle;
        void drawfade( );
        void drawInKurio( );
        void drawInBoss( );
        void roadTexture( );
        void drawTex( cinder::vec2 pos, cinder::vec2 size, float rotate, cinder::gl::TextureRef tex, cinder::ColorA color );
        void Easingdraw( float& t, float t_time, cinder::vec2 startpos, cinder::vec2 endpos,
                         cinder::vec2 size, float rotate, cinder::gl::TextureRef tex, cinder::ColorA color,
                         Easing::EasingType x_type, Easing::EasingType y_type, bool count = true );
        void crossdraw( );
        bool countend( float t ) {
            return t >= 1.0f;
        }
        void PlaySE(Sound::cSE& se, bool& endflag );
        void pushKurikoShuriken( );
        void pushBossShuriken( );
        void drawSlash( );
        void drawBackground( );
        void drawFlash( );
        void drawLogo( );
        void drawMainKuriko( );
        void drawStartIcon( );
        void skip( );
        void drawKunai( );
        void drawMaru( );
    };
}