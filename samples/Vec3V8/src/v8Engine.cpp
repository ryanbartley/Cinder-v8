//
//  v8Engine.cpp
//  Vec3V8
//
//  Created by Ryan Bartley on 7/17/14.
//
//

#include "v8Engine.h"
#include "cinder/Log.h"
#include "Vec3v82.h"

using namespace v8;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace v8Engine {

EngineRef Engine::create( v8::Isolate *isolate )
{
	return EngineRef( new Engine( isolate ) );
}

Engine::~Engine()
{
	// Dispose the persistent handles.  When noone else has any
	// references to the objects stored in the handles they will be
	// automatically reclaimed.
	Vec3<float>::destroy();
//	Vec3<int32_t>::destroy();
	mContext.Reset();
}

void Engine::initialize()
{
	// Create a handle scope to hold the temporary references.
	HandleScope handleScope(getIsolate());
	
	// Create a template for the global object where we set the
	// built-in global functions.
	Handle<ObjectTemplate> global = ObjectTemplate::New( getIsolate() );
	Vec3<float>::init( global, getIsolate() );
	
	global->Set(String::NewFromUtf8(getIsolate(), "log"),
				FunctionTemplate::New(getIsolate(), LogCallback));
	
	
	// Each processor gets its own context so different processors don't
	// affect each other. Context::New returns a persistent handle which
	// is what we need for the reference to remain after we return from
	// this method. That persistent handle has to be disposed in the
	// destructor.
	v8::Handle<v8::Context> context = Context::New(getIsolate(), NULL, global);
	
	mContext.Reset(getIsolate(), context);
	
	Context::Scope scope(context);
	
	//create function template for our constructor
	//it will call the constructPoint function
	//set the function in the global scope -- that is, set "Point" to the constructor
	
	executeScript( "test.js" );
}

bool Engine::executeScript( const std::string &assetName )
{
	HandleScope handle_scope(getIsolate());
	v8::Local<v8::Context> context = v8::Local<v8::Context>::New(getIsolate(), mContext);
	Context::Scope scope(context);
	auto fileContents = ci::app::loadAsset( assetName );
	auto chars = static_cast<char*>(fileContents->getBuffer().getData());
	chars[fileContents->getBuffer().getDataSize()] = '\0';
	Handle<String> result =
	String::NewFromUtf8( getIsolate(),
						chars,
						String::kNormalString);
	std::cout << chars << std::endl;
	return executeScript( result );
}

bool Engine::executeScript( v8::Handle<v8::String> script )
{
	// We're just about to compile the script; set up an error handler to
	// catch any exceptions the script might throw.
	TryCatch try_catch;
	
	// Compile the script and check for errors.
	Handle<Script> compiled_script = Script::Compile(script);
	if (compiled_script.IsEmpty()) {
		String::Utf8Value error(try_catch.Exception());
		CI_LOG_E( *error );
		// The script failed to compile; bail out.
		return false;
	}
	
	// Run the script!
	Handle<Value> result = compiled_script->Run();
	if (result.IsEmpty()) {
		// The TryCatch above is still in effect and will have caught the error.
		String::Utf8Value error(try_catch.Exception());
		CI_LOG_E( *error );
		// Running the script failed; bail out.
		return false;
	}
	return true;
}

}