#pragma once

#include "rtfw/refcountedmodule.hpp"

#define RTFW_MODULE(name) ::rtfw::RefCountedModule<name> RTFW_MODULE_ ## name{#name}

namespace rtfw {

template<class T>
ModuleRef<T> module(){
	auto ptr = detail::typed_module<T>();
	if(!ptr) throw std::runtime_error("No module found");
	return ptr->module_ref();
}
}
