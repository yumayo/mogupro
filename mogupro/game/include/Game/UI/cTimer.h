#pragma once
#include <Node/renderer.hpp>
#include <Utility/cStateMachineBehaviour.h>
namespace Game
{
namespace UI
{
class cTimer : public Node::Renderer::sprite
{
public:
	CREATE_H(cTimer, cinder::vec2 contentSize)
	{
		CREATE(cTimer, contentSize);
	}
	bool init(cinder::vec2 contentSize);
	void update(float delta) override;
	std::function<void()> lastOneMinutesCall;
	std::function<void(std::string const& number)> countDownCall;
private:
	softptr<Node::Renderer::letter> letter;
	Utility::cStateMachineBehaviour sMac;
};
}
}
