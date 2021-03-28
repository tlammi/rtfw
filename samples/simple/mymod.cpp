#include "rtfw/module.hpp"

#include <iostream>

class MyMod{
public:

	static rtfw::Config config(){
		return {};
	}

	MyMod(const rtfw::Config& conf){
		std::cerr << "mymod ctor\n";
	}

	~MyMod(){
		std::cerr << "mymod dtor\n";
	}
};


RTFW_MODULE(MyMod);
