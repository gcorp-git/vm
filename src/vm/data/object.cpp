#pragma once

#include <stdexcept>
#include <string>
#include <functional>
#include <algorithm>
#include <vector>
#include <list>
#include <tuple>
#include <unordered_map>


namespace VM {

	uint64_t _OBJECT_ID = 0;

	enum OBJECT_TYPE_ENUM {
		TYPE_NONE,
		TYPE_BOOL,
		TYPE_BYTE,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_STRING,
		TYPE_CALLABLE,
		TYPE_EXTERNAL,
		TYPE_ARRAY,
		TYPE_MAP,
		TYPE_LIST,
		TYPE_CLASS,
		TYPE_INSTANCE,
	};

	enum CALLABLE_TYPE_ENUM {
		CALLABLE_SOFT_BLOCK,
		CALLABLE_HARD_BLOCK,
		CALLABLE_FUNCTION,
	};

	struct Scope;

	struct Object {
		uint64_t id = 0;
		OBJECT_TYPE_ENUM type = TYPE_NONE;
		void* data = NULL;
		Scope* scope = NULL;
		std::unordered_map<uint64_t, Scope*> scopes = {};
		bool isBeingDestroyed = false;
	};

	typedef std::function<Object*(Scope*, std::vector<Object*>*, std::unordered_map<std::string, Object*>*)> ExternalFunction;

	struct ObjectBool {
		bool value = false;
	};

	struct ObjectByte {
		uint8_t value = 0;
	};

	struct ObjectInt {
		int64_t value = 0;
	};

	struct ObjectFloat {
		double value = 0.0;
	};

	struct ObjectString {
		std::string value = "";
	};

	struct VM_BYTECODE {
		uint8_t* data = NULL;
		uint64_t offset = 0;
		uint64_t size = 0;
	};

	struct ObjectCallable {
		CALLABLE_TYPE_ENUM type = CALLABLE_SOFT_BLOCK;
		VM_BYTECODE* bc = NULL;
		uint64_t offset = 0;
		uint64_t size = 0;
	};

	struct ObjectExternal {
		CALLABLE_TYPE_ENUM type = CALLABLE_SOFT_BLOCK;
		ExternalFunction function;
	};

	struct ObjectArray {
		std::vector<Object*> data;
	};

	struct ObjectMap {
		std::unordered_map<std::string, Object*> data;
	};

	struct ObjectList {
		std::vector<Object*> arr;
		std::unordered_map<std::string, Object*> map;
	};

	struct ObjectClass {
		std::unordered_map<std::string, Object*> data;
	};

	struct ObjectInstance {
		std::unordered_map<std::string, Object*> data;
		Object* _class = NULL;
	};

	std::unordered_map<uint64_t, Object*> objects = {};
	std::unordered_map<uint64_t, ObjectBool*> object_bool_map = {};
	std::unordered_map<uint64_t, ObjectByte*> object_byte_map = {};
	std::unordered_map<uint64_t, ObjectInt*> object_int_map = {};
	std::unordered_map<uint64_t, ObjectFloat*> object_float_map = {};
	std::unordered_map<uint64_t, ObjectString*> object_string_map = {};
	std::unordered_map<uint64_t, ObjectCallable*> object_callable_map = {};
	std::unordered_map<uint64_t, ObjectExternal*> object_external_map = {};
	std::unordered_map<uint64_t, ObjectArray*> object_array_map = {};
	std::unordered_map<uint64_t, ObjectMap*> object_map_map = {};
	std::unordered_map<uint64_t, ObjectList*> object_list_map = {};
	std::unordered_map<uint64_t, ObjectClass*> object_class_map = {};
	std::unordered_map<uint64_t, ObjectInstance*> object_instance_map = {};

	void scope_attach( Scope* scope, Object* o );
	void scope_detach( Scope* scope, Object* o );

	Object* object_create( Scope* scope, OBJECT_TYPE_ENUM type, void* data ) {
		auto o = new Object();

		o->id = ++_OBJECT_ID;
		o->type = type;
		o->data = data;
		o->scope = scope;
		o->scopes = {};
		o->isBeingDestroyed = false;

		objects.insert({ o->id, o });

		scope_attach( scope, o );

		return o;
	}

	Object* object_create_none( Scope* scope ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );

		return object_create( scope, TYPE_NONE, NULL );
	}

	Object* object_create_bool( Scope* scope, bool value=false ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );

		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectBool();

		_o->value = value;

		object_bool_map.insert({ id, _o });

		return object_create( scope, TYPE_BOOL, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_byte( Scope* scope, uint8_t value=0 ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectByte();

		_o->value = value;

		object_byte_map.insert({ id, _o });

		return object_create( scope, TYPE_BYTE, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_int( Scope* scope, int64_t value=0 ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectInt();

		_o->value = value;

		object_int_map.insert({ id, _o });

		return object_create( scope, TYPE_INT, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_float( Scope* scope, double value=0.0 ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectFloat();

		_o->value = value;

		object_float_map.insert({ id, _o });

		return object_create( scope, TYPE_FLOAT, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_string( Scope* scope, std::string value="" ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectString();

		_o->value = value;

		object_string_map.insert({ id, _o });

		return object_create( scope, TYPE_STRING, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_callable(
		Scope* scope,
		CALLABLE_TYPE_ENUM type,
		VM_BYTECODE* bc,
		uint64_t offset=0,
		uint64_t size=0
	) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectCallable();

		_o->type = type;
		_o->bc = bc;
		_o->offset = offset;
		_o->size = size;

		object_callable_map.insert({ id, _o });

		return object_create( scope, TYPE_CALLABLE, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_external(
		Scope* scope,
		ExternalFunction function
	) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectExternal();

		_o->function = function;

		object_external_map.insert({ id, _o });

		return object_create( scope, TYPE_EXTERNAL, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_array( Scope* scope ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectArray();

		_o->data = {};

		object_array_map.insert({ id, _o });

		return object_create( scope, TYPE_ARRAY, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_map( Scope* scope ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectMap();

		_o->data = {};

		object_map_map.insert({ id, _o });

		return object_create( scope, TYPE_MAP, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_list( Scope* scope ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectList();

		_o->arr = {};
		_o->map = {};

		object_list_map.insert({ id, _o });

		return object_create( scope, TYPE_LIST, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_class( Scope* scope ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectClass();

		_o->data = {};

		object_class_map.insert({ id, _o });

		return object_create( scope, TYPE_CLASS, reinterpret_cast<void*>( _o ) );
	}

	Object* object_create_instance( Scope* scope, Object* c ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		// todo: object_create_instance()

		auto id = _OBJECT_ID + 1;

		auto _o = new ObjectInstance();

		_o->data = {};
		_o->_class = c;

		// todo: copy class props into instance

		// todo: inheritance

		object_instance_map.insert({ id, _o });

		return object_create( scope, TYPE_INSTANCE, reinterpret_cast<void*>( _o ) );
	}

	void object_destroy( Object* o ) {
		if ( o->isBeingDestroyed ) return;

		auto found = objects.find( o->id );

		if ( found == objects.end() ) return;
		
		o->isBeingDestroyed = true;

		objects.erase( found );

		if ( !objects.size() ) _OBJECT_ID = 0;

		for ( auto it : o->scopes ) scope_detach( it.second, o );

		switch ( o->type ) {
			case TYPE_BOOL: {
				auto _o = object_bool_map[ o->id ];
				auto found = object_bool_map.find( o->id );
				object_bool_map.erase( found );
				delete _o;
			} break;
			case TYPE_BYTE: {
				auto _o = object_byte_map[ o->id ];
				auto found = object_byte_map.find( o->id );
				object_byte_map.erase( found );
				delete _o;
			} break;
			case TYPE_INT: {
				auto _o = object_int_map[ o->id ];
				auto found = object_int_map.find( o->id );
				object_int_map.erase( found );
				delete _o;
			} break;
			case TYPE_FLOAT: {
				auto _o = object_float_map[ o->id ];
				auto found = object_float_map.find( o->id );
				object_float_map.erase( found );
				delete _o;
			} break;
			case TYPE_STRING: {
				auto _o = object_string_map[ o->id ];
				auto found = object_string_map.find( o->id );
				object_string_map.erase( found );
				delete _o;
			} break;
			case TYPE_CALLABLE: {
				auto found = object_callable_map.find( o->id );
				auto _o = object_callable_map[ o->id ];
				object_callable_map.erase( found );
				delete _o;
			} break;
			case TYPE_EXTERNAL: {
				auto found = object_external_map.find( o->id );
				auto _o = object_external_map[ o->id ];
				object_external_map.erase( found );
				delete _o;
			} break;
			case TYPE_ARRAY: {
				auto _o = object_array_map[ o->id ];
				auto found = object_array_map.find( o->id );
				object_array_map.erase( found );
				delete _o;
			} break;
			case TYPE_MAP: {
				auto _o = object_map_map[ o->id ];
				auto found = object_map_map.find( o->id );
				object_map_map.erase( found );
				delete _o;
			} break;
			case TYPE_LIST: {
				auto _o = object_list_map[ o->id ];
				auto found = object_list_map.find( o->id );
				object_list_map.erase( found );
				delete _o;
			} break;
			case TYPE_CLASS: {
				auto _o = object_class_map[ o->id ];
				auto found = object_class_map.find( o->id );
				object_class_map.erase( found );
				delete _o;
			} break;
			case TYPE_INSTANCE: {
				auto _o = object_instance_map[ o->id ];
				auto found = object_instance_map.find( o->id );
				object_instance_map.erase( found );
				delete _o;
			} break;
		}

		delete o;
	}
	
}
