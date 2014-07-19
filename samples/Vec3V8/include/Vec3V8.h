//
//  Vec3V82.h
//  Vec3V8
//
//  Created by Ryan Bartley on 7/19/14.
//
//

#include "v8.h"

#include "boost/any.hpp"

namespace v8Engine {
	
template<typename T>
class Vec3 {
public:
	
	static inline void init( v8::Handle<v8::ObjectTemplate> global, v8::Isolate* isolate );
	static inline v8::Handle<v8::Object> wrap( v8::Isolate* isolate, ci::Vec3<T>* vec );
	static inline ci::Vec3<T>* unwrap( v8::Handle<v8::Object> obj );
	static inline void destroy();
	
	static inline void create(const v8::FunctionCallbackInfo<v8::Value>& args);
	static inline void getX( v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info );
	static inline void setX( v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info );
	static inline void getY( v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info );
	static inline void setY( v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info );
	static inline void getZ( v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info );
	static inline void setZ( v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info );
	
	static inline void add(const v8::FunctionCallbackInfo<v8::Value>& args);
	static inline void addEq(const v8::FunctionCallbackInfo<v8::Value>& args);
	static inline void multEq(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Persistent<v8::FunctionTemplate> kConstructor;
	static v8::Persistent<v8::FunctionTemplate> kPrototype;
	static const std::string kConstructorName;
};
	
template<typename T>
v8::Persistent<v8::FunctionTemplate> Vec3<T>::kConstructor;
template<typename T>
v8::Persistent<v8::FunctionTemplate> Vec3<T>::kPrototype;
	
// TODO: Test with the type mixing
template<>
const std::string Vec3<float>::kConstructorName("Vec3f");
template<>
const std::string Vec3<int32_t>::kConstructorName("Vec3i");
template<>
const std::string Vec3<double>::kConstructorName("Vec3d");
template<typename T>
const std::string Vec3<T>::kConstructorName("");

template<typename T>
void Vec3<T>::init( v8::Handle<v8::ObjectTemplate> global, v8::Isolate *isolate )
{
	v8::HandleScope handle_scope(isolate);
	
	v8::Local<v8::FunctionTemplate> proto = v8::FunctionTemplate::New(isolate);
	proto->SetClassName( v8::String::NewFromUtf8( isolate, kConstructorName.c_str() ) );
	v8::Local<v8::ObjectTemplate> proto_inst = proto->InstanceTemplate();
	proto_inst->SetInternalFieldCount(1);
	proto_inst->SetAccessor( v8::String::NewFromUtf8( isolate, "x"),
							getX, setX );
	proto_inst->SetAccessor( v8::String::NewFromUtf8( isolate, "y"),
							getY, setY );
	proto_inst->SetAccessor( v8::String::NewFromUtf8( isolate, "z"),
							getZ, setZ );
	proto_inst->Set(v8::String::NewFromUtf8(isolate, "multEq", v8::String::kInternalizedString),
					v8::FunctionTemplate::New( isolate, multEq ) );
	proto_inst->Set(v8::String::NewFromUtf8(isolate, "add", v8::String::kInternalizedString),
					v8::FunctionTemplate::New( isolate, add ) );
	proto_inst->Set(v8::String::NewFromUtf8(isolate, "addEq", v8::String::kInternalizedString),
					v8::FunctionTemplate::New( isolate, addEq ) );
	
	kPrototype.Reset( isolate, proto );
	
	global->Set( v8::String::NewFromUtf8( isolate, kConstructorName.c_str() ),
				v8::FunctionTemplate::New( isolate, create ) );
}

template<typename T>
void Vec3<T>::destroy()
{
	Vec3::kPrototype.Reset();
}
	
template<typename T>
v8::Handle<v8::Object> Vec3<T>::wrap( v8::Isolate *isolate, ci::Vec3<T> *vec )
{
	v8::EscapableHandleScope handle_scope(isolate);
	
	v8::Handle<v8::FunctionTemplate> temp1 =
	v8::Local<v8::FunctionTemplate>::New( isolate, kPrototype );
	
	v8::Local<v8::Object> result = temp1->InstanceTemplate()->NewInstance();
	
	v8::Handle<v8::External> wrappedVec = v8::External::New(isolate, vec );
	
	result->SetInternalField( 0, wrappedVec );
	
	return handle_scope.Escape( result );
}

template<typename T>
ci::Vec3<T>* Vec3<T>::unwrap( v8::Handle<v8::Object> obj )
{
	v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();
	return static_cast<ci::Vec3<T>*>(ptr);
}

template<typename T>
void Vec3<T>::create(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	// throw if called without `new'
	if (!args.IsConstructCall()) {
		CI_LOG_E("ERROR: Can't call this without new");
		args.GetReturnValue().SetUndefined();
		return;
	}
	
	//start a handle scope
	v8::HandleScope handle_scope(args.GetIsolate());
	ci::Vec3<T>* newVec = nullptr;
	if( args.Length() == 1 ) {
		newVec = new ci::Vec3<T>( *(unwrap( args[0]->ToObject()) ) );
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
		args.GetReturnValue().Set( wrap(args.GetIsolate(), newVec) );
		return;
	}
	else {
		std::cout << "Hello I'm NOT in here" << std::endl;
		args.GetReturnValue().SetUndefined();
	}
}
	
template<typename T>
inline void Vec3<T>::getX( v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info )
{
	// Extract the C++ request object from the JavaScript wrapper.
	ci::Vec3<T>* request = unwrap( info.Holder() );
	
	// Fetch the path.
	float x = request->x;
	
	// Wrap the result in a JavaScript string and return it.
	info.GetReturnValue().Set(x);
}
	
template<typename T>
inline void Vec3<T>::setX(v8::Local<v8::String> property,
						  v8::Local<v8::Value> value,
						  const v8::PropertyCallbackInfo<void>& info)
{
	// Extract the C++ request object from the JavaScript wrapper.
	ci::Vec3<T>* request = unwrap( info.Holder() );
	// Fetch the path.
	request->x = value->ToNumber()->NumberValue();

}

template<typename T>
inline void Vec3<T>::getY( v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info )
{
	// Extract the C++ request object from the JavaScript wrapper.
	ci::Vec3<T>* request = unwrap( info.Holder() );

	// Fetch the path.
	float y = request->y;

	// Wrap the result in a JavaScript string and return it.
	info.GetReturnValue().Set(y);
}

template<typename T>
inline void Vec3<T>::setY(v8::Local<v8::String> property,
						  v8::Local<v8::Value> value,
						  const v8::PropertyCallbackInfo<void>& info)
{
	// Extract the C++ request object from the JavaScript wrapper.
	ci::Vec3<T>* request = unwrap( info.Holder() );
	// Fetch the path.
	request->y = value->ToNumber()->NumberValue();
}

template<typename T>
inline void Vec3<T>::getZ( v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info )
{
	// Extract the C++ request object from the JavaScript wrapper.
	ci::Vec3<T>* request = unwrap( info.Holder() );

	// Fetch the path.
	float z = request->z;

	// Wrap the result in a JavaScript string and return it.
	info.GetReturnValue().Set(z);
}

template<typename T>
inline void Vec3<T>::setZ(v8::Local<v8::String> property,
						  v8::Local<v8::Value> value,
						  const v8::PropertyCallbackInfo<void>& info)
{
	// Extract the C++ request object from the JavaScript wrapper.
	ci::Vec3<T>* request = unwrap( info.Holder() );
	// Fetch the path.
	request->z = value->ToNumber()->NumberValue();

}

template<typename T>
inline void Vec3<T>::multEq(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::EscapableHandleScope scope(args.GetIsolate());
	std::cout << "I'm in mulEq" << std::endl;
	
	if(args.Length() == 1 && args[0]->IsObject() )
	{
		//Get the self object,
		v8::Local<v8::Object> self = args.Holder();

		v8::Local<v8::Object> rhObj = args[0]->ToObject();
		//Fetch the c++ pointer.
		//Get back to the type
		ci::Vec3<T>* thisVec3 = unwrap( self );
		ci::Vec3<T>* rhVec3 = unwrap( rhObj );

		if( thisVec3 && rhVec3 ) {
			std::cout << "This is a vec3f and I'm about to change the stuff" << std::endl;
			*thisVec3 *= *rhVec3;
		}
		else {
			std::cout << "This didn't cast" << std::endl;
		}
	}
	else {
		args.GetReturnValue().SetUndefined();
	}
}

template<typename T>
inline void Vec3<T>::addEq(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::EscapableHandleScope scope(args.GetIsolate());

	if(args.Length() == 1 && args[0]->IsObject() )
	{
		//Get the self object,
		v8::Local<v8::Object> me = args.Holder();

		v8::Local<v8::Object> rhObj = args[0]->ToObject();
		//Fetch the c++ pointer.
		//Get back to the type
		ci::Vec3<T>* thisVec3 = unwrap( me );
		ci::Vec3<T>* rhVec3 = unwrap( rhObj );

		if( thisVec3 && rhVec3 ) {
			std::cout << "This is a vec3f and I'm about to change the stuff" << std::endl;
			*thisVec3 += *rhVec3;
		}
		else {
			std::cout << "This didn't cast" << std::endl;
		}
	}
	else {
		args.GetReturnValue().SetUndefined();
	}
}

template<typename T>
inline void Vec3<T>::add(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::EscapableHandleScope scope(args.GetIsolate());

	if(args.Length() == 1 && args[0]->IsObject() )
	{
		//Get the self object,
		v8::Local<v8::Object> self = args.Holder();

		v8::Local<v8::Object> rhObj = args[0]->ToObject();
		//Fetch the c++ pointer.
		//Get back to the type
		ci::Vec3<T>* thisVec3 = unwrap( self );
		ci::Vec3<T>* rhVec3 = unwrap( rhObj );

		if( thisVec3 && rhVec3 ) {
			std::cout << "This is a vec3f and I'm about to change the stuff" << std::endl;
			ci::Vec3<T>* returnVec = new ci::Vec3<T>(*thisVec3 + *rhVec3);
			args.GetReturnValue().Set( wrap( args.GetIsolate(), returnVec ) );
		}
		else {
			std::cout << "This didn't cast" << std::endl;
		}
	}
	else {
		args.GetReturnValue().SetUndefined();
	}
}
	
}