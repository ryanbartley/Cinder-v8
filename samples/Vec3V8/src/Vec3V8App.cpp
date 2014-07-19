#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "v8Engine.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Vec3V8App : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	v8Engine::EngineRef v8;
};

void Vec3V8App::setup()
{
	v8::V8::InitializeICU();
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8 = v8Engine::Engine::create( isolate );
}

void Vec3V8App::mouseDown( MouseEvent event )
{
	v8->executeScript( "test.js" );
}

void Vec3V8App::update()
{
}

void Vec3V8App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( Vec3V8App, RendererGl )
