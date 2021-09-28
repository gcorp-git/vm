#pragma once

#include "../../data/all.cpp"


namespace VM {
	
	bool object_has_class( Object* o, Object* c ) {
		if ( o->type != TYPE_INSTANCE ) return false;
		if ( c->type != TYPE_CLASS ) return false;

		auto _o = reinterpret_cast<ObjectInstance*>( o->data );
		
		return _o->_class->id == c->id;
	}

}
