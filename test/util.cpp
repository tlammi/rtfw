#include "rtfw/detail/util.hpp"
#include <gtest/gtest.h>

namespace detail = rtfw::detail;


TEST(Defer, Normal){

	int i = 0;
	{
		detail::Defer d{[&](){
			++i;
		}};
	}
	ASSERT_EQ(i, 1);
}

TEST(Defer, Exception){

	int i=0;
	try{
		detail::Defer d{[&](){
			++i;
		}};
		throw std::exception();
	} catch(const std::exception& e){}
	ASSERT_EQ(i, 1);
}
