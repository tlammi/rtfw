#include "rtfw/detail/core.hpp"



namespace rtfw{
namespace detail{

using namespace std::literals::chrono_literals;

Core::Core(){}

Core::~Core(){}
void Core::run(){
	ListHead* iterator = ModuleHolder::instances.first();
	while(iterator){
		auto* target = iterator;
		iterator = List::next(*iterator);
		init_stack_.push_back(*target);
		static_cast<ModuleHolder*>(target)->init();
		ModuleHolder::instances.push_front(*target);
	}

	std::this_thread::sleep_for(10s);

}

Core core{};
}
}
