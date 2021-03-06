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
	c1.name() = "asdf";
	ASSERT_EQ(c1.name(), "asdf");
}

TEST(Config, Access){
	Config c0{"config", {"key0", "key1", "asdf"}};
	c0["key0"] = "100";
	c0["key1"] = "another";
	c0["key2"] = "A";

	ASSERT_EQ(c0.value<int>("key0"), 100);
	ASSERT_EQ(c0.value<std::string>("key1"), "another");
	ASSERT_EQ(c0.value("asdf"), "");
	ASSERT_ANY_THROW(c0.value("does not exist"));

}

TEST(Config, ConstAccess){
	Config c0{"config", {"key0", "key1", "asdf"}};
	c0["key0"] = "100";
	auto lambda = [](const Config& c){
		EXPECT_EQ(c.value<std::string>("key0"), "100");
	};
	lambda(c0);
}

TEST(Config, Iterate){
	Config c{"conf", {"a", "b", "c", "d"}};
	c["a"] = "A";
	c["b"] = "B";
	c["c"] = "C";
	c["d"] = "D";

	auto iter = c.begin();
	ASSERT_EQ(iter->first, "a");
	ASSERT_EQ(iter->second, "A");
	++iter;
	ASSERT_EQ(iter->first, "b");
	ASSERT_EQ(iter->second, "B");
	++iter;
	ASSERT_EQ(iter->first, "c");
	ASSERT_EQ(iter->second, "C");
	++iter;
	ASSERT_EQ(iter->first, "d");
	ASSERT_EQ(iter->second, "D");
	++iter;
	ASSERT_EQ(iter, c.end());

}
