#pragma once

#include "../../data/all.cpp"


namespace VM {
	
	bool object_is_true( Object* o ) {
		switch ( o->type ) {
			case TYPE_NONE: {
				return false;
			} break;
			case TYPE_BOOL: {
				auto _o = reinterpret_cast<ObjectBool*>( o->data );
				return _o->value != false;
			} break;
			case TYPE_BYTE: {
				auto _o = reinterpret_cast<ObjectByte*>( o->data );
				return _o->value != 0;
			} break;
			case TYPE_INT: {
				auto _o = reinterpret_cast<ObjectInt*>( o->data );
				return _o->value != 0;
			} break;
			case TYPE_FLOAT: {
				auto _o = reinterpret_cast<ObjectFloat*>( o->data );
				return _o->value != 0.0;
			} break;
			case TYPE_STRING: {
				auto _o = reinterpret_cast<ObjectString*>( o->data );
				return !_o->value.empty();
			} break;
			case TYPE_CALLABLE: {
				return true;
			} break;
			case TYPE_EXTERNAL: {
				return true;
			} break;
			case TYPE_ARRAY: {
				auto _o = reinterpret_cast<ObjectArray*>( o->data );
				return _o->data.size() != 0;
			} break;
			case TYPE_MAP: {
				auto _o = reinterpret_cast<ObjectMap*>( o->data );
				return _o->data.size() != 0;
			} break;
			case TYPE_LIST: {
				auto _o = reinterpret_cast<ObjectList*>( o->data );
				return _o->arr.size() != 0 || _o->map.size() != 0;
			} break;
			case TYPE_CLASS: {
				auto _o = reinterpret_cast<ObjectClass*>( o->data );
				return _o->data.size() != 0;
			} break;
			case TYPE_INSTANCE: {
				auto _o = reinterpret_cast<ObjectInstance*>( o->data );
				return _o->data.size() != 0;
			} break;
			default: throw std::runtime_error( "Incorrect operand type" );
		}
	}

}
