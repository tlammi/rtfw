#pragma once

#include "rtfw/detail/module.hpp"
#include "rtfw/detail/core.hpp"


#define RTFW_MODULE(cls) \
	::rtfw::detail::ModuleHolderImpl< cls > RTFW_MODULE_HOLDER_ ## cls { #cls }

namespace rtfw{

template<class T>
T* module(){
	return detail::core.module<T>();
}

}
