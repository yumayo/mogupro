#pragma once
#include <Scene/cSceneBase.h>
namespace Scene
{
namespace Member
{
class cEnd final : public cSceneBase
{
public:
    cEnd( ) = default;
    ~cEnd( ) = default;
public:
	void setup( ) override;
	void shutDown( ) override;
	void update( float t ) override;
	void draw( ) override;
	void draw2D( ) override;
	void resize( ) override;
	inline std::string const getName( ) { return typeid( *this ).name( ); }
};
}
}
