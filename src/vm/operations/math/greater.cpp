#pragma once

#include "../../data/all.cpp"


namespace VM {
	
	bool greater( Object* a, Object* b ) {
		if ( a->type != b->type ) throw std::runtime_error( "Mismatched operand types" );

		switch ( a->type ) {
			case TYPE_NONE: {
				return false;
			} break;
			case TYPE_BOOL: {
				auto _a = reinterpret_cast<ObjectBool*>( a->data );
				auto _b = reinterpret_cast<ObjectBool*>( b->data );
				return _a->value > _b->value;
			} break;
			case TYPE_BYTE: {
				auto _a = reinterpret_cast<ObjectByte*>( a->data );
				auto _b = reinterpret_cast<ObjectByte*>( b->data );
				return _a->value > _b->value;
			} break;
			case TYPE_INT: {
				auto _a = reinterpret_cast<ObjectInt*>( a->data );
				auto _b = reinterpret_cast<ObjectInt*>( b->data );
				return _a->value > _b->value;
			} break;
			case TYPE_FLOAT: {
				auto _a = reinterpret_cast<ObjectFloat*>( a->data );
				auto _b = reinterpret_cast<ObjectFloat*>( b->data );
				return _a->value > _b->value;
			} break;
			case TYPE_STRING: {
				auto _a = reinterpret_cast<ObjectString*>( a->data );
				auto _b = reinterpret_cast<ObjectString*>( b->data );
				return _a->value > _b->value;
			} break;
			case TYPE_CALLABLE: {
				return false;
			} break;
			case TYPE_EXTERNAL: {
				return false;
			} break;
			case TYPE_ARRAY: {
				auto _a = reinterpret_cast<ObjectArray*>( a->data );
				auto _b = reinterpret_cast<ObjectArray*>( b->data );
				return _a->data.size() > _b->data.size();
			} break;
			case TYPE_MAP: {
				auto _a = reinterpret_cast<ObjectMap*>( a->data );
				auto _b = reinterpret_cast<ObjectMap*>( b->data );
				return _a->data.size() > _b->data.size();
			} break;
			case TYPE_LIST: {
				auto _a = reinterpret_cast<ObjectList*>( a->data );
				auto _b = reinterpret_cast<ObjectList*>( b->data );
				return ( _a->arr.size() + _a->map.size() ) > ( _b->arr.size() + _b->map.size() );
			} break;
			case TYPE_CLASS: {
				auto _a = reinterpret_cast<ObjectClass*>( a->data );
				auto _b = reinterpret_cast<ObjectClass*>( b->data );
				return _a->data.size() > _b->data.size();
			} break;
			case TYPE_INSTANCE: {
				auto _a = reinterpret_cast<ObjectInstance*>( a->data );
				auto _b = reinterpret_cast<ObjectInstance*>( b->data );
				return _a->data.size() > _b->data.size();
			} break;
			default: throw std::runtime_error( "Incorrect operand type" );
		}
	}

}
