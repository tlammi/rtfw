#include "rtfw/detail/list.hpp"

#include <gtest/gtest.h>


namespace detail = rtfw::detail;

struct A: public detail::ListHead {
	A(int i): detail::ListHead{}, i{i} {}
	A(const A&) = delete;
	int i;
};

TEST(List, Size){
	detail::List l;
	A a{0};
	A b{0};
	ASSERT_EQ(l.size(), 0);
	l.push_back(a);
	ASSERT_EQ(l.size(), 1);
	l.push_back(b);
	ASSERT_EQ(l.size(), 2);
	{
		A c{0};
		l.push_back(c);
		ASSERT_EQ(l.size(), 3);
	}
	ASSERT_EQ(l.size(), 2);
}


TEST(List, Push){
	A a{0};
	A b{1};
	A c{2};
	detail::List l;
	l.push_back(b);
	l.push_front(a);
	l.push_back(c);
	auto* iter = l.first();
	ASSERT_EQ(static_cast<A*>(iter)->i, 0);
	iter = detail::List::next(*iter);
	ASSERT_EQ(static_cast<A*>(iter)->i, 1);
	iter = detail::List::next(*iter);
	ASSERT_EQ(static_cast<A*>(iter)->i, 2);
	iter = detail::List::next(*iter);
	ASSERT_EQ(iter, nullptr);
}

TEST(List, Insert){
	A a{0};
	A b{1};
	A c{2};
	A d{3};
	A e{4};
	detail::List l;
	l.push_front(a);
	l.insert_before(a, b);
	l.insert_before(a, d);
	l.insert_after(a, c);
	l.insert_after(a, e);

	auto* iter = l.first();
	ASSERT_EQ(static_cast<A*>(iter)->i, 1);
	iter = detail::List::next(*iter);
	ASSERT_EQ(static_cast<A*>(iter)->i, 3);
	iter = detail::List::next(*iter);
	ASSERT_EQ(static_cast<A*>(iter)->i, 0);
	iter = detail::List::next(*iter);
	ASSERT_EQ(static_cast<A*>(iter)->i, 4);
	iter = detail::List::next(*iter);
	ASSERT_EQ(static_cast<A*>(iter)->i, 2);
	ASSERT_EQ(iter, l.last());

}


TEST(List, Iterate){
	A a{0};
	A b{1};
	A c{2};
	A d{3};
	A e{4};
	A f{5};
	
	detail::List l;
	l.push_back(a);
	l.push_back(b);
	l.push_back(c);
	l.push_back(d);
	l.push_back(e);
	l.push_back(f);

	auto* iter = l.first();
	int expected=0;
	while(iter){
		ASSERT_EQ(static_cast<A*>(iter)->i, expected);
		iter = detail::List::next(*iter);
		++expected;
	}
	iter = l.last();
	--expected;
	while(iter){
		ASSERT_EQ(static_cast<A*>(iter)->i, expected);
		iter = l.prev(*iter);
		--expected;
	}

}

TEST(List, Pop){
 	A a{0};
	A b{1};
	A c{2};
	detail::List l;
	l.push_back(a);
	l.push_back(b);
	l.push_back(c);
	ASSERT_EQ(l.size(), 3);
	l.pop(b);
	ASSERT_EQ(l.size(), 2);
	ASSERT_EQ(l.first(), &a);
	ASSERT_EQ(l.last(), &c);

}


TEST(List, Move){
 	A a{0};
	A b{1};
	A c{2};

	detail::List l1;
	detail::List l2;

	l1.push_back(a);
	l1.push_back(b);
	l1.push_back(c);

	l2.push_back(b);

	ASSERT_EQ(l1.size(), 2);
	ASSERT_EQ(l2.size(), 1);


	ASSERT_EQ(l2.first(), &b);
	ASSERT_EQ(l2.last(), &b);

	ASSERT_EQ(l1.first(), &a);
	ASSERT_EQ(l1.last(), &c);

	ASSERT_EQ(detail::List::next(*l1.first()), l1.last());
	ASSERT_EQ(detail::List::next(*l2.first()), nullptr);
}


TEST(List, Find){
	A a{0};
	A b{0};
	A c{0};


	detail::List l;
	l.push_back(a);
	l.push_back(b);

	ASSERT_TRUE(l.find(a));
	ASSERT_TRUE(l.find(b));
	ASSERT_FALSE(l.find(c));
}

