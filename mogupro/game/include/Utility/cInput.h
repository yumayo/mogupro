#pragma once

#include <iostream>
#include <unordered_map>
#include <set>
#include <cinder/app/App.h>
#include <Utility/cSingletonAble.h>
#include <Windows.h>
#define ENV Utility::cInputAll::getInstance()

namespace Utility
{
class cInputAll : public ::Utility::cSingletonAble<cInputAll>
{
private:
    //�L�[�{�[�h
    std::unordered_map<std::string, int> serch;
    std::unordered_map<int, int> keys;
    std::set<int> press;
    std::set<int> push;
    std::set<int> pull;

    //�}�E�X
    ci::vec2 mouse_pos;
    ci::vec2 inc_pos;
    bool cursor_captured;
    POINT last_cursor_pos;
	bool mouse_active;

    //�Q�[���p�b�h
    std::set<int> padpush;
    std::set<int> padpress;
    std::set<int> padpull;
    std::map<int, float> pad_stick_axis_value;
    void padAxisSetup();


public:
    cInputAll();

    // �Q�[���p�b�h
    void padSetup();
    void padUpdate();
    void padProcessEvent();

    void setPadPush( const int& num );
    void setPadPress( const int& num );
    void setPadPull( const int& num );
    void erasePadPress( const int& num );

    bool isPadPush( const int& num );
    bool isPadPress( const int& num );
    bool isPadPull( const int& num );

    void setPadAxis( const int& num, const float& value_ );

    float getPadAxis( const int& pad_num );

    enum PadNum
    {
        STICK_LEFT_HORIZONTAL = 0,
        STICK_LEFT_VERTICAL = 1,
        STICK_RIGHT_HORIZONTAL = 2,
        STICK_RIGHT_VERTICAL = 3,
        CROSS_HORIZONTAL = 4,
        CROSS_VERTICAL = 5,
        BUTTON_1 = 0,
        BUTTON_2 = 1,
        BUTTON_3 = 2,
        BUTTON_4 = 3,
        BUTTON_5 = 4,
        BUTTON_6 = 5,
        BUTTON_7 = 6,
        BUTTON_8 = 7,
        BUTTON_9 = 8,
        BUTTON_10 = 9,
        BUTTON_11 = 10,
        BUTTON_12 = 11,

    };


    //�L�[�{�[�h
    void keyDown( const ci::app::KeyEvent& event );
    void keyUp( const ci::app::KeyEvent& event );

    //�}�E�X
    void mouseMove( const ci::app::MouseEvent& event );
    void mouseDrag( const ci::app::MouseEvent& event );
    void mouseDown( const ci::app::MouseEvent& event );
    void mouseUp( const ci::app::MouseEvent& event );
    void setMouseControl( const bool& flag )
    {
        mouse_active = flag;
    }

    bool pressKey( const int& pressed_key );
    bool pushKey( const int& pressed_key );
    bool pullKey( const int& pressed_key );

    void flashInput();
};
}
