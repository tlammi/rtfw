#include "rtfw/module.hpp"

struct Mod2;
struct Mod3;

struct Mod1{

	static rtfw::Config config(){
		return {};
	}
	
	Mod1(const rtfw::Config&): ref{rtfw::module<Mod2>()}{}
	Mod2* ref;
};


struct Mod2{

	static rtfw::Config config(){
		return {};
	}
	
	Mod2(const rtfw::Config&): ref{rtfw::module<Mod3>()}{}
	Mod3* ref;
};


struct Mod3{

	static rtfw::Config config(){
		return {};
	}
	
	Mod3(const rtfw::Config&): ref{rtfw::module<Mod1>()}{}
	Mod1* ref;
};


RTFW_MODULE(Mod1);
RTFW_MODULE(Mod2);
RTFW_MODULE(Mod3);
