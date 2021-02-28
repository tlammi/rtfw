#include "rtfw/module.hpp"

struct Mod2;
struct Mod3;

struct Mod1{
	
	Mod1(): selfref{rtfw::module<Mod2>()}{}
	rtfw::ModuleRef<Mod2> selfref;
};


struct Mod2{
	
	Mod2(): selfref{rtfw::module<Mod3>()}{}
	rtfw::ModuleRef<Mod3> selfref;
};


struct Mod3{
	
	Mod3(): selfref{rtfw::module<Mod1>()}{}
	rtfw::ModuleRef<Mod1> selfref;
};


RTFW_MODULE(Mod1);
RTFW_MODULE(Mod2);
RTFW_MODULE(Mod3);
