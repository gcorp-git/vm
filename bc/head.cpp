#pragma once

#include "../src/vm/vm.cpp"


using namespace VM;

uint8_t* p = NULL;
uint64_t size = 0;

static void save( std::string filename, VM_BYTECODE bc ) {
	std::fstream file( filename.c_str(), std::ios::out | std::ios::binary );

	if ( !file.is_open() ) {
		throw std::runtime_error( "Could not open file: " + filename );
	}

	file.write( (char*)bc.data, bc.size );
	file.close();
}

void _b( VM_OP_ENUM op ) {
	size += 1;
	*p++ = (uint8_t)op;
}
void _b( VM_OP_ENUM op, uint8_t b0 ) {
	size += 2;
	*p++ = (uint8_t)op;
	*p++ = b0;
}
void _b( VM_OP_ENUM op, uint8_t b0, uint8_t b1 ) {
	size += 3;
	*p++ = (uint8_t)op;
	*p++ = b0; *p++ = b1;
}
void _b( VM_OP_ENUM op, uint8_t b0, uint8_t b1, uint8_t b2 ) {
	size += 4;
	*p++ = (uint8_t)op;
	*p++ = b0; *p++ = b1; *p++ = b2;
}

void _b( uint8_t b0 ) {
	size += 1;
	*p++ = b0;
}
void _b( uint8_t b0, uint8_t b1 ) {
	size += 2;
	*p++ = b0; *p++ = b1;
}
void _b( uint8_t b0, uint8_t b1, uint8_t b2 ) {
	size += 3;
	*p++ = b0; *p++ = b1; *p++ = b2;
}
void _b( uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3 ) {
	size += 4;
	*p++ = b0; *p++ = b1; *p++ = b2; *p++ = b3;
}
void _b(
	uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3,
	uint8_t b4
) {
	size += 5;
	*p++ = b0; *p++ = b1; *p++ = b2; *p++ = b3;
	*p++ = b4;
}
void _b(
	uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3,
	uint8_t b4, uint8_t b5
) {
	size += 6;
	*p++ = b0; *p++ = b1; *p++ = b2; *p++ = b3;
	*p++ = b4; *p++ = b5;
}
void _b(
	uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3,
	uint8_t b4, uint8_t b5, uint8_t b6
) {
	size += 7;
	*p++ = b0; *p++ = b1; *p++ = b2; *p++ = b3;
	*p++ = b4; *p++ = b5; *p++ = b6;
}
void _b(
	uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3,
	uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7
) {
	size += 8;
	*p++ = b0; *p++ = b1; *p++ = b2; *p++ = b3;
	*p++ = b4; *p++ = b5; *p++ = b6; *p++ = b7;
}
