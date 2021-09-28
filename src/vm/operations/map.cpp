#pragma once

#include "../data/all.cpp"


namespace VM {
	
	bool object_map_in( Object* map, std::string key ) {
		auto _map = reinterpret_cast<ObjectMap*>( map->data );

		return _map->data.find( key ) != _map->data.end();
	}

	bool object_map_has( Object* map, Object* o ) {
		auto _map = reinterpret_cast<ObjectMap*>( map->data );

		// todo: object_map_has()
		// c++20 has .contains()

		throw std::runtime_error( "THIS OPERATION IS NOT IMPLEMENTED YET" );
		return false;
	}

	void object_map_set( Object* map, std::string key, Object* o ) {
		auto _map = reinterpret_cast<ObjectMap*>( map->data );

		scope_attach( map->scope, o );

		_map->data.insert_or_assign( key, o );
	}

	Object* object_map_get( Object* map, std::string key ) {
		auto _map = reinterpret_cast<ObjectMap*>( map->data );

		auto found = _map->data.find( key );

		if ( found == _map->data.end() ) {
			throw std::runtime_error( "MAP: Trying to access unexisting key" );
		}

		return found->second;
	}

	void object_map_remove( Object* map, std::string key ) {
		auto _map = reinterpret_cast<ObjectMap*>( map->data );
		
		auto found = _map->data.find( key );

		if ( found == _map->data.end() ) {
			throw std::runtime_error( "MAP: Trying to access unexisting key" );
		}

		scope_detach( map->scope, found->second );
	}

	void object_map_clear( Object* map ) {
		auto _map = reinterpret_cast<ObjectMap*>( map->data );
		
		for ( auto it : _map->data ) scope_detach( map->scope, it.second );

		_map->data.clear();
	}

	uint64_t object_map_size( Object* map ) {
		auto _map = reinterpret_cast<ObjectMap*>( map->data );

		return _map->data.size();
	}

}
