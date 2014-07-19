#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Interfaces.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ProcessRequestsApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void ProcessRequestsApp::setup()
{
	v8::V8::InitializeICU();
	map<string, string> options;
	string file("Process.js");
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	Handle<String> source = ReadFile(isolate, file);
	if (source.IsEmpty()) {
		fprintf(stderr, "Error reading '%s'.\n", file.c_str());
		return;
	}
	JsHttpRequestProcessor processor(isolate, source);
	map<string, string> output;
	if (!processor.Initialize(&options, &output)) {
		fprintf(stderr, "Error initializing processor.\n");
		return;
	}
	if (!ProcessEntries(&processor, kSampleSize, kSampleRequests))
		return;
	PrintMap(&output);
}

void ProcessRequestsApp::mouseDown( MouseEvent event )
{
}

void ProcessRequestsApp::update()
{
}

void ProcessRequestsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( ProcessRequestsApp, RendererGl )
