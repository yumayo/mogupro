#pragma once
#include <cinder/gl/gl.h>

namespace Particle
{
enum class ParticleType
{
    NONE,
    SCATTER,
    SPLASH,
};

enum class ParticleTextureType
{
    NONE,
    SAND,
    SPARK,

};

class cParticle
{
public:

    cParticle();
    ~cParticle();


private:

    ci::vec3 position;

};

class cParticleManager
{
public:

    cParticleManager();
    ~cParticleManager();

private:

    void create( const ci::vec3& position, const std::string name );


};
}
