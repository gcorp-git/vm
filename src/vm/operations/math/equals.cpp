#pragma once

#include "../../data/all.cpp"


namespace VM {
	
	bool equals( Object* a, Object* b ) {
		switch ( a->type ) {
			case TYPE_NONE: {
				return true;
			} break;
			case TYPE_BOOL: {
				auto _a = reinterpret_cast<ObjectBool*>( a->data );
				auto _b = reinterpret_cast<ObjectBool*>( b->data );
				return _a->value == _b->value;
			} break;
			case TYPE_BYTE: {
				auto _a = reinterpret_cast<ObjectByte*>( a->data );
				auto _b = reinterpret_cast<ObjectByte*>( b->data );
				return _a->value == _b->value;
			} break;
			case TYPE_INT: {
				auto _a = reinterpret_cast<ObjectInt*>( a->data );
				auto _b = reinterpret_cast<ObjectInt*>( b->data );
				return _a->value == _b->value;
			} break;
			case TYPE_FLOAT: {
				auto _a = reinterpret_cast<ObjectFloat*>( a->data );
				auto _b = reinterpret_cast<ObjectFloat*>( b->data );
				return _a->value == _b->value;
			} break;
			case TYPE_STRING: {
				auto _a = reinterpret_cast<ObjectString*>( a->data );
				auto _b = reinterpret_cast<ObjectString*>( b->data );
				return _a->value == _b->value;
			} break;
			case TYPE_CALLABLE: {
				return a->id == b->id;
			} break;
			case TYPE_EXTERNAL: {
				return a->id == b->id;
			} break;
			case TYPE_ARRAY: {
				return a->id == b->id;
			} break;
			case TYPE_MAP: {
				return a->id == b->id;
			} break;
			case TYPE_LIST: {
				return a->id == b->id;
			} break;
			case TYPE_CLASS: {
				return a->id == b->id;
			} break;
			case TYPE_INSTANCE: {
				return a->id == b->id;
			} break;
			default: throw std::runtime_error( "Incorrect operand type" );
		}
	}

}
