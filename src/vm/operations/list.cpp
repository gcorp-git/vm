#pragma once

#include "../data/all.cpp"


namespace VM {
	
	bool object_list_in( Object* list, std::string key ) {
		auto _list = reinterpret_cast<ObjectList*>( list->data );

		auto found = _list->map.find( key );
		
		return found != _list->map.end();
	}

	bool object_list_has( Object* list, Object* o ) {
		
		// todo: object_list_has()
		// returns true if either _list->arr or _list->map contains `o`
		
		throw std::runtime_error( "THIS OPERATION IS NOT IMPLEMENTED YET" );
		return false;
	}

	void object_list_set_index( Object* list, int64_t index, Object* o ) {
		auto _list = reinterpret_cast<ObjectList*>( o->data );

		uint64_t abs_index = index >= 0 ? index : _list->arr.size() + index;

		if ( abs_index > _list->arr.size() ) {
			throw std::runtime_error( "LIST: out of range" );
		}

		scope_attach( list->scope, o );

		if ( abs_index == _list->arr.size() ) {
			_list->arr.push_back( o );
		} else {
			scope_detach( list->scope, _list->arr[ abs_index ] );
			_list->arr[ abs_index ] = o;
		}
	}

	Object* object_list_get_index( Object* list, int64_t index ) {
		auto _list = reinterpret_cast<ObjectList*>( list->data );

		uint64_t abs_index = index >= 0 ? index : _list->arr.size() + index;

		if ( abs_index >= _list->arr.size() ) {
			throw std::runtime_error( "LIST: out of range" );
		}

		return _list->arr[ abs_index ];
	}

	void object_list_set_key( Object* list, std::string key, Object* o ) {
		auto _list = reinterpret_cast<ObjectList*>( list->data );

		scope_attach( list->scope, o );

		_list->map.insert_or_assign( key, o );
	}

	Object* object_list_get_key( Object* list, std::string key ) {
		auto _list = reinterpret_cast<ObjectList*>( list->data );

		auto found = _list->map.find( key );

		if ( found == _list->map.end() ) {
			throw std::runtime_error( "LIST: Trying to access unexisting key" );
		}

		return found->second;
	}

	void object_list_push( Object* list, Object* o ) {
		auto _list = reinterpret_cast<ObjectList*>( list->data );

		scope_attach( list->scope, o );

		_list->arr.push_back( o );
	}

	Object* object_list_pop( Object* list ) {
		auto _list = reinterpret_cast<ObjectList*>( list->data );

		if ( !_list->arr.size() ) {
			throw std::runtime_error( "ARRAY: out of range" );
		}

		auto o = _list->arr.back();
		
		scope_detach( list->scope, o );

		_list->arr.pop_back();

		return o;
	}

	uint64_t object_list_size_index( Object* list ) {
		auto _list = reinterpret_cast<ObjectList*>( list->data );

		return _list->arr.size();
	}

	uint64_t object_list_size_key( Object* list ) {
		auto _list = reinterpret_cast<ObjectList*>( list->data );

		return _list->map.size();
	}

	void object_list_clear( Object* list ) {
		auto _list = reinterpret_cast<ObjectList*>( list->data );

		for ( auto it : _list->arr ) scope_detach( list->scope, it );
		for ( auto it : _list->map ) scope_detach( list->scope, it.second );

		_list->arr.clear();
		_list->map.clear();
	}

}
