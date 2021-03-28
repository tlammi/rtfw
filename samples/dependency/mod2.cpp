#include "rtfw/module.hpp"
#include "mod1.hpp"


#include <iostream>

struct Mod2{

	static rtfw::Config config(){
		return {};
	}

	Mod2(): mod1_{rtfw::module<Mod1>()}{
		std::cerr << "Mod2 ctor\n";
		std::cerr << "Mod2: " << mod1_->foo() << '\n';
	}

	~Mod2(){
		std::cerr << "Mod2 dtor\n";
	}
	
	Mod1* mod1_;
};


RTFW_MODULE(Mod2);
