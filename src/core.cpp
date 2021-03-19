#include "rtfw/core.hpp"
#include "rtfw/module.hpp"

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

}

namespace detail{

List init_stack{};

}

void run(){
	signal(SIGINT, signal_handler);
	detail::List init_stack;
	auto* init_module = detail::ModuleHolder::instances.first();
	while(running){
		std::cerr << "running\n";
		std::this_thread::sleep_for(100ms);
	}
}
}
