#pragma once

#include <fstream>
#include "op.cpp"


namespace VM {

	void _init();
	void _reset();
	void _debug_error( std::string );
	void _debug_exit();

	void run( VM_BYTECODE* bc, Scope* scope ) {
		_init();
		_reset();

		auto o = object_create_callable( scope, CALLABLE_SOFT_BLOCK, bc, 0, bc->size );
		
		_STATE->scope = scope;

		call( o );

		_SESSION_SCOPE = _STATE->scope;

		while ( !_STATE->stop && _STATE->offset < _STATE->bc->size ) {
			_STATE->stop_current_iteration = false;
			_STATE->pos = _STATE->bc->data + _STATE->offset;
			_STATE->op = static_cast<VM_OP_ENUM>( *_STATE->pos );

			try {
				try {
					op_next();
				} catch ( const std::runtime_error& e ) {
					_error( std::string( e.what() ) );
				}
			} catch ( VM_ERROR &e ) {
				_debug_error( std::string( e.what() ) );
				return;
			}
		}

		_debug_exit();
	}

	VM_BYTECODE* load( std::string filename ) {
		std::ifstream file( filename.c_str(), std::ios::in | std::ios::binary );

		if ( !file.is_open() ) {
			throw std::runtime_error( "Could not open file: " + filename );
		}

		file.seekg( 0, std::ios::end );
		auto size = file.tellg();
		file.seekg( 0, std::ios::beg );

		auto bc = new VM_BYTECODE();

		bc->size = size;

		auto memory = malloc( bc->size );

		bc->data = reinterpret_cast<uint8_t*>( memory );
		
		file.read( (char*)bc->data, bc->size );
		file.close();

		return bc;
	}

	void _init() {
		if ( _STATE ) return;

		_STATE = new VM_STATE();

		_STATE->stop = false;
		_STATE->stop_current_iteration = false;
		_STATE->bc = NULL;
		_STATE->scope = NULL;
		_STATE->offset = 0;
		_STATE->pos = NULL;
		_STATE->callstack = {};
		_STATE->sygnal = VM_SYGNAL_NONE;
		_STATE->op = OP_NOOP;
	}

	void _reset() {
		reg_reset();
		args_reset();
		kargs_reset();
		call_reset();

		_STATE->stop = false;
		_STATE->stop_current_iteration = false;
		_STATE->bc = NULL;
		_STATE->scope = NULL;
		_STATE->offset = 0;
		_STATE->pos = NULL;
		_STATE->sygnal = VM_SYGNAL_NONE;
		_STATE->op = OP_NOOP;
	}

	void _debug_error( std::string message ) {
		std::cout << std::endl;
		std::cout << "VM ERROR: " << message << std::endl;
		std::cout << "VM LAST OP: " << _get_op_name( _STATE->op );
		std::cout << " at " << _STATE->offset << std::endl;

		while ( _STATE->callstack.size() ) {
			auto _call = _STATE->callstack.back();
			uint8_t* p = _STATE->bc->data + _call->caller_offset;
			VM_OP_ENUM op = static_cast<VM_OP_ENUM>( *p );

			std::cout << "...before: " << _get_op_name( op );
			std::cout << " at " << (uint64_t)_call->caller_offset;
			std::cout << " in " << (uint64_t)_call->callable->offset;
			std::cout << std::endl;

			delete _STATE->callstack[ _STATE->callstack.size() - 1 ];
			_STATE->callstack.pop_back();
		}
	}

	void _debug_exit() {
		// todo: at this moment _VM_SCOPE doesn't exist,
		// therefore register contains mostly garbage

		std::cout << std::endl << std::endl;
		reg_debug();

		std::cout << "OBJECTS:" << std::endl;
		_debug_all_objects();

		std::cout << "SCOPES" << std::endl;
		for ( auto it : _SCOPES ) {
			std::cout << it.second->id << " : " << it.second->closures.size() << std::endl;
		}
		std::cout << std::endl;
	}

}
