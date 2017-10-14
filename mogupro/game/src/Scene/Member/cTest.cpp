#include <Scene/Member/cTest.h>
#include <cinder/app/AppBase.h>
#include <Utility/Input.h>
#include <Scene/cSceneManager.h>
#include <Utility/Input.h>
using namespace ci;
using namespace ci::app;
using namespace std;
void cTest::setup()
{

}

void cTest::shutDown()
{

}

void cTest::update()
{

	console() << "update" << std::endl;

	if (ENV->pushKey(KeyEvent::KEY_RETURN))
	{
		cSceneManager::getInstance()->shift(new cTest2());
	}
}

void cTest::draw()
{

}

void cTest::resize()
{

}

void cTest2::setup()
{

}

void cTest2::shutDown()
{


}

void cTest2::update()
{
	console() << "update2" << std::endl;
}

void cTest2::draw()
{

}

void cTest2::resize()
{

}