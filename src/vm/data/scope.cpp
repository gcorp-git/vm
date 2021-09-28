#pragma once

#include "object.cpp"


namespace VM {

	uint64_t _SCOPE_ID = 0;

	struct Scope {
		uint64_t id = 0;
		Scope* parent = NULL;
		std::unordered_map<uint64_t, uint64_t> counters = {};
		std::unordered_map<uint64_t, Object*> objects = {};
		std::unordered_map<std::string, Object*> names = {};
		std::unordered_map<uint64_t, Object*> closures = {};
		bool isMarkedForDestroy = false;
		bool isBeingDestroyed = false;
	};

	std::unordered_map<uint64_t, Scope*> _SCOPES = {};

	void _scope_on_detach( Scope* scope, Object* o );

	bool scope_exists( uint64_t id ) {
		return _SCOPES.find( id ) != _SCOPES.end();
	}

	Scope* scope_create() {
		auto scope = new Scope();
		scope->id = ++_SCOPE_ID;
		scope->parent = NULL;
		scope->counters = {};
		scope->objects = {};
		scope->names = {};
		scope->closures = {};
		scope->isMarkedForDestroy = false;
		scope->isBeingDestroyed = false;

		_SCOPES.insert({ scope->id, scope });

		return _SCOPES[ scope->id ];
	}

	void scope_destroy( Scope* scope ) {
		if ( scope->isBeingDestroyed ) return;

		if ( scope->closures.size() ) {
			scope->isMarkedForDestroy = true;
			return;
		}

		scope->isBeingDestroyed = true;

		for ( auto it : scope->objects ) _scope_on_detach( scope, it.second );

		scope->counters.clear();
		scope->objects.clear();

		auto found = _SCOPES.find( scope->id );
		
		if ( found != _SCOPES.end() ) {
			_SCOPES.erase( found );

			if ( !_SCOPES.size() ) _SCOPE_ID = 0;

			delete scope;
		}

		scope->names.clear();
	}

	void scope_attach( Scope* scope, Object* o ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		
		auto fc = scope->counters.find( o->id );

		if ( fc != scope->counters.end() ) {
			scope->counters[ o->id ]++;
			return;
		}

		if ( scope->id != o->scope->id ) o->scope->closures.insert({ o->id, o });

		switch ( o->type ) {
			case TYPE_ARRAY: {
				auto _o = reinterpret_cast<ObjectArray*>( o->data );
				for ( auto it : _o->data ) scope_attach( scope, it );
			} break;
			case TYPE_MAP: {
				auto _o = reinterpret_cast<ObjectMap*>( o->data );
				for ( auto it : _o->data ) scope_attach( scope, it.second );
			} break;
			case TYPE_LIST: {
				auto _o = reinterpret_cast<ObjectList*>( o->data );
				for ( auto it : _o->arr ) scope_attach( scope, it );
				for ( auto it : _o->map ) scope_attach( scope, it.second );
			} break;
			case TYPE_CLASS: {
				auto _o = reinterpret_cast<ObjectClass*>( o->data );
				for ( auto it : _o->data ) scope_attach( scope, it.second );
			} break;
			case TYPE_INSTANCE: {
				auto _o = reinterpret_cast<ObjectInstance*>( o->data );
				for ( auto it : _o->data ) scope_attach( scope, it.second );
				if ( _o->_class ) scope_attach( scope, _o->_class );
			} break;
		}

		scope->counters.insert({ o->id, 1 });
		scope->objects.insert({ o->id, o });

		o->scopes.insert({ scope->id, scope });
	}

	void scope_detach( Scope* scope, Object* o ) {
		auto fc = scope->counters.find( o->id );

		if ( fc == scope->counters.end() ) return;

		scope->counters[ o->id ]--;

		if ( scope->counters[ o->id ] ) return;
		
		scope->counters.erase( fc );
		scope->objects.erase( scope->objects.find( o->id ) );

		_scope_on_detach( scope, o );
	}

	void scope_name_set( Scope* scope, Object* name, Object* o ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		if ( name->type != TYPE_STRING ) throw std::runtime_error( "Incorrect name type" );

		auto _name = reinterpret_cast<ObjectString*>( name->data );

		auto found = scope->names.find( _name->value );

		if ( found == scope->names.end() ) {
			bool parentHasName = false;
			auto parent = scope->parent;

			while ( parent && !parentHasName ) {
				auto it = parent->names.find( _name->value );
				if ( it != parent->names.end() ) {
					parentHasName = true;
					break;
				}
				parent = parent->parent;
			}

			if ( parentHasName ) {
				scope_name_set( parent, name, o );
				return;
			}
		}

		scope->names.insert_or_assign( _name->value, o );
	}

	Object* scope_name_get( Scope* scope, Object* name ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		if ( name->type != TYPE_STRING ) throw std::runtime_error( "Incorrect name type" );

		auto _name = reinterpret_cast<ObjectString*>( name->data );

		auto found = scope->names.find( _name->value );

		if ( found == scope->names.end() ) {
			if ( !scope->parent ) return NULL;
			
			return scope_name_get( scope->parent, name );
		}

		return found->second;
	}

	void scope_name_remove( Scope* scope, Object* name ) {
		if ( !scope ) throw std::runtime_error( "Scope not found" );
		if ( name->type != TYPE_STRING ) throw std::runtime_error( "Incorrect name type" );

		auto _name = reinterpret_cast<ObjectString*>( name->data );

		auto found = scope->names.find( _name->value );

		if ( found == scope->names.end() ) return;

		scope->names.erase( found );
	}

	void _scope_on_detach( Scope* scope, Object* o ) {
		auto fs = o->scopes.find( scope->id );
		if ( fs != o->scopes.end() ) o->scopes.erase( fs );

		// if ( !o->scopes.size() || o->scopes.size() == 1 && o->scopes.find( o->scope->id ) != o->scopes.end() ) {
		if ( true ) {
			auto fo = o->scope->closures.find( o->id );
			if ( fo != o->scope->closures.end() ) o->scope->closures.erase( fo );
			if ( o->scope->isMarkedForDestroy ) scope_destroy( o->scope );
		}

		if ( o->scopes.size() ) return;

		switch ( o->type ) {
			case TYPE_ARRAY: {
				auto _o = reinterpret_cast<ObjectArray*>( o->data );
				for ( auto it : _o->data ) scope_detach( scope, it );
			} break;
			case TYPE_MAP: {
				auto _o = reinterpret_cast<ObjectMap*>( o->data );
				for ( auto it : _o->data ) scope_detach( scope, it.second );
			} break;
			case TYPE_LIST: {
				auto _o = reinterpret_cast<ObjectList*>( o->data );
				for ( auto it : _o->arr ) scope_detach( scope, it );
				for ( auto it : _o->map ) scope_detach( scope, it.second );
			} break;
			case TYPE_CLASS: {
				auto _o = reinterpret_cast<ObjectClass*>( o->data );
				for ( auto it : _o->data ) scope_detach( scope, it.second );
			} break;
			case TYPE_INSTANCE: {
				auto _o = reinterpret_cast<ObjectInstance*>( o->data );
				for ( auto it : _o->data ) scope_detach( scope, it.second );
				if ( _o->_class ) scope_detach( scope, _o->_class );
			} break;
		}

		object_destroy( o );
	}

}
