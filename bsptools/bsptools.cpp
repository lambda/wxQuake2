#include <iostream>

extern "C" {
#	define false QFALSE
#	define true QTRUE
#	include "q_shared.h"
#	include "qfiles.h"
#	undef false
#	undef true
};

int main(int argc, char **argv) {
	std::cout << "Hello, world!" << std::endl;
	return 0;
}

