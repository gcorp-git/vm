#pragma once

#include "error.cpp"
#include "reg.cpp"
#include "args.cpp"
#include "kargs.cpp"


namespace VM {

	Object* _RESULT = NULL;

	void _upscope();
	void _continue();
	void _break();

	void result_save( Object* o ) {
		if ( !o ) _error( "SAVE: NULL value" );
		
		if ( _RESULT ) scope_detach( _SESSION_SCOPE, _RESULT );

		_RESULT = o;

		scope_attach( _SESSION_SCOPE, _RESULT );
	}

	Object* result_load() {
		if ( !_RESULT ) _error( "LOAD: NULL value" );

		return _RESULT;
	}

	void result_clear() {
		if ( _RESULT ) scope_detach( _SESSION_SCOPE, _RESULT );

		_RESULT = NULL;
	}

	void call( Object* o ) {
		if ( o->type != TYPE_CALLABLE ) {
			_error( "Incorrect callable object: " + _get_object_as_string( o ) );
		}

		auto _call = new CALL_DATA();

		_call->object = o;
		_call->callable = reinterpret_cast<ObjectCallable*>( o->data );
		_call->caller_bc = _STATE->bc;
		_call->caller_offset = _STATE->offset;
		_call->caller_scope = _STATE->scope;

		_STATE->callstack.push_back( _call );

		if ( _STATE->callstack.size() > CALLSTACK_MAX_SIZE ) _error( "CALLSTACK OVERFLOW" );

		if ( _call->callable->type == CALLABLE_FUNCTION ) {
			_STATE->scope = scope_create();
			_STATE->scope->parent = o->scope;

			reg_next();
			result_clear();
		} else {
			auto lastScope = _STATE->scope;
			_STATE->scope = scope_create();
			_STATE->scope->parent = lastScope;
		}

		_STATE->bc = _call->callable->bc;
		_STATE->offset = _call->callable->offset;
		_STATE->sygnal = VM_SYGNAL_NONE;
	}

	void ret() {
		if ( !_STATE->callstack.size() ) _error( "CALLSTACK UNDERFLOW" );

		auto _sygnal = _STATE->sygnal;

		_STATE->sygnal = VM_SYGNAL_NONE;

		if ( _sygnal == VM_SYGNAL_NONE ) { _break(); return; }

		while ( true ) {
			auto _call = _STATE->callstack.back();

			if ( _call->callable->type == CALLABLE_FUNCTION ) break;
			if ( _call->callable->type == CALLABLE_HARD_BLOCK ) {
				if ( _sygnal == VM_SYGNAL_BREAK ) break;
				if ( _sygnal == VM_SYGNAL_CONTINUE ) break;
			}

			_upscope();
		}

		switch ( _sygnal ) {
			case VM_SYGNAL_BREAK:
			case VM_SYGNAL_RETURN:
				_break();
				break;
			case VM_SYGNAL_CONTINUE:
			case VM_SYGNAL_TAIL:
				_continue();
				break;
		}
	}

	void call_reset() {
		while ( _STATE->callstack.size() ) {
			auto _call = _STATE->callstack.back();
			scope_detach( _STATE->scope, _call->object );
			_STATE->callstack.pop_back();
		}

		result_clear();
	}

	void _upscope() {
		auto _call = _STATE->callstack.back();

		if ( _call->callable->type == CALLABLE_FUNCTION ) {
			if ( _call->caller_scope != NULL ) {
				scope_destroy( _STATE->scope );
				_STATE->scope = _call->caller_scope;
			}
		} else {
			if ( _STATE->scope->parent != NULL ) {
				auto parent = _STATE->scope->parent;
				scope_destroy( _STATE->scope );
				_STATE->scope = parent;
			}
		}

		_STATE->bc = _call->caller_bc;
		_STATE->offset = _call->caller_offset;

		if ( !_STATE->callstack.size() ) _error( "CALLSTACK UNDERFLOW" );

		delete _STATE->callstack[ _STATE->callstack.size() - 1 ];

		_STATE->callstack.pop_back();
	}

	void _continue() {
		auto _call = _STATE->callstack.back();

		auto parent = _STATE->scope->parent;
		scope_destroy( _STATE->scope );
		_STATE->scope = scope_create();
		_STATE->scope->parent = parent;

		_call->isLooped = true;

		if ( _call->callable->type == CALLABLE_FUNCTION ) {
			result_clear();
		}

		_STATE->offset = _call->callable->offset;
		_STATE->sygnal = VM_SYGNAL_NONE;
	}

	void _break() {
		auto _call = _STATE->callstack.back();
		
		_call->isLooped = false;

		if ( _call->callable->type == CALLABLE_FUNCTION ) {
			reg_previous();
		}

		_upscope();
	}

}
