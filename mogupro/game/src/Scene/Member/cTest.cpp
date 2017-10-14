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

	th = std::thread(
		[this]
	{
		while (c < 100)
		{

			for (int i = 0; i < 10000000; ++i)
			{

			}
			++ c;
		}

		isEndThread = true;
	}
	);

	
}

void cTest::shutDown()
{

}

void cTest::update()
{

	console() << "Count : " << c << std::endl;

	//if (isEndThread)
	{
		th.join();
	}
	console() << "AAAAAAA" << std::endl;
	
}

void cTest::draw()
{

}

void cTest::draw2D()
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

void cTest2::draw2D()
{

}

void cTest2::resize()
{

}