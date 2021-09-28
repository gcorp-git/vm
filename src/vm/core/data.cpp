#pragma once

#include "../data/all.cpp"
#include "../operations/all.cpp"


namespace VM {

	uint64_t CALLSTACK_MAX_SIZE = 0xFF;

	enum VM_OP_ENUM {
		OP_NOOP,

		OP_REG_NONE,
		OP_REG_BOOL,
		OP_REG_BYTE,
		OP_REG_INT,
		OP_REG_FLOAT,
		OP_REG_STRING,
		OP_REG_STRING_LONG,
		OP_REG_CALLABLE,
		OP_REG_CALLABLE_LONG,
		OP_REG_ARRAY,
		OP_REG_MAP,
		OP_REG_LIST,
		OP_REG_CLASS,
		OP_REG_INSTANCE,

		OP_MOVE,

		OP_NAME_SET,
		OP_NAME_GET,
		OP_NAME_REMOVE,

		OP_IF,
		OP_ELSE,
		OP_IF_ELSE,

		OP_ARGS_GET,
		OP_ARGS_SET,
		OP_ARGS_CLEAR,
		OP_KARGS_GET,
		OP_KARGS_SET,
		OP_KARGS_CLEAR,

		OP_CALL,
		OP_LIB,
		OP_EXT,

		OP_BREAK,
		OP_CONTINUE,
		OP_RETURN,
		OP_TAIL,

		OP_SAVE,
		OP_LOAD,

		OP_HALT,
	};

	enum VM_SYGNAL_ENUM {
		VM_SYGNAL_NONE,
		VM_SYGNAL_BREAK,
		VM_SYGNAL_CONTINUE,
		VM_SYGNAL_RETURN,
		VM_SYGNAL_TAIL,
	};

	struct CALL_DATA {
		Object* object = NULL;
		ObjectCallable* callable = NULL;
		VM_BYTECODE* caller_bc = NULL;
		uint64_t caller_offset = 0;
		Scope* caller_scope = NULL;
		uint8_t regSize = 0;
		bool isLooped = false;
	};
	
	struct VM_STATE {
		bool stop;
		bool stop_current_iteration;
		VM_BYTECODE* bc;
		uint64_t offset;
		uint8_t* pos;
		Scope* scope;
		std::vector<CALL_DATA*> callstack;
		VM_SYGNAL_ENUM sygnal;
		VM_OP_ENUM op;
	};

	VM_STATE* _STATE = NULL;

	Scope* _SESSION_SCOPE = NULL;

}
