#pragma once

#include "../error.cpp"
#include "../reg.cpp"
#include "../args.cpp"


namespace VM {

	void lib_basic_is_true() {
		auto a = reg_get( *(_STATE->pos + 2) );
		auto o = object_create_bool( _STATE->scope, object_is_true( a ) );

		reg_set( *(_STATE->pos + 1), o );

		_STATE->offset += 2;
	}

	void lib_basic_has_type() {
		auto a = reg_get( *(_STATE->pos + 2) );
		auto type = static_cast<OBJECT_TYPE_ENUM>( *(_STATE->pos + 3) );
		auto o = object_create_bool( _STATE->scope, object_has_type( a, type ) );

		reg_set( *(_STATE->pos + 1), o );

		_STATE->offset += 3;
	}

	void lib_basic_has_class() {
		auto a = reg_get( *(_STATE->pos + 2) );
		auto c = reg_get( *(_STATE->pos + 3) );
		auto o = object_create_bool( _STATE->scope, object_has_class( a, c ) );

		reg_set( *(_STATE->pos + 1), o );

		_STATE->offset += 3;
	}

}
