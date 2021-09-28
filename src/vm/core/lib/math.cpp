#pragma once

#include "../error.cpp"
#include "../reg.cpp"
#include "../args.cpp"


namespace VM {

	void lib_math_eq() {
		auto a = reg_get( *(_STATE->pos + 2) );
		auto b = reg_get( *(_STATE->pos + 3) );
		auto o = object_create_bool( _STATE->scope, equals( a, b ) );

		reg_set( *(_STATE->pos + 1), o );

		_STATE->offset += 3;
	}
	void lib_math_gt() {
		auto a = reg_get( *(_STATE->pos + 2) );
		auto b = reg_get( *(_STATE->pos + 3) );
		auto o = object_create_bool( _STATE->scope, greater( a, b ) );

		reg_set( *(_STATE->pos + 1), o );

		_STATE->offset += 3;
	}
	void lib_math_add() {
		auto a = reg_get( *(_STATE->pos + 2) );
		auto b = reg_get( *(_STATE->pos + 3) );
		auto o = add( _STATE->scope, a, b );

		reg_set( *(_STATE->pos + 1), o );

		_STATE->offset += 3;
	}

}
