#pragma once

#include <string_view>
#include <stdexcept>


namespace rtfw{
namespace detail{

template<class T>
void assert_eq(const T& a, const T& b, std::string_view msg_on_error){
	if(a != b) throw std::runtime_error(std::string(msg_on_error));
}
}
}
