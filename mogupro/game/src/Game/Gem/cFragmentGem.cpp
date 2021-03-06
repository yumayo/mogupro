#include <Game/Gem/cFragmentGem.h>
#include <Log/Log.h>
#include <Game\cLightManager.h>
#include <cinder/Rand.h>
namespace Game
{
	namespace Gem
	{
		class OneGem : public Node::node
		{
		private:
			cFragmentGem* gem;
		public:
			CREATE_H(OneGem, cFragmentGem* gem)
			{
				CREATE(OneGem, gem);
			}
			bool init(cFragmentGem* gem)
			{
				this->gem = gem;
				return true;
			}
			void update(float t) override
			{
				set_position_3d(gem->getPos());
			}
			void render() override
			{
				Resource::cFbxManager::getInstance()->draw("Gemstone");
			}
		};

		cFragmentGem::cFragmentGem(int id, vec3 position, vec3 scale, ColorA color, GemType type, vec3 direction) :
			mId(id), mPosition(position), mScale(scale), mColorA(color), mType(type), mDirection(direction), mAabb(position, scale), mRb(mAabb)
		{

			mSinRotate   = 0.0f;
			mPutPos      = vec3(0.0f);
			mSpeed       = vec3(0, 10, 0);

		};


		cFragmentGem::~cFragmentGem()
		{
		}


		void cFragmentGem::setup()
		{
			mVisible = true;
			popNode = Node::node::create();
			auto oneGem = popNode->add_child( OneGem::create(this) );
			oneGem->set_color(mColorA);
			oneGem->set_schedule_update();
			oneGem->set_rotation(ci::toRadians(0.0F));
			oneGem->set_axis(vec3(1,0,0));
			oneGem->run_action(Node::Action::sequence::create(Node::Action::ease<ci::EaseOutCubic>::create(Node::Action::axis_by::create(0.5, vec3(0, 0, 0)))));
			oneGem->run_action(Node::Action::sequence::create(Node::Action::ease<ci::EaseOutCubic>::create(Node::Action::rotate_by::create(0.5, 810))));
		}


		void cFragmentGem::draw()
		{
			if (!mVisible) return;
			gl::pushModelMatrix();
			popNode->entry_render(mat4());
			gl::popModelMatrix();

		}


		void cFragmentGem::lateUpdate(const float& delta_time)
		{
			popNode->entry_update(delta_time);
		}


		void cFragmentGem::update()
		{
		}


		void cFragmentGem::setIsRigid(bool isRigid)
		{

		}


		void cFragmentGem::setVisible(bool visible)
		{
			mVisible = visible;
		}
	}
}
