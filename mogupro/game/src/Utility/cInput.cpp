#include <Utility/cInput.h>
#include <cinder/app/App.h>
#include <Node/action.hpp>
#include <CameraManager/cCameraManager.h>

extern "C"
{
	#include <GamePad.h>
}
#pragma comment(lib,"winmm.lib")
#if _DEBUG
#pragma comment(lib,"Gamepad_d.lib")
#else
#pragma comment(lib,"Gamepad.lib")
#endif 
namespace Utility
{
#pragma region 外部からアクセスされる関数群
cInputAll::cInputAll( )
	: cursor_captured( false )
	, mouse_active( false )
{
	mouse_cursor.x = 0;
	mouse_cursor.y = 0;
}
void cInputAll::setMouseControl( const bool & flag )
{
	mouse_active = flag;
}
void cInputAll::disableMouseButton( )
{
	mouseState.disable( );
}
void cInputAll::enableMouseButton( )
{
	mouseState.enable( );
}
void cInputAll::disableKeyButton( )
{
	keyState.disable( );
}
void cInputAll::enableKeyButton( )
{
	keyState.enable( );
}
void cInputAll::disablePadButton( )
{
	padState.disable( );
}
void cInputAll::enablePadButton( )
{
	padState.enable( );
}
void cInputAll::disablePadAxis( )
{
	usePadAxis = false;
}
void cInputAll::enablePadAxis( )
{
	usePadAxis = true;
}
ci::vec2 cInputAll::getMouseVec( )
{
	return mouse_vec;
}
ci::vec2 cInputAll::getMousePos( )
{
	return mouse_pos;
}
bool cInputAll::pressKey( const int& pressed_key )
{
	return keyState.press( pressed_key ) || mouseState.press( pressed_key );
}
bool cInputAll::pushKey( const int & pressed_key )
{
	return keyState.push( pressed_key ) || mouseState.push( pressed_key );
}
bool cInputAll::pullKey( const int & pressed_key )
{
	return keyState.pull( pressed_key ) || mouseState.pull( pressed_key );
}
bool cInputAll::pressKey( )
{
	return keyState.press( ) || mouseState.press( );
}
bool cInputAll::pushKey( )
{
	return keyState.push( ) || mouseState.push( );
}
bool cInputAll::pullKey( )
{
	return keyState.pull( ) || mouseState.pull( );
}
bool cInputAll::isPadPush( const int & num )
{
	return padState.push( num );
}
bool cInputAll::isPadPress( const int & num )
{
	return padState.press( num );
}
bool cInputAll::isPadPull( const int & num )
{
	return padState.pull( num );
}
void cInputAll::setMouseCursorAvtive(bool flag)
{
	mouse_active_cursor = flag;
}
//０で左スティックの左右（ｘ）
//１で左スティックの上下（ｙ）※注意　上下が逆
//２、３が右スティック
float cInputAll::getPadAxis( const int & pad_num )
{
	if ( !usePadAxis ) return 0.0F;
	if ( pad_stick_axis_value.find( pad_num ) == pad_stick_axis_value.cend( ) )
		return 0.0f;

	//何もしなくても動くので制限
	if ( std::abs( pad_stick_axis_value.find( pad_num )->second ) <= 0.2f )
		return 0.0f;

	return pad_stick_axis_value.find( pad_num )->second;
}
void cInputAll::flashInput( )
{
	keyState.flush( );
	mouseState.flush( );
	padState.flush( );
}
#pragma endregion
#pragma region プライベート関数群
void cInputAll::setup( )
{
	Gamepad_deviceAttachFunc( [ ] ( Gamepad_device* device, void * context )
	{
		// TODO: パッドを差した時の処理
		ci::app::console( ) << "Device ID " << device->deviceID << " attached (vendor = " << device->vendorID << "; product = " << device->productID << ") with context" << context << std::endl;
	}, (void *)0x1 );
	Gamepad_deviceRemoveFunc( [ ] ( Gamepad_device* device, void * context )
	{
		// TODO: パッドを抜いた時の処理
		ci::app::console( ) << "Device ID " << device->deviceID << " removed with context " << context << std::endl;
	}, (void *)0x2 );
	Gamepad_buttonDownFunc( [ ] ( Gamepad_device* device, unsigned int buttonID, double timestamp, void * context )
	{
		ENV->padDown( buttonID );
	}, (void *)0x3 );
	Gamepad_buttonUpFunc( [ ] ( Gamepad_device* device, unsigned int buttonID, double timestamp, void * context )
	{
		ENV->padUp( buttonID );
	}, (void *)0x4 );
	Gamepad_axisMoveFunc( [ ] ( Gamepad_device* device, unsigned int axisID, float value, float lastValue, double timestamp, void * context )
	{
		if ( value < 0.3f || value > 0.3f )
		{
			ENV->padAxis( axisID, value );
		}
	}, (void *)0x5 );
	Gamepad_init( );

	pad_stick_axis_value.insert( std::make_pair( STICK_LEFT_HORIZONTAL, 0.0f ) );
	pad_stick_axis_value.insert( std::make_pair( STICK_LEFT_VERTICAL, 0.0f ) );
	pad_stick_axis_value.insert( std::make_pair( STICK_RIGHT_HORIZONTAL, 0.0f ) );
	pad_stick_axis_value.insert( std::make_pair( STICK_RIGHT_VERTICAL, 0.0f ) );
	pad_stick_axis_value.insert( std::make_pair( CROSS_HORIZONTAL, 0.0f ) );
	pad_stick_axis_value.insert( std::make_pair( CROSS_VERTICAL, 0.0f ) );

	// ゲームパッドの抜き差し状況の更新をします。
	handle = Utility::schedule( 1.0F, [ this ]
	{
		Gamepad_detectDevices( );
	} );
}
void cInputAll::preUpdate( float delta )
{
	keyState.preUpdate( );
	mouseState.preUpdate( );
	padState.preUpdate( );
	Gamepad_processEvents( );
}
void cInputAll::cleanup( )
{
	Gamepad_shutdown( );
}
void cInputAll::keyDown( const ci::app::KeyEvent& event )
{
	keyState.down( event.getCode( ) );
}
void cInputAll::keyUp( const ci::app::KeyEvent& event )
{
	keyState.up( event.getCode( ) );
}
void cInputAll::padAxis( const int & num, const float & value )
{
	pad_stick_axis_value.find( num )->second = value;
}
void cInputAll::padDown( const int & num )
{
	padState.down( num );
}
void cInputAll::padUp( const int & num )
{
	padState.up( num );
}
void mouseCursolFixed( const ci::app::MouseEvent& event, ci::vec2& inc_pos,
					   ci::vec2& mouse_vec, bool& cursor_captured, ci::ivec2& last_cursor_pos,
						POINT& mouse_cursor,bool mouse_active_cursor) {
	//１フレーム目以降
	if ( cursor_captured ) {

		if (!mouse_active_cursor)return;

		POINT pt;

		GetCursorPos( &pt );
		POINT delta;
		delta.x = pt.x - last_cursor_pos.x;
		delta.y = pt.y - last_cursor_pos.y;
		//inputに動かさないboolを持たせて上げる
		if ( delta.x != 0 && delta.y != 0 ) {
			SetCursorPos( last_cursor_pos.x, last_cursor_pos.y );
			CAMERA->addCameraAngle(ci::vec2(-delta.x, -delta.y) * 0.005f);
		}
	}
	//１フレーム目は何もしない
	else {

		SetCursorPos( ci::app::getWindowPos( ).x + ( ci::app::getWindowSize( ).x / 2 ),
					  ci::app::getWindowPos( ).y + ( ci::app::getWindowSize( ).y / 2 ) );
		POINT pt;
		GetCursorPos( &pt );
		last_cursor_pos.x = pt.x;
		last_cursor_pos.y = pt.y;
		mouse_vec.x = last_cursor_pos.x;
		mouse_vec.y = last_cursor_pos.y;
		cursor_captured = true;
	}
}
void setMousePos( ci::vec2& pos ) {
	POINT pt;
	GetCursorPos( &pt );
	pos.x = pt.x;
	pos.y = pt.y;
}
void cInputAll::mouseMove( const ci::app::MouseEvent & event )
{
	setMousePos( mouse_pos );
	if ( !mouse_active ) return;
	mouseCursolFixed( event, inc_pos, mouse_vec, cursor_captured, last_cursor_pos,mouse_cursor,mouse_active_cursor);
}
void cInputAll::mouseDrag( const ci::app::MouseEvent & event )
{
	if ( !mouse_active ) return;
	mouseCursolFixed( event, inc_pos, mouse_vec, cursor_captured, last_cursor_pos, mouse_cursor, mouse_active_cursor);
}
void cInputAll::mouseDown( const ci::app::MouseEvent& event )
{
	if ( event.isLeft( ) )
	{
		mouseState.down( ci::app::MouseEvent::LEFT_DOWN );
	}
	if ( event.isRight( ) )
	{
		mouseState.down( ci::app::MouseEvent::RIGHT_DOWN );
	}
	if ( event.isMiddle( ) )
	{
		mouseState.down( ci::app::MouseEvent::MIDDLE_DOWN );
	}
}
void cInputAll::mouseUp( const ci::app::MouseEvent& event )
{
	if ( event.isLeft( ) )
	{
		mouseState.up( ci::app::MouseEvent::LEFT_DOWN );
	}
	if ( event.isRight( ) )
	{
		mouseState.up( ci::app::MouseEvent::RIGHT_DOWN );
	}
	if ( event.isMiddle( ) )
	{
		mouseState.up( ci::app::MouseEvent::MIDDLE_DOWN );
	}
}
#pragma endregion
}
