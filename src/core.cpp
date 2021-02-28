#include "rtfw/core.hpp"
#include "rtfw/refcountedmodule.hpp"

#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>
#include <set>

#include <signal.h>

namespace rtfw{

using namespace std::literals::chrono_literals;

namespace {

std::atomic_bool running = true;

void signal_handler(int){
	running = false;
}

void init(std::set<RefCounter>& refs){
	std::cerr << "Initializing\n";
	for(auto [ptr, name] : detail::generic_modules()){
		std::cerr << "Initializing " << name << '\n';
		refs.insert(RefCounter{ptr});
	}
}

void teardown(std::set<RefCounter>& refs){
	std::cerr << "Tearing down\n";
	refs.clear();
}
}

void run(){
	signal(SIGINT, signal_handler);
	std::set<RefCounter> refs{};
	init(refs);
	while(running){
		std::cerr << "running\n";
		std::this_thread::sleep_for(100ms);
	}
	teardown(refs);
}
}
