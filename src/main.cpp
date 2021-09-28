#include "vm/vm.cpp"


using namespace VM;

int main() {
	auto bc = load( "test.gbc" );
	auto scope = scope_create();

	auto _ext_test = [ scope ]( Scope* _scope, std::vector<Object*>* args, std::unordered_map<std::string, Object*>* kargs) -> Object* {
		std::cout << ">>> EXT TEST CALLED <<<";
		return object_create_int( _scope, 1111 );
	};

	Object* name = object_create_string( scope, std::string( "TEST" ) );
	Object* ext = object_create_external( scope, _ext_test );
	
	scope_name_set( scope, name, ext );

	run( bc, scope );
}