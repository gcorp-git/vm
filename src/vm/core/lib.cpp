#pragma once

#include "lib/basic.cpp"
#include "lib/math.cpp"
#include "lib/io.cpp"
#include "lib/array.cpp"


namespace VM {

	enum VM_LIB_ENUM {
		VM_LIB_BASIC,
		VM_LIB_MATH,
		VM_LIB_IO,
		VM_LIB_ARRAY,
	};

	void lib_basic();
	void lib_math();
	void lib_io();
	void lib_array();

	void lib() {
		auto name = static_cast<VM_LIB_ENUM>( *_STATE->pos );

		_STATE->offset += 1;
		_STATE->pos += 1;
		
		switch ( name ) {
			case VM_LIB_BASIC: lib_basic(); break;
			case VM_LIB_MATH: lib_math(); break;
			case VM_LIB_IO: lib_io(); break;
			case VM_LIB_ARRAY: lib_array(); break;
			default: _error( "Unknown VM lib" );
		}
	}

	enum VM_LIB_BASIC_ENUM {
		VM_LIB_BASIC_IS_TRUE,
		VM_LIB_BASIC_HAS_TYPE,
		VM_LIB_BASIC_HAS_CLASS,
	};

	void lib_basic() {
		auto f = static_cast<VM_LIB_BASIC_ENUM>( *_STATE->pos );

		switch ( f ) {
			case VM_LIB_BASIC_IS_TRUE: lib_basic_is_true(); break;
			case VM_LIB_BASIC_HAS_TYPE: lib_basic_has_type(); break;
			case VM_LIB_BASIC_HAS_CLASS: lib_basic_has_class(); break;
			default: _error( "Unknown VM lib function" );
		}
	}

	enum VM_LIB_MATH_ENUM {
		VM_LIB_MATH_EQ,
		VM_LIB_MATH_GT,
		VM_LIB_MATH_ADD,
	};

	void lib_math() {
		auto f = static_cast<VM_LIB_MATH_ENUM>( *_STATE->pos );

		switch ( f ) {
			case VM_LIB_MATH_EQ: lib_math_eq(); break;
			case VM_LIB_MATH_GT: lib_math_gt(); break;
			case VM_LIB_MATH_ADD: lib_math_add(); break;
			default: _error( "Unknown VM lib function" );
		}
	}

	enum VM_LIB_IO_ENUM {
		VM_LIB_IO_DEBUG,
	};

	void lib_io() {
		auto f = static_cast<VM_LIB_IO_ENUM>( *_STATE->pos );

		switch ( f ) {
			case VM_LIB_IO_DEBUG: lib_io_debug(); break;
			default: _error( "Unknown VM lib function" );
		}
	}

	enum VM_LIB_ARRAY_ENUM {
		VM_LIB_ARRAY_PUSH,
		VM_LIB_ARRAY_POP,
		VM_LIB_ARRAY_SET,
		VM_LIB_ARRAY_GET,
	};

	void lib_array() {
		auto f = static_cast<VM_LIB_ARRAY_ENUM>( *_STATE->pos );

		switch ( f ) {
			case VM_LIB_ARRAY_PUSH: lib_array_push(); break;
			case VM_LIB_ARRAY_POP: lib_array_pop(); break;
			case VM_LIB_ARRAY_SET: lib_array_set(); break;
			case VM_LIB_ARRAY_GET: lib_array_get(); break;
			default: _error( "Unknown VM lib function" );
		}
	}

}
