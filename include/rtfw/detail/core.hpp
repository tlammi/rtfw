#pragma once


#include <thread>
#include <chrono>
#include <sstream>

#include "rtfw/detail/list.hpp"
#include "rtfw/detail/module.hpp"

namespace rtfw{
namespace detail{

class Core{
public:
	Core();
	~Core();
	
	void run();

	template<class T>
	T* module(){
		auto* holder = ModuleHolderImpl<T>::instance;
		if(!holder) throw std::runtime_error("Module not found");
		if(!holder->module()){
			if(init_stack_.find(*holder)){
				std::stringstream ss;
				ss << "Circular dependency: ";
				for(auto* iter = init_stack_.first(); iter; iter = List::next(*iter)){
					ss << static_cast<ModuleHolder*>(iter)->name();
					ss << " -> ";
				}
				ss << holder->name();
				throw std::runtime_error(ss.str());
			}
			init_stack_.push_back(*holder);
			holder->init();
			ModuleHolder::instances.push_front(*holder);
		}
		return holder->module();
	}

private:

	List init_stack_{};
};

extern Core core;

}
}
