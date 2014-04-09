#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "v8.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace v8;

class HelloWorldApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void HelloWorldApp::setup()
{
	// Get the default Isolate created at startup.
	Isolate* isolate = Isolate::GetCurrent();
	
	// Create a stack-allocated handle scope.
	HandleScope handle_scope(isolate);
	
	// Create a new context.
	Handle<Context> context = Context::New(isolate);
	
	// Enter the context for compiling and running the hello world script.
	Context::Scope context_scope(context);
	
	// Create a string containing the JavaScript source code.
	Handle<String> source = String::NewFromUtf8(isolate, "'Hello' + ', World!'");
	
	// Compile the source code.
	Handle<Script> script = Script::Compile(source);
	
	// Run the script to get the result.
	Handle<Value> result = script->Run();
	
	// Convert the result to an UTF8 string and print it.
	String::Utf8Value utf8(result);
	printf("%s\n", *utf8);
}

void HelloWorldApp::mouseDown( MouseEvent event )
{
}

void HelloWorldApp::update()
{
}

void HelloWorldApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( HelloWorldApp, RendererGl )
