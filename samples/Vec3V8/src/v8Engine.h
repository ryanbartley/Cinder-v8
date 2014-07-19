//
//  v8Engine.h
//  Vec3V8
//
//  Created by Ryan Bartley on 7/17/14.
//
//

#include "v8.h"

namespace v8Engine {
	
typedef std::shared_ptr<class Engine> EngineRef;

class Engine {
public:
	
	static EngineRef create( v8::Isolate* isolate );
	
	~Engine();
	
	bool executeScript( const std::string &assetName );
	bool executeScript( v8::Handle<v8::String> script );
	
private:
	Engine( v8::Isolate* isolate ) : mIsolate( isolate ) { initialize(); }
	
	void initialize();
	
	v8::Isolate* getIsolate() { return mIsolate; }
	
	v8::Isolate* mIsolate;
	v8::Handle<v8::String> mCurrentScript;
	v8::Persistent<v8::Context> mContext;
};

static void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
	using namespace v8;
	if (args.Length() < 1) return;
	HandleScope scope(args.GetIsolate());
	for( int i = 0; i < args.Length(); i++ ) {
		Handle<Value> arg = args[i];
		String::Utf8Value value(arg);
		std::cout << *value << std::endl;
	}
}

}