#pragma once

#include "../data/all.cpp"

// todo: @tmp
#include <iostream>
#include <string>


namespace VM {
	
	std::string _get_object_as_string( Object* );

	std::string _get_object_type_as_string( Object* o ) {
		switch ( o->type ) {
			case TYPE_NONE: return "none";
			case TYPE_BOOL: return "bool";
			case TYPE_BYTE: return "byte";
			case TYPE_INT: return "int";
			case TYPE_FLOAT: return "float";
			case TYPE_STRING: return "string";
			case TYPE_CALLABLE: return "callable";
			case TYPE_EXTERNAL: return "external";
			case TYPE_ARRAY: return "array";
			case TYPE_MAP: return "map";
			case TYPE_LIST: return "list";
			case TYPE_CLASS: return "class";
			case TYPE_INSTANCE: return "instance";
			default: return "???";
		}
	}

	std::string _get_object_scopes_as_string( Object* o ) {
		std::string s = " ";

		for ( auto it : o->scopes ) {
			s += std::to_string( it.first ) + " ";
		}

		return s;
	}

	std::string _get_object_value_as_string( Object* o ) {
		switch ( o->type ) {
			case TYPE_NONE: {
				return "";
			} break;
			case TYPE_BOOL: {
				auto _o = reinterpret_cast<ObjectBool*>( o->data );
				return _o->value ? "true" : "false";
			} break;
			case TYPE_BYTE: {
				auto _o = reinterpret_cast<ObjectByte*>( o->data );
				return std::to_string( (uint64_t)_o->value );
			} break;
			case TYPE_INT: {
				auto _o = reinterpret_cast<ObjectInt*>( o->data );
				return std::to_string( _o->value );
			} break;
			case TYPE_FLOAT: {
				auto _o = reinterpret_cast<ObjectFloat*>( o->data );
				return std::to_string( _o->value );
			} break;
			case TYPE_STRING: {
				auto _o = reinterpret_cast<ObjectString*>( o->data );
				return _o->value;
			} break;
			case TYPE_CALLABLE: {
				auto _o = reinterpret_cast<ObjectCallable*>( o->data );
				auto offset = _o->offset;
				auto size = _o->size;
				
				std::string type;

				switch ( _o->type ) {
					case CALLABLE_SOFT_BLOCK: type = "soft-block"; break;
					case CALLABLE_HARD_BLOCK: type = "hard-block"; break;
					case CALLABLE_FUNCTION: type = "function"; break;
				}

				return "{" + type + ":" + std::to_string( offset ) + ":" + std::to_string( size ) + "}";
			} break;
			case TYPE_EXTERNAL: {
				return "{...}";
			} break;
			case TYPE_ARRAY: {
				std::string s = "";

				s += "{";

				auto _o = reinterpret_cast<ObjectArray*>( o->data );

				if ( _o->data.size() ) {
					s += " ";

					for ( auto it : _o->data ) {
						s += _get_object_as_string( it ) + " ";
					}
				}

				s += "}";

				return s;
			} break;
			case TYPE_MAP: {
				std::string s = "";

				s += "{";

				auto _o = reinterpret_cast<ObjectMap*>( o->data );

				if ( _o->data.size() ) {
					s += " ";

					for ( auto it : _o->data ) {
						s += "\"" + it.first + "\"=" + _get_object_as_string( it.second ) + " ";
					}
				}

				s += "}";

				return s;
			} break;
			case TYPE_LIST: {
				std::string s = "";

				s += "{";

				auto _o = reinterpret_cast<ObjectList*>( o->data );

				if ( _o->arr.size() ) {
					s += " ";

					for ( auto it : _o->arr ) {
						s += _get_object_as_string( it );
						s += " ";
					}
				}

				if ( _o->map.size() ) {
					s += " ";

					for ( auto it : _o->map ) {
						s += "\"" + it.first + "\"=" + _get_object_as_string( it.second );
						s += " ";
					}
				}

				s += "}";

				return s;
			} break;

			// ...

			default: {
				return "???";
			} break;
		}
	}

	std::string _get_object_as_string( Object* o ) {
		if ( !o ) return "NULL";

		return _get_object_type_as_string( o )
			+ "<" + std::to_string( o->id ) + ":" + std::to_string( o->scopes.size() ) + ">"
			+ "{" + _get_object_scopes_as_string( o ) + "}"
			+ "[" + _get_object_value_as_string( o ) + "]";
	}

	void _debug_all_objects() {
		for ( auto it : objects ) std::cout << _get_object_as_string( it.second ) << "\n";

		std::cout << std::endl;
	}

}
