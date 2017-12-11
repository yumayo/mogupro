#pragma once
#include <cinder/app/App.h>
#include <unordered_map>
#include <deque>
#include "EaseType.h"
#include "Utility/cSingletonAble.h"

#define Easing cEase::getInstance()

// イージングの元
class EaseOrigin
{
public:
    EaseOrigin() {}
    EaseOrigin( float begin, float end, float end_frame, EaseType type );

    void setup( float begin, float end, float end_frame, EaseType type );

    void update( const float& delta_time );

    float currentTargetValue();

    // 終わったらtrue
    bool isDone();

    void countReset() { count_ = 0.0f; }

    float getBegin() { return begin_; }
    void setBegin( float begin ) { begin_ = begin; }

private:

    float begin_;
    float end_;
    float end_frame_;
    std::function<float( float, float, float )> easefunc_;
    float count_;
};


// ターゲットに追加されているイージングを実行するクラス
class RunEase
{
public:
    RunEase();

    // 値の更新
    void update( const float& delta_time );

    // イージングがすべて終わっているかどうか
    bool isEaseEnd() { return ease_accum.empty(); }

    // floatのイージングスタックを追加する関数
    void add( float& target, float end, float end_frame, EaseType ease_type );

    // waitを追加する関数
    void addWait( float& target, int wait );

    void setIsStop( const bool& value ) { is_stop = value; }
    void setIsLoop( const bool& value ) { is_loop = value; }

    // スタックの中身を消去
    void clear();

private:

    // イージングの更新
    void action( const float& delta_time );

    // スタックをpop_frontする関数
    void pop();

    // 先頭スタックを後ろに追加
    void loop();

    std::deque<EaseOrigin> ease_accum;
    float* target_;
    bool is_stop;
    bool is_loop;
};

/*  使い方

    if( Easing.isEaseEnd( 動かすfloatの値 ) )
    {
        Easing.add( 動かすfloatの値,
                    終わりのfloatの値,
                    何フレームかけるか,
                    イージングの種類 enum EaseType );
        
        Easing.wait( 動かすfloatの値,
                     止めているフレーム時間 );
    }

*/

// イージングを管理するクラス
class cEase : public Utility::cSingletonAble<cEase>
{
public:
 
    // イージングの更新
    void update( const float& delta_time );

    // イージングを追加する関数
    // target       イージングする float の値
    // end          イージングが終了したときになる値
    // end_frame    何フレームで終わるか
    // ease_type    どのイージング関数を使うか
    void add( float& target, const float& end, float end_frame, EaseType ease_type );

    // イージングを追加する関数
    // target       イージングする vec3 の値
    // end          イージングが終了したときになる値
    // end_frame    何フレームで終わるか
    // ease_type    どのイージング関数を使うか
    void add( ci::vec3 & target, const ci::vec3& end, float end_frame, EaseType ease_type );

    // イージングを追加する関数
    // target       イージングする vec2 の値
    // end          イージングが終了したときになる値
    // end_frame    何フレームで終わるか
    // ease_type    どのイージング関数を使うか
    void add( ci::vec2 & target, const ci::vec2& end, float end_frame, EaseType ease_type );

    // イージングを止める処理を追加する関数
    // target       イージングを止めるfloatの値
    // wait_count   止める時間
    void wait( float& target, int wait_count );
    // イージングを止める処理を追加する関数
    // target       イージングを止めるvec3の値
    // wait_count   止める時間
    void wait( ci::vec3 & target, int wait_count );
    // イージングを止める処理を追加する関数
    // target       イージングを止めるvec3の値
    // wait_count   止める時間
    void wait( ci::vec2 & target, int wait_count );

    // イージングの更新を止める関数
    void stop( float& target, const bool& is_enable = true );

    // イージングの更新を止める関数
    void stop( ci::vec3& target, const bool& is_enable = true );

    // イージングの更新を止める関数
    void stop( ci::vec2& target, const bool& is_enable = true );

    // イージングスタックをループさせる
    void loop( float& target, const bool& is_enable = true );

    // イージングスタックをループさせる
    void loop( ci::vec3& target, const bool& is_enable = true );

    // イージングスタックをループさせる
    void loop( ci::vec2& target, const bool& is_enable = true );

    // 引数の値のイージングのスタックをすべて消去する関数
    void kill( float& target );

    // 引数の値のイージングのスタックをすべて消去する関数
    void kill( ci::vec3 & target );

    // 引数の値のイージングのスタックをすべて消去する関数
    void kill( ci::vec2 & target );

    // 引数の値のイージングが終わっているかどうかを返す関数
    // target       終わったか調べるfloatの値
    bool isEaseEnd( float& target );

    // 引数の値のイージングが終わっているかどうかを返す関数
    // target       終わったか調べるfloatの値
    bool isEaseEnd( ci::vec3& target );


    // 全てのイージングを終了させる関数
    void allClear();

private:
    std::unordered_map<float*, RunEase> ease;
};

