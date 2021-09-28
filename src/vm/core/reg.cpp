#pragma once 

#include "error.cpp"


namespace VM {

	struct VM_REGISTER {
		std::vector<Object*> data;
		uint64_t offset;
		uint64_t top;
	};

	VM_REGISTER* _REG = NULL;

	void _error_reg_out_of_range( uint8_t index );

	void reg_set( uint8_t index, Object* o ) {
		if ( index > _REG->top ) _error_reg_out_of_range( index );
		if ( index == _REG->top ) _REG->top++;
		
		auto abs_index = _REG->offset + index;

		if ( abs_index == _REG->data.size() ) {
			_REG->data.push_back( o );
		} else {
			scope_detach( _SESSION_SCOPE, _REG->data[ abs_index ] );
			_REG->data[ abs_index ] = o;
		}

		scope_attach( _SESSION_SCOPE, o );
	}

	Object* reg_get( uint8_t index ) {
		if ( index >= _REG->top ) _error_reg_out_of_range( index );

		auto abs_index = _REG->offset + index;

		return _REG->data[ abs_index ];
	}

	void reg_move( uint8_t index, uint8_t from ) {
		if ( index > _REG->top ) _error_reg_out_of_range( index );
		if ( from >= _REG->top ) _error_reg_out_of_range( from );

		if ( index == from ) return;
		if ( index == _REG->top ) _REG->top++;

		auto abs_index = _REG->offset + index;
		auto o = _REG->data[ _REG->offset + from ];

		if ( abs_index == _REG->data.size() ) {
			_REG->data.push_back( o );
		} else {
			scope_detach( _SESSION_SCOPE, _REG->data[ abs_index ] );
			_REG->data[ abs_index ] = o;
		}

		scope_attach( _SESSION_SCOPE, o );
	}

	void reg_next() {
		auto _call = _STATE->callstack.back();
		
		_call->regSize = _REG->top;
		_REG->offset += _REG->top;
		_REG->top = 0;
	}

	void reg_previous() {
		auto _call = _STATE->callstack.back();

		for ( uint64_t i = _REG->top; i > 0; --i ) {
			scope_detach( _SESSION_SCOPE, _REG->data[ _REG->offset + i - 1 ] );
			_REG->data.pop_back();
		}

		_REG->offset -= _call->regSize;
		_REG->top = _call->regSize;
	}

	void reg_clear() {
		for ( uint64_t i = _REG->top; i > 0; --i ) {
			scope_detach( _SESSION_SCOPE, _REG->data[ _REG->offset + i - 1 ] );
			_REG->data.pop_back();
		}

		_REG->top = 0;
	}

	void reg_reset() {
		if ( !_REG ) {
			_REG = new VM_REGISTER();
			
			_REG->data = {};
			_REG->offset = 0;
			_REG->top = 0;
		}

		_REG->data.clear();
		_REG->offset = 0;
		_REG->top = 0;
	}

	void _error_reg_out_of_range( uint8_t index ) {
		auto idx = std::to_string( index );
		auto size = std::to_string( _REG->top );
		_error( "REG set: out of range (" + idx + " of " + size + ")" );
	}

	// todo: @tmp
	void reg_debug() {
		std::cout << "REG:" << std::endl;

		uint8_t index = 0;

		for ( auto it : _REG->data ) {
			std::cout << "0x"; printf( "%02X", index ); std::cout << ": ";
			std::cout << _get_object_as_string( it );
			std::cout << std::endl;
			
			index++;
		}

		std::cout << std::endl;
	}

}