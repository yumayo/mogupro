#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "Scene/cSceneManager.h"
#include "Scene/Member/cGameMain.h"
#include <CameraManager/cCameraManager.h>
#include <Utility/cInput.h>
#include <Utility/cTimeMeasurement.h>
#include <Scene/Member/cMatching.h>
#include <Scene/Member/cMatchingServer.h>
#include <Scene/Member/cTitle.h>
#include <Log/Log.h>
#include <Sound/Stereophonic.h>
#include <phonon.h>
using namespace ci;
using namespace ci::app;
using namespace std;

#pragma comment(lib,"phonon.lib")
class gameApp : public App
{
private:
    float prevSeconds = 0.0F;
public:
    void setup( ) override;
    void mouseDown( MouseEvent event ) override;
    void mouseMove( MouseEvent event ) override;
    void mouseDrag( MouseEvent event ) override;
    void mouseUp( MouseEvent event )   override;
    void keyDown( KeyEvent event )     override;
    void keyUp( KeyEvent event )       override;
    void update( ) override;
    void draw( ) override;
    void cleanup( ) override;
};

std::shared_ptr < ci::vec3> pos(new ci::vec3(0, 0, 0));
std::shared_ptr < ci::vec3> dir(new ci::vec3(0, 0, 1));
std::shared_ptr < ci::vec3> soundPos(new ci::vec3(0, 0, 50));

IPLfloat32 iplDataDry[512];
IPLfloat32 iplDataSpatilized[1024];

//buffer object
IPLAudioBuffer bufFromOf; //from OF to Phonon
IPLAudioBuffer bufFromPhoton; //from Phonon back to OF

							  //setup stuff
IPLContext iplContext;
IPLRenderingSettings iplRenderSettings;

//renderer & effects
IPLhandle *binRen; //binarual renderer
IPLhandle *binEff; //object based binaural effect

void gameApp::setup( )
{
	Sound::StereophonicManager::getInstance()->open();
	Sound::Wav wav(ci::app::getAssetPath("BGM/test.wav").string());
    cSceneManager::getInstance( )->change<Scene::Member::cTitle>( );
    cSceneManager::getInstance( )->now( ).setup( );


	iplRenderSettings.frameSize = 512;
	iplRenderSettings.samplingRate = 44100;
	iplRenderSettings.convolutionType = IPL_CONVOLUTIONTYPE_PHONON;

	//-- buffer dry (for OF)
	IPLAudioFormat formatIn;
	formatIn.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS; // intended to be played back by a single speaker
	formatIn.channelLayout = IPL_CHANNELLAYOUT_STEREO; //play over headphones
	formatIn.numSpeakers = 2; //number of channels in the audio data, input is mono, but buffer In MUST HAVE SAME CHANNELS WITH BUFFER OUT, so 2 here
	formatIn.channelOrder = IPL_CHANNELORDER_INTERLEAVED; //LRLRLR...
														  //setup two speakers, one on left, one on right, assume radius is 400
	IPLVector3 speakers[2];
	speakers[0].x = -400;
	speakers[0].y = 0;
	speakers[0].z = 0;
	speakers[1].x = 400;
	speakers[1].y = 0;
	speakers[1].z = 0;
	formatIn.speakerDirections = speakers;

	bufFromOf.format = formatIn;
	bufFromOf.numSamples = 512;
	bufFromOf.interleavedBuffer = new IPLfloat32[bufFromOf.numSamples * formatIn.numSpeakers];

	//-- buffer spatialize (for Phonon)
	IPLAudioFormat formatOut;
	formatOut.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS; // intended to be played back by a single speaker
	formatOut.channelLayout = IPL_CHANNELLAYOUT_STEREO; //play over headphones
	formatOut.numSpeakers = 2; //number of channels in the audio data
	formatOut.channelOrder = IPL_CHANNELORDER_INTERLEAVED; //LRLRLR...
	formatOut.speakerDirections = speakers;

	bufFromPhoton.format = formatOut;
	bufFromPhoton.numSamples = 512;
	bufFromPhoton.interleavedBuffer = new IPLfloat32[bufFromPhoton.numSamples * formatOut.numSpeakers]; //output is stereo, so multiple by 2 channels

																										//--- create renderer & effects
	binRen = new IPLhandle; //IMPORTANT, need to allocate it first, if not, will raise error
	IPLHrtfParams hrtfParams{ IPL_HRTFDATABASETYPE_DEFAULT, nullptr, 0, nullptr, nullptr, nullptr };

	IPLerror check = iplCreateBinauralRenderer(iplContext, iplRenderSettings, hrtfParams, binRen);
	if (check != IPL_STATUS_SUCCESS)
		cout << "ERROR iplCreateBinauralRenderer: " << check << endl;

	binEff = new IPLhandle;
	check = iplCreateBinauralEffect(*binRen, formatIn, formatOut, binEff); //IMPORTANT: use *binRen, if not, will error
	if (check != IPL_STATUS_SUCCESS)
		cout << "ERROR iplCreateBinauralEffect: " << check << endl;


	/*Sound::StereophonicManager::getInstance()->add(wav, pos, dir, soundPos);
	int a = 0;*/
}

void gameApp::mouseDown( MouseEvent event )
{
	ENV->mouseDown(event);
    CAMERA->shakeCamera( 0.1f, 0.1f );
}

void gameApp::mouseUp( MouseEvent event )
{
    ENV->mouseUp( event );
}

void gameApp::keyDown( KeyEvent event )
{
    ENV->keyDown( event );
}

void gameApp::keyUp( KeyEvent event )
{
    ENV->keyUp( event );
}

void gameApp::mouseMove( MouseEvent event )
{
    ENV->mouseMove( event );
}

void gameApp::mouseDrag( MouseEvent event )
{
    ENV->mouseDrag( event );
}

void gameApp::update( )
{
	float elapsedSeconds = app::getElapsedSeconds( );
    float delta = elapsedSeconds - prevSeconds;
    prevSeconds = elapsedSeconds;

	 float t = 0.0F;
	 if ( getAverageFps( ) == 0.0F )
	 {
		 t = 1.0F / getFpsSampleInterval( );
	 }
	 else
	 {
		 t = 1.0F / getAverageFps( );
	 }
    cSceneManager::getInstance( )->now( ).update( t );
	CAMERA->update(delta);
}

void gameApp::draw( )
{
    gl::clear( Color( 0, 0, 0 ) );

    CAMERA->bind3D( );
    cSceneManager::getInstance( )->now( ).draw( );
    CAMERA->unBind3D( );

    CAMERA->bind2D( );
    cSceneManager::getInstance( )->now( ).draw2D( );
    CAMERA->unBind2D( );

    ENV->flashInput( );
}

void gameApp::cleanup()
{
    cSceneManager::getInstance()->now().shutDown();
	iplDestroyBinauralEffect(binEff);
	iplDestroyBinauralRenderer(binRen);
}

CINDER_APP( gameApp, RendererGl, [ ] ( App::AppBase::Settings* s ) { s->setWindowSize( 1600, 900 ); } )
