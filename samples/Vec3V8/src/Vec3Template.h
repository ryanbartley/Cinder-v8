//
//  Vec3.h
//  Vec3V8
//
//  Created by Ryan Bartley on 7/18/14.
//
//

#include "v8.h"
#include "cinder/Log.h"

#pragma once

using namespace v8;

namespace v8Engine {

template<typename T>
class Vec3 {
public:
	static inline void init( Handle<ObjectTemplate> global, Isolate* isolate );
	static inline void create(const v8::FunctionCallbackInfo<v8::Value>& args);
	static inline v8::Handle<v8::FunctionTemplate> MakeVec3( v8::Isolate* isolate );
	static inline void destroy();

	static Persistent<FunctionTemplate> kVec3Template;
	static std::string kConstructorName;
};
	
template<typename T>
inline void getX(v8::Local<String> name, const PropertyCallbackInfo<v8::Value> &info);
template<typename T>
inline void setX(Local<String> property,
				 Local<Value> value,
				 const PropertyCallbackInfo<void>& info);
template<typename T>
inline void getY(v8::Local<String> name, const PropertyCallbackInfo<v8::Value> &info);
template<typename T>
inline void setY(Local<String> property,
				 Local<Value> value,
				 const PropertyCallbackInfo<void>& info);
template<typename T>
inline void getZ(v8::Local<String> name, const PropertyCallbackInfo<v8::Value> &info);
template<typename T>
inline void setZ(Local<String> property,
				 Local<Value> value,
				 const PropertyCallbackInfo<void>& info);
template<typename T>
inline void add(const v8::FunctionCallbackInfo<v8::Value>& args);
template<typename T>
inline void addEq(const v8::FunctionCallbackInfo<v8::Value>& args);
template<typename T>
inline void multEq(const v8::FunctionCallbackInfo<v8::Value>& args);
	
template<typename T>
Persistent<FunctionTemplate> Vec3<T>::kVec3Template;
	
template<>
std::string Vec3<float>::kConstructorName("Vec3f");
	
template<>
std::string Vec3<int32_t>::kConstructorName("Vec3i");
	
template<typename T>
std::string Vec3<T>::kConstructorName(typeid(T).name());
	
template<typename T>
void Vec3<T>::init( Handle<ObjectTemplate> global, Isolate* isolate )
{
	EscapableHandleScope handle_scope(isolate);
	
	if(kVec3Template.IsEmpty()) {
		Handle<FunctionTemplate> raw_temp = MakeVec3( isolate );
		kVec3Template.Reset( isolate, raw_temp );
	}
	
	Local<FunctionTemplate> temp = Local<FunctionTemplate>::New( isolate, kVec3Template );
	
	
//	Local<FunctionTemplate> type =
//	type->SetClassName( String::NewFromUtf8( isolate, kConstructorName.c_str() ) );
//	type->InstanceTemplate()->SetInternalFieldCount(1);
//	auto prototype = type->PrototypeTemplate();
//
//	prototype->SetAccessor(String::NewFromUtf8(isolate, "x", String::kInternalizedString),
//						getX<T> );
//	prototype->SetAccessor(String::NewFromUtf8(isolate, "y", String::kInternalizedString),
//						getY<T>, setY<T>);
//	prototype->SetAccessor(String::NewFromUtf8(isolate, "z", String::kInternalizedString),
//						getZ<T>, setZ<T>);
//	prototype->Set(String::NewFromUtf8(isolate, "multEq", String::kInternalizedString),
//				FunctionTemplate::New( isolate, multEq<T> ) );
//	prototype->Set(String::NewFromUtf8(isolate, "addEq", String::kInternalizedString),
//				FunctionTemplate::New( isolate, addEq<T> ) );
//	prototype->Set(String::NewFromUtf8(isolate, "add", String::kInternalizedString),
//				FunctionTemplate::New( isolate, add<T> ) );
//	kVec3Template.Reset( isolate, type );
//	global->Set( String::NewFromUtf8(isolate, kConstructorName.c_str() ),
//				type );
}
	
template<typename T>
inline v8::Handle<v8::FunctionTemplate> Vec3<T>::MakeVec3( v8::Isolate* isolate )
{
	EscapableHandleScope escapable_handle(isolate);
	
	Local<FunctionTemplate> func = FunctionTemplate::New( isolate );
	func->SetClassName( String::NewFromUtf8( isolate, kConstructorName.c_str() ) );
	
	Handle<ObjectTemplate> result = func->InstanceTemplate();
	
	result->SetInternalFieldCount(1);

	result->SetAccessor(String::NewFromUtf8(isolate, "x", String::kInternalizedString),
						   getX<T> );
	result->SetAccessor(String::NewFromUtf8(isolate, "y", String::kInternalizedString),
						   getY<T>, setY<T>);
	result->SetAccessor(String::NewFromUtf8(isolate, "z", String::kInternalizedString),
						   getZ<T>, setZ<T>);
	result->Set(String::NewFromUtf8(isolate, "multEq", String::kInternalizedString),
				   FunctionTemplate::New( isolate, multEq<T> ) );
	result->Set(String::NewFromUtf8(isolate, "addEq", String::kInternalizedString),
				   FunctionTemplate::New( isolate, addEq<T> ) );
	result->Set(String::NewFromUtf8(isolate, "add", String::kInternalizedString),
				   FunctionTemplate::New( isolate, add<T> ) );
	
	return escapable_handle.Escape(func);
}

template<typename T>
inline v8::Handle<v8::Object> wrapVec3(v8::Isolate* isolate, ci::Vec3<T>* vec) {
	// Handle scope for temporary handles.
	EscapableHandleScope handle_scope(isolate);
	
	// FUCKKKKKKKKKKKKKKKKKKKK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	// Fetch the template for creating JavaScript http request wrappers.
	// It only has to be created once, which we do on demand.
	Handle<FunctionTemplate> templ =
	Local<FunctionTemplate>::New(isolate, Vec3<T>::kVec3Template);
	
	std::cout << "I'm wrapping this over and over" << std::endl;
	// Create an empty http request wrapper.
	Local<Object> result = templ->InstanceTemplate()->NewInstance();
//
//	// Wrap the raw C++ pointer in an External so it can be referenced
//	// from within JavaScript.
	Handle<External> request_ptr = External::New(isolate, vec);
//
//	// Store the request pointer in the JavaScript wrapper.
	result->SetInternalField(0, request_ptr);
//
//	// Return the result through the current handle scope.  Since each
//	// of these handles will go away when the handle scope is deleted
//	// we need to call Close to let one, the result, escape into the
//	// outer handle scope.
	return handle_scope.Escape(result);
}

template<typename T>
inline static ci::Vec3<T>* unwrapVec3(Handle<Object> obj) {
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();
	return static_cast<ci::Vec3<T>*>(ptr);
}

template<typename T>
void Vec3<T>::destroy()
{
	Vec3<T>::kVec3Template.Reset();
}

template<typename T>
void Vec3<T>::create(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	static int count = 0;
	String::Utf8Value name(args.Callee()->GetName()->ToString());
	std::cout << count++ << " Num Args: " << *(name) << std::endl;
	
	// throw if called without `new'
	if (!args.IsConstructCall()) {
		CI_LOG_E("ERROR: Can't call this without new");
		args.GetReturnValue().SetUndefined();
	}
	
	//start a handle scope
	v8::HandleScope handle_scope(args.GetIsolate());
	ci::Vec3<T>* newVec = nullptr;
	if( args.Length() == 1 ) {
		newVec = new ci::Vec3<T>( *(unwrapVec3<T>( args[0]->ToObject()) ) );
		std::cout << "Copy Constructor" << std::endl;
	}
	else if( args.Length() == 3 ) {
		//get an x and y
		double x = args[0]->NumberValue();
		double y = args[1]->NumberValue();
		double z = args[2]->NumberValue();
		
		//generate a new point
		newVec = new ci::Vec3<T>(x, y, z);
		std::cout << "3 elements constructor" << std::endl;
	}
	
	if( newVec ) {
		std::cout << "Hello I'm in here" << std::endl;
//		args.GetReturnValue().Set( wrapVec3<T>(args.GetIsolate(), newVec) );
		return;
	}
	else {
		std::cout << "Hello I'm NOT in here" << std::endl;
//		args.GetReturnValue().Set( wrapVec3<T>( args.GetIsolate(), new ci::Vec3<T>() ) );
	}
}


template<typename T>
inline void getX(v8::Local<String> name, const PropertyCallbackInfo<v8::Value> &info)
{
	std::cout << "COnstructor: " << Vec3<T>::kVec3Template.IsEmpty() << std::endl;
	// Extract the C++ request object from the JavaScript wrapper.
	ci::Vec3<T>* request = unwrapVec3<T>(info.Holder());
	
	// Fetch the path.
	float x = request->x;
	v8::String::Utf8Value param1(name->ToString());
	std::cout << *(param1) << std::endl;
	// Wrap the result in a JavaScript string and return it.
	info.GetReturnValue().Set(String::NewFromUtf8( info.GetIsolate(),
												  std::to_string(x).c_str(),
												  String::kNormalString ));
}

