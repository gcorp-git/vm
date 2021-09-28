#pragma once

#include "../../data/all.cpp"


namespace VM {
	
	Object* add( Scope* scope, Object* a, Object* b ) {
		if ( a->type != b->type ) throw std::runtime_error( "Mismatched operand types" );

		switch ( a->type ) {
			case TYPE_NONE: throw std::runtime_error( "Incorrect operand type" );
			case TYPE_BOOL: {
				auto _a = reinterpret_cast<ObjectBool*>( a->data );
				auto _b = reinterpret_cast<ObjectBool*>( b->data );
				return object_create_bool( scope, _a->value + _b->value != 0 );
			} break;
			case TYPE_BYTE: {
				auto _a = reinterpret_cast<ObjectByte*>( a->data );
				auto _b = reinterpret_cast<ObjectByte*>( b->data );
				return object_create_byte( scope, _a->value + _b->value );
			} break;
			case TYPE_INT: {
				auto _a = reinterpret_cast<ObjectInt*>( a->data );
				auto _b = reinterpret_cast<ObjectInt*>( b->data );
				return object_create_int( scope, _a->value + _b->value );
			} break;
			case TYPE_FLOAT: {
				auto _a = reinterpret_cast<ObjectFloat*>( a->data );
				auto _b = reinterpret_cast<ObjectFloat*>( b->data );
				return object_create_float( scope, _a->value + _b->value );
			} break;
			case TYPE_STRING: {
				auto _a = reinterpret_cast<ObjectString*>( a->data );
				auto _b = reinterpret_cast<ObjectString*>( b->data );
				return object_create_string( scope, _a->value + _b->value );
			} break;
			case TYPE_CALLABLE: throw std::runtime_error( "Incorrect operand type" );
			case TYPE_EXTERNAL: throw std::runtime_error( "Incorrect operand type" );
			case TYPE_ARRAY: {
				// todo: add arrays
				throw std::runtime_error( "NOT IMPLEMENTED YET" );
			} break;
			case TYPE_MAP: {
				// todo: add objects
				throw std::runtime_error( "NOT IMPLEMENTED YET" );
			} break;
			case TYPE_LIST: {
				// todo: add lists
				throw std::runtime_error( "NOT IMPLEMENTED YET" );
			} break;
			case TYPE_CLASS: {
				// todo: add classes
				throw std::runtime_error( "NOT IMPLEMENTED YET" );
			} break;
			case TYPE_INSTANCE: {
				// todo: add instance
				
				// call magic method :add()
				
				throw std::runtime_error( "NOT IMPLEMENTED YET" );
			} break;
			default: throw std::runtime_error( "Incorrect operand type" );
		}
	}

}
