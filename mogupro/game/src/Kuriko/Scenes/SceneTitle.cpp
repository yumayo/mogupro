# include "SceneTitle.h"
#include <Utility/cInput.h>
#include <cinder/Rand.h>
#include <Resource/cImageManager.h>
using namespace ci;

namespace User
{
    SceneTitle::SceneTitle( )
        : isEnd( false )
        , endt_( 0.0f )
        , isblackout( false )
		, bgm(Resource::BGM["kuriko/mainbgm1.wav"])
		, cross_se(Resource::SE["kuriko/titlecross.wav"])
		, start_se(Resource::SE["kuriko/titlestart.wav"])
    {
        roadTexture( );

        for ( int i = 0; i < TitleEasing::TitleMAX; i++ ) {
            t_[i] = 0.0f;
        }
        for ( int i = 0; i < SE_END::SE_MAX; i++ ) {
            se_ends[i] = false;
        }
        pushKurikoShuriken( );
        pushBossShuriken( );
        //bgm.setLooping( true );
        //bgm.setGain( 0.7f );
        //bgm.play( );
        enablenext = false;
        iconangle = 0.0f;
        kunaiangle = 0.0f;
        haguruma.push_back( Haguruma( vec2( cinder::app::getWindowWidth( )*0.1f, cinder::app::getWindowHeight( )*0.1f ),
                                      vec2( 400, 400 ), 0.3f, 1.5f, 4.f, HagurumaType::LEFTHAGURUMA, false ) );
        marucount = 0;
    }

    SceneTitle::~SceneTitle( )
    {
        //bgm.stop( );
    }

    void SceneTitle::resize( )
    {

    }

    void SceneTitle::update( )
    {
        // スラッシュとの当たり判定を取るには、円の中心ポジションと半径を入れます。
        //if (slashInput.IsHitCircle(startButtonPosition, 125)) isEnd = true;
        if ( !isblackout ) {
            isblackout = isStartTouch( );
        }
        skip( );
        if ( countend( t_[TitleEasing::TitleMAX - 1] ) )
            enablenext = true;

        if ( isblackout ) {
            haguruma[0].setUpdate( UPDATETYPE::ENDUPDATE );
            Easing::tCount( endt_, 2.0f );
        }

        if ( countend( t_[LOGOSLIDE] ) && ( endt_ <= 0.f ) )
            haguruma[0].setUpdate( UPDATETYPE::STARTUPDATE );

        if ( endt_ >= 1.0f )isEnd = true;
        if ( countend( t_[ONEDELAY] ) ) {
			if (on_cross_se_play) on_cross_se_play();
            PlaySE( cross_se, se_ends[SE_END::SE_CROSS] );
        }
        for ( int i = 0; i < kuriko_shuriken.size( ); i++ ) {
            kuriko_shuriken[i].update( 15.f );
        }
        if ( countend( t_[KURIKODELAY] ) ) {
            for ( int i = 0; i < boss_kunai.size( ); i++ ) {
                boss_kunai[i].update( -15.f );
            }
        }
        haguruma[0].update( );

    }
    void SceneTitle::draw( )
    {
		ci::gl::ScopedGlslProg glsl(cinder::gl::getStockShader(cinder::gl::ShaderDef().texture().color()));
		ci::gl::ScopedBlendAlpha alpha;
		ci::gl::ScopedMatrices m;
		cinder::gl::setMatricesWindow(ci::app::getWindowSize());
		ci::gl::ScopedColor col(Color::white());
        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }
    void SceneTitle::select( )
    {

    }

    void SceneTitle::beginDrawUI( )
    {
		
    }
    void SceneTitle::drawUI( )
    {
        //drawTexture( ci::vec2( 0, 0 ), cinder::app::getWindowSize( ), GData::FindTexture( "Textures/Title/title.png" ), ci::ColorA::white( ) );

        // drawTextureでアルファブレンディングをOFFにしているので。
		ci::gl::ScopedBlendAlpha alpha;

        drawInKurio( );
        drawInBoss( );
        crossdraw( );
        drawSlash( );
        drawBackground( );
        drawMainKuriko( );
        //haguruma[0].draw( );
        drawLogo( );
        drawKunai( );
        //drawStartIcon( );
        drawFlash( );
        drawfade( );

    }
    void SceneTitle::endDrawUI( )
    {

    }

    void SceneTitle::skip( )
    {
        if ( ENV->pushKey( app::MouseEvent::LEFT_DOWN ) )
        {
            enablenext = true;
            for ( int i = 0; i < TitleEasing::TitleMAX; i++ ) {
                t_[i] = true;
            }
        }
    }
    void SceneTitle::drawKunai( )
    {
        if ( !countend( t_[LOGOSLIDE] ) )return;

		vec2 endpos = cinder::app::getWindowSize() / 2;
		vec2 size = cinder::app::getWindowSize();

		{
			gl::pushModelView();
			gl::translate(endpos + vec2(0, 15 * sin(kunaiangle)));
			gl::ScopedTextureBind tex(textures[T_MAINKUNAI]);
			gl::color(ColorA(1, 1, 1, 1));
			gl::drawSolidRect(Rectf(-size / 2.f, size / 2.f));
			gl::popModelView();
			kunaiangle += 0.05;
		}
        
		{
			gl::pushModelView();
			gl::translate(endpos + vec2(0, 60 * sin(kunaiangle)));
			gl::ScopedTextureBind tex(textures[T_SUBKUNAI]);
			gl::color(ColorA(1, 1, 1, 1));
			gl::drawSolidRect(Rectf(-size / 2.f, size / 2.f));
			gl::popModelView();
			kunaiangle += 0.05;
		}
    }
    void SceneTitle::drawMaru( )
    {
    }
	// ここでシーンチェンジ
    bool SceneTitle::isStartTouch( )
    {
        /*if ( ENV->pressKey( app::MouseEvent::LEFT_DOWN ) )
        {
            gameStartGauge.Update( true );
            marucount++;
        }


        if ( !isAnyInput ) {
            gameStartGauge.Update( false );
            marucount = 0;
        }


        if ( marucount >= gameStartGauge.GetLoadFrame( ) ) {
            start_se->Play( );
            return true;
        }*/
        return false;
    }
    void SceneTitle::drawfade( )
    {
        gl::pushModelView( );
        gl::translate( cinder::app::getWindowSize( ) / 2 );
        gl::color( ColorA( 0, 0, 0, endt_ ) );
        gl::drawSolidRect( Rectf( -cinder::app::getWindowSize( ) / 2, cinder::app::getWindowSize( ) / 2 ) );
        gl::popModelView( );
    }
    void SceneTitle::drawInKurio( )
    {
        if ( countend( t_[TitleEasing::ONEDELAY] ) )return;
        float posx = cinder::app::getWindowWidth( ) / 2 + cinder::app::getWindowWidth( ) / 6.f;
        Easingdraw( t_[KURIKOBACK], 0.6f, vec2( posx, -1.5f*cinder::app::getWindowHeight( ) / 2 ),
                    vec2( cinder::app::getWindowWidth( ) / 2 + cinder::app::getWindowWidth( ) / 6.f, cinder::app::getWindowHeight( ) / 2 ), vec2( cinder::app::getWindowWidth( ) / 3, cinder::app::getWindowHeight( ) ),
                    0.f, textures[TexType::T_KURIKOBACK],
                    ColorA( 1, 1, 1, 1 ), Easing::EasingType::Linear, Easing::EasingType::CubicIn );

        if ( countend( t_[KURIKOBACK] ) )
            Easing::tCount( t_[KURIKOBACKDELAY], 0.4f );
        for ( int i = 0; i < kuriko_shuriken.size( ); i++ ) {
            kuriko_shuriken[i].draw( posx );
        }
        if ( countend( t_[KURIKOBACKDELAY] ) ) {
            if ( !countend( t_[KURIKO] ) ) {
                Easingdraw( t_[KURIKO], 0.6f, vec2( posx, -1.5f*cinder::app::getWindowHeight( ) / 2 ),
                            vec2( posx, cinder::app::getWindowHeight( ) / 2 - 150 ), vec2( cinder::app::getWindowWidth( ) / 2, cinder::app::getWindowHeight( ) ),
                            0.f, textures[TexType::T_KURIKO],
                            ColorA( 1, 1, 1, 1 ), Easing::EasingType::Linear, Easing::EasingType::CubicIn );
            }
            else {
                Easingdraw( t_[KURIKODELAY], 1.3f, vec2( posx, cinder::app::getWindowHeight( ) / 2 - 150 ),
                            vec2( posx, cinder::app::getWindowHeight( ) / 2 ), vec2( cinder::app::getWindowWidth( ) / 2, cinder::app::getWindowHeight( ) ),
                            0.f, textures[TexType::T_KURIKO],
                            ColorA( 1, 1, 1, 1 ), Easing::EasingType::Linear, Easing::EasingType::Linear );
            }

        }
    }
    void SceneTitle::drawInBoss( )
    {
        if ( countend( t_[TitleEasing::ONEDELAY] ) )return;
        if ( !countend( t_[KURIKODELAY] ) )return;
        float posx = cinder::app::getWindowWidth( ) / 2 - cinder::app::getWindowWidth( ) / 6.f;
        Easingdraw( t_[BOSSBACK], 0.6f, vec2( posx, 1.75f*cinder::app::getWindowHeight( ) ),
                    vec2( posx, cinder::app::getWindowHeight( ) / 2 ), vec2( cinder::app::getWindowWidth( ) / 3, cinder::app::getWindowHeight( ) ),
                    0.f, textures[TexType::T_BOSSBACK],
                    ColorA( 1, 1, 1, 1 ), Easing::EasingType::Linear, Easing::EasingType::CubicIn );

        for ( int i = 0; i < boss_kunai.size( ); i++ ) {
            boss_kunai[i].draw( posx );
        }

        if ( countend( t_[BOSSBACK] ) )
            Easing::tCount( t_[BOSSBACKDELAY], 1.0f );
        if ( countend( t_[BOSSBACKDELAY] ) ) {
            if ( !countend( t_[BOSS] ) ) {
                Easingdraw( t_[BOSS], 0.6f, vec2( cinder::app::getWindowWidth( ) / 2 - cinder::app::getWindowWidth( ) / 6.f, 1.75f*cinder::app::getWindowHeight( ) ),
                            vec2( cinder::app::getWindowWidth( ) / 2 - cinder::app::getWindowWidth( ) / 6.f, cinder::app::getWindowHeight( ) / 2 + 150 ), vec2( cinder::app::getWindowWidth( ) / 2, cinder::app::getWindowHeight( ) ),
                            0.f, textures[TexType::T_BOSS],
                            ColorA( 1, 1, 1, 1 ), Easing::EasingType::Linear, Easing::EasingType::CubicIn );
            }
            else {
                Easingdraw( t_[BOSSDELAY], 1.3f, vec2( cinder::app::getWindowWidth( ) / 2 - cinder::app::getWindowWidth( ) / 6.f, cinder::app::getWindowHeight( ) / 2 + 150 ),
                            vec2( cinder::app::getWindowWidth( ) / 2 - cinder::app::getWindowWidth( ) / 6.f, cinder::app::getWindowHeight( ) / 2 ), vec2( cinder::app::getWindowWidth( ) / 2, cinder::app::getWindowHeight( ) ),
                            0.f, textures[TexType::T_BOSS],
                            ColorA( 1, 1, 1, 1 ), Easing::EasingType::Linear, Easing::EasingType::Linear );
            }
        }
        if ( countend( t_[BOSSDELAY] ) )
            Easing::tCount( t_[ONEDELAY], 0.55f );
    }

    void SceneTitle::drawTex( cinder::vec2 pos, cinder::vec2 size, float rotate, cinder::gl::TextureRef tex, cinder::ColorA color )
    {
        gl::pushModelView( );
        gl::translate( pos );
        gl::rotate( rotate );
        gl::scale( size );
        gl::color( color );
		gl::ScopedTextureBind tb(tex);
        gl::drawSolidRect( Rectf( -vec2( 0.5f, 0.5f ), vec2( 0.5f, 0.5f ) ) );
        gl::popModelView( );
    }

    void SceneTitle::Easingdraw( float & t, float t_time, cinder::vec2 startpos, cinder::vec2 endpos, cinder::vec2 size, float rotate, cinder::gl::TextureRef tex, cinder::ColorA color, Easing::EasingType x_type, Easing::EasingType y_type, bool count )
    {
        if ( count )
            Easing::tCount( t, t_time );
        vec2 pos;
        pos.x = Easing::getEasing[x_type]( t, startpos.x, endpos.x );
        pos.y = Easing::getEasing[y_type]( t, startpos.y, endpos.y );
        drawTex( pos, size, rotate, tex, color );
    }

    void SceneTitle::crossdraw( )
    {
        if ( !countend( t_[TitleEasing::ONEDELAY] ) )return;

        float posx = cinder::app::getWindowWidth( ) / 2 + cinder::app::getWindowWidth( ) / 6.f;
        float endposx = -1.5f * cinder::app::getWindowWidth( ) / 2;
        float bossposx = cinder::app::getWindowWidth( ) / 2 - cinder::app::getWindowWidth( ) / 6.f;
        float bossendposx = 1.75f * cinder::app::getWindowWidth( );
        float posy = cinder::app::getWindowHeight( ) / 2;
        float kunaipos;
        float shurikenpos;
        Easing::EasingType type = Easing::EasingType::CubicInOut;
        kunaipos = Easing::getEasing[type]( t_[CROSS], posx, endposx );
        shurikenpos = Easing::getEasing[type]( t_[CROSS], bossposx, bossendposx );
        Easingdraw( t_[CROSS], 1.0f, vec2( posx, posy ),
                    vec2( endposx, posy ), vec2( cinder::app::getWindowWidth( ) / 3, cinder::app::getWindowHeight( ) ),
                    0.f, textures[TexType::T_KURIKOBACK],
                    ColorA( 1, 1, 1, 1 ), type, Easing::EasingType::Linear );
        Easingdraw( t_[CROSS], 0.7f, vec2( bossposx, posy ),
                    vec2( bossendposx, posy ), vec2( cinder::app::getWindowWidth( ) / 3, cinder::app::getWindowHeight( ) ),
                    0.f, textures[TexType::T_BOSSBACK],
                    ColorA( 1, 1, 1, 1 ), type, Easing::EasingType::Linear, false );

        for ( int i = 0; i < kuriko_shuriken.size( ); i++ ) {
            kuriko_shuriken[i].draw( shurikenpos );
        }
        for ( int i = 0; i < boss_kunai.size( ); i++ ) {
            boss_kunai[i].draw( shurikenpos );
        }

        Easingdraw( t_[CROSS], 0.7f, vec2( posx, posy ),
                    vec2( endposx, posy ), vec2( cinder::app::getWindowWidth( ) / 2, cinder::app::getWindowHeight( ) ),
                    0.f, textures[TexType::T_KURIKO],
                    ColorA( 1, 1, 1, 1 ), type, Easing::EasingType::Linear, false );
        Easingdraw( t_[CROSS], 0.7f, vec2( bossposx, posy ),
                    vec2( bossendposx, posy ), vec2( cinder::app::getWindowWidth( ) / 2, cinder::app::getWindowHeight( ) ),
                    0.f, textures[TexType::T_BOSS],
                    ColorA( 1, 1, 1, 1 ), type, Easing::EasingType::Linear, false );
    }

    void SceneTitle::PlaySE( Sound::cSE& se, bool & endflag )
    {
        if ( !endflag ) {
            se.play( );
            endflag = true;
        }
    }

    void SceneTitle::pushKurikoShuriken( )
    {
        for ( int i = 0; i < 30; i++ ) {
            vec2 pos = vec2( randFloat( -150, 150 ), ( i*-350 ) - 600 );
            vec2 size = vec2( 200, 200 );
            kuriko_shuriken.push_back( TitleCircle( pos, size, textures[T_KUNAI] ) );
        }
    }

    void SceneTitle::pushBossShuriken( )
    {
        for ( int i = 0; i < 30; i++ ) {
            vec2 pos = vec2( randFloat( -150, 150 ), cinder::app::getWindowHeight( ) + ( i*+350 ) + 600 );
            vec2 size = vec2( 200, 200 );
            boss_kunai.push_back( TitleCircle( pos, size, textures[T_SHURIKEN] ) );
        }
    }

    void SceneTitle::drawSlash( )
    {
        if ( !countend( t_[ONEDELAY] ) )return;
        if ( countend( t_[CROSS] ) )return;

        float alfa = Easing::getEasing[Easing::CubicIn]( t_[CROSS], 1.0f, 0.f );
        vec2 size( cinder::app::getWindowWidth( )*1.3, cinder::app::getWindowHeight( ) / 4.f );

		{
			gl::pushModelView();
			gl::translate(cinder::app::getWindowSize() / 2);
			gl::rotate(-45);
			gl::color(ColorA(1, 0, 0, alfa));
			gl::ScopedTextureBind tb(textures[T_SLASH]);
			gl::drawSolidRect(Rectf(-size / 2.0F, size / 2.0F));
			gl::popModelView();
		}
        
		{
			gl::pushModelView();
			gl::translate(cinder::app::getWindowSize() / 2);
			gl::rotate(45);
			gl::color(ColorA(1, 1, 1, alfa));
			gl::ScopedTextureBind tb(textures[T_SLASH]);
			gl::drawSolidRect(Rectf(-size / 2.0F, size / 2.0F));
			gl::popModelView();
		}
    }
    void SceneTitle::drawBackground( )
    {
        if ( !countend( t_[CROSS] ) ) return;
        Easing::tCount( t_[UP], 3.0f );
        float alfa = Easing::getEasing[Easing::QuadIn]( t_[UP], 0.f, 1.f );
        vec2 size;
        vec2 endsize = cinder::app::getWindowSize( );
        vec2 startsize = endsize*10.0f;
        size.x = Easing::getEasing[Easing::CubicIn]( t_[UP], startsize.x, endsize.x );
        size.y = Easing::getEasing[Easing::CubicIn]( t_[UP], startsize.y, endsize.y );
        gl::pushModelView( );
        gl::translate( cinder::app::getWindowSize( ) / 2 );
        gl::color( ColorA( 1, 1, 1, alfa ) );
		gl::ScopedTextureBind tb(textures[T_BACKGROUND]);
        gl::drawSolidRect( Rectf( -size / 2.0F, size / 2.0F) );
        gl::popModelView( );
    }
    void SceneTitle::drawFlash( )
    {
        if ( !countend( t_[LOGOSLIDE] ) ) return;
        if ( countend( t_[FLASH] ) ) return;
        Easing::tCount( t_[FLASH], 0.2f );
        gl::pushModelView( );
        gl::translate( cinder::app::getWindowSize( ) / 2 );
        gl::color( ColorA( 1, 1, 1, Easing::getEasing[Easing::Return]( t_[FLASH], 0, 1 ) ) );
        gl::drawSolidRect( Rectf( -cinder::app::getWindowSize( ) / 2, cinder::app::getWindowSize( ) / 2 ) );
        gl::popModelView( );
    }

    void SceneTitle::drawLogo( )
    {
        if ( !countend( t_[UP] ) )return;
        vec2 startpos = vec2( -cinder::app::getWindowWidth( ) / 2.0f, cinder::app::getWindowHeight( ) / 1.3f);
        vec2 endpos = vec2( cinder::app::getWindowWidth( ) / 5.6f, cinder::app::getWindowHeight( ) / 1.3f);
        vec2 size;
        size.x = cinder::app::getWindowWidth( ) / 2.7f;
        size.y = size.x*( ( float( textures[T_LOGO]->getHeight( ) ) / float( textures[T_LOGO]->getWidth( ) ) ) );
        int num = 3;
        if ( countend( t_[TitleEasing::LOGOSLIDE] ) ) {
            num = 1;
        }
        for ( int i = num - 1; i >= 0; i-- ) {
            vec2 pos;
            pos.x = Easing::getEasing[Easing::Linear]( t_[TitleEasing::LOGOSLIDE] - 2 * i*( 1.0f / ( 60.0f*1.0f ) ), startpos.x, endpos.x );
            pos.y = Easing::getEasing[Easing::Linear]( t_[TitleEasing::LOGOSLIDE] - i*( 1.0f / ( 60.0f*1.0f ) ), startpos.y, endpos.y );
            gl::pushModelView( );
            gl::translate( pos );
			gl::ScopedTextureBind tb(textures[T_LOGO]);
            float color = 1 - float( i ) / float( num );
            gl::color( ColorA( color, color, color, color ) );
            gl::drawSolidRect( Rectf( -size / 2.f, size / 2.f ) );
            gl::popModelView( );
        }


        Easing::tCount( t_[TitleEasing::LOGOSLIDE], 1.0f );
    }
    void SceneTitle::drawMainKuriko( )
    {
        if ( !countend( t_[UP] ) )return;
        vec2 startpos = vec2( 1.5f*cinder::app::getWindowWidth( ), cinder::app::getWindowHeight( ) / 2.f );
        vec2 endpos = cinder::app::getWindowSize( ) / 2;
        vec2 size = cinder::app::getWindowSize( );
        int num = 3;
        if ( countend( t_[TitleEasing::LOGOSLIDE] ) ) {
            num = 1;
        }
        for ( int i = num - 1; i >= 0; i-- ) {
            vec2 pos;
            pos.x = Easing::getEasing[Easing::Linear]( t_[TitleEasing::LOGOSLIDE] - 2 * i*( 1.0f / ( 60.0f*1.0f ) ), startpos.x, endpos.x );
            pos.y = Easing::getEasing[Easing::Linear]( t_[TitleEasing::LOGOSLIDE] - i*( 1.0f / ( 60.0f*1.0f ) ), startpos.y, endpos.y );
            gl::pushModelView( );
            gl::translate( pos );
			gl::ScopedTextureBind tb(textures[T_MAINKURIKO]);
            float color = 1 - float( i ) / float( num );
            gl::color( ColorA( color, color, 1, color ) );
            gl::drawSolidRect( Rectf( -size / 2.f, size / 2.f ) );
            gl::popModelView( );
        }
    }
    void SceneTitle::drawStartIcon( )
    {
        if ( !countend( t_[FLASH] ) )return;
        vec2 pos = vec2( cinder::app::getWindowWidth( ) / 3.6f, cinder::app::getWindowHeight( ) / 1.1f );
        vec2 size;
        size.x = cinder::app::getWindowWidth( ) / 5.f;
        size.y = size.x*( ( float( textures[T_START]->getHeight( ) ) / float( textures[T_START]->getWidth( ) ) ) );
        gl::pushModelView( );
        gl::translate( pos );
        iconangle += 0.05f;
        gl::color( ColorA( 1, 1, 1, 0.5f + 0.5*sin( iconangle ) ) );
		gl::ScopedTextureBind tb(textures[T_START]);
        gl::drawSolidRect( Rectf( -size / 2.0F, size / 2.0F ) );
        gl::popModelView( );
    }
    void SceneTitle::roadTexture( )
    {
        std::string str[TexType::TEXMAX];
        str[T_KURIKO] = "titlekuriko";
        str[T_BOSS] = "titleboss";
        str[T_KURIKOBACK] = "kurikoback";
        str[T_BOSSBACK] = "bossback";
        str[T_SHURIKEN] = "shuriken";
        str[T_KUNAI] = "titleguard";
        str[T_SLASH] = "titleslash";
        str[T_BACKGROUND] = "title";
        str[T_LOGO] = "logo";
        str[T_MAINKURIKO] = "mainkuriko";
        str[T_START] = "start";
        str[T_MAINKUNAI] = "mainkunai";
        str[T_SUBKUNAI] = "titlekunai";
        for ( int i = 0; i < TexType::TEXMAX; i++ ) {
            textures[i] = Resource::IMAGE["kuriko/Title/" + str[i] + ".png"];
        }
    }
}