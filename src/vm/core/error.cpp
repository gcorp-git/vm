#pragma once

#include "data.cpp"
#include "debug.cpp"


namespace VM {
	
	// todo: @tmp
	struct VM_ERROR : public std::exception {
	public:
		VM_ERROR( std::string msg ) : error_message( msg.c_str() ) {}
		const char* what() const throw () {
			return error_message.c_str();
		}
	protected:
		std::string error_message;
	};

	// todo: @tmp
	void _error( std::string message ) {
		throw VM_ERROR( message );
	}

}
