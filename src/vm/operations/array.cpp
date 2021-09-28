#pragma once

#include "../data/all.cpp"


namespace VM {
	
	bool object_array_has( Object* arr, Object* o ) {
		auto _map = reinterpret_cast<ObjectMap*>( arr->data );

		// todo: object_array_has()

		throw std::runtime_error( "THIS OPERATION IS NOT IMPLEMENTED YET" );
		return false;
	}

	void object_array_set( Object* arr, int64_t index, Object* o ) {
		auto _arr = reinterpret_cast<ObjectArray*>( arr->data );

		uint64_t abs_index = index >= 0 ? index : _arr->data.size() + index;

		if ( abs_index > _arr->data.size() ) {
			throw std::runtime_error( "ARRAY: out of range" );
		}

		scope_attach( arr->scope, o );

		if ( abs_index == _arr->data.size() ) {
			_arr->data.push_back( o );
		} else {
			scope_detach( arr->scope, _arr->data[ abs_index ] );
			_arr->data[ abs_index ] = o;
		}
	}

	Object* object_array_get( Object* arr, int64_t index ) {
		auto _arr = reinterpret_cast<ObjectArray*>( arr->data );

		uint64_t abs_index = index >= 0 ? index : _arr->data.size() + index;

		if ( abs_index >= _arr->data.size() ) {
			throw std::runtime_error( "ARRAY: out of range" );
		}

		return _arr->data[ abs_index ];
	}

	void object_array_push( Object* arr, Object* o ) {
		auto _arr = reinterpret_cast<ObjectArray*>( arr->data );

		scope_attach( arr->scope, o );

		_arr->data.push_back( o );
	}

	Object* object_array_pop( Object* arr ) {
		auto _arr = reinterpret_cast<ObjectArray*>( arr->data );

		if ( !_arr->data.size() ) {
			throw std::runtime_error( "ARRAY: out of range" );
		}

		auto o = _arr->data.back();
		
		scope_detach( arr->scope, o );

		_arr->data.pop_back();

		return o;
	}

	void object_array_clear( Object* arr ) {
		auto _arr = reinterpret_cast<ObjectArray*>( arr->data );

		for ( auto it : _arr->data ) scope_detach( arr->scope, it );

		_arr->data.clear();
	}

	uint64_t object_array_size( Object* arr ) {
		auto _arr = reinterpret_cast<ObjectArray*>( arr->data );

		return _arr->data.size();
	}
	
}
