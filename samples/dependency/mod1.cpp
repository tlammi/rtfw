#include "mod1.hpp"

#include <iostream>
#include "rtfw/module.hpp"

Mod1::Mod1(){
	std::cerr << "Mod1 ctor\n";
}

Mod1::~Mod1(){
	std::cerr << "Mod1 dtor\n";
}

int Mod1::foo() const {
	return 42;
}


RTFW_MODULE(Mod1);
