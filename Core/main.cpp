#include <iostream>
#include "Output.h"
#include <string>
#include <fstream>
#include <ostream>

#include "Core.h"



int main() {
	print<std::string>(info, "ReTTo 1.0.0");
	print<std::string>(warning, "ReTTo 1.0.0 is made for AMD Radeon RX6700");

	Core::runWindow();
	return 0;
}