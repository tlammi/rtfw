#include "rtfw/detail/assertion.hpp"

#include <gtest/gtest.h>


namespace detail = rtfw::detail;

TEST(Assertion, Equal){
	ASSERT_NO_THROW(detail::assert_eq(1, 1, "msg"));
	ASSERT_ANY_THROW(detail::assert_eq(1, 2, "msg"));
}

TEST(Assertion, True){
	ASSERT_NO_THROW(detail::assert_true(true, "msg"));
	ASSERT_ANY_THROW(detail::assert_true(false, "msg"));
}

TEST(Assertion, False){
	ASSERT_NO_THROW(detail::assert_false(false, "msg"));
	ASSERT_ANY_THROW(detail::assert_false(true, "msg"));
}
