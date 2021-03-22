#include <gtest/gtest.h>

#include "rtfw/detail/vectormap.hpp"

using rtfw::detail::VectorMap;



TEST(VectorMap, Ctor){
	VectorMap<int, int> empty{};
	VectorMap<std::string, int> v{{"1", "2", "3"}};
}

TEST(VectorMap, Indexing){
	VectorMap<std::string, int> m{{"a", "b", "c"}};

	m["a"] = 1;
	m["b"] = 2;
	ASSERT_EQ(m["a"], 1);
	ASSERT_EQ(m["b"], 2);
	ASSERT_ANY_THROW(m["asdf"]);


	[](const auto& m){
		EXPECT_EQ(m["a"], 1);
		EXPECT_EQ(m["b"], 2);
		EXPECT_ANY_THROW(m["asdf"]);
	}(m);
}


TEST(VectorMap, Size){
	VectorMap<int, double> empty{};
	ASSERT_EQ(empty.size(), 0);
	VectorMap<int, float> v{{1,2,3,4}};
	ASSERT_EQ(v.size(), 4);
}


TEST(VectorMap, HasKey){
	VectorMap<int, int> v{{1,2,3}};
	ASSERT_TRUE(v.has_key(1));
	ASSERT_FALSE(v.has_key(4));
}


TEST(VectorMap, HasValue){
	VectorMap<int, unsigned int> v{{1,2,3}};
	v[1] = 0;
	v[2] = 100;
	v[3] = 1000;

	ASSERT_TRUE(v.has_value(0));
	ASSERT_TRUE(v.has_value(100));
	ASSERT_TRUE(v.has_value(1000));
	ASSERT_FALSE(v.has_value(2));
}


TEST(VectorMap, Iterate){
	VectorMap<int, std::string> v{{4,1,2,3}};

	v[1] = "10";
	v[2] = "2";
	v[3] = "1";
	v[4] = "7";
	
	int last = 0;
	for(const auto& iter: v){
		ASSERT_GT(iter.first, last);
		last = iter.first;
	}

}
