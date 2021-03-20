#include "rtfw/detail/core.hpp"


#include <iostream>



namespace rtfw{
namespace detail{
using namespace std::literals::chrono_literals;


namespace {

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
	init(init_stack_, ModuleHolder::instances);
	std::this_thread::sleep_for(1s);
	deinit(ModuleHolder::instances);
}

Core core{};
}
}
