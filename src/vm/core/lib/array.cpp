#pragma once

#include "../error.cpp"
#include "../reg.cpp"
#include "../args.cpp"


namespace VM {

	int64_t _lib_array_get_index_value( Object* index );

	void lib_array_push() {
		auto arr = reg_get( *(_STATE->pos + 1) );
		auto o = reg_get( *(_STATE->pos + 2) );

		object_array_push( arr, o );

		_STATE->offset += 2;
	}
	void lib_array_pop() {
		auto arr = reg_get( *(_STATE->pos + 2) );
		auto o = object_array_pop( arr );

		reg_set( *(_STATE->pos + 1), o );

		_STATE->offset += 2;
	}
	void lib_array_set() {
		auto arr = reg_get( *(_STATE->pos + 1) );
		auto index = _lib_array_get_index_value( reg_get( *(_STATE->pos + 2) ) );
		auto o = reg_get( *(_STATE->pos + 3) );
		
		object_array_set( arr, index, o );

		_STATE->offset += 3;
	}
	void lib_array_get() {
		auto arr = reg_get( *(_STATE->pos + 2) );
		auto index = _lib_array_get_index_value( reg_get( *(_STATE->pos + 3) ) );
		auto o = object_array_get( arr, index );

		reg_set( *(_STATE->pos + 1), o );

		_STATE->offset += 3;
	}

	int64_t _lib_array_get_index_value( Object* index ) {
		switch ( index->type ) {
			case TYPE_BYTE: {
				auto _index = reinterpret_cast<ObjectByte*>( index->data );
				return static_cast<int64_t>( _index->value );
			} break;
			case TYPE_INT: {
				auto _index = reinterpret_cast<ObjectInt*>( index->data );
				return _index->value;
			} break;
		}

		_error( "LIB: incorrect operand type" );
		
		return 0;
	}

}
