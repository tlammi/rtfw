#pragma once

#include <string_view>
#include <stdexcept>


namespace rtfw{
namespace detail{

template<class T>
constexpr void assert_eq(const T& a, const T& b, std::string_view msg_on_error){
	if(a != b) throw std::runtime_error(std::string(msg_on_error));
}


template<class T>
constexpr void assert_true(const T& a, std::string_view msg_on_error){
	if(!a) throw std::runtime_error(std::string(msg_on_error));
}

template<class T>
constexpr void assert_false(const T& a, std::string_view msg_on_error){
	if(a) throw std::runtime_error(std::string(msg_on_error));
}

}
}
