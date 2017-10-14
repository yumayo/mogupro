#pragma once
#include "../cSceneBase.h"
#include <thread>
class cTest : public cSceneBase
{
public:
	void setup();
	void shutDown();
	void update();
	void draw();
	void resize();
private:
	std::thread th;
public:
	int c = 0;
	bool isEndThread = false;

};

class cTest2 : public cSceneBase
{
public:
	void setup();
	void shutDown();
	void update();
	void draw();
	void resize();
private:

};