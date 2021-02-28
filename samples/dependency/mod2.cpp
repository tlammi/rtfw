#include "rtfw/module.hpp"
#include "mod1.hpp"

struct Mod2{
	Mod2(): mod1_{rtfw::module<Mod1>()}{
		std::cerr << "Mod2 ctor\n";
		std::cerr << "Mod2: " << mod1_->foo() << '\n';
	}

	~Mod2(){
		std::cerr << "Mod2 dtor\n";
	}
	
	rtfw::ModuleRef<Mod1> mod1_;
};


RTFW_MODULE(Mod2);
