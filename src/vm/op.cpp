#pragma once

#include "core/call.cpp"
#include "core/lib.cpp"


namespace VM {

	void _debug_op();
	std::string _get_op_name( VM_OP_ENUM op );

	uint64_t read8bytes( uint64_t offset ) {
		uint8_t* p = reinterpret_cast<uint8_t*>( _STATE->bc->data ) + offset;
		uint64_t B0, B1, B2, B3, B4, B5, B6, B7;

		B0 = *(p + 0); B1 = *(p + 1); B2 = *(p + 2); B3 = *(p + 3);
		B4 = *(p + 4); B5 = *(p + 5); B6 = *(p + 6); B7 = *(p + 7);

		return (
			B0 << 56 | B1 << 48 | B2 << 40 | B3 << 32 |
			B4 << 24 | B5 << 16 | B6 <<  8 | B7
		);
	}

	void _op_noop() {
		//
	}
	void _op_reg_none() {
		reg_set( *(_STATE->pos + 1), object_create_none( _STATE->scope ) );
		_STATE->offset += 1;
	}
	void _op_reg_bool() {
		bool value = *(_STATE->pos + 2);
		reg_set( *(_STATE->pos + 1), object_create_bool( _STATE->scope, value ) );
		_STATE->offset += 2;
	}
	void _op_reg_byte() {
		uint8_t value = *(_STATE->pos + 2);
		reg_set( *(_STATE->pos + 1), object_create_byte( _STATE->scope, value ) );
		_STATE->offset += 2;
	}
	void _op_reg_int() {
		int64_t value = static_cast<int64_t>( read8bytes( _STATE->offset + 2 ) );
		reg_set( *(_STATE->pos + 1), object_create_int( _STATE->scope, value ) );
		_STATE->offset += 9;
	}
	void _op_reg_float() {
		double value = static_cast<double>( read8bytes( _STATE->offset + 2 ) );
		reg_set( *(_STATE->pos + 1), object_create_float( _STATE->scope, value ) );
		_STATE->offset += 9;
	}
	void _op_reg_string() {
		uint64_t offset = _STATE->offset + 3;
		uint64_t size = *(_STATE->pos + 2);
		std::string value( reinterpret_cast<char*>( _STATE->bc->data ) + offset, size );
		reg_set( *(_STATE->pos + 1), object_create_string( _STATE->scope, value ) );
		_STATE->offset += 2 + size;
	}
	void _op_reg_string_long() {
		uint64_t offset = _STATE->offset + 10;
		uint64_t size = read8bytes( _STATE->offset + 2 );
		std::string value( reinterpret_cast<char*>( _STATE->bc->data ) + offset, size );
		reg_set( *(_STATE->pos + 1), object_create_string( _STATE->scope, value ) );
		_STATE->offset += 1 + 8 + size;
	}
	void _op_reg_callable() {
		CALLABLE_TYPE_ENUM type = static_cast<CALLABLE_TYPE_ENUM>( *(_STATE->pos + 2) );
		uint64_t offset = _STATE->offset + 4;
		uint64_t size = *(_STATE->pos + 3);
		reg_set( *(_STATE->pos + 1), object_create_callable(
			_STATE->scope,
			type,
			_STATE->bc,
			offset,
			size
		));
		_STATE->offset += 3 + size;
	}
	void _op_reg_callable_long() {
		CALLABLE_TYPE_ENUM type = static_cast<CALLABLE_TYPE_ENUM>( *(_STATE->pos + 2) );
		uint64_t offset = _STATE->offset + 11;
		uint64_t size = read8bytes( _STATE->offset + 3 );
		reg_set( *(_STATE->pos + 1), object_create_callable(
			_STATE->scope,
			type,
			_STATE->bc,
			offset,
			size
		));
		_STATE->offset += 2 + 8 + size;
	}
	void _op_reg_array() {
		reg_set( *(_STATE->pos + 1), object_create_array( _STATE->scope ) );
		_STATE->offset += 1;
	}
	void _op_reg_map() {
		reg_set( *(_STATE->pos + 1), object_create_map( _STATE->scope ) );
		_STATE->offset += 1;
	}
	void _op_reg_list() {
		reg_set( *(_STATE->pos + 1), object_create_list( _STATE->scope ) );
		_STATE->offset += 1;
	}
	void _op_reg_class() {
		reg_set( *(_STATE->pos + 1), object_create_class( _STATE->scope ) );
		_STATE->offset += 1;
	}
	void _op_reg_instance() {
		auto c = reg_get( *(_STATE->pos + 2) );
		auto o = object_create_instance( _STATE->scope, c );

		// todo: instance constructor()
		// (CALLABLE, EXTERNAL)

		reg_set( *(_STATE->pos + 1), o );
		_STATE->offset += 2;
	}
	void _op_move() {
		reg_move( *(_STATE->pos + 1), *(_STATE->pos + 2) );
		_STATE->offset += 2;
	}
	void _op_name_set() {
		auto name = reg_get( *(_STATE->pos + 1) );
		auto o = reg_get( *(_STATE->pos + 2) );
		scope_name_set( _STATE->scope, name, o );
		_STATE->offset += 2;
	}
	void _op_name_get() {
		auto name = reg_get( *(_STATE->pos + 2) );
		auto o = scope_name_get( _STATE->scope, name );
		reg_set( *(_STATE->pos + 1), o );
		_STATE->offset += 2;
	}
	void _op_name_remove() {
		auto name = reg_get( *(_STATE->pos + 1) );
		scope_name_remove( _STATE->scope, name );
		_STATE->offset += 1;
	}
	void _op_if() {
		_STATE->offset += 2;
		if ( object_is_true( reg_get( *(_STATE->pos + 1) ) ) ) {
			call( reg_get( *(_STATE->pos + 2) ) );
			_STATE->stop_current_iteration = true;
		}
	}
	void _op_else() {
		_STATE->offset += 2;
		if ( object_is_true( reg_get( *(_STATE->pos + 1) ) ) ) {
			call( reg_get( *(_STATE->pos + 2) ) );
			_STATE->stop_current_iteration = true;
		}
	}
	void _op_if_else() {
		_STATE->offset += 3;
		if ( object_is_true( reg_get( *(_STATE->pos + 1) ) ) ) {
			call( reg_get( *(_STATE->pos + 2) ) );
		} else {
			call( reg_get( *(_STATE->pos + 3) ) );
		}
		_STATE->stop_current_iteration = true;
	}
	void _op_args_set() {
		args_set( *(_STATE->pos + 1), reg_get( *(_STATE->pos + 2) ) );
		_STATE->offset += 2;
	}
	void _op_args_get() {
		reg_set( *(_STATE->pos + 1), args_get( *(_STATE->pos + 2) ) );
		_STATE->offset += 2;
	}
	void _op_args_clear() {
		args_clear();
		_STATE->offset += 0;
	}
	void _op_kargs_set() {
		auto key = reg_get( *(_STATE->pos + 1) );
		auto o = reg_get( *(_STATE->pos + 2) );
		if ( key->type != TYPE_STRING ) _error( "KARGS SET: incorrect key type" );
		auto _key = reinterpret_cast<ObjectString*>( key->data );
		kargs_set( _key->value, o );
		_STATE->offset += 2;
	}
	void _op_kargs_get() {
		auto key = reg_get( *(_STATE->pos + 2) );
		if ( key->type != TYPE_STRING ) _error( "KARGS GET: incorrect key type" );
		auto _key = reinterpret_cast<ObjectString*>( key->data );
		reg_set( *(_STATE->pos + 1), kargs_get( _key->value ) );
		_STATE->offset += 2;
	}
	void _op_kargs_clear() {
		kargs_clear();
		_STATE->offset += 0;
	}
	void _op_call() {
		_STATE->offset += 2;
		call( reg_get( *(_STATE->pos + 1) ) );
		_STATE->stop_current_iteration = true;
	}
	void _op_lib() {
		_STATE->offset += 1;
		_STATE->pos += 1;
		lib();
	}
	void _op_ext() {
		auto o = reg_get( *(_STATE->pos + 2) );
		if ( o->type != TYPE_EXTERNAL ) _error( "EXT: incorrect object type" );
		auto _o = reinterpret_cast<ObjectExternal*>( o->data );
		reg_set( *(_STATE->pos + 1), _o->function( _STATE->scope, &_ARGS, &_KARGS ) );
		_STATE->offset += 2;
	}
	void _op_break() {
		_STATE->sygnal = VM_SYGNAL_BREAK;
		_STATE->stop_current_iteration = true;
	}
	void _op_continue() {
		_STATE->sygnal = VM_SYGNAL_CONTINUE;
		_STATE->stop_current_iteration = true;
	}
	void _op_return() {
		_STATE->sygnal = VM_SYGNAL_RETURN;
		_STATE->stop_current_iteration = true;
	}
	void _op_tail() {
		_STATE->sygnal = VM_SYGNAL_TAIL;
		_STATE->stop_current_iteration = true;
	}
	void _op_save() {
		result_save( reg_get( *(_STATE->pos + 1) ) );
		_STATE->offset += 1;
	}
	void _op_load() {
		reg_set( *(_STATE->pos + 1), result_load() );
		_STATE->offset += 1;
	}
	void _op_halt() {
		_STATE->stop = true;
	}

	void op_next() {
		// todo: @tmp
		_debug_op();

		switch ( _STATE->op ) {
			case OP_NOOP: _op_noop(); break;
			
			case OP_REG_NONE: _op_reg_none(); break;
			case OP_REG_BOOL: _op_reg_bool(); break;
			case OP_REG_BYTE: _op_reg_byte(); break;
			case OP_REG_INT: _op_reg_int(); break;
			case OP_REG_FLOAT: _op_reg_float(); break;
			case OP_REG_STRING: _op_reg_string(); break;
			case OP_REG_STRING_LONG: _op_reg_string_long(); break;
			case OP_REG_CALLABLE: _op_reg_callable(); break;
			case OP_REG_CALLABLE_LONG: _op_reg_callable_long(); break;
			case OP_REG_ARRAY: _op_reg_array(); break;
			case OP_REG_MAP: _op_reg_map(); break;
			case OP_REG_LIST: _op_reg_list(); break;
			case OP_REG_CLASS: _op_reg_class(); break;
			case OP_REG_INSTANCE: _op_reg_instance(); break;
			
			case OP_MOVE: _op_move(); break;

			case OP_NAME_SET: _op_name_set(); break;
			case OP_NAME_GET: _op_name_get(); break;
			case OP_NAME_REMOVE: _op_name_remove(); break;
			
			case OP_IF: _op_if(); break;
			case OP_ELSE: _op_else(); break;
			case OP_IF_ELSE: _op_if_else(); break;
			
			case OP_ARGS_GET: _op_args_get(); break;
			case OP_ARGS_SET: _op_args_set(); break;
			case OP_ARGS_CLEAR: _op_args_clear(); break;
			case OP_KARGS_GET: _op_kargs_get(); break;
			case OP_KARGS_SET: _op_kargs_set(); break;
			case OP_KARGS_CLEAR: _op_kargs_clear(); break;
			
			case OP_CALL: _op_call(); break;
			case OP_LIB: _op_lib(); break;
			case OP_EXT: _op_ext(); break;
			
			case OP_BREAK: _op_break(); break;
			case OP_CONTINUE: _op_continue(); break;
			case OP_RETURN: _op_return(); break;
			case OP_TAIL: _op_tail(); break;
			
			case OP_SAVE: _op_save(); break;
			case OP_LOAD: _op_load(); break;
			
			case OP_HALT: _op_halt(); break;
			default: _error( "Invalid VM operation" );
		}

		if ( _STATE->stop_current_iteration ) {
			if ( _STATE->sygnal != VM_SYGNAL_NONE ) ret();
			return;
		}

		_STATE->offset += 1;

		auto _call = _STATE->callstack.back();

		if ( _STATE->offset >= _call->callable->offset + _call->callable->size ) ret();
		
		if ( !_STATE->callstack.size() ) return;

		if ( _STATE->offset < _STATE->bc->size ) {
			_STATE->pos = _STATE->bc->data + _STATE->offset;
		}
	}

	// todo: @tmp
	void _debug_op() {
		auto _call = _STATE->callstack.back();

		if (
			!_call->isLooped ||
			_STATE->op == OP_BREAK || _STATE->op == OP_RETURN ||
			_STATE->op == OP_CONTINUE || _STATE->op == OP_TAIL
		) {
			std::cout << std::endl;
			for ( uint64_t i = 0; i < _STATE->callstack.size() - 1; ++i ) std::cout << "  ";
			printf( "%04X", _STATE->offset );
			std::cout << ": " << _get_op_name( _STATE->op ) << " { ";
			for ( auto it : _SCOPES ) std::cout << it.second->id << " ";
			std::cout << "} ";
		}
	}

	// todo: @tmp
	std::string _get_op_name( VM_OP_ENUM op ) {
		switch ( op ) {
			case OP_NOOP: return "OP_NOOP";
			case OP_REG_NONE: return "OP_REG_NONE";
			case OP_REG_BOOL: return "OP_REG_BOOL";
			case OP_REG_BYTE: return "OP_REG_BYTE";
			case OP_REG_INT: return "OP_REG_INT";
			case OP_REG_FLOAT: return "OP_REG_FLOAT";
			case OP_REG_STRING: return "OP_REG_STRING";
			case OP_REG_STRING_LONG: return "OP_REG_STRING_LONG";
			case OP_REG_CALLABLE: return "OP_REG_CALLABLE";
			case OP_REG_CALLABLE_LONG: return "OP_REG_CALLABLE_LONG";
			case OP_REG_ARRAY: return "OP_REG_ARRAY";
			case OP_REG_MAP: return "OP_REG_MAP";
			case OP_REG_LIST: return "OP_REG_LIST";
			case OP_REG_CLASS: return "OP_REG_CLASS";
			case OP_REG_INSTANCE: return "OP_REG_INSTANCE";
			case OP_MOVE: return "OP_MOVE";
			case OP_NAME_SET: return "OP_NAME_SET";
			case OP_NAME_GET: return "OP_NAME_GET";
			case OP_NAME_REMOVE: return "OP_NAME_REMOVE";
			case OP_IF: return "OP_IF";
			case OP_ELSE: return "OP_ELSE";
			case OP_IF_ELSE: return "OP_IF_ELSE";
			case OP_ARGS_GET: return "OP_ARGS_GET";
			case OP_ARGS_SET: return "OP_ARGS_SET";
			case OP_ARGS_CLEAR: return "OP_ARGS_CLEAR";
			case OP_KARGS_GET: return "OP_KARGS_GET";
			case OP_KARGS_SET: return "OP_KARGS_SET";
			case OP_KARGS_CLEAR: return "OP_KARGS_CLEAR";
			case OP_CALL: return "OP_CALL";
			case OP_LIB: return "OP_LIB";
			case OP_EXT: return "OP_EXT";
			case OP_BREAK: return "OP_BREAK";
			case OP_CONTINUE: return "OP_CONTINUE";
			case OP_RETURN: return "OP_RETURN";
			case OP_TAIL: return "OP_TAIL";
			case OP_SAVE: return "OP_SAVE";
			case OP_LOAD: return "OP_LOAD";
			case OP_HALT: return "OP_HALT";
		}

		return "???";
	}

}
