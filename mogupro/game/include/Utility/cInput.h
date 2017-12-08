#pragma once
#include <iostream>
#include <unordered_map>
#include <set>
#include <Utility/cSingletonAble.h>
#include <Node/node.h>
#include <Utility/Schedule.h>
#define ENV Utility::cInputAll::getInstance()
class gameApp;
namespace Utility
{
class cInputAll : public ::Utility::cSingletonAble<cInputAll>
{
public:
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
public:
	cInputAll( );
	void setMouseControl( const bool& flag );
	void disableKeyButton( );
	void enableKeyButton( );
	void disableMouseButton( );
	void enableMouseButton( );
	void disablePadButton( );
	void enablePadButton( );
	void disablePadAxis( );
	void enablePadAxis( );
	ci::vec2 getMouseVec( );
	ci::vec2 getMousePos( );
	bool pressKey( const int& pressed_key );
	bool pushKey( const int& pressed_key );
	bool pullKey( const int& pressed_key );
	bool pushKey( );
	bool pressKey( );
	bool pullKey( );
	float getPadAxis( const int& pad_num );
	bool isPadPush( const int& num );
	bool isPadPress( const int& num );
	bool isPadPull( const int& num );
	void flashInput( );
private:
	friend class gameApp;
	void setup( );
	void preUpdate( float delta );
	void cleanup( );
	void keyDown( const ci::app::KeyEvent& event );
	void keyUp( const ci::app::KeyEvent& event );
	void mouseMove( const ci::app::MouseEvent& event );
	void mouseDrag( const ci::app::MouseEvent& event );
	void mouseDown( const ci::app::MouseEvent& event );
	void mouseUp( const ci::app::MouseEvent& event );
private:
	void padAxis( const int& num, const float& value );
	void padDown( const int& num );
	void padUp( const int& num );
private:
	class State
	{
	private:
		struct
		{
			std::set<int> press;
			std::set<int> push;
			std::set<int> pull;
		}front, back;
		// 1 enable 2 disable;
		int button = 0;
		bool enabled = true;
	public:
		void enable( )
		{
			button = 1;
			enabled = true;
			back = front;
		}
		void disable( )
		{
			button = 2;
			enabled = false;
			back = front;
		}
		void preUpdate( )
		{
			switch ( button )
			{
			case 1:// enable
			{
				front = back;
				front.pull.clear( );
				for ( auto& code : front.press )
				{
					front.push.insert( code );
				}
			}
				break;
			case 2:// disable
			{
				front = back;
				front.push.clear( );
				for ( auto& code : front.press )
				{
					front.pull.insert( code );
				}
				front.press.clear( );
			}
				break;
			default:
				break;
			}
			button = 0;
		}
		bool push( int value )
		{
			return front.push.find( value ) != front.push.end( );
		}
		bool press( int value )
		{
			return front.press.find( value ) != front.press.end( );
		}
		bool pull( int value )
		{
			return front.pull.find( value ) != front.pull.end( );
		}
		bool push( )
		{
			return !front.push.empty( );
		}
		bool press( )
		{
			return !front.press.empty( );
		}
		bool pull( )
		{
			return !front.pull.empty( );
		}
		void down( int value )
		{
			if ( !enabled ) return;
			if ( !press( value ) )
			{
				front.push.insert( value );
			}
			front.press.insert( value );
		}
		void up( int value )
		{
			if ( !enabled ) return;
			front.press.erase( value );
			front.pull.insert( value );
		}
		void flush( )
		{
			front.push.clear( );
			front.pull.clear( );
			back.push.clear( );
			back.pull.clear( );
		}
	};

	//キーボード
	State keyState;
	std::unordered_map<std::string, int> serch;
	std::unordered_map<int, int> keys;

	//マウス
	State mouseState;
	ci::vec2 inc_pos;
	bool cursor_captured;
	ci::ivec2 last_cursor_pos;
	bool mouse_active;

	//ゲームパッド
	State padState;
	std::map<int, float> pad_stick_axis_value;
	bool usePadAxis = true;

	//前のフレームと今のフレームのマウスのベクトル
	ci::vec2 mouse_vec;
	//マウスの位置
	ci::vec2 mouse_pos;

	Utility::ScheduleHandle handle;
};
}
