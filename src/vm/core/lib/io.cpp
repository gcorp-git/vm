#pragma once

#include "../error.cpp"
#include "../reg.cpp"
#include "../args.cpp"


namespace VM {

	void lib_io_debug() {
		auto o = reg_get( *(_STATE->pos + 1) );

		std::cout << _get_object_as_string( o );

		_STATE->offset += 1;
	}

}
