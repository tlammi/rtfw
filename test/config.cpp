#include "rtfw/config.hpp"

#include <gtest/gtest.h>

using rtfw::Config;

TEST(Config, Ctor){
	Config c0{};
	Config c1{"myconfig", {"key0", "key1"}};
	Config c2{"config"};
}


TEST(Config, Name){
	Config c0{};
	ASSERT_EQ(c0.name(), "");
	Config c1{"name"};
	ASSERT_EQ(c1.name(), "name");
	c1.set_name("asdf");
	ASSERT_EQ(c1.name(), "asdf");
}

TEST(Config, Access){
	Config c0{"config", {"key0", "key1", "asdf"}};
	c0.set("key0", "100");
	c0.set("key1", "another");
	c0.set("asdf", "A");

	ASSERT_EQ(c0.value<int>("key0"), 100);
	ASSERT_EQ(c0.value<std::string>("key1"), "another");
	ASSERT_EQ(c0.value("asdf"), "A");
	ASSERT_ANY_THROW(c0.value("does not exist"));

}

TEST(Config, ConstAccess){
	Config c0{"config", {"key0", "key1", "asdf"}};
	c0.set("key0", "100");
	auto lambda = [](const Config& c){
		EXPECT_EQ(c.value<std::string>("key0"), "100");
	};
	lambda(c0);
}

