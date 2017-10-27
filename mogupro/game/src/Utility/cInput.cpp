#include <Utility/cInput.h>
#include <cinder/app/App.h>
#include <CameraManager/cCameraManager.h>

extern "C"
{
#include <Utility/sGamepad.h>
}

#pragma comment(lib,"winmm.lib")
#if _DEBUG
#pragma comment(lib,"Gamepad_d.lib")
#else
#pragma comment(lib,"Gamepad.lib")
#endif 

static bool verbose = true;

#define POLL_ITERATION_INTERVAL 30


void onButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);
void onButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);
void onAxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context);
void onDeviceAttached(struct Gamepad_device * device, void * context);
void onDeviceRemoved(struct Gamepad_device * device, void * context);

void Utility::cInputAll::padAxisSetup()
{
	pad_stick_axis_value.insert(std::make_pair(STICK_LEFT_HORIZONTAL, 0.0f));
	pad_stick_axis_value.insert(std::make_pair(STICK_LEFT_VERTICAL, 0.0f));
	pad_stick_axis_value.insert(std::make_pair(STICK_RIGHT_HORIZONTAL, 0.0f));
	pad_stick_axis_value.insert(std::make_pair(STICK_RIGHT_VERTICAL, 0.0f));
	pad_stick_axis_value.insert(std::make_pair(CROSS_HORIZONTAL, 0.0f));
	pad_stick_axis_value.insert(std::make_pair(CROSS_VERTICAL, 0.0f));
}

Utility::cInputAll::cInputAll()
{
	cursor_captured = false;
	mouse_active = false;
}

void mouseCursolFixed(const ci::app::MouseEvent& event, ci::vec2& inc_pos,
	ci::vec2& mouse_pos, bool& cursor_captured, POINT& last_cursor_pos) {

	
	if (cursor_captured) {
		POINT pt;

		GetCursorPos(&pt);
		POINT delta;
		delta.x = pt.x - last_cursor_pos.x;
		delta.y = pt.y - last_cursor_pos.y;

		if (delta.x != 0 && delta.y != 0) {
			SetCursorPos(last_cursor_pos.x, last_cursor_pos.y);
			CAMERA->setCameraAngle(ci::vec2(-delta.x, -delta.y) * 0.005f);
		}
	}
	else {

		SetCursorPos(ci::app::getWindowPos().x + (ci::app::getWindowSize().x / 2),
			ci::app::getWindowPos().y + (ci::app::getWindowSize().y / 2));
		GetCursorPos(&last_cursor_pos);
		mouse_pos.x = last_cursor_pos.x;
		mouse_pos.y = last_cursor_pos.y;
		cursor_captured = true;
	}
}

void Utility::cInputAll::padSetup()
{
	Gamepad_deviceAttachFunc(onDeviceAttached, (void *)0x1);
	Gamepad_deviceRemoveFunc(onDeviceRemoved, (void *)0x2);
	Gamepad_buttonDownFunc(onButtonDown, (void *)0x3);
	Gamepad_buttonUpFunc(onButtonUp, (void *)0x4);
	Gamepad_axisMoveFunc(onAxisMoved, (void *)0x5);
	Gamepad_init();
	padAxisSetup();
}

void Utility::cInputAll::padUpdate()
{
	static unsigned int iterationsToNextPoll = POLL_ITERATION_INTERVAL;

	iterationsToNextPoll--;
	if (iterationsToNextPoll == 0) {
		Gamepad_detectDevices();
		iterationsToNextPoll = POLL_ITERATION_INTERVAL;
	}
}


void Utility::cInputAll::padProcessEvent()
{
	Gamepad_processEvents();
}

void Utility::cInputAll::setPadPush(const int & num)
{
	if (padpress.find(num) == padpress.end())
		padpush.emplace(num);
}

void Utility::cInputAll::setPadPress(const int & num)
{
	padpress.emplace(num);
}

void Utility::cInputAll::setPadPull(const int & num)
{
	padpull.emplace(num);
}

void Utility::cInputAll::erasePadPress(const int & num)
{
	padpress.erase(padpress.find(num));
}

bool Utility::cInputAll::isPadPush(const int & num)
{
	if (padpush.find(num) == padpush.end())
		return false;
	padpush.erase(padpush.find(num));
	return true;
}

bool Utility::cInputAll::isPadPress(const int & num)
{
	if (padpress.find(num) == padpress.end())
		return false;
	return true;
}

bool Utility::cInputAll::isPadPull(const int & num)
{
	if (padpull.find(num) == padpull.end())
		return false;
	padpull.erase(padpull.find(num));
	return true;
}

void Utility::cInputAll::setPadAxis(const int & num, const float & value_)
{
	pad_stick_axis_value.find(num)->second = value_;
}


//０で左スティックの左右（ｘ）
//１で左スティックの上下（ｙ）※注意　上下が逆
//２、３が右スティック
float Utility::cInputAll::getPadAxis(const int & pad_num)
{
	if (pad_stick_axis_value.find(pad_num) == pad_stick_axis_value.cend())
		return 0.0f;

	//何もしなくても動くので制限
	if (std::abs(pad_stick_axis_value.find(pad_num)->second) <= 0.2f)
		return 0.0f;

	return pad_stick_axis_value.find(pad_num)->second;
}

void Utility::cInputAll::keyDown(const ci::app::KeyEvent& event) {
	
	if (press.find(event.getCode()) == press.end()) 
		push.insert(event.getCode());
	
	press.insert(event.getCode());
}
void Utility::cInputAll::keyUp(const ci::app::KeyEvent& event)
{
	pull.insert(event.getCode());
	press.erase(event.getCode());
}


void Utility::cInputAll::mouseMove(const ci::app::MouseEvent & event)
{
	if (!mouse_active) return;
	mouseCursolFixed(event, inc_pos, mouse_pos, cursor_captured, last_cursor_pos);
}

void Utility::cInputAll::mouseDrag(const ci::app::MouseEvent & event)
{
	if (!mouse_active) return;
	mouseCursolFixed(event, inc_pos, mouse_pos, cursor_captured, last_cursor_pos);
}

void Utility::cInputAll::mouseDown(const ci::app::MouseEvent& event)
{
	if (event.isLeft())
	{
		press.insert(ci::app::MouseEvent::LEFT_DOWN);
		push.insert(ci::app::MouseEvent::LEFT_DOWN);
	}
	if (event.isRight())
	{
		press.insert(ci::app::MouseEvent::RIGHT_DOWN);
		push.insert(ci::app::MouseEvent::RIGHT_DOWN);
	}
	if (event.isMiddle())
	{
		press.insert(ci::app::MouseEvent::MIDDLE_DOWN);
		push.insert(ci::app::MouseEvent::MIDDLE_DOWN);
	}
}

void Utility::cInputAll::mouseUp(const ci::app::MouseEvent& event)
{
	if (event.isLeft())
	{
		pull.insert(ci::app::MouseEvent::LEFT_DOWN);
		press.erase(ci::app::MouseEvent::LEFT_DOWN);
	}
	if (event.isRight())
	{
		pull.insert(ci::app::MouseEvent::RIGHT_DOWN);
		press.erase(ci::app::MouseEvent::RIGHT_DOWN);
	}
	if (event.isMiddle())
	{
		pull.insert(ci::app::MouseEvent::MIDDLE_DOWN);
		press.erase(ci::app::MouseEvent::MIDDLE_DOWN);
	}
}

bool Utility::cInputAll::pressKey(const int& pressed_key) {
	if (press.find(pressed_key) != press.end()) {
		return true;
	}
	return false;
}

bool Utility::cInputAll::pushKey(const int & pressed_key)
{
	if (push.find(pressed_key) == push.end())
		return false;
	push.erase(push.find(pressed_key));
	return true;
}

bool Utility::cInputAll::pullKey(const int & pressed_key)
{
	if (pull.find(pressed_key) == pull.end())
		return false;
	pull.erase(pull.find(pressed_key));
	return true;
}

void Utility::cInputAll::flashInput()
{
	push.clear();
	pull.clear();
	padpush.clear();
	padpull.clear();

}



void onButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context)
{
	if (verbose)
	{
		//console() << "Button " << buttonID << " down on device " << device->deviceID << " at " << timestamp << " with context " << context << std::endl;

		ENV->setPadPush(buttonID);
		ENV->setPadPress(buttonID);
	}
}

void onButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context)
{
	if (verbose)
	{
		//console() << "Button " << buttonID << " up on device " << device->deviceID << " at " << timestamp << " with context " << context << std::endl;

		ENV->setPadPull(buttonID);
		ENV->erasePadPress(buttonID);
	}
}

void onAxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context)
{
	if (verbose && (value < 0.3f || value > 0.3f)) // reduce the output noise by making a dead zone
	{
		ci::app::console() << "Axis " << axisID << " moved from " << lastValue << " to " << value << " on device " << device->deviceID << " at " << timestamp << " with context " << context << std::endl;
		ENV->setPadAxis(axisID, value);
	}
}

void onDeviceAttached(struct Gamepad_device * device, void * context)
{
	if (verbose)
	{
		//パッドを差した時の処理
		ci::app::console() << "Device ID " << device->deviceID << " attached (vendor = " << device->vendorID << "; product = " << device->productID << ") with context" << context << std::endl;
	}
}

void onDeviceRemoved(struct Gamepad_device * device, void * context)
{
	if (verbose)
	{
		//パッドを抜いた時の処理
		ci::app::console() << "Device ID " << device->deviceID << " removed with context " << context << std::endl;
	}
}



