#pragma once 

#include "error.cpp"


namespace VM {

	std::vector<Object*> _ARGS = {};

	void args_set( uint8_t index, Object* o ) {
		if ( index > _ARGS.size() ) _error( "ARGS SET: out of range" );

		if ( index == _ARGS.size() ) {
			_ARGS.push_back( o );
		} else {
			scope_detach( _SESSION_SCOPE, _ARGS[ index ] );
			_ARGS[ index ] = o;
		}

		scope_attach( _SESSION_SCOPE, o );
	}

	Object* args_get( uint8_t index ) {
		if ( index >= _ARGS.size() ) _error( "ARGS GET: out of range" );

		return _ARGS[ index ];
	}

	void args_clear() {
		for ( auto it : _ARGS ) scope_detach( _SESSION_SCOPE, it );

		_ARGS.clear();
	}

	void args_reset() {
		_ARGS.clear();
	}

}