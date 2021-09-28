#pragma once

#include "../../data/all.cpp"


namespace VM {
	
	bool object_has_type( Object* o, OBJECT_TYPE_ENUM type ) {
		return o->type == type;
	}

}
