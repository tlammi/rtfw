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

TEST(SplitStr, Simple){
	auto splitted = detail::split("this.is.a.string", ".");
	ASSERT_EQ(splitted.size(), 4);
	ASSERT_EQ(splitted.at(0), "this");
	ASSERT_EQ(splitted.at(1), "is");
	ASSERT_EQ(splitted.at(2), "a");
	ASSERT_EQ(splitted.at(3), "string");
}

TEST(SplitStr, Empty){
	auto splitted = detail::split("", ".");
	ASSERT_EQ(splitted.size(), 1);
}

TEST(SplitStr, NoSeparator){
	auto splitted = detail::split("hello", "/");
	ASSERT_EQ(splitted.size(), 1);
	ASSERT_EQ(splitted.at(0), "hello");
}

TEST(SplitStr, SequentialSeparators){
	auto splitted = detail::split("a.....b", ".");
	ASSERT_EQ(splitted.size(), 6);
	ASSERT_EQ(splitted.at(0), "a");
	ASSERT_EQ(splitted.at(1), "");
	ASSERT_EQ(splitted.at(2), "");
	ASSERT_EQ(splitted.at(3), "");
	ASSERT_EQ(splitted.at(4), "");
	ASSERT_EQ(splitted.at(5), "b");
}

