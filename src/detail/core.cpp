#include "rtfw/detail/core.hpp"
#include "rtfw/detail/util.hpp"

#include <iostream>



namespace rtfw{
namespace detail{
using namespace std::literals::chrono_literals;


namespace {


void config(List& module_list){
	auto* iter = module_list.first();
	while(iter){
		auto* ptr = static_cast<ModuleHolder*>(iter);
		std::cerr << "configuring " << ptr->name() << '\n';
		ptr->config();
		iter = List::next(*iter);
	}
}

void init(List& init_stack, List& module_list){
	ListHead* iterator = module_list.first();
	while(iterator){
		auto* target = iterator;
		iterator = List::next(*iterator);
		init_stack.push_back(*target);
		std::cerr << "initializing " << static_cast<ModuleHolder*>(target)->name() << '\n';
		static_cast<ModuleHolder*>(target)->init();
		module_list.push_front(*target);
	}
}


void deinit(List& module_list){
	ListHead* iterator = module_list.first();
	while(iterator){
		auto* target = static_cast<ModuleHolder*>(iterator);
		std::cerr << "destroying " << target->name() << '\n';
		iterator = List::next(*iterator);
		target->clear();
	}

}

}

Core::Core(){}

Core::~Core(){}

void Core::run(){

	// Lambda is called in object destructor
	Defer defer{[&](){
		setup_ = false;
	}};

	setup_ = false;
	rtfw_thread_id_ = std::this_thread::get_id();
	config(ModuleHolder::instances);
	init(init_stack_, ModuleHolder::instances);
	setup_ = true;
	std::this_thread::sleep_for(1s);
	deinit(ModuleHolder::instances);
}

Core core{};
}
}

