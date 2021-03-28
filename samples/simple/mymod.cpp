#include "rtfw/module.hpp"

#include <iostream>

class MyMod{
public:

	static rtfw::Config config(){
		return {};
	}

	MyMod(){
		std::cerr << "mymod ctor\n";
	}

	~MyMod(){
		std::cerr << "mymod dtor\n";
	}
};


RTFW_MODULE(MyMod);
