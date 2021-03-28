#pragma once

#include "rtfw/module.hpp"

struct Mod1{

	static rtfw::Config config(){
		return {};
	}

	Mod1(const rtfw::Config& conf);
	~Mod1();
	int foo() const;
};
