#pragma once 

#include "error.cpp"


namespace VM {

	std::unordered_map<std::string, Object*> _KARGS = {};

	void kargs_set( std::string key, Object* o ) {
		auto found = _KARGS.find( key );

		if ( found != _KARGS.end() ) scope_detach( _SESSION_SCOPE, _KARGS[ key ] );

		_KARGS.insert_or_assign( key, o );

		scope_attach( _SESSION_SCOPE, o );
	}

	Object* kargs_get( std::string key ) {
		auto found = _KARGS.find( key );

		if ( found == _KARGS.end() ) _error( "KARGS GET: key not found" );

		return _KARGS[ key ];
	}

	void kargs_clear() {
		for ( auto it : _KARGS ) scope_detach( _SESSION_SCOPE, it.second );

		_KARGS.clear();
	}

	void kargs_reset() {
		_KARGS.clear();
	}

}
